#include "EB_I2C.h"
#include "SHT20.h"
#include "EBGlobal.h"

const u2_t POLYNOMIAL_SHT20 = 0x131;  //P(x)=x^8+x^5+x^4+1 = 100110001
//==============================================================================
u1_t MT_SHT2x_CheckCrc(u1_t data[], u1_t nbrOfBytes, u1_t checksum)
//==============================================================================
{
  u1_t crc = 0;	
  u1_t byteCtr;
  //calculates 8-Bit checksum with given polynomial
  for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
  { crc ^= (data[byteCtr]);
    for (u1_t bit = 8; bit > 0; --bit)
    { if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL_SHT20;
      else crc = (crc << 1);
    }
  }
  if (crc != checksum) return CHECKSUM_ERROR;
  else return 0;
}

//===========================================================================
u1_t MT_SHT2x_ReadUserRegister(u1_t *pRegisterValue)
//===========================================================================
{
  u1_t checksum;   //variable for checksum byte
  u1_t error=0;    //variable for error code

  MT_I2C_Start();
  MT_I2C_SendByte (I2C_ADR_W);
  error += MT_I2C_WaitAck();
  MT_I2C_SendByte (USER_REG_R);
  error += MT_I2C_WaitAck();
  
  MT_I2C_Start();
  MT_I2C_SendByte (I2C_ADR_R);
  error += MT_I2C_WaitAck();
  *pRegisterValue = MT_I2C_GetByte();MT_I2C_Ack();
  
  checksum=MT_I2C_GetByte();MT_I2C_NAck();
  error |= MT_SHT2x_CheckCrc (pRegisterValue,1,checksum);
  MT_I2C_Stop();
  return error;
}

//===========================================================================
u1_t MT_SHT2x_WriteUserRegister(u1_t *pRegisterValue)
//===========================================================================
{
  u1_t error=0;   //variable for error code
  
  MT_I2C_Start();
  MT_I2C_SendByte (I2C_ADR_W);
  error += MT_I2C_WaitAck();
  MT_I2C_SendByte (USER_REG_W);
  error += MT_I2C_WaitAck();
  MT_I2C_SendByte (*pRegisterValue);
  error += MT_I2C_WaitAck();
  MT_I2C_Stop();
  return error;
}

//===========================================================================
u1_t MT_SHT2x_MeasureHM(etSHT2xMeasureType eSHT2xMeasureType, nt16 *pMeasurand)
//===========================================================================
{
  u1_t  checksum;   //checksum
  u1_t  data[2];    //data array for checksum verification
  u1_t  error=0;    //error variable


  //-- write I2C Sensor address and command --
  MT_I2C_Start();
  MT_I2C_SendByte (I2C_ADR_W); // I2C Adr
  error += MT_I2C_WaitAck();
  switch(eSHT2xMeasureType)
  { case HUMIDITY: MT_I2C_SendByte (TRIG_RH_MEASUREMENT_HM);error += MT_I2C_WaitAck(); break;
    case TEMP    : MT_I2C_SendByte (TRIG_T_MEASUREMENT_HM); error += MT_I2C_WaitAck(); break;
    default: ;//assert(0);
  }
  //-- wait until hold master is released --
  MT_I2C_Start();
  MT_I2C_SendByte (I2C_ADR_R);
  error += MT_I2C_WaitAck();
  //SCL_DDR_IN();
  mpos_osfun.DelayUs(150000);
  //SCL_DDR_OUT();
  pMeasurand->s16.u8H = data[0] = MT_I2C_GetByte();MT_I2C_Ack();
  pMeasurand->s16.u8L = data[1] = MT_I2C_GetByte();MT_I2C_Ack();
  checksum=MT_I2C_GetByte();MT_I2C_NAck();
  //-- verify checksum --
  error |= MT_SHT2x_CheckCrc (data,2,checksum);
  MT_I2C_Stop();
  return error;
}
//===========================================================================
u1_t MT_SHT2x_MeasurePoll(etSHT2xMeasureType eSHT2xMeasureType, nt16 *pMeasurand)
//===========================================================================
{
  u1_t  checksum;   //checksum
  u1_t  data[2];    //data array for checksum verification
  u1_t  error=0;    //error variable

  //-- write I2C Sensor address and command --
  MT_I2C_Start();
  MT_I2C_SendByte (I2C_ADR_W); // I2C Adr
  error += MT_I2C_WaitAck();
  switch(eSHT2xMeasureType)
  { case HUMIDITY: MT_I2C_SendByte (TRIG_RH_MEASUREMENT_POLL);error += MT_I2C_WaitAck(); break;
    case TEMP    : MT_I2C_SendByte (TRIG_T_MEASUREMENT_POLL); error += MT_I2C_WaitAck(); break;
    default: ;//assert(0);
  }
  mpos_osfun.DelayUs(30);
  MT_I2C_Stop();
  mpos_osfun.DelayUs(150000);
  //-- poll every 10ms for measurement ready. Timeout after 20 retries (200ms)--
  MT_I2C_Start();
  MT_I2C_SendByte(I2C_ADR_R);
  error = MT_I2C_WaitAck();
  //-- read two data bytes and one checksum byte --
  pMeasurand->s16.u8H = data[0] = MT_I2C_GetByte();MT_I2C_Ack();
  pMeasurand->s16.u8L = data[1] = MT_I2C_GetByte();MT_I2C_Ack();
  checksum=MT_I2C_GetByte();MT_I2C_NAck();

  //-- verify checksum --
  error |= MT_SHT2x_CheckCrc (data,2,checksum);
  MT_I2C_Stop();
  return error;
}

//===========================================================================
u1_t MT_SHT2x_SoftReset()
//===========================================================================
{
  u1_t  error=0;           //error variable

  MT_I2C_Start();
  MT_I2C_SendByte (I2C_ADR_W); // I2C Adr
  error += MT_I2C_WaitAck();
  MT_I2C_SendByte (SOFT_RESET);          
  error += MT_I2C_WaitAck();// Command
  MT_I2C_Stop();

  mpos_driver.kickdog();   mpos_osfun.DelayUs(300000);  // wait till Sensor has restarted
  return error;
}

//==============================================================================
float MT_SHT2x_CalcRH(u2_t u16sRH)
//==============================================================================
{
  float humidityRH;              // variable for result

  u16sRH &= ~0x0003;          // clear bits [1..0] (status bits)
  //-- calculate relative humidity [%RH] --

  humidityRH = -6.0 + 125.0/65536 * (float)u16sRH; // RH= -6 + 125 * SRH/2^16
  return humidityRH;
}

//==============================================================================
float MT_SHT2x_CalcTemperatureC(u2_t u16sT)
//==============================================================================
{
  float temperatureC;            // variable for result

  u16sT &= ~0x0003;           // clear bits [1..0] (status bits)
  //u16sT = (u16sT>>2);
  //-- calculate temperature [?C] --
  temperatureC= -46.85 + 175.72/65536 *(float)u16sT; //T= -46.85 + 175.72 * ST/2^16
  return temperatureC;
}

//==============================================================================
u1_t MT_SHT2x_GetSerialNumber(u1_t u8SerialNumber[])
//==============================================================================
{
  u1_t  error=0;                          //error variable

  //Read from memory location 1
  MT_I2C_Start();
  MT_I2C_SendByte (I2C_ADR_W);    //I2C address
  error += MT_I2C_WaitAck();
  MT_I2C_SendByte (0xFA);         //Command for readout on-chip memory
  error += MT_I2C_WaitAck();
  MT_I2C_SendByte (0x0F);         //on-chip memory address
  error += MT_I2C_WaitAck();

  MT_I2C_Start();
  MT_I2C_SendByte (I2C_ADR_R);    //I2C address
  error += MT_I2C_WaitAck();

  u8SerialNumber[5] = MT_I2C_GetByte();  MT_I2C_Ack(); //Read SNB_3
  MT_I2C_GetByte();  MT_I2C_Ack();        //Read CRC SNB_3 (CRC is not analyzed)
  u8SerialNumber[4] = MT_I2C_GetByte(); MT_I2C_Ack(); //Read SNB_2  
  MT_I2C_GetByte();  MT_I2C_Ack();                    //Read CRC SNB_2 (CRC is not analyzed)  
  u8SerialNumber[3] = MT_I2C_GetByte();MT_I2C_Ack();  //Read SNB_1
  MT_I2C_GetByte();   MT_I2C_Ack();                      //Read CRC SNB_1 (CRC is not analyzed)
  u8SerialNumber[2] = MT_I2C_GetByte(); MT_I2C_Ack(); //Read SNB_0
  MT_I2C_GetByte();  MT_I2C_NAck();                //Read CRC SNB_0 (CRC is not analyzed)
  MT_I2C_Stop();

  //Read from memory location 2
  MT_I2C_Start();
  MT_I2C_SendByte (I2C_ADR_W);    //I2C address
  error += MT_I2C_WaitAck();
  MT_I2C_SendByte (0xFC);         //Command for readout on-chip memory
  error += MT_I2C_WaitAck();
  MT_I2C_SendByte (0xC9);         //on-chip memory address
  error += MT_I2C_WaitAck();
  
  MT_I2C_Start();
  MT_I2C_SendByte (I2C_ADR_R);    //I2C address
  error += MT_I2C_WaitAck();
  u8SerialNumber[1] = MT_I2C_GetByte(); MT_I2C_Ack();//Read SNC_1
  u8SerialNumber[0] = MT_I2C_GetByte(); MT_I2C_Ack();//Read SNC_0
  MT_I2C_GetByte();  MT_I2C_Ack();                   //Read CRC SNC0/1 (CRC is not analyzed)
  u8SerialNumber[7] = MT_I2C_GetByte();MT_I2C_Ack(); //Read SNA_1
  u8SerialNumber[6] = MT_I2C_GetByte();MT_I2C_Ack(); //Read SNA_0
  MT_I2C_GetByte();  MT_I2C_NAck();               //Read CRC SNA0/1 (CRC is not analyzed)
  MT_I2C_Stop();
  return error;
}
//==============================================================================
bool MT_SHT2x_MeasureTH(u1_t buff[])
//==============================================================================
{
    nt16 sRH;
    nt16 sT; 
    u1_t error=0;
    u2_t temperatureC;
    u2_t humidityRH;
    error += MT_SHT2x_MeasurePoll(TEMP, &sT);
    error += MT_SHT2x_MeasurePoll(HUMIDITY, &sRH);
    if(error!=0) 
    {
      error=0;
      MT_SHT2x_SoftReset();
      error += MT_SHT2x_MeasurePoll(TEMP, &sT);
      error += MT_SHT2x_MeasurePoll(HUMIDITY, &sRH);
      if(error!=0) return false;
    }
    temperatureC = (u2_t)(MT_SHT2x_CalcTemperatureC(sT.u16)*10)+1000;  //temperature = test*10+1000
    humidityRH = (u2_t)(MT_SHT2x_CalcRH(sRH.u16)*10);           //humidity = test*10
    buff[0] = temperatureC;
    buff[1] = (temperatureC>>8);
    buff[2] = humidityRH;
    buff[3] = (humidityRH>>8);    
    return true;
}
