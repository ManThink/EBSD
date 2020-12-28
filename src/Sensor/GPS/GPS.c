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
static u1_t MT_GetComma(uchar num,char* str);
static double MT_Get_Double_Number(char *s);
static float MT_Get_Float_Number(char *s);
void MT_GPSInitial(void);

u1_t  GPS_GxRMC_Display_ONLY[38]={0x24,0x50,0x43,0x41,0x53,0x30,0x33,0x2C, 0x30,0x2C,0x30,0x2C,0x30,0x2C,0x30,0x2C, 0x31,0x2C,0x30,0x2C,0x30,0x2C,0x30,0x2C,0x30,0x2C,0x30,0x2C,0x2C,0x2C,0x30,0x2C,
0x30,0x2A,0x30,0x33,0x0D,0x0A};
u1_t  Save_Parameter[12]={0x24,0x50,0x43,0x41,0x53,0x30,0x30,0x2A,0x30,0x31,0x0D,0x0A};
/********************************************************
* Function for GPSInitial 
*
********************************************************/
void MT_GPSInitial(void)
{
     mpos_driver.UART2_SendBuffer(GPS_GxRMC_Display_ONLY,38);
     mpos_driver.kickdog();  
     mpos_osfun.DelayUs(10000);
     mpos_driver.UART2_SendBuffer(Save_Parameter,12);
     return;
}
/********************************************************
* Function for GpsConvertPositionIntoBinary 
* Convert the longitude and latitude of double type into character type
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
// function£ºParse GPRMC information of GPS module into recognizable data
// Para: original information structure,recognizable data structure
// return£º
//	     1: Finished parsing GPRMC
//           0: No parsing or invalid data
//====================================================================//
int MT_GPS_RMC_Parse(char *line,GPS_INFO *GPS)
{
  uchar ch, status, tmp;
  float lati_cent_tmp, lati_second_tmp;
  float long_cent_tmp, long_second_tmp;
  float speed_tmp;
  char *buf = line;
  ch = buf[5];
  status = buf[MT_GetComma(2, buf)];
	
  if (ch == 'C')  //if the fifth character is C£¬($GPRMC)
  {
      if(status == 'A') 
      {
          GPS -> NS      = buf[MT_GetComma(4, buf)];
          GPS -> EW      = buf[MT_GetComma(6, buf)];
                        
          GPS->latitude  = MT_Get_Double_Number(&buf[MT_GetComma(3, buf)]);
          GPS->longitude = MT_Get_Double_Number(&buf[MT_GetComma( 5, buf)]);

          GPS->latitude_Degree  = (int)GPS->latitude / 100;       //get latitude
          lati_cent_tmp         = (GPS->latitude - GPS->latitude_Degree * 100);
          GPS->latitude_Cent    = (int)lati_cent_tmp;
          lati_second_tmp       = (lati_cent_tmp - GPS->latitude_Cent) * 60;
          GPS->latitude_Second  = (int)lati_second_tmp;

          if(GPS->latitude_Degree!=0)  
          GPS->latitude = GPS->latitude_Degree + ((float)GPS->latitude_Cent)/60 + ((float)GPS->latitude_Second)/3600;
          else;
          if(GPS -> NS=='S') GPS->latitude = -(GPS->latitude);
                           
          GPS->longitude_Degree = (int)GPS->longitude / 100;	//get longitude
          long_cent_tmp         = (GPS->longitude - GPS->longitude_Degree * 100);
          GPS->longitude_Cent   = (int)long_cent_tmp;    
          long_second_tmp       = (long_cent_tmp - GPS->longitude_Cent) * 60;
          GPS->longitude_Second = (int)long_second_tmp;

          if(GPS->longitude_Degree!=0)                          
          GPS->longitude = GPS->longitude_Degree + ((float)GPS->longitude_Cent)/60 + ((float)GPS->longitude_Second)/3600;
          else;
          if(GPS -> EW=='W') GPS->longitude = -(GPS->longitude);
          MT_GpsConvertPositionIntoBinary();
          speed_tmp      = MT_Get_Float_Number(&buf[MT_GetComma(7, buf)]);    //speed(unit£ºnautical mile/hour)
          GPS->speed     = speed_tmp * 1.85;                           //1 nautical mile=1.85 kilometre
          GPS->direction = MT_Get_Float_Number(&buf[MT_GetComma(8, buf)]);//get direction 
          GPS->D.hour    = (buf[7] - '0') * 10 + (buf[8] - '0')+8;		//get time
          GPS->D.minute  = (buf[9] - '0') * 10 + (buf[10] - '0');
          GPS->D.second  = (buf[11] - '0') * 10 + (buf[12] - '0');
          tmp = MT_GetComma(9, buf);
          GPS->D.day     = (buf[tmp + 0] - '0') * 10 + (buf[tmp + 1] - '0'); 
          GPS->D.month   = (buf[tmp + 2] - '0') * 10 + (buf[tmp + 3] - '0');
          GPS->D.year    = (buf[tmp + 4] - '0') * 10 + (buf[tmp + 5] - '0')+2000;
          return 1;
      }               		
  }
	
	return 0;
}



//====================================================================//
// function£º Convert a string to a single precision 
// para£ºcharacter string
// return£ºSingle precision after conversion
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
// function£º Converts the character before the first comma of a given string to single precision
// para£ºcharacter string
// return£ºSingle precision after conversion
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
// function£º Convert a string to a double precision value
// para£ºcharacter string
// return£ºdouble precision value after conversion
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
// function£º Converts the character before the first comma of a given string to double precision
// para£ºcharacter string
// return£ºDouble precision after conversion 
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
// function: calculate the position of each comma in the string
// Parameter:The numbei of the comma, the string to be found
// return£ºThe position of the comma
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




