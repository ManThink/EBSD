/*HEADER**********************************************************************
*
* Copyright 2008 Freescale Semiconductor, Inc.
* Copyright 2004-2008 Embedded Access Inc.
*
* This software is owned or controlled by Freescale Semiconductor.
* Use of this software is governed by the Freescale MQX RTOS License
* distributed with this Material.
* See the MQX_RTOS_LICENSE file distributed for more details.
*
* Brief License Summary:
* This software is provided in source form for you to use free of charge,
* but it is not open source software. You are allowed to use this software
* but you cannot redistribute it or derivative works of it in source form.
* The software may be used only in connection with a product containing
* a Freescale microprocessor, microcontroller, or digital signal processor.
* See license agreement file for full license terms including other
* restrictions.
*****************************************************************************
*
* Comments:
*
*   Example using RTCS Library.
*
*
*END************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "GPS.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "EBGlobal.h"

GPS_INFO        MT_GPS_Information;

const int32_t MaxNorthPosition = 8388607;       // 2^23 - 1
const int32_t MaxSouthPosition = 8388608;       // -2^23
const int32_t MaxEastPosition = 8388607;        // 2^23 - 1    
const int32_t MaxWestPosition = 8388608;        // -2^23
/*******************************************************************************
* Function Name  : SIM5310_RESET
* Description    : Configures the USART4.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static u1_t MT_GetComma(uchar num,char* str);
//static int MT_Get_Int_Number(char *s);
static double MT_Get_Double_Number(char *s);
static float MT_Get_Float_Number(char *s);
static void MT_UTC2BTC(DATE_TIME *GPS);
void MT_GPSInitial(void);

u1_t  GPS_GxRMC_Display_YES[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x04,0x01,0x01,0x01,0x01,0x01,0x01,0x09,0x54};
u1_t  GPS_GxGGA_Display_NO[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24};
u1_t  GPS_GxGLL_Display_NO[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x2B};
u1_t  GPS_GxGSA_Display_NO[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x32};
u1_t  GPS_GxGSV_Display_NO[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x39};
u1_t  GPS_GxVTG_Display_NO[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x47};
u1_t  GPS_GxGRS_Display_NO[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x06,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x4D};
u1_t  GPS_GxGST_Display_NO[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x07,0x00,0x00,0x00,0x00,0x00,0x01,0x06,0x54};
u1_t  GPS_GxZDA_Display_NO[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x08,0x00,0x00,0x00,0x00,0x00,0x01,0x08,0x5C};
u1_t  GPS_GxGBS_Display_NO[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x09,0x00,0x00,0x00,0x00,0x00,0x01,0x08,0x62};
u1_t  GPS_GxZTM_Display_NO[16]={0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x0A,0x00,0x00,0x00,0x00,0x00,0x01,0x09,0x69};
u1_t  Save_Parameter[21]={0xB5,0x62,0x06,0x09,0x0D,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x21,0xAF};
u1_t * GPSInit[]={GPS_GxGGA_Display_NO,GPS_GxGLL_Display_NO,GPS_GxGSA_Display_NO,
            GPS_GxGSV_Display_NO, GPS_GxVTG_Display_NO,GPS_GxGRS_Display_NO, GPS_GxGST_Display_NO,
            GPS_GxZDA_Display_NO,GPS_GxGBS_Display_NO,GPS_GxZTM_Display_NO,Save_Parameter};
/********************************************************
* Function for GPSInitial 
*
********************************************************/
void MT_GPSInitial(void)
{
     u1_t i;
     for(i=0;i<10;i++)
     {
        mpos_driver.UART2_SendBuffer(GPSInit[i],16);
        mpos_driver.kickdog();  
        mpos_osfun.DelayUs(10000);
     }
     mpos_driver.UART2_SendBuffer(GPSInit[i],21);
     return;
}
/********************************************************
* Function for GpsConvertPositionIntoBinary 
* 把double型的经纬度分别转换成字符型的
*
********************************************************/
void MT_GpsConvertPositionIntoBinary( void )
{
    long double temp;

    if( MT_GPS_Information.latitude >= 0 ) // North
    {    
        temp = MT_GPS_Information.latitude * MaxNorthPosition;
        MT_GPS_Information.LatitudeBinary = (int32_t)(temp / 90);
    }
    else                // South
    {    
        temp = MT_GPS_Information.latitude * MaxSouthPosition;
        MT_GPS_Information.LatitudeBinary = (int32_t)(temp / 90);
    }

    if( MT_GPS_Information.longitude >= 0 ) // East
    {    
        temp = MT_GPS_Information.longitude * MaxEastPosition;
        MT_GPS_Information.LongitudeBinary = (int32_t)(temp / 180);
    }
    else                // West
    {    
        temp = MT_GPS_Information.longitude * MaxWestPosition;
        MT_GPS_Information.LongitudeBinary = (int32_t)(temp / 180);
    }
}

//====================================================================//
// 语法格式：int GPS_RMC_Parse(char *line, GPS_INFO *GPS)  
// 实现功能：把gps模块的GPRMC信息解析为可识别的数据
// 参    数：存放原始信息字符数组、存储可识别数据的结构体
// 返 回 值：
//	     1: 解析GPRMC完毕
//           0: 没有进行解析，或数据无效
//====================================================================//
int MT_GPS_RMC_Parse(char *line,GPS_INFO *GPS)
{
	uchar ch, status, tmp;
        DATE_STRUCT     date_time;
	float lati_cent_tmp, lati_second_tmp;
	float long_cent_tmp, long_second_tmp;
	float speed_tmp;
	char *buf = line;
	ch = buf[5];
	status = buf[MT_GetComma(2, buf)];

	if (ch == 'C')  //如果第五个字符是C，($GPRMC)
	{
                if ((status == 'A')||(status == 'V'))  //如果数据有效，则分析
		{
			if(status == 'A') 
                        {
                          GPS -> NS       = buf[MT_GetComma(4, buf)];
                          GPS -> EW       = buf[MT_GetComma(6, buf)];
                        
                          GPS->latitude   = MT_Get_Double_Number(&buf[MT_GetComma(3, buf)]);
                          GPS->longitude  = MT_Get_Double_Number(&buf[MT_GetComma( 5, buf)]);

                          GPS->latitude_Degree  = (int)GPS->latitude / 100;       //分离纬度
                          lati_cent_tmp         = (GPS->latitude - GPS->latitude_Degree * 100);
                          GPS->latitude_Cent    = (int)lati_cent_tmp;
                          lati_second_tmp       = (lati_cent_tmp - GPS->latitude_Cent) * 60;
                          GPS->latitude_Second  = (int)lati_second_tmp;

                          if(GPS->latitude_Degree!=0)  
                          GPS->latitude = GPS->latitude_Degree + ((float)GPS->latitude_Cent)/60 + ((float)GPS->latitude_Second)/3600;
                          else;
                          if(GPS -> NS=='S') GPS->latitude = -(GPS->latitude);//
                           
                          GPS->longitude_Degree = (int)GPS->longitude / 100;	//分离经度
                          long_cent_tmp         = (GPS->longitude - GPS->longitude_Degree * 100);
                          GPS->longitude_Cent   = (int)long_cent_tmp;    
                          long_second_tmp       = (long_cent_tmp - GPS->longitude_Cent) * 60;
                          GPS->longitude_Second = (int)long_second_tmp;

                          if(GPS->longitude_Degree!=0)                          
                          GPS->longitude = GPS->longitude_Degree + ((float)GPS->longitude_Cent)/60 + ((float)GPS->longitude_Second)/3600;
                          else;
                          if(GPS -> EW=='W') GPS->longitude = -(GPS->longitude);
                          MT_GpsConvertPositionIntoBinary();
                          speed_tmp      = MT_Get_Float_Number(&buf[MT_GetComma(7, buf)]);    //速度(单位：海里/时)
                          GPS->speed     = speed_tmp * 1.85;                           //1海里=1.85公里
                          GPS->direction = MT_Get_Float_Number(&buf[MT_GetComma(8, buf)]); //角度
                          GPS->D.hour    = (buf[7] - '0') * 10 + (buf[8] - '0')+8;		//时间
                          GPS->D.minute  = (buf[9] - '0') * 10 + (buf[10] - '0');
                          GPS->D.second  = (buf[11] - '0') * 10 + (buf[12] - '0');
                          tmp = MT_GetComma(9, buf);
                          GPS->D.day     = (buf[tmp + 0] - '0') * 10 + (buf[tmp + 1] - '0'); //日期
                          GPS->D.month   = (buf[tmp + 2] - '0') * 10 + (buf[tmp + 3] - '0');
                          GPS->D.year    = (buf[tmp + 4] - '0') * 10 + (buf[tmp + 5] - '0')+2000;
                          return 1;
                        }
                        //else;// RunStatus.Varible.State.GPS=0;  
                        if((buf[7]!=',')&&(buf[15]!=','))
                        {
                          GPS->D.hour    = (buf[7] - '0') * 10 + (buf[8] - '0')+8;		//时间
                          GPS->D.minute  = (buf[9] - '0') * 10 + (buf[10] - '0');
                          GPS->D.second  = (buf[11] - '0') * 10 + (buf[12] - '0');
                          date_time.MILLISEC = (buf[14] - '0') * 100 + (buf[15] - '0')* 10;
                          //date_time.MILLISEC = 0;
                          date_time.MILLISEC += ((UartBusiness.RxLenth*0.26) + 63);
                        }
                        else return 0;
       
			tmp = MT_GetComma(9, buf);
			GPS->D.day     = (buf[tmp + 0] - '0') * 10 + (buf[tmp + 1] - '0'); //日期
			GPS->D.month   = (buf[tmp + 2] - '0') * 10 + (buf[tmp + 3] - '0');
			GPS->D.year    = (buf[tmp + 4] - '0') * 10 + (buf[tmp + 5] - '0')+2000;
                        
                        if((buf[tmp + 0]==',')&&(buf[tmp + 5]==',')) return 0;
                          
			MT_UTC2BTC(&GPS->D);
			
		}		
	}
	
	return 0;
}

//====================================================================//
// 语法格式: static int Str_To_Int(char *buf)
// 实现功能： 把一个字符串转化成整数
// 参    数：字符串
// 返 回 值：转化后整数值
//====================================================================//
/*static int MT_Str_To_Int(char *buf)
{
  
	int rev = 0;
	int dat;
	char *str = buf;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
		}

		rev = rev * 10 + dat;
		str ++;
	}

	return rev;
}
*/
//====================================================================//
// 语法格式: static int Get_Int_Number(char *s)
// 实现功能：把给定字符串第一个逗号之前的字符转化成整型
// 参    数：字符串
// 返 回 值：转化后整数值
//====================================================================//
//static int MT_Get_Int_Number(char *s)
//{
//	char buf[10];
//	uchar i;
//	int rev;
//	i=MT_GetComma(1, s);
//	i = i - 1;
//	strncpy(buf, s, i);
//	buf[i] = 0;
//	rev=MT_Str_To_Int(buf);
//	return rev;	
//}

//====================================================================//
// 语法格式: static float Str_To_Float(char *buf)
// 实现功能： 把一个字符串转化成浮点数
// 参    数：字符串
// 返 回 值：转化后单精度值
//====================================================================//
static float MT_Str_To_Float(char *buf)
{
	float rev = 0;
	float dat;
	int integer = 1;
	char *str = buf;
	int i;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str ++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str ++;
	}
	return rev;

}
												
//====================================================================//
// 语法格式: static float Get_Float_Number(char *s)
// 实现功能： 把给定字符串第一个逗号之前的字符转化成单精度型
// 参    数：字符串
// 返 回 值：转化后单精度值
//====================================================================//
static float MT_Get_Float_Number(char *s)
{
	char buf[10];
	uchar i;
	float rev;
	i=MT_GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev=MT_Str_To_Float(buf);
	return rev;	
}

//====================================================================//
// 语法格式: static double Str_To_Double(char *buf)
// 实现功能： 把一个字符串转化成浮点数
// 参    数：字符串
// 返 回 值：转化后双精度值
//====================================================================//
static double MT_Str_To_Double(char *buf)
{
	double rev = 0;
	double dat;
	int integer = 1;
	char *str = buf;
	int i;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str ++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str ++;
	}
	return rev;
}
												
//====================================================================//
// 语法格式: static double Get_Double_Number(char *s)
// 实现功能：把给定字符串第一个逗号之前的字符转化成双精度型
// 参    数：字符串
// 返 回 值：转化后双精度值
//====================================================================//
static double MT_Get_Double_Number(char *s)
{
	char buf[10];
	uchar i;
	double rev;
	i=MT_GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev=MT_Str_To_Double(buf);
	return rev;	
}

//====================================================================//
// 语法格式：static uchar GetComma(uchar num,char *str)
// 实现功能：计算字符串中各个逗号的位置
// 参    数：查找的逗号是第几个的个数，需要查找的字符串
// 返 回 值：0
//====================================================================//
static uchar MT_GetComma(uchar num,char *str)
{
	uchar i,j = 0;
	uchar len=strlen(str);
	for(i = 0;i < len;i ++)
	{
		if(str[i] == ',')
			j++;
		if(j == num)
			return i + 1;	
	}

	return 0;	
}

//====================================================================//
// 语法格式：void UTC2BTC(DATE_TIME *GPS)
// 实现功能：转化时间为北京时区的时间
// 参    数：存放时间的结构体
// 返 回 值：无
//====================================================================//
static void MT_UTC2BTC(DATE_TIME *GPS)
{
	GPS->second ++;  
	if(GPS->second > 59)
	{
		GPS->second = 0;
		GPS->minute ++;
		if(GPS->minute > 59)
		{
			GPS->minute = 0;
			GPS->hour ++;
		}
	}	

    GPS->hour = GPS->hour + 0;
	if(GPS->hour > 23)
	{
		GPS->hour -= 24;
		GPS->day += 1;
		if(GPS->month == 2 ||
		   		GPS->month == 4 ||
		   		GPS->month == 6 ||
		   		GPS->month == 9 ||
		   		GPS->month == 11 )
		{
			if(GPS->day > 30)
			{
		   		GPS->day = 1;
				GPS->month++;
			}
		}
		else
		{
			if(GPS->day > 31)
			{	
		   		GPS->day = 1;
				GPS->month ++;
			}
		}
		if(GPS->year % 4 == 0 )
		{
	   		if(GPS->day > 29 && GPS->month == 2)
			{		
	   			GPS->day = 1;
				GPS->month ++;
			}
		}
		else
		{
	   		if(GPS->day > 28 &&GPS->month == 2)
			{
	   			GPS->day = 1;
				GPS->month ++;
			}
		}
		if(GPS->month > 12)
		{
			GPS->month -= 12;
			GPS->year ++;
		}		
	}
}




