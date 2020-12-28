#include "BSE01CHook.h"
#include "EBGlobal.h"
#include "BSE01CType.h"
#include "EB_I2C.h"
#include "SHT20.h"
U_MT_SensorPARA MT_Sensor;
S_periodTask MT_TaskSensorTx;

void MT_PowerON()
{
      mpos_driver.GPIO_PinSet(4, SET); 
}

void MT_PowerOFF()
{
      mpos_driver.GPIO_PinSet(4, RESET);
      mpos_driver.GPIO_PinSet(14, RESET) ; 
      mpos_driver.GPIO_PinSet(15, RESET) ; 
      mpos_driver.GPIO_Config(6,GPIO_Mode_IPU); 
      mpos_driver.GPIO_Config(7,GPIO_Mode_IPU);
      mpos_driver.GPIO_Config(9,GPIO_Mode_IPD);
}

void MT_MyParaInit()
{
      MT_Sensor.MPRegister.Command=0x82;
      MT_Sensor.MPRegister.InforFormat=0x21;
      MT_Sensor.MPRegister.MType=0x16;
      MT_Sensor.MPRegister.AlarmStatus.Bits.Sht20Status=0;
      MT_Sensor.MPRegister.AlarmStatus.Bits.Ds18b20Status=0;
      MT_Sensor.MPRegister.AlarmStatus.Bits.AccStatus=0;
      MT_Sensor.MPRegister.Sht20Temp[0]=25;
      MT_Sensor.MPRegister.Sht20Temp[1]=0;
      MT_Sensor.MPRegister.Sht20Hum[0]=50;
      MT_Sensor.MPRegister.Sht20Hum[1]=0;
      MT_Sensor.MPRegister.Vol=33;
      MT_Sensor.MPRegister.RSSI=0;
      MT_Sensor.MPRegister.SNR=0;      
}

u1_t  MT_BoardInit()
{   
    mpos_driver.GPIO_Config(PIN_I2C_SCL,GPIO_Mode_OPP);
    mpos_driver.GPIO_PinSet(PIN_I2C_SCL, SET) ;    
    mpos_driver.GPIO_Config(PIN_I2C_SDA,GPIO_Mode_OPP);
    mpos_driver.GPIO_PinSet(PIN_I2C_SDA, SET) ;
    mpos_driver.GPIO_Config(2,GPIO_Mode_OPP);
    mpos_driver.GPIO_Config(4,GPIO_Mode_OPP);
    mpos_driver.GPIO_PinSet(2, RESET);
    MT_MyParaInit();    
    return 1;
}
void MT_SensorTx(void *task,void *p)
{      
    MT_PowerON();
    MT_Sensor.MPRegister.AlarmStatus.Bits.Sht20Status = ~MT_SHT2x_MeasureTH(&MT_Sensor.MPRegister.Sht20Temp[0]);  //Test temperature and humidity by SHT20 uint:¡æ & %RH 
    //TH_Sensor.MPRegister.Vol = (u1_t)((5.118*RunStatus.Varible.BattLevel)/100+20);   
    MT_Sensor.MPRegister.Vol = RunStatus.Varible.BattLevel;  //Test voltage by MCU uint:100mV
    MT_Sensor.MPRegister.RSSI = RunStatus.Varible.RSSI;                                               //The signal RSSI of module
    MT_Sensor.MPRegister.SNR = RunStatus.Varible.SNR;                                                 //The signal SNR of module                                             //Test 3 axis acceleration by LIS2DH12
    mpos_lws.LW_TxData((unsigned char*)(MT_Sensor.Bytes),INFOR_LEN,SENSOR_PORT, LWOP_LTC);
    MT_PowerOFF();
}
void MT_SetupSensorTxTask(s8_t period) //period:ms
{
  TaskInstallWithInit(&MT_TaskSensorTx,period,MT_SensorTx,NULL,0xFFFFFFFF);
  mpos_osfun.Task_Restart(&MT_TaskSensorTx);
}

u1_t MT_Init()
{
  MT_SetupSensorTxTask(30000);
  return 1;
}

u1_t EB_HookInit()
{
  mt_eb_runpara.fb_uart2=0;
  EB_Hook.Hook_BoardInit=MT_BoardInit;
  EB_Hook.Hook_APPInit=MT_Init;
  EB_Hook.Hook_WKIRQ=NULL;
  EB_Hook.Hook_UartUserDeal=NULL;
  EB_Hook.Hook_DataDn=NULL;
  return 0;
}


