#ifndef _MY_TYPE_H
#define _MY_TYPE_H
#include "EBGlobal.h"
//#include "string.h"

typedef union {
  u2_t u16;               // element specifier for accessing whole u16
  s2_t i16;                // element specifier for accessing whole i16
  struct {
    u1_t u8L;              // element specifier for accessing low u8
    u1_t u8H;              // element specifier for accessing high u8
  }PACKED s16;                // element spec. for acc. struct with low or high u8
}PACKED nt16;

typedef struct t_MT_Sensor_para
    {
        u1_t    Command;		      
        u1_t    InforFormat; 
        u1_t    MType;
	union {
	u1_t   AlarmState;				
	struct {	
                       u1_t        Ds18b20Status :1;
                       u1_t        AccStatus:1;
                       u1_t        RFU1 :2;
                       u1_t        StaticStatus:1;
                       u1_t        RFU2 :1;
                       u1_t        RFU3 :2;
                }Bits;
         }AlarmStatus;   
         u1_t   Ds18b20Temp[2];     
         u1_t   Vol; 
         u1_t   RSSI;
         u1_t   SNR;
         u2_t   XVar;
         u2_t   YVar;
         u2_t   ZVar;
         
         s2_t   Buff_X[3];
         s2_t   Buff_Y[3];
         s2_t   Buff_Z[3];
         u1_t   Buff_len;
         
    }PACKED T_MT_Sensor_PARA; 

typedef union U_MT_Sensor_PARA
{
  T_MT_Sensor_PARA MPRegister;
  unsigned char Bytes[36];
} PACKED U_MT_SensorPARA;

typedef struct u_senspropertypara
{
        u1_t MType;
	u1_t Hwversion;
	u1_t Fmversion; 
        u1_t UpInterval[2];
	u1_t Resv[6];
}PACKED MT_Sensor_InfoPara;

extern U_MT_SensorPARA MT_Sensor;

#endif