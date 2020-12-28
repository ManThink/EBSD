#include <stdint.h>
#include "EBGlobal.h"
#ifndef         _GPS_H
#define 	_GPS_H

#define uchar unsigned char
#define uint  unsigned int

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
	double  latitude;  
	double  longitude; 
	int32_t LongitudeBinary;
        int32_t LatitudeBinary;
	int     latitude_Degree;	
	int		latitude_Cent;		
	int   	latitude_Second;    
	int     longitude_Degree;	
	int		longitude_Cent;		
	int   	longitude_Second;   
	float 	speed;      
	float 	direction;  
	float 	height_ground;    
	float 	height_sea;       
	int     satellite;
	uchar 	NS;
	uchar 	EW;
	DATE_TIME D;
        int    delay1;
        int    delay2;
}GPS_INFO;




extern int MT_GPS_RMC_Parse(char *line,GPS_INFO *GPS);
extern void MT_Int_To_Str(int x,char *Str);
extern void MT_GPSInitial(void);
extern GPS_INFO        MT_GPS_Information;
#endif


/* EOF */
