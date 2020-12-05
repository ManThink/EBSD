#include "EB_I2C.h"
#include "EBGlobal.h"

/********************************************************
* Function for SDA_IN 
* input£º none
* output: none
*
********************************************************/
u1_t MT_SDA_IN(void)
{
    return mpos_driver.GPIO_PinGet(PIN_I2C_SDA);
}
/********************************************************
* Function for SDA_IN 
* input£º none
* output: none
*
********************************************************/
u1_t MT_SCL_IN(void)
{
    return mpos_driver.GPIO_PinGet(PIN_I2C_SCL);
}
/********************************************************
* Function for I2C1 Initial 
* input£º none
* output: none
*
********************************************************/
void MT_I2C_Init(void)
{
    mpos_driver.GPIO_Config(PIN_I2C_SCL,GPIO_Mode_OPP);
    mpos_driver.GPIO_PinSet(PIN_I2C_SCL, SET) ;
    
    mpos_driver.GPIO_Config(PIN_I2C_SDA,GPIO_Mode_OPP);
    mpos_driver.GPIO_PinSet(PIN_I2C_SDA, SET) ;
}
/********************************************************
* Function for I2C_Start 
* input£º none
* output: none
*
********************************************************/
bool MT_I2C_Start(void)
{
    MT_SDA_H();
    MT_SCL_H();
    MT_I2C_DELAY();
    MT_SDA_L();
    MT_I2C_DELAY();
    MT_SCL_L();
    return true;
}
/********************************************************
* Function for I2C_Stop 
* input£º none
* output: none
*
********************************************************/
void MT_I2C_Stop(void)
{
    MT_SCL_L();
    MT_SDA_L();
    MT_I2C_DELAY();
    MT_SCL_H();
    MT_SDA_H();
    MT_I2C_DELAY();
}
/********************************************************
* Function for I2C_Ack 
* input£º none
* output: none
*
********************************************************/
void MT_I2C_Ack(void)
{
    MT_SCL_L();
    MT_I2C_DELAY();
    MT_SDA_L();
    MT_I2C_DELAY();
    MT_SCL_H();
    MT_I2C_DELAY();
    MT_SCL_L();
    MT_I2C_DELAY();
}
/********************************************************
* Function for I2C_NAck 
* input£º none
* output: none
*
********************************************************/
void MT_I2C_NAck(void)
{
    MT_SCL_L();
    MT_I2C_DELAY();
    MT_SDA_H();
    MT_I2C_DELAY();
    MT_SCL_H();
    MT_I2C_DELAY();
    MT_SCL_L();
    MT_I2C_DELAY();
}
/********************************************************
* Function for I2C_WaitAck 
* input£º none
* output: none
*
********************************************************/
bool MT_I2C_WaitAck(void)
{
    u1_t err_time=0;
    MT_SDA_DDR_IN();
    //SDA_H();
    MT_I2C_DELAY();
    MT_SCL_H();
    MT_I2C_DELAY();
    while(MT_SDA_IN())
    {
        err_time++;
        if(err_time>200)
        {
            MT_I2C_Stop();
            return true;
        }
    }
    MT_SCL_L();
    MT_SDA_DDR_OUT();
    return false;
}
/********************************************************
* Function for I2C_SendByte 
* input£º none
* output: none
*
********************************************************/
void MT_I2C_SendByte(u1_t data)
{
    volatile u1_t i;
    
    i = 8;
    while(i--)
    {
        if(data & 0x80) MT_SDA_H();
        else MT_SDA_L();
        data <<= 1;
        MT_I2C_DELAY();
        MT_SCL_H();
        MT_I2C_DELAY();
        MT_SCL_L();
        MT_I2C_DELAY();
    }

}
/********************************************************
* Function for I2C_GetByte 
* input£º none
* output: none
*
********************************************************/
u1_t MT_I2C_GetByte(void)
{
    u1_t i,byte;
    
    i = 8;
    byte = 0;

    MT_SDA_DDR_IN();
    MT_I2C_DELAY();
    while(i--)
    {
        MT_SCL_L();
        MT_I2C_DELAY();
        MT_SCL_H();
        MT_I2C_DELAY();
        byte = (byte<<1)|(MT_SDA_IN() & 1);
    }
    MT_SCL_L();
    MT_SDA_DDR_OUT();
    MT_I2C_DELAY();
    return byte;
}
/********************************************************
* Function for I2C_GetByte 
* input£º none
* output: none
*
********************************************************/
int MT_I2C_WriteBuffer(u1_t chipAddr, u1_t addr, u1_t *buf, u2_t len)
{
    u1_t err;

    err = 0;
    //chipAddr <<= 1;
    
    MT_I2C_Start();
    MT_I2C_SendByte(chipAddr);
    err += MT_I2C_WaitAck();

    MT_I2C_SendByte(addr);
    err += MT_I2C_WaitAck();

    while(len--)
    {
        MT_I2C_SendByte(*buf++);
        err += MT_I2C_WaitAck();  
    }

    MT_I2C_Stop();
    return err;
}
/********************************************************
* Function for I2C_WriteSingleRegister 
* input£º none
* output: none
*
********************************************************/
int MT_I2C_WriteSingleRegister(u1_t chipAddr, u1_t addr, u1_t data)
{
    return MT_I2C_WriteBuffer(chipAddr, addr, &data, 1);
}

/********************************************************
* Function for I2C_ReadBuffer 
* input£º none
* output: none
*
********************************************************/
int MT_I2C_ReadBuffer(u1_t chipAddr, u1_t addr, u1_t *buf, u2_t len)
{

    u1_t err;
 
    err = 0;
    
    MT_I2C_Start();
    MT_I2C_SendByte(chipAddr);
    err += MT_I2C_WaitAck();
    
    if(len>1) addr|=0x80;
    MT_I2C_SendByte(addr);
    err += MT_I2C_WaitAck();

    
    MT_I2C_Start();
    MT_I2C_SendByte(chipAddr+1);
    err += MT_I2C_WaitAck();
    
    while(len--)
    {
        *buf++ = MT_I2C_GetByte();
        if(len)
        {
            MT_I2C_Ack();
        }
    }
    
    MT_I2C_NAck();
    MT_I2C_Stop();
    
    return err;
}
/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/ 








