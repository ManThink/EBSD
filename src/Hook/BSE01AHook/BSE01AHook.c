#include "BSE01AHook.h"
#include "EBGlobal.h"
#include "BSE01AType.h"
U_MT_SensorPARA MT_Sensor;
S_periodTask MT_TaskSensorTx;
S_periodTask MT_TaskSensorWait;
/*  water logging resistance£º
    water logging1/5 = 20MR
    water logging1/2 = 6MR
    water logging1/1 = 4MR
*/
void MT_MyParaInit()
{
      MT_Sensor.MPRegister.Command=0x82;
      MT_Sensor.MPRegister.InforFormat=0x21;
      MT_Sensor.MPRegister.MType=0x12;
      MT_Sensor.MPRegister.AlarmStatus.Bits.WaterSensor=0;
      MT_Sensor.MPRegister.Vol=33;
      MT_Sensor.MPRegister.RSSI=0;
      MT_Sensor.MPRegister.SNR=0;      
}
u1_t  MT_BoardInit()
{     
    mpos_driver.GPIO_Config(PIN_WATER_SENSOR,GPIO_Mode_IFT);
    mpos_driver.GPIO_IT_Config(PIN_WATER_SENSOR,GPIO_IT_RisingFallingEdge);
    mpos_driver.GPIO_Config(2,GPIO_Mode_OPP);
    mpos_driver.GPIO_Config(3,GPIO_Mode_IPU);
    mpos_driver.GPIO_Config(5,GPIO_Mode_IPU);
    mpos_driver.GPIO_Config(4,GPIO_Mode_OPP);
    mpos_driver.GPIO_Config(14,GPIO_Mode_OPP);
    mpos_driver.GPIO_Config(15,GPIO_Mode_OPP);
    mpos_driver.GPIO_PinSet(4, RESET);
    mpos_driver.GPIO_PinSet(14, RESET); 
    mpos_driver.GPIO_PinSet(15, RESET); 
    mpos_driver.GPIO_Config(7,GPIO_Mode_IPU);
    mpos_driver.GPIO_Config(8,GPIO_Mode_IPU);
    mpos_driver.GPIO_PinSet(2, RESET) ;
    mpos_driver.GPIO_Config(9,GPIO_Mode_IPD);
    MT_MyParaInit();    
    return 1;
}
void MT_SensorWait(void *task,void *p)
{       
  if(MT_Sensor.MPRegister.AlarmStatus.Bits.WaterSensor == mpos_driver.GPIO_PinGet(PIN_WATER_SENSOR))
  {
    MT_Sensor.MPRegister.AlarmStatus.Bits.WaterSensor = ~mpos_driver.GPIO_PinGet(PIN_WATER_SENSOR);
    MT_Sensor.MPRegister.RSSI = RunStatus.Varible.RSSI;                                               //The signal RSSI of module
    MT_Sensor.MPRegister.SNR = RunStatus.Varible.SNR;                                                 //The signal SNR of module                                             //Test 3 axis acceleration by LIS2DH12
    mpos_lws.LW_TxData((unsigned char*)(MT_Sensor.Bytes),INFOR_LEN,SENSOR_PORT, LWOP_LTC);      
  } 
   mpos_driver.GPIO_IT_Config(PIN_WATER_SENSOR,GPIO_IT_RisingFallingEdge);
 }
void MT_SetupSensorWaitTask(s8_t period) //period:ms
{
  TaskInstallWithInit(&MT_TaskSensorWait,period,MT_SensorWait,NULL,0x00000001);
  mpos_osfun.Task_Restart(&MT_TaskSensorWait);
}

void MT_SensorTx(void *task,void *p)
{                   
    MT_Sensor.MPRegister.AlarmStatus.Bits.WaterSensor = ~mpos_driver.GPIO_PinGet(PIN_WATER_SENSOR);  //Test temperature and humidity by SHT20 uint:¡æ & %RH  
    //TH_Sensor.MPRegister.Vol = RunStatus.Varible.BattLevel;  //Test voltage by MCU uint:100mV
    MT_Sensor.MPRegister.RSSI = RunStatus.Varible.RSSI;                                               //The signal RSSI of module
    MT_Sensor.MPRegister.SNR = RunStatus.Varible.SNR;                                                 //The signal SNR of module                                             //Test 3 axis acceleration by LIS2DH12
    mpos_lws.LW_TxData((unsigned char*)(MT_Sensor.Bytes),INFOR_LEN,SENSOR_PORT, LWOP_LTC); 
    MT_SetupSensorWaitTask(60000);
}
void MT_SetupSensorTxTask(s8_t period) //period:ms
{
  TaskInstallWithInit(&MT_TaskSensorTx,period,MT_SensorTx,NULL,0xFFFFFFFF);
  mpos_osfun.Task_Restart(&MT_TaskSensorTx);
}


u1_t MT_Init()
{
  MT_SetupSensorTxTask(((UserAppPara->ProductInfo.UploadPeriod[1]<<8)+UserAppPara->ProductInfo.UploadPeriod[0])*1000);
  return 1;
}
u1_t  MT_WaterSensor(u1_t pin,s8_t now)
{
  switch(pin)
  {
    case PIN_WATER_SENSOR:
          mpos_osfun.Task_ExcuteNow(&MT_TaskSensorTx);
          mpos_driver.GPIO_IT_Diable(PIN_WATER_SENSOR);
         break;
  }
  return 0;
}
u1_t EB_HookInit()
{
  mt_eb_runpara.fb_uart2=0;
  EB_Hook.Hook_BoardInit=MT_BoardInit;
  EB_Hook.Hook_APPInit=MT_Init;
  EB_Hook.Hook_WKIRQ=MT_WaterSensor;
  EB_Hook.Hook_UartUserDeal=NULL;
  EB_Hook.Hook_DataDn=NULL;
  return 0;
}


