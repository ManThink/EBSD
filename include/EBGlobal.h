#ifndef _MY_GLOBAL_H
#define _MY_GLOBAL_H
#include "AppConst.h"
#include "MP_BASE_Type.h"
#include "MP_OS_Type.h"
#include "MP_LWS_Type.h"
#include "MP_OS.h"
#include "MP_EB_Type.h"

#ifdef  _APP_DEBUGING
extern void UART1_SendString(unsigned char * sendData);
extern void UART1_Sendhex(unsigned char* data,unsigned short len);
extern void UART1_SendDec(signed long long data);
#define appDebug 	UART1_SendString 
#define appDebugDec     UART1_SendDec
#define appDebugHex     UART1_Sendhex
#else
#define appDebug         	
#define appDebugDec 
#define appDebugHex 
#endif

#define LLWU_MODE_DISABLE 0
#define LLWU_MODE_RISING 1
#define LLWU_MODE_FALLING 2
#define LLWU_MODE_ANY 3

#define _AUX_TIME_EXSHORT       50
#define _AUX_TIME_SHORT         100
#define _AUX_TIME_MIDDLE        500
#define _AUX_TIME_LONG          1000
#define _AUX_TIME_EXLONG        2000
#define _AUX_TIME_FOREVER       100000

#define _WKING_FOREVER          0x01
#define _WKING_GNERAL           0x02
#define _WKING_NONE             0x00

#define _APP_MULADDR_FILLIDX  120
#define _APP_MULADDR_FILLLEN  121
#define _APP_MULADDR_ADDRLIST	122

extern MT_UartBusiness	UartBusiness;// __attribute__((section(".ARM.__at_0x20003000"))); //0x2f0
extern MT_QueryTaskPara	QueryTaskPara[MAX_QUERYPARA];//__attribute__((section(".ARM.__at_0x20002280"))); //0x48
extern MT_BllPara 	BllPara; //__attribute__((section(".ARM.__at_0x20002380")));//0x28
extern MT_Dtu_AppPara *UserAppPara;// __attribute__((section(".ARM.__at_0x200023C0"))); //0x04
extern u1_t	BllParaBuf[BLLPARA_LEN+QUERY_RANDOM_LEN+UP_LORA_RANDOM_LEN]; //__attribute__((section(".ARM.__at_0x200023d0"))); 	//0x280	
extern SEMPHORE        SerialSem ;
extern bool mt_dtu_resetAfterLWT    ;
extern bool          mt_uart2;
extern u1_t CalcTempBuf[128];
extern u1_t CurrentDeviceId;
extern MT_App_RunStatus AppRunStatus;
extern SMT_QUE_BASE_TIME mt_up_event[MAX_UPTEMPLT] ;
extern SMT_QUE_BASE_TIME * mt_up_event_header;
extern SMT_QUE_BASE_TIME   mt_query_event[MAX_QUERYPARA];
extern SMT_QUE_BASE_TIME * mt_query_event_header;
extern SMT_QUE_BASE_TIME * mt_up_event_random;
extern SMT_QUE_BASE_TIME * mt_up_event_heart;
extern SMT_QUE_BASE_TIME * mt_query_event_random;
extern SMT_QUE_BASE_TIME * mt_query_event_warning;
extern S_periodTask	        mt_TaskQuery;
extern S_periodTask	        mt_TaskUp;
extern S_periodTask	    	mt_TaskBios;
extern S_periodTask	        mt_TaskWakeupTimeout;
extern S_periodTask	    	mt_TaskRunLedBlink;
extern S_periodTask	    	mt_TaskAuxLedOFF;
extern s8_t	        	mt_keyTicks  ;
extern s8_t	        	mt_keyCounter;
extern u1_t 			mt_OpCond[8];
extern u2_t 		    	mt_HeartTicks;
extern u1_t	        	mt_sensordata[256];
extern SMT_EGBS_RUN_PARA	mt_eb_runpara;
extern MT_Dtu_RunPara RunParaShadow;
extern s8_t MulDev_QueryStartTime;
extern u1_t MulDev_QueryStartId;
extern void Buf485Clear();
extern void use485();
extern void release485();
extern void Buffer485Send(u1_t * buffer, u2_t size);
extern void TaskInstallWithInit(S_periodTask * task,s8_t period,void ( *Function )( void * task, void * para ),void * para,u4_t cycles);
extern void TaskFunc_Mornitor(void *task,void *p);
extern void TaskFunc_LoraTx(void * task,void *p);
extern void TaskFunc_QueryData(void * task, void * para);
extern void TaskFunc_FrameOver(void * task, void * para);
extern void TaskFunc_UartTimeout(void * task, void * para);
extern void LD_LoraDnProcess(u1_t port,u1_t * Buffer, u2_t len);
extern void LD_swrxdone (u1_t port,u1_t * Buffer, u2_t len);
extern unsigned short  (* HookUARTUserDeal)(unsigned char * ReceiveData,unsigned short lenth,unsigned char * OutPut);
extern void (* Hook_fuota_uart2_ack)(mt_ev_t    event,u1_t port, u1_t * txbuffer,u1_t len,u1_t ackdelay);
extern void (* Hook_fuota_uart1_ack)(mt_ev_t    event,u1_t port, u1_t * txbuffer,u1_t len,u1_t ackdelay);
extern void appEventLWS( mt_ev_t ev ,u1_t port,u1_t * Buffer, u2_t len);
extern u1_t EB_HookInit();
extern MT_EB_Hook EB_Hook;
extern u1_t BufferSendUART1(u1_t * data, u2_t len);
extern u1_t BufferSendUART2(u1_t * data, u2_t len);
extern void app_AuxBlink(s8_t time);
extern void PP_GetAppPara();
extern void   MyHookInit();  
extern void   MyBoardInit();
extern u1_t App_Init();
#endif
