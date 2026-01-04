/*
编写者：Kevin
网址：http://RobotControl.taobao.com
作者E-mail：1609370741@qq.com
编译环境：MDK-Lite  Version: 5.17
初版时间: 2016-1-31
测试： 本程序已在【君悦智控】的STM32Core平台上完成测试
功能：
用STM32Core平台SPI接口读取JY901的数据，然后通过串口1打印到串口助手。
接线
USB-TTL工具                 STM32Core              JY901
VCC          -----           VCC        ----        VCC
TX           -----           RX1
RX           -----           TX1
GND          -----           GND        ----        GND
                             PA4        ----        CS
							 PA5        ----        SCK
                             PA6        ----        MISO
							 PA7        ----        MOSI
------------------------------------
*/
 
#include <string.h>
#include <stdio.h>
#include "Main.h"
#include "REG.h"
#include "UART1.h"
#include "delay.h"
#include "SPI.h"
#include "IO.h"

void ShortToChar(short sData,unsigned char cData[])
{
    cData[0]=sData&0xff;
    cData[1]=sData>>8;
}
short CharToShort(unsigned char cData[])
{
    return ((short)cData[1]<<8)|cData[0];
}

static SPin CS = {GPIOA, GPIO_Pin_4,GPIO_Mode_Out_PP,GPIO_Speed_2MHz};
int main(void)
{
    unsigned char chrTemp[30];
    float a[3],w[3],h[3],Angle[3];

    SysTick_Init();
    Initial_UART1(115200);
    DIO_Initial(CS);
    WriteDO(CS, 1);
    SPI_Initial(SPI_CPOL_High, SPI_CPHA_2Edge);

    while (1)
    {
        delay_ms(100);
        SPIReadBytes(CS, AX, 24, &chrTemp[0]);
        a[0] = (float)CharToShort(&chrTemp[0])/32768*16;
        a[1] = (float)CharToShort(&chrTemp[2])/32768*16;
        a[2] = (float)CharToShort(&chrTemp[4])/32768*16;
        w[0] = (float)CharToShort(&chrTemp[6])/32768*2000;
        w[1] = (float)CharToShort(&chrTemp[8])/32768*2000;
        w[2] = (float)CharToShort(&chrTemp[10])/32768*2000;
        h[0] = CharToShort(&chrTemp[12]);
        h[1] = CharToShort(&chrTemp[14]);
        h[2] = CharToShort(&chrTemp[16]);
        Angle[0] = (float)CharToShort(&chrTemp[18])/32768*180;
        Angle[1] = (float)CharToShort(&chrTemp[20])/32768*180;
        Angle[2] = (float)CharToShort(&chrTemp[22])/32768*180;
      

        printf("0x50:  a:%.3f %.3f %.3f w:%.3f %.3f %.3f  h:%.0f %.0f %.0f  Angle:%.3f %.3f %.3f \r\n",a[0],a[1],a[2],w[0],w[1],w[2],h[0],h[1],h[2],Angle[0],Angle[1],Angle[2]);


    }
}



