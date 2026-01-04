/*
功能：

用STM32Core平台串口2读取HWT906的数据，然后通过串口1打印到串口助手,串口助手波特率要选为115200。
注意：示例程序输出的是ASCLL码，用16进制（HEX）显示是不能看到准确数据的。

接线
USB-TTL工具                 STM32Core              HWT906
VCC          -----           VCC        ----        VCC
TX           -----           RX1
RX           -----           TX1
GND          -----           GND        ----        GND
                             RX2        ----        TX
							 TX2        ----        RX
------------------------------------
*/
 
#include <string.h>
#include <stdio.h>
#include "Main.h"
#include "REG.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "UART1.h"
#include "UART2.h"
#include "delay.h"

struct STime    stcTime;
struct SAcc     stcAcc;
struct SGyro    stcGyro;
struct SAngle   stcAngle;
struct SMag     stcMag;
struct SDStatus stcDStatus;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SGPSV    stcGPSV;
struct SQ       stcQ;

//CopeSerialData为串口2中断调用函数，串口每收到一个数据，调用一次这个函数。
void CopeSerial2Data(unsigned char ucData)
{
    static unsigned char ucRxBuffer[250];
    static unsigned char ucRxCnt = 0;

    ucRxBuffer[ucRxCnt++]=ucData;	//将收到的数据存入缓冲区中
    if (ucRxBuffer[0]!=0x55) //数据头不对，则重新开始寻找0x55数据头
    {
        ucRxCnt=0;
        return;
    }
    if (ucRxCnt<11)
    {
        return;   //数据不满11个，则返回
    }
    else
    {
        switch(ucRxBuffer[1])//判断数据是哪种数据，然后将其拷贝到对应的结构体中，有些数据包需要通过上位机打开对应的输出后，才能接收到这个数据包的数据
        {
        case 0x50:
            memcpy(&stcTime,&ucRxBuffer[2],8);
            break;//memcpy为编译器自带的内存拷贝函数，需引用"string.h"，将接收缓冲区的字符拷贝到数据结构体里面，从而实现数据的解析。
        case 0x51:
            memcpy(&stcAcc,&ucRxBuffer[2],8);
            break;
        case 0x52:
            memcpy(&stcGyro,&ucRxBuffer[2],8);
            break;
        case 0x53:
            memcpy(&stcAngle,&ucRxBuffer[2],8);
            break;
        case 0x54:
            memcpy(&stcMag,&ucRxBuffer[2],8);
            break;
        case 0x55:
            memcpy(&stcDStatus,&ucRxBuffer[2],8);
            break;
        case 0x56:
            memcpy(&stcPress,&ucRxBuffer[2],8);
            break;
        case 0x57:
            memcpy(&stcLonLat,&ucRxBuffer[2],8);
            break;
        case 0x58:
            memcpy(&stcGPSV,&ucRxBuffer[2],8);
            break;
        case 0x59:
            memcpy(&stcQ,&ucRxBuffer[2],8);
            break;
        }
        ucRxCnt=0;//清空缓存区
    }
}
int main(void)
{
    SysTick_Init();
    Initial_UART1(115200);
    Initial_UART2(921600);

    while (1)
    {
        delay_ms(500);
		//输出时间
		printf("Time:20%02d-%02d-%02d %02d:%02d:%.3f\r\n",stcTime.ucYear,stcTime.ucMonth,stcTime.ucDay,stcTime.ucHour,stcTime.ucMinute,(float)stcTime.ucSecond+(float)stcTime.usMiliSecond/1000);
		//输出加速度
		printf("Acc:%.3f %.3f %.3f\r\n",(float)stcAcc.a[0]/32768*16,(float)stcAcc.a[1]/32768*16,(float)stcAcc.a[2]/32768*16);
		//输出角速度
		printf("Gyro:%.3f %.3f %.3f\r\n",(float)stcGyro.w[0]/32768*2000,(float)stcGyro.w[1]/32768*2000,(float)stcGyro.w[2]/32768*2000);
		//输出角度
		printf("Angle:%.3f %.3f %.3f\r\n",(float)stcAngle.Angle[0]/32768*180,(float)stcAngle.Angle[1]/32768*180,(float)stcAngle.Angle[2]/32768*180);
		//输出磁场
		printf("Mag:%d %d %d\r\n",stcMag.h[0],stcMag.h[1],stcMag.h[2]);
        //输出四元素
		printf("Four elements:%.5f %.5f %.5f %.5f\r\n\r\n",(float)stcQ.q[0]/32768,(float)stcQ.q[1]/32768,(float)stcQ.q[2]/32768,(float)stcQ.q[3]/32768);
    }
}



