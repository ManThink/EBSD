#include "BST0xHook.h"
U_MT_SensorPARA MT_Sensor;
u1_t  MT_BoardInit()
{
  mpos_driver.GPIO_Config(GPS_POWERBAK,GPIO_Mode_OPP);
  mpos_driver.GPIO_Config(2,GPIO_Mode_IPU);
  mpos_driver.GPIO_Config(GPS_POWER,GPIO_Mode_OPP);
  mpos_driver.GPIO_Config(6,GPIO_Mode_IPD);
  mpos_driver.GPIO_Config(7,GPIO_Mode_IPD);
  mpos_driver.GPIO_Config(8,GPIO_Mode_IPD);
  mpos_driver.GPIO_Config(10,GPIO_Mode_IPD);
  mpos_driver.GPIO_Config(14,GPIO_Mode_OPP);
  mpos_driver.GPIO_Config(15,GPIO_Mode_OPP);
  mpos_driver.GPIO_PinSet(GPS_POWERBAK, SET);
  mpos_driver.GPIO_PinSet(GPS_POWER, SET);
  mpos_driver.GPIO_Config(9,GPIO_Mode_OPP);
  mpos_driver.GPIO_PinSet(9, RESET) ;
  mpos_driver.GPIO_PinSet(14, SET) ;
  mpos_driver.GPIO_PinSet(15, SET) ;
  return 1;
}
void MT_releaseUART2()
{
   UartBusiness.RxBuf[0] = 0x4F;
   UartBusiness.RxBuf[1] = 0x4B;
   UartBusiness.RxLenth = 107;
}


u1_t  MT_Init()
{ 
  MT_GPSInitial();
  MT_Sensor.MPRegister.Command = 0x82;
  MT_Sensor.MPRegister.InforFormat = 0x21;
  MT_Sensor.MPRegister.MType = 0x14;
  UserAppPara->SysPara.QueryTimeout.bits.Count = 180;
  UserAppPara->SysPara.QueryTimeout.bits.Unit =0;
  return 1;
}

void MT_DataAcq(void *task,void *p)
{
  QueryTaskPara[1].QueryPara->QueryPeriod.bits.Count = (UserAppPara->ProductInfo.UploadPeriod[1]<<8)+UserAppPara->ProductInfo.UploadPeriod[0];
  QueryTaskPara[1].QueryPara->QueryPeriod.bits.Unit = 1;  //0:second;1:min;2:hour
}

void MT_DataCvt(u1_t *data,u2_t len,void *p)
{       
    MT_GPS_Information.delay1 += 1;
    if (data == NULL)
    {
      mpos_lws.LW_TxData((u1_t*) (MT_Sensor.Bytes),4,12,LWOP_LTC);  //time out
      return;
    }
    if(MT_GPS_RMC_Parse((char *)data,&MT_GPS_Information))
    {
      MT_GPS_Information.delay2 += 1;
      MT_Sensor.MPRegister.latitude[0] = (u1_t)(0xFF &  MT_GPS_Information.LatitudeBinary);
      MT_Sensor.MPRegister.latitude[1] = (u1_t)(0xFF & (MT_GPS_Information.LatitudeBinary >>  8));
      MT_Sensor.MPRegister.latitude[2] = (u1_t)(0xFF & (MT_GPS_Information.LatitudeBinary >> 16));
      MT_Sensor.MPRegister.NS = (u1_t)(0xFF &  MT_GPS_Information.NS);
      MT_Sensor.MPRegister.longitude[0] = (u1_t)(0xFF &  MT_GPS_Information.LongitudeBinary);
      MT_Sensor.MPRegister.longitude[1] = (u1_t)(0xFF & (MT_GPS_Information.LongitudeBinary >>  8));
      MT_Sensor.MPRegister.longitude[2] = (u1_t)(0xFF & (MT_GPS_Information.LongitudeBinary >> 16));
      MT_Sensor.MPRegister.EW = (u1_t)(0xFF &  (MT_GPS_Information.EW));                                   
      MT_Sensor.MPRegister.year = (u1_t) (MT_GPS_Information.D.year % 1000);
      MT_Sensor.MPRegister.month = (u1_t) MT_GPS_Information.D.month;
      MT_Sensor.MPRegister.day = (u1_t) MT_GPS_Information.D.day;
      MT_Sensor.MPRegister.hour = (u1_t) MT_GPS_Information.D.hour;
      MT_Sensor.MPRegister.min = (u1_t) MT_GPS_Information.D.minute;
      MT_Sensor.MPRegister.seconds = (u1_t) MT_GPS_Information.D.second;
      MT_Sensor.MPRegister.Vol = RunStatus.Varible.BattLevel;  //Test voltage by MCU uint:100mV
      MT_Sensor.MPRegister.RSSI = RunStatus.Varible.RSSI;      //The signal RSSI of module
      MT_Sensor.MPRegister.SNR = RunStatus.Varible.SNR;         //The signal SNR of module
      MT_Sensor.MPRegister.Duration = MT_GPS_Information.delay1;
      if(MT_GPS_Information.delay2 > 6)
      {
        mpos_lws.LW_TxData((u1_t*) (MT_Sensor.Bytes),21,12,LWOP_LTC);
        MT_GPS_Information.delay1 = 0;
        MT_GPS_Information.delay2 = 0;
        MT_releaseUART2();      //Release uart to make moudle sleep
      }
      return;
    }
  return;
}


u1_t EB_HookInit()
{
  mt_eb_runpara.fb_cp=3;
  mt_eb_runpara.fb_uart2=1;
  EB_Hook.Hook_BoardInit = MT_BoardInit;
  EB_Hook.Hook_APPInit = MT_Init;
  EB_Hook.Hook_DataCvt = MT_DataCvt;
  EB_Hook.Hook_DataAcq = MT_DataAcq;
  return 0;
}

