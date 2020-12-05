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
  
#define I2C_ADDRESS_SHORT   0                                               /* 7λ�ӵ�ַģʽ          */
#define I2C_ADDRESS_LONG    1                                               /* 10λ�ӵ�ַģʽ         */
    
#define NONE_SUBA           0                                               /* ���ӵ�ַģʽ           */
#define ONE_BYTE_SUBA       1                                               /* ���ֽ��ӵ�ַģʽ       */
#define TWO_BYTE_SUBA       2                                               /* ˫�ֽ��ӵ�ַģʽ       */
  
#define I2C1_MODE                   0 

#define I2C1_SLAVER_ADDRESS_MODE    0

#define I2C1_BAUD_RATE              100000

#define I2C1_SLAVER_ADR             0xA0  
#define I2C1_RANGE_SLAVER_ADR       0xAB
#define I2C1_SLAVER_ADR_MATCH       0x0  

/* ���㲨���ʹ�ʽ BT = BusCLK/(1 << MUL)/SCL_DIVIDE */  
#define I2C1_MUL             1             /* ��Ƶ����           */
#define I2C1_BT_ERROR        50            /* I2C������������  */  

#define DEFAULT_BUS_CLOCK    24000000u     /* Default bus clock value */  

typedef struct t_i2c_para
    {
        volatile u1_t     sla;              /* I2C�����ӵ�ַ              */
        volatile u4_t     suba;             /* I2C�ڼ����ӵ�ַ            */
        volatile u1_t     suba_num;         /* I2C�ӵ�ַ����              */
        volatile u1_t     *buf;             /* ���ݻ�����ָ��         */
        volatile u4_t     num;              /* Ҫ��ȡ��д������ݸ���     */
        volatile u1_t     end;              /* I2C���߽�����־            */
        volatile u1_t     restart;          /* I2C�����ظ���־            */
        
        volatile u1_t     irq_status;       /* I2C�ж϶�д��������        */                                                                      /* 0--дģʽ                  */
        volatile u1_t     sla_dir;          /* �ӻ����� 1--��ȡ����  0--д����*/
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
























