#ifndef __LIS2DH12__H
#define __LIS2DH12__H
#include "EBGlobal.h"
#include "EB_I2C.h"
#include "ST11TAType.h"

#define ARC_PARA            0.866 // cos30бу
#define BUFF_LEN              3

extern int MT_I2C_ReadBuffer(u1_t chipAddr, u1_t addr, u1_t *buf, u2_t len);
extern int MT_I2C_WriteBuffer(u1_t chipAddr, u1_t addr, u1_t *buf, u2_t len);
extern u1_t MT_LIS2DH12_Init(void);
extern u1_t MT_LIS2DH12_ReadAcc(void);
extern void MT_LIS2DH12_ReadStatus(void);
extern void MT_LIS2DH12_AngleCheck(void); 
extern void MT_LIS2DH12_LowPower(void);
extern u2_t MT_Var_Get(s2_t *buff,u1_t len);
extern s2_t MT_AVG_Get(s2_t *buff,u1_t len);
#endif