#include "DS18B20.h"
#include "EBGlobal.h"

/********************************************************
* Function for Read 1-wire data 
* input£º none
* output: none
*
********************************************************/
u1_t MT_R_DS_IN(void)
{
  return mpos_driver.GPIO_PinGet(PIN_DS18B20_IO);
}
/********************************************************
* Function for DS18B20_Iint 
* input£º none
* output: none
*
********************************************************/
bool MT_DS18B20_Iint(void)
{
  bool status; 
  MT_W_DS_OUT();
  MT_W_DS_H();
  asm("NOP");
  MT_W_DS_L();
  mpos_osfun.DelayUs(1000); //pull down bus, 
  MT_W_DS_IN(); //release bus 
  mpos_osfun.DelayUs(100); //delay time to read
  status = (bool)MT_R_DS_IN(); 
  mpos_osfun.DelayUs(142); //141.95us 
  //DS = 1; 
  asm("NOP");
  return status;
}
/********************************************************
* Function for DS_WriteByte 
* input£º none
* output: none
*
********************************************************/
void MT_DS_WriteByte(u1_t dat)  
{ 
    u1_t i; 
    for(i=0;i<8;i++) 
    { 
        MT_W_DS_OUT();
        MT_W_DS_H(); 
        mpos_osfun.DelayUs(2); //2us
        MT_W_DS_L(); 
        mpos_osfun.DelayUs(3); //15us
        if((dat & 0x01)==0x01) MT_W_DS_IN();
        else MT_W_DS_L();
        mpos_osfun.DelayUs(35); //60us 
        //mpos_osfun.DelayUs(20); //60us
        MT_W_DS_IN();
        mpos_osfun.DelayUs(2); //60us
        dat >>= 1; 
    } 

}

//void DS_WriteByte(u1_t dat)  
//{ 
//    u1_t i; 
//    for(i=0;i<8;i++) 
//    { 
//        W_DS_OUT();
//        asm("NOP");
//        W_DS_L(); 
//        mpos_osfun.DelayUs(1);//generate the write sequence
//        if((dat & 0x01)==0x01) W_DS_H();
//        else W_DS_L();
//        mpos_osfun.DelayUs(77); //76.95us 
//        W_DS_IN(); //relase the bus to ready to the next write 
//        asm("NOP"); 
//        dat >>= 1; 
//    } 
//}
/********************************************************
* Function for DS_ReadByte 
* input£º none
* output: none
*
********************************************************/
u1_t MT_DS_ReadByte(void) 
{ 
    u1_t dat,i,j; 
    for(i=0;i<8;i++) 
    { 
        MT_W_DS_OUT();
        MT_W_DS_H(); 
        mpos_osfun.DelayUs(5); //15us
        MT_W_DS_L(); 
        asm("NOP");
        MT_W_DS_IN();
        //asm("NOP");//release the bus 
        j = MT_R_DS_IN();
        mpos_osfun.DelayUs(20); //76.95us 
        asm("NOP"); 
        dat = (j<<7)|(dat>>1); //data from lsb to the msb
    } 
    return dat; 
}

//u1_t DS_ReadByte(void) 
//{ 
//    u1_t dat,i,j; 
//    for(i=0;i<8;i++) 
//    { 
//        W_DS_OUT();
//        asm("NOP");
//        W_DS_L(); 
//        asm("NOP"); //generate the write sequence 
//        W_DS_IN();
//        asm("NOP");//release the bus 
//        j = R_DS_IN();
//        mpos_osfun.DelayUs(77); //76.95us 
//        asm("NOP"); 
//        dat = (j<<7)|(dat>>1); //data from lsb to the msb
//    } 
//    return dat; 
//}
/********************************************************
* Function for DS18B20_ReadTemp 
* input£º none
* output: none
*
********************************************************/
bool MT_DS18B20_ReadTemp(u1_t *temp)
{
    u1_t lsb,msb;
    u2_t calc;
    
    mpos_osfun.disableInterrupts();
    if(MT_DS18B20_Iint())return false; //initial the DS18B20 
    MT_DS_WriteByte(0xcc); //ignore the ROM instruction
    MT_DS_WriteByte(0x44); //the temp convert instruction 
    mpos_osfun.enableInterrupts();
    mpos_driver.kickdog(); 
    mpos_osfun.DelayUs(1000000);
    mpos_driver.kickdog();
    mpos_osfun.disableInterrupts();
    if(MT_DS18B20_Iint())return false; //initial the DS18B20  
    MT_DS_WriteByte(0xcc); //ignore the ROM instruction 
    MT_DS_WriteByte(0xbe); //read the temp register£» 
    mpos_osfun.DelayUs(1000000);
    mpos_driver.kickdog();
    lsb  = MT_DS_ReadByte(); //the low 8 bit 
    msb  = MT_DS_ReadByte(); //the higy 8 bit 
    mpos_osfun.enableInterrupts();
    //*temp = (s1_t)((((s2_t)msb << 8) + lsb)*0.0625); 
    calc = (u2_t)((((float)((msb << 8) + lsb))*0.0625)*10+1000); 
    *temp = calc;
    *temp++;
    *temp = calc>>8;
    return true;
}

/*********************************************************************************************************
  END FILE 
*********************************************************************************************************/ 








