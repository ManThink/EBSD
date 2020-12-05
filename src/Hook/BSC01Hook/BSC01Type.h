#ifndef _MY_TYPE_H
#define _MY_TYPE_H
#include "EBGlobal.h"
#define BSC01_UPPORT 23
typedef struct s_MT_UpData
{
	u1_t Header;
	u1_t Version;
	u1_t DevId;
	u1_t AcqTimeout;
	u4_t AcqTime; //UTC seconds of acq task excuted.
	u1_t SensorData[36];
	u1_t Resv[3];
	u1_t Tail;
}PACKED SMT_UpData; 

#endif