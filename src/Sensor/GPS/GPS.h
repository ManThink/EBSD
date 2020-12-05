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
*   Configurable information for the RTCS examples.
*
*
*END************************************************************************/
#include <stdint.h>
#include "EBGlobal.h"
#ifndef         _GPS_H
#define 	_GPS_H

#define uchar unsigned char
#define uint  unsigned int
/* --- PRIVATE CONSTANTS ---------------------------------------------------- */

#define		TS_CPS				1E6 /* count-per-second of the timestamp counter */
#define		PLUS_10PPM			1.00001
#define		MINUS_10PPM			0.99999
#define		DEFAULT_BAUDRATE	B9600

#define LGW_GPS_SUCCESS	 0
#define LGW_GPS_ERROR	-1
/* -------------------------------------------------------------------------- */

typedef struct date_struct
{
   /*! \brief Range from 1970 to 2099. */
   int16_t YEAR;

   /*! \brief Range from 1 to 12. */
   int16_t MONTH;

   /*! \brief Range from 1 to 31 (depending on month). */
   int16_t DAY;

   /*! \brief Range from 0 to 23. */
   int16_t HOUR;

   /*! \brief Range from 0 to 59. */
   int16_t MINUTE;

   /*! \brief Range from 0 to 59. */
   int16_t SECOND;

   /*! \brief Range from 0 to 999. */
   int16_t MILLISEC;
   
   /*! \brief Day of the week. Sunday is day 0. Range is from 0 to 6. */
   int16_t WDAY;
   
   /*! \brief Day of the year. Range is from 0 to 365. */
   int16_t YDAY;

} DATE_STRUCT, * DATE_STRUCT_PTR;
typedef struct{
	int year;  
	int month; 
	int  day;
	int hour;
	int minute;
	int second;
}DATE_TIME;

typedef  struct{
	double  latitude;  //����
	double  longitude; //γ��
	int32_t LongitudeBinary;
        int32_t LatitudeBinary;
	int     latitude_Degree;	//��
	int		latitude_Cent;		//��
	int   	latitude_Second;    //��
	int     longitude_Degree;	//��
	int		longitude_Cent;		//��
	int   	longitude_Second;   //��
	float 	speed;      //�ٶ�
	float 	direction;  //����
	float 	height_ground;    //ˮƽ��߶�
	float 	height_sea;       //���θ߶�
	int     satellite;
	uchar 	NS;
	uchar 	EW;
	DATE_TIME D;
        int    delay;
}GPS_INFO;

typedef struct t_GPSFramePara
{
    unsigned char  rev_start;     //���տ�ʼ��־
    unsigned char   rev_stop ;     //����ֹͣ��־
    unsigned char   gps_flag;      //GPS�����־
    unsigned char   change_page;   //��ҳ��ʾ��־
    unsigned char error_num ;
    GPS_INFO   GPS;
} TGPSFRAMEPARA;

typedef union t_SendTimes
{
   unsigned char byte[4];
   unsigned long dword;
}TSENDTIMES;

/*@brief Time solution required for timestamp to absolute time conversion*/
typedef struct time_struct
{

   /*! \brief The number of seconds in the time. */
   uint32_t     SECONDS;

   /*! \brief The number of milliseconds in the time. */
   uint32_t     MILLISECONDS;

} TIME_STRUCT, * TIME_STRUCT_PTR;
struct tref {
	uint32_t        systime; 	/*!> system time when solution was calculated */
	uint32_t	count_us; 	/*!> reference concentrator internal timestamp */
	TIME_STRUCT     utc; 	/*!> reference UTC time (from GPS) */
	double		xtal_err;	/*!> raw clock error (eg. <1 'slow' XTAL) */
};
typedef uint32_t    time_t;

extern int MT_GPS_RMC_Parse(char *line,GPS_INFO *GPS);
extern void MT_Int_To_Str(int x,char *Str);
extern void MT_GPSInitial(void);
extern GPS_INFO        MT_GPS_Information;
#endif


/* EOF */
