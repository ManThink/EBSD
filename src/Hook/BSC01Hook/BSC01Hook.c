#include "EBGlobal.h"
#include "BSC01Type.h"
SMT_UpData MT_UpData;
void MT_DataCvt(u1_t *buf,u2_t len,void *para)
{
  u1_t AcqTaskIdx=*(u1_t *)para; //acq task index,identical to the index in jason
  MT_UpData.AcqTimeout=1; //when AcqTimeout is 1,the application data in uploading data should be ignored.
  MT_UpData.AcqTime=mpos_lws.LWS_GetSysTime(4)/1000; //get the utc time
  if(buf==NULL)
  {
    //there is no response from rs485 after acq cmd is sent.
    //acq task is time out,processing the application data according to the business,such as fill all application data with 0xFF or 0x00.
 }
  else
  {
    //acq task is not time out,copy sensor data to upload buffer,you can process the sensor data according your application
    if(AcqTaskIdx==1) //sensor ack data:20 03 20 43 67 80 00 40 B5 C2 8F 44 A4 10 00 00 00 00 00 3A 83 12 6F 00 00 00 00 00 00 00 00 42 47 EB 85 A7 59
    {
       if(len==37&&buf[0]==0x20&&buf[1]==0x03) //validate data
       {
          MT_UpData.AcqTimeout=0;
          mpos_osfun.memcpy1(MT_UpData.SensorData,buf,32);
       }
    }
    else if(AcqTaskIdx==2)
    {
      if(len==9&&buf[0]==0x20&&buf[1]==0x03) //validate data
      {
        MT_UpData.AcqTimeout=0;
        mpos_osfun.memcpy1(MT_UpData.SensorData+32,buf,4); //sensor ack data:20 03 04 43 B8 73 33 3B B5
      }
    }
 }
 if(AcqTaskIdx==2) //when the last query task is excuted,upload data.
 {
     mpos_lws.LW_TxData((u1_t *)&MT_UpData,sizeof(SMT_UpData),BSC01_UPPORT,LWOP_LTU); //send unconfirm packet,23 is the port of LoRaWAN,which can be used to distinguish different applications.
     //mpos_lws.LW_TxData(MT_UpData,sizeof(SMT_UpData),BSC01_UPPORT,LWOP_LTC); //send confirm packet,23 is the port of LoRaWAN,which can be used to distinguish different applications.
 }
}

void MT_InitUpdata()
{
  MT_UpData.Header=0x80;
  MT_UpData.DevId=1;
  MT_UpData.Version=1;
  MT_UpData.Tail=0x16;
}
u1_t EB_HookInit()
{
   
   mt_eb_runpara.fb_rs485=1;
   mt_eb_runpara.fb_cp=3;       //control the power of rs485 to save battery.and output 1 when power on
   mt_eb_runpara.fb_uart2=1;    //rs485 is using uart2.
   EB_Hook.Hook_DataCvt=MT_DataCvt;//data processing hook
   MT_InitUpdata();
   return 0;
}


