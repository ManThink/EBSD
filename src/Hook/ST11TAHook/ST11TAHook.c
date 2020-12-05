#include "ST11TAHook.h"
#include "EBGlobal.h"
#include "ST11TAType.h"
#include "EB_I2C.h"
#include "LIS2DH12.h"
#include "DS18B20.h"
U_MT_SensorPARA MT_Sensor;
S_periodTask MT_TaskSensorTx;
S_periodTask MT_TaskSensorDetect;

void MT_MyParaInit()
{
      MT_Sensor.MPRegister.Command=0x82;
      MT_Sensor.MPRegister.InforFormat=0x21;
      MT_Sensor.MPRegister.MType=0x13;
      MT_Sensor.MPRegister.AlarmStatus.Bits.Ds18b20Status=0;
      MT_Sensor.MPRegister.AlarmStatus.Bits.AccStatus=0;
      MT_Sensor.MPRegister.AlarmStatus.Bits.StaticStatus=0;
      MT_Sensor.MPRegister.Ds18b20Temp[0]=0x04;//25¡æ
      MT_Sensor.MPRegister.Ds18b20Temp[0]=0xE2;
      MT_Sensor.MPRegister.Vol = RunStatus.Varible.BattLevel=0xC3;
      MT_Sensor.MPRegister.RSSI = RunStatus.Varible.RSSI = 60;
      MT_Sensor.MPRegister.SNR = RunStatus.Varible.SNR = 5;      
}

u1_t  MT_BoardInit()
{   
    mpos_driver.GPIO_Config(PIN_LIS2DH12_INT1,GPIO_Mode_IPD);
    mpos_driver.GPIO_Config(PIN_LIS2DH12_INT2,GPIO_Mode_IPD);
    mpos_driver.GPIO_Config(PIN_I2C_SCL,GPIO_Mode_OPP);
    mpos_driver.GPIO_PinSet(PIN_I2C_SCL, RESET) ;    
    mpos_driver.GPIO_Config(PIN_I2C_SDA,GPIO_Mode_OPP);
    mpos_driver.GPIO_PinSet(PIN_I2C_SDA, RESET) ;
    mpos_driver.GPIO_Config(PIN_DS18B20_POWER,GPIO_Mode_OPP); 
    mpos_driver.GPIO_PinSet(PIN_DS18B20_POWER, SET) ; 
    mpos_driver.GPIO_Config(PIN_LED_POWER,GPIO_Mode_OPP); 
    mpos_driver.GPIO_PinSet(PIN_LED_POWER, RESET) ;
    MT_MyParaInit();   
    mpos_osfun.DelayUs(1000000); mpos_driver.kickdog();
    mpos_osfun.DelayUs(1000000); mpos_driver.kickdog();
    mpos_osfun.DelayUs(1000000); mpos_driver.kickdog();
    mpos_osfun.DelayUs(1000000); mpos_driver.kickdog();
    mpos_osfun.DelayUs(1000000); mpos_driver.kickdog();
    MT_Sensor.MPRegister.AlarmStatus.Bits.AccStatus = ~MT_LIS2DH12_Init();
    return 1;
}
void MT_SensorTx(void *task,void *p)
{ 
    mpos_driver.GPIO_PinSet(PIN_DS18B20_POWER, SET) ; 
    mpos_osfun.DelayUs(100000); 
    MT_Sensor.MPRegister.AlarmStatus.Bits.Ds18b20Status = ~MT_DS18B20_ReadTemp(MT_Sensor.MPRegister.Ds18b20Temp);    
    MT_Sensor.MPRegister.Vol = RunStatus.Varible.BattLevel;  //Test voltage by MCU uint:100mV
    MT_Sensor.MPRegister.RSSI = RunStatus.Varible.RSSI;                                               //The signal RSSI of module
    MT_Sensor.MPRegister.SNR = RunStatus.Varible.SNR;                                                 //The signal SNR of module                                             //Test 3 axis acceleration by LIS2DH12
    mpos_lws.LW_TxData((unsigned char*)(MT_Sensor.Bytes),INFOR_LEN,SENSOR_PORT, LWOP_LTU);  
    mpos_driver.GPIO_PinSet(PIN_DS18B20_POWER, RESET) ;  
}
void MT_SensorDetect(void *task,void *p)
{                                                                  
    if(MT_LIS2DH12_ReadAcc())
    {
        MT_Sensor.MPRegister.AlarmStatus.Bits.AccStatus = ~MT_LIS2DH12_Init();   
        MT_LIS2DH12_ReadAcc();
    }
    if(MT_Var_Get((s2_t*)MT_Sensor.MPRegister.Buff_X,BUFF_LEN)>30) {MT_Sensor.MPRegister.AlarmStatus.Bits.StaticStatus=0;return;}
    if(MT_Var_Get((s2_t*)MT_Sensor.MPRegister.Buff_Y,BUFF_LEN)>30) {MT_Sensor.MPRegister.AlarmStatus.Bits.StaticStatus=0;return;}
    if(MT_Var_Get((s2_t*)MT_Sensor.MPRegister.Buff_Z,BUFF_LEN)>30) {MT_Sensor.MPRegister.AlarmStatus.Bits.StaticStatus=0;return;}
    //MT_Sensor.MPRegister.XVar = MT_Var_Get((s2_t*)MT_Sensor.MPRegister.Buff_X,BUFF_LEN);//TEST
    //MT_Sensor.MPRegister.YVar = MT_Var_Get((s2_t*)MT_Sensor.MPRegister.Buff_Y,BUFF_LEN);//TEST
    //MT_Sensor.MPRegister.ZVar = MT_Var_Get((s2_t*)MT_Sensor.MPRegister.Buff_Z,BUFF_LEN);//TEST
    //mpos_osfun.Task_ExcuteNow(&MT_TaskSensorTx);//TEST
    if(MT_Sensor.MPRegister.AlarmStatus.Bits.StaticStatus==0)
    {
        //mpos_osfun.Task_ExcuteNow(&MT_TaskSensorTx);//TX 3 times
    }
    MT_Sensor.MPRegister.AlarmStatus.Bits.StaticStatus=1;
}
void MT_SetupSensorTxTask(s8_t period) //period:ms
{
  TaskInstallWithInit(&MT_TaskSensorTx,period,MT_SensorTx,NULL,0xFFFFFFFF);
  mpos_osfun.Task_Restart(&MT_TaskSensorTx);
}
void MT_SetupSensorDetectTask(s8_t period) //period:ms
{
  TaskInstallWithInit(&MT_TaskSensorDetect,period,MT_SensorDetect,NULL,0xFFFFFFFF);
  mpos_osfun.Task_Restart(&MT_TaskSensorDetect);
}
u1_t MT_Init()
{
  //MT_SetupSensorTxTask(60000);//tx task 3 hour 
  MT_SetupSensorTxTask(((UserAppPara->ProductInfo.UploadPeriod[1]<<8)+UserAppPara->ProductInfo.UploadPeriod[0])*1000);
  MT_SetupSensorDetectTask(300000);// detect task 5 min
  //MT_SetupSensorDetectTask(60000);// detect task 1 min
  mpos_osfun.Task_ExcuteNow(&MT_TaskSensorTx);
  return 1;
}

u1_t EB_HookInit()
{
  EB_Hook.Hook_BoardInit=MT_BoardInit;
  EB_Hook.Hook_APPInit=MT_Init;
  EB_Hook.Hook_WKIRQ=NULL;
  EB_Hook.Hook_UartUserDeal=NULL;
  EB_Hook.Hook_DataDn=NULL;
  return 0;
}


