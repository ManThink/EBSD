#ifndef _EB_I2C_H
#define _EB_I2C_H
#include "EBGlobal.h"
/*********************************************************************************************************
** global
*********************************************************************************************************/
#define MT_SDA_DDR_OUT()       do {mpos_driver.GPIO_Config(PIN_I2C_SDA,GPIO_Mode_OPP);}while(0)
#define MT_SDA_DDR_IN()        do {mpos_driver.GPIO_Config(PIN_I2C_SDA,GPIO_Mode_IFT);}while(0)
#define MT_SCL_DDR_OUT()       do {mpos_driver.GPIO_Config(PIN_I2C_SCL,GPIO_Mode_OPP);}while(0)
#define MT_SCL_DDR_IN()        do {mpos_driver.GPIO_Config(PIN_I2C_SCL,GPIO_Mode_IFT);}while(0)
#define MT_SDA_H()             do {mpos_driver.GPIO_PinSet(PIN_I2C_SDA, SET);}while(0)
#define MT_SDA_L()             do {mpos_driver.GPIO_PinSet(PIN_I2C_SDA, RESET);}while(0)
#define MT_SCL_H()             do {mpos_driver.GPIO_PinSet(PIN_I2C_SCL, SET);}while(0)
#define MT_SCL_L()             do {mpos_driver.GPIO_PinSet(PIN_I2C_SCL, RESET);}while(0)
#define MT_I2C_DELAY()         mpos_osfun.DelayUs(6);
/*********************************************************************************************************
 define
*********************************************************************************************************/
#define PIN_I2C_SDA         15
#define PIN_I2C_SCL         14

//#define MKL_I2C0            0
//#define MKL_I2C1            1
    
#define I2C_MASTER          0
#define I2C_SLAVER          1
  
#define I2C_ADDRESS_SHORT   0                                               /* 7-bit slave address mode          */
#define I2C_ADDRESS_LONG    1                                               /* 10-bit slave address mode         */
    
#define NONE_SUBA           0                                               /* No subaddress mode           */
#define ONE_BYTE_SUBA       1                                               /* 1-byte subaddress mode       */
#define TWO_BYTE_SUBA       2                                               /* 2-byte subaddress mode        */
  
#define I2C1_MODE                   0 

#define I2C1_SLAVER_ADDRESS_MODE    0

#define I2C1_BAUD_RATE              100000

#define I2C1_SLAVER_ADR             0xA0  
#define I2C1_RANGE_SLAVER_ADR       0xAB
#define I2C1_SLAVER_ADR_MATCH       0x0  

/* Formula for calculating baud rate BT = BusCLK/(1 << MUL)/SCL_DIVIDE */  
#define I2C1_MUL             1             /* Frequency division setting          */
#define I2C1_BT_ERROR        50            /* Maximum error of I2C baud rate  */  

#define DEFAULT_BUS_CLOCK    24000000u     /* Default bus clock value */  

typedef struct t_i2c_para
    {
        volatile u1_t     sla;              /* I2C device slave address              */
        volatile u4_t     suba;             /* Internal subaddress of I2C device            */
        volatile u1_t     suba_num;         /* I2C subaddress length             */
        volatile u1_t     *buf;             /* Data buffer pointer         */
        volatile u4_t     num;              /* Number of data to read and write     */
        volatile u1_t     end;              /* I2C bus end flag            */
        volatile u1_t     restart;          /* I2C bus repeat flag            */
        
        volatile u1_t     irq_status;       /* I2C interrupt read / write operation control      */                                                                  
        volatile u1_t     sla_dir;          /* Slave control 1 -- read operation 0 -- write operation*/
    }PACKED T_I2C_PARA;

extern void MT_I2C_Init(void);
extern bool MT_I2C_Start(void);
extern void MT_I2C_Stop(void);
extern void MT_I2C_SendByte(u1_t data);
extern u1_t MT_I2C_GetByte(void);
extern u1_t MT_I2C1_SendByte(u1_t sla, u1_t *dat);
extern int MT_I2C_WriteBuffer(u1_t chipAddr, u1_t addr, u1_t *buf, u2_t len);
extern int MT_I2C_ReadBuffer(u1_t chipAddr, u1_t addr, u1_t *buf, u2_t len);
extern void delay_us(u2_t nCount);
extern u1_t MT_SCL_IN(void);
extern void MT_I2C_Init(void);
extern void MT_I2C_Ack(void);
extern void MT_I2C_NAck(void);
extern bool MT_I2C_WaitAck(void);
#endif
























