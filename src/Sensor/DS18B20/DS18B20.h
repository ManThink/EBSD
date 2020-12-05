#ifndef __DS18B20__H
#define __DS18B20__H
#include "EBGlobal.h"
/*********************************************************************************************************
** define
*********************************************************************************************************/
#define PIN_DS18B20_IO         2


#define MT_W_DS_OUT()        mpos_driver.GPIO_Config(PIN_DS18B20_IO, GPIO_Mode_OOD)
#define MT_W_DS_IN()         mpos_driver.GPIO_Config(PIN_DS18B20_IO, GPIO_Mode_IFT)
#define MT_W_DS_H()          mpos_driver.GPIO_PinSet(PIN_DS18B20_IO, SET)
#define MT_W_DS_L()          mpos_driver.GPIO_PinSet(PIN_DS18B20_IO, RESET)

/*********************************************************************************************************
** function
*********************************************************************************************************/
extern u1_t MT_R_DS_IN(void);
extern bool MT_DS18B20_Iint(void);
extern void MT_DS_WriteByte(u1_t dat);
extern u1_t MT_DS_ReadByte(void);
extern bool MT_DS18B20_ReadTemp(u1_t *temp);
#endif
























