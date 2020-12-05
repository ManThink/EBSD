#ifndef _MP_OS_H_
#define _MP_OS_H_
#include "MP_OS_type.h"
#include "MP_LWS_type.h"
  

//API hook function of interrupt
extern  void (* HookExcuteOnce1)();//0x04
extern  void (* HookExcuteOnce2)();//0x04
extern  void (* HookExcuteOnce3)();
extern  void (* HOOK_EXTI1_IRQHandler)(void);
extern  void (* HOOK_EXTI0_IRQHandler)(void);
extern  void (* HOOK_EXTI3_IRQHandler)(void);
extern  void (* HOOK_EXTIWK_IRQHandler)(void);
extern  void (* HOOK_GPIO_IRQHandler)(u1_t pin);
extern  void (*	HOOK_USART1_IRQHandler)(void);
extern  void (*	HOOK_USART2_IRQHandler)(u1_t data);
extern  void (*	HOOK_RTC_IRQHandler)(void);
extern  void (* HOOK_LPTIM_IRQHandler)(void);
extern void (* HookAPPParaChanged)(u1_t * paras);
extern void (* mp_userInit)(void);
extern bool ( *HookUserCheck)( void );
extern unsigned char (*Hook_SendBuildFram) (unsigned char* data,unsigned short len) ;
#ifndef _MPOS_RAMLOAD_LIB
extern void enableInterrupts();
extern void disableInterrupts();
extern void Clock_Init(void);
extern void IWdgInit();
extern void Hal_Init(void);       
extern void SysParaInit(void);
extern bool PTaskInit(u1_t TaskNO,void ( *callback )( void ), u4_t timeout );
extern bool PTaskStart(u1_t TaskNO);
extern bool PTaskStop(u1_t TaskNO);
extern bool PTaskIsRuning(u1_t TaskNO);

extern void (*Task_sem_init)(SEMPHORE * mysem, int counts,int maxvalue);
extern void Task_sem_release(SEMPHORE * mysem);
extern bool Task_sem_waitOne(S_periodTask * task, SEMPHORE * mysem,s8_t waitTime);
extern void TaskSetPeriod(S_periodTask * task,s8_t period);
extern void TaskExcuteNow(S_periodTask * task);
extern void TaskRestart(S_periodTask * task);
extern void TaskSetup(S_periodTask * task);
extern void TaskRemove(S_periodTask * task);
extern void TaskStop(S_periodTask * task);


extern void Flash_SizeSelect(u2_t size);
extern u4_t LWS_GetUTCTime();
extern u1_t  ADC_GetVoltageValue(void);
extern u1_t  ADC_GetTempValue(void);
extern void paraAPPGet(u1_t * para,u1_t len) ;
extern void paraAPPSave(u1_t * para,u1_t len);
extern u1_t ADC_GetTempValue(void);
extern u1_t ADC_GetVoltageValue(void);
extern void memcpy1( u1_t *dst, u1_t *src, u2_t size );
extern u2_t os_rlsbf2 (u1_t * buf);
extern u4_t os_rlsbf4 (u1_t * buf);
extern u8_t os_rlsbf8 (u1_t * buf) ;
extern u2_t  CUSUM_CRC(u1_t *buf,u2_t lenth);
extern u2_t mp_modeACRC(u1_t *buf,u4_t lenth,u2_t poly);
extern u2_t mp_modeBCRC (u1_t * data, u2_t len,u2_t poly);
extern u2_t os_crc16 (u1_t * data, u2_t len);
extern u1_t crc8_ccit(const u1_t * data,u4_t size); 
extern void DelayUs(u8_t nCount);
#endif
#endif
