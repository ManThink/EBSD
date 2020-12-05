#include "LIS2DH12.h"
//#include "EBGlobal.h"
#include "lis2dh12_reg.h"
#include "math.h"
#include "ST11TAHook.h"

/* Private variables ---------------------------------------------------------*/
static axis3bit16_t data_raw_acceleration;
//static axis1bit16_t data_raw_temperature;
static uint8_t whoamI;
/********************************************************
* Function for platform_write 
*
********************************************************/
int32_t MT_platform_write(void *handle, uint8_t Reg, uint8_t *Bufp,uint16_t len)
{
    /* enable auto incremented in multiple read/write commands */
    //Reg |= 0x80; 
    return MT_I2C_WriteBuffer(LIS2DH12_I2C_ADD_L, Reg, Bufp, len);
}
/********************************************************
* Function for platform_read 
*
********************************************************/
int32_t MT_platform_read(void *handle, uint8_t Reg, uint8_t *Bufp,uint16_t len)
{
    /* enable auto incremented in multiple read/write commands */
    //Reg |= 0x80;
    return MT_I2C_ReadBuffer(LIS2DH12_I2C_ADD_L,Reg,Bufp,len);
}
/********************************************************
* Function for Meter_InitIo 
*
********************************************************/
u1_t MT_LIS2DH12_Init(void)
{
  uint8_t i;
  lis2dh12_ctx_t dev_ctx;             // Initialize mems driver interface
  dev_ctx.write_reg = MT_platform_write;
  dev_ctx.read_reg = MT_platform_read;
  whoamI = 0;                         //Check device ID
  for(i=0;i<5;i++)
  {
    MT_lis2dh12_device_id_get(&dev_ctx, &whoamI);
    if (whoamI == LIS2DH12_ID) break;
    mpos_osfun.DelayUs(8000);
  }
  if(i>=5) return false;
  MT_lis2dh12_pin_sdo_sa0_mode_set(&dev_ctx,LIS2DH12_PULL_UP_DISCONNECT);//disable the pullup 
  //lis2dh12_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);  //Enable Block Data Update
  MT_lis2dh12_filter_reference_get(&dev_ctx, &whoamI);
  MT_lis2dh12_data_rate_set(&dev_ctx, LIS2DH12_ODR_10Hz);//output data to 10 Hz
  MT_lis2dh12_operating_mode_set(&dev_ctx,LIS2DH12_HR_12bit); // set lp==1;
  MT_lis2dh12_full_scale_set(&dev_ctx, LIS2DH12_2g);//Set full scale
  MT_lis2dh12_temperature_meas_set(&dev_ctx, LIS2DH12_TEMP_ENABLE);//Enable temperature sensor 
  return true;
}
/********************************************************
* Function for LIS2DH12_ReadAcc
*
********************************************************/
u1_t MT_LIS2DH12_ReadAcc(void)
{
   u1_t err;
   err = MT_I2C_ReadBuffer(LIS2DH12_I2C_ADD_L,LIS2DH12_OUT_X_L,data_raw_acceleration.u8bit,6);
   MT_Sensor.MPRegister.Buff_X[MT_Sensor.MPRegister.Buff_len] = (s2_t)(LIS2DH12_FROM_FS_2g_HR_TO_mg( data_raw_acceleration.i16bit[0] ));
   MT_Sensor.MPRegister.Buff_Y[MT_Sensor.MPRegister.Buff_len] = (s2_t)(LIS2DH12_FROM_FS_2g_HR_TO_mg( data_raw_acceleration.i16bit[1] ));
   MT_Sensor.MPRegister.Buff_Z[MT_Sensor.MPRegister.Buff_len] = (s2_t)(LIS2DH12_FROM_FS_2g_HR_TO_mg( data_raw_acceleration.i16bit[2] )); 
   if((MT_Sensor.MPRegister.Buff_len++)>BUFF_LEN) MT_Sensor.MPRegister.Buff_len=0;
   return err;
}
/********************************************************
* Function for LIS2DH12_AngleCheck
*
********************************************************/
void MT_LIS2DH12_LowPower(void)
{
  uint8_t temp;
  lis2dh12_ctx_t dev_ctx;             // Initialize mems driver interface
  dev_ctx.write_reg = MT_platform_write;
  dev_ctx.read_reg = MT_platform_read;
  MT_lis2dh12_pin_sdo_sa0_mode_set(&dev_ctx,LIS2DH12_PULL_UP_DISCONNECT);//disable the pullup 
  //lis2dh12_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);  //Enable Block Data Update
  MT_lis2dh12_filter_reference_get(&dev_ctx, &temp);
  MT_lis2dh12_data_rate_set(&dev_ctx, LIS2DH12_POWER_DOWN);//output data to 10 Hz
  MT_lis2dh12_operating_mode_set(&dev_ctx,LIS2DH12_LP_8bit); // set lp==1;
  MT_lis2dh12_full_scale_set(&dev_ctx, LIS2DH12_2g);//Set full scale
  MT_lis2dh12_temperature_meas_set(&dev_ctx, LIS2DH12_TEMP_DISABLE);//Enable temperature Sensor
}
/********************************************************
* Function for LIS2DH12_Read
*
********************************************************/
void MT_LIS2DH12_ReadXYZACC(uint8_t buff[])
{
   float acceleration_mg[3];
   MT_I2C_ReadBuffer(LIS2DH12_I2C_ADD_L,LIS2DH12_OUT_X_L,data_raw_acceleration.u8bit,6);
   acceleration_mg[0] = LIS2DH12_FROM_FS_2g_LP_TO_mg( data_raw_acceleration.i16bit[0] );
   acceleration_mg[1] = LIS2DH12_FROM_FS_2g_LP_TO_mg( data_raw_acceleration.i16bit[1] );
   acceleration_mg[2] = LIS2DH12_FROM_FS_2g_LP_TO_mg( data_raw_acceleration.i16bit[2] );
   buff[0] = (int16_t)acceleration_mg[0];
   buff[1] = ((int16_t)acceleration_mg[0])>>8;
   buff[2] = (int16_t)acceleration_mg[1];
   buff[3] = ((int16_t)acceleration_mg[1])>>8;
   buff[4] = (int16_t)acceleration_mg[2];
   buff[5] = ((int16_t)acceleration_mg[2])>>8;   
}
/********************************************************
* Function for Var_Get
*
********************************************************/
u2_t MT_Var_Get(s2_t *buff,u1_t len)
{
    u1_t i;
    s4_t sum = 0;
    s2_t aver;
    u2_t var=0;
    for(i=0;i<len;i++) sum += buff[i];
    aver = (s2_t)(sum/len);
    for(i=0;i<len;i++) var += (buff[i]-aver)*(buff[i]-aver);
    var = var/len;
    return var;
}
/********************************************************
* Function for Var_Get
*
********************************************************/
s2_t MT_AVG_Get(s2_t *buff,u1_t len)
{
    u1_t i;
    s4_t sum = 0;
    s2_t aver;
    for(i=0;i<len;i++) sum += buff[i];
    aver = (s2_t)(sum/len);
    return aver;
}
