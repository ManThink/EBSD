#ifndef _MP_EB_TYPE_H
#define _MP_EB_TYPE_H
#include "MP_OS_Type.h"
#include "CrcCal.h"
#define UART_MAX_RXBUFLEN 1024 
#define UART_MAX_TXBUFLEN 256 
typedef struct t_uart_packet
{
  S_periodTask     Uart_FrameOver; //72 bytes
  S_periodTask     UartRecv_Timeout; //72 bytes
  unsigned int     RxLenth;     
  unsigned int     TxLenth;	
  unsigned char    RxBuf[UART_MAX_RXBUFLEN];
  unsigned char    TxBuf[UART_MAX_TXBUFLEN]; 
} PACKED MT_UartBusiness;

/*******32 bytes to record the status of app********/
typedef struct s_AppRunStatus
{
  u1_t PreTxCnt; //Tiks before sending
  
  u1_t softrst:1; //Software Reset
  u1_t SwUp:1; //Upload through SW channel
  u1_t Resv:6;

  u1_t BllParaError:1; //Error parsing business parameters
  u1_t QueryTimeout:1; //
  u1_t QueryNeverAck:1; //
  u1_t AckCrcErr:1; //Serial port receive check sum error
  u1_t AckLenErr:1; //Serial port receive lenth error¯¯
  u1_t Div0Error:1; //The divisor is 0
  u1_t Uart2Rxing:1; //UART2 is receiving data
  u1_t Uart2Txing:1; //UART2 is sending data
  
  u1_t  LoraTxFailCnt; //Continuous failure times of Lora transmission
  u1_t	JoinFailCnt; //Continuous failure times of join to LoraWan network
  u1_t  ClassSwitchFailCnt; //Continuous failure times of switch classmode
  u1_t  RadioBusyCnt; //count of RF busy
  u1_t 	NoQueryCnt; //Number of consecutive no query task runs
  u1_t  NoLoraUpCnt; //Number of consecutive no upload task runs
  u1_t 	ReQueryCnt; //Number of re query
  u1_t QueryTimeoutCnt; //Continuous query timeout times
  u1_t Uart2Sts1; //UART2 status register 1 (only saved in case of serial port receiving error)
  u1_t Uart2Sts2;//UART2 status register 2 (only saved in case of serial port receiving error)
  u1_t Resv2[19]; 
} PACKED MT_App_RunStatus;

/**************************************************************************************************
***************************************************************************************************/
typedef struct CopyRule
{
  u1_t SrcIdx;
  u1_t DesIdx;
  u1_t Len;
  u1_t SrcSelect:4;
  u1_t DesSelect:4;
  u1_t Invert:1;//
  u1_t CopyCond:3;
  u1_t GroupSize:4;
  u1_t DesTmpltIdx:6; //if the des is up buffer,DesTmpltIdx is the uptmplt index,
	//if the des is query buffer,DesTmpltIdx is the querytmplt index.
	u1_t Step:2; 
	u1_t SrcTmpltIdx:6;
	u1_t Resv1:2;
  u1_t Resv;
}PACKED MT_CopyRule;
typedef struct sIdxPara
{
  u1_t Idx:7;
  u1_t Direct:1; //0-idx is  from the first to the end,1-idx is from the end to the first.
}PACKED MT_IdxPara;
typedef struct UpPara_Header
{
  u1_t  UpTmpltLen;
  u1_t  ConfirmDuty; // could be reserved
  u1_t  UpPort;
  MT_IdxPara	CrcIdx;
  u2_t  UpPeriod;
  u1_t  CvtNum:4;
  u1_t	Type:3;//0:normal,1: no query ,2: fault deal 3:
  u1_t  txstatus:1; // the result of tx
  u1_t	CrcFuncIdx:4;
  u1_t  CopyNum:4;
  u1_t  unfixLen:1;
  u1_t  CrcEndian:1;
  u1_t  UpCond:3;//if(mt_OpCond[COND_UP]&(1<<UpCond))>0,upload data.else do not upload data.
  u1_t  RandomUp:1;//if RandomUp=1,upload random other than periodically.
  u1_t  Resv1:2;
  u1_t 	Resv2[3];
}PACKED MT_UpPara_Header;
typedef struct sUpTemplate
{
	MT_UpPara_Header Header;
	u1_t 	*Tmplt;
	u1_t	*CvtIdx;
	u1_t	*CopyIdx;
}PACKED MT_UpTmplt;
/*
////////////////////////////////////////////////////////////////////
query template
////////////////////////////////////////////////////////////////////
*/

typedef struct sQueryDevTmpltHdr //12bytes
{
  u1_t	QueryCmdLen;
  u1_t	TagNum:4;
  u1_t	Resv:4;
  MT_IdxPara QueryCrc_DesIdx;
  MT_IdxPara AckCrc_DesIdx;
  u1_t  QueryCrc_CalcIdx:4; // if value is 0xf means no CRC function, used to check the ack packet and the tx to RS-485 sub devices 
  u1_t  AckCrc_CalIdx:4; // if value is 0xf means no CRC function, used to calculate the crc of packets sent to msp
  u1_t  QueryCrc_Endian:1;
  u1_t  QueryCond:3; //0-6 if the value of mt_OpCond[QueryCond] is 0,excute the query;else do not excute the query
  u1_t  CopyNum:4;
  u1_t  AckCrc_Endian:1;
  u1_t	CvtNum:7;
  u1_t  AckLen;
  u4_t	FixedTime:27; //if FixedAcq=1,means  the dtu should acq at the moment in FixedTime(unit:ms);
  u4_t  FixedAcq:1; //0-the FixedTime is no used;1-the dtu should acq at the moment in FixedTime(unit:ms).
  u4_t	IFSelect:3; //select the interface to send the query cmd.0-uart1;1-uart2;2-spi;3-i2c;4-gpio;5-ad.7-Secondary development acquisition
  u4_t  CheckLen:1;
}PACKED MT_QueryDev_TmpltHdr;
typedef struct sQueryDevTmplt
{
  MT_QueryDev_TmpltHdr QTHdr;
  u1_t	*QueryCmd;
  u1_t	*TagIdx;
  u1_t	*CvtIdx;
  u1_t	*CopyIdx;
}PACKED MT_QueryDev_Tmplt;
typedef struct sFillWord
{
  u2_t	Idx:7;
  u2_t	Dir:1;
  u2_t	Content:8;
}PACKED MT_FillVar;
typedef union u_period
{
  u2_t Bytes;
  struct Period
  {
    u2_t Count:14; //
    u2_t Unit:2;//
  } bits;
}  UMT_Period;

typedef struct sQueryDev_Para 
{
  UMT_Period QueryPeriod;
  u1_t	QueryTmpltIdx:6;
  u1_t  Active:1;
  u1_t  MulDev_NewGrpStart:1;
  u1_t  Resv;
} MT_QueryDev_Para;
typedef struct sMT_Tag
{
  u2_t	Idx:7;	//the direction of the index start ,0-start from the first byte,1-start from the last byte
  u2_t	Dir:1;
  u2_t 	Content:8;
}PACKED MT_Tag;
typedef struct s_OperateRule
{
	
	u1_t tKeyLen;
  u1_t tableIndex;
	u1_t tValueLen;
	u1_t tkeyValueNum;  // changed according to the Operator
  u1_t Operator:6;
  u1_t tKeySigned:1;
	u1_t tValueSigned:1;
}PACKED SMT_OPRule; 
typedef struct sDataInfo
{
  u4_t BufSelect:4;
  u4_t DataLen:4;
	u4_t StartIdx:8;
  u4_t DataType:4;//0=unsigned int;1=signed int; 2=float;  
  u4_t Endian:1; //0=little-endian;1=big-endian
  u4_t Resv:3;
	u4_t TmpltIdx:6; //if BufSelect="query" or BufSelect="up",the TmpltIdx indicate which tmplt shouled be used,because there is more than one tmplt.
	u4_t Resv2:2;
}PACKED MT_DataInfo;

typedef struct s_OperateRuleb
{
	u1_t Operator:6;
	u1_t Resv2:2;
	MT_DataInfo Data;
}PACKED SMT_OPRuleB;


typedef struct sConvertRule
{
  MT_DataInfo SrcData;
  MT_DataInfo DesData;
  u1_t CvtCond:3; //0-cvt whenever;1-cvt only when query timeout;2-cvt only when query is not timeout 3~7-resv
  u1_t ActionAfterCvt:3; //0-no action;1-upload;2-upload according to the result of cvt;3-remove the querytmplt from the eventlis;4-remove the uptmplt from the eventlist 
  u1_t Resv:2;
  u1_t OpNum:3;
  u1_t Resv1:5;
  u1_t OpIdx[6];
}PACKED MT_CvtRule;

typedef struct sQueryTaskPara
{
  MT_QueryDev_Para	*QueryPara;
  MT_QueryDev_Tmplt	*Tmplt;
}PACKED MT_QueryTaskPara; //used for passing para to query task

/**************************************************************************************************
***************************************************************************************************/
typedef struct BllParaHdr
{
  u1_t	HdrStart;
  u1_t	CopyNum;
  u2_t	BllParaLen; //Total bytes of all bllpara
  u1_t 	TableNum;
  u1_t	QueryParaNum;
  u1_t	QueryTmpltNum;
  u1_t	DevAck_TagNum;
  u1_t	CvtNum;
  u1_t	UpTmpltNum;
  u1_t	CrcCalcNum; //number of calculations for crc
  u1_t	OpNum;
  u1_t  OpPadding:2;
  u1_t  Resv1:6;
  u1_t	Resv[3];
}PACKED MT_BllParaHdr;

typedef struct sBllPara
{
	MT_BllParaHdr		*BllHdr; //16
	MT_QueryDev_Para	*QueryPara; //4*number of query cmds 
	MT_Tag		*AckTag;	//2*number of tags 
	MT_CvtRule			*AckCvt;	//16*number of cvts 
	MT_CrcPara			*CrcPara;	//4*number of crc
	MT_CopyRule 		*CopyRule;
	SMT_OPRuleB		*opRule;
	u2_t 			*QueryTmpltIdx;	//relatvie addrs of QueryTmplt in BllParaBuf 8
	u2_t			*UpTmpltIdx;	//relative addrs of UpTmplt in BllParaBuf 8	
	u1_t    	* table;
}PACKED MT_BllPara;
typedef struct s_MT_edgebusRunpara
{
	u8_t    cpstatus:1;   
	u8_t    rsv1:3;
	u8_t	init4G:4;
	u8_t    rsv2:24;
	u8_t    fb_uart1:1;
	u8_t    fb_uart2:1;
	u8_t    fb_wkhost:1;
	u8_t    fb_wked:1;
	u8_t    fb_rs485:1;
	u8_t    fb_4greset:1;
	u8_t    fb_mbustxen:1;
	u8_t 	fb_ledrun:1;
	u8_t    fb_ledaux:1;
	u8_t    fb_infosend:1;
	u8_t    fb_uart2bois:1;
	u8_t    fb_uart1bois:1;
        u8_t    fb_iicpower:1;
        u8_t    fb_key:1;
        u8_t    fb_cp:2; //power controlbit,bit0:enable,bit1:valid value of power on.0-not used,1-used,output 1 when poweron,3-used,output 0 when poweron.
	u8_t    rsv4:16;
}PACKED SMT_EGBS_RUN_PARA;

typedef struct s_MT_DTUrunpara
{	
    u1_t fieldMode:1;
    u1_t RelayEnable:1;
    u1_t rsv1:6;
    u1_t wreset:1;
    u1_t wEtest:1;
    u1_t wjoinreset:1;
    u1_t wFtest:1;
    u1_t wfunction:4;
    u1_t rsv2:8;
    u1_t WakeupInEn:1; //1:enable input to wakeup our mcu
    u1_t WakeupOutEn:1;//1:enable output to wakeup customer's mcu
    u1_t rsv3:2;
    u1_t backHaul:3;
    u1_t rsv4:1;
    u1_t Baudrate;//0
    u1_t Databits:4;
    u1_t Stopbits:2;//1
    u1_t Ovenbits:2;
    u1_t keep485rx:1;
    u1_t Battery:1;
    u1_t uart1Used:1;
    u1_t RawBit:1;//1-transparent mode,send data from uart transparently by lora and send data from lora to uart.
    u1_t SwUp:1;//0-lorawan channel;1-sw channel
    u1_t TdmaBit:1;//when this bit is set,the time of uplink will be discreted by TDMA according to the deveui
    u1_t SWInfo:1;//1:module will send its information by sleep-wakeup channel when it is powered on.
    u1_t Wait60s:1;//1:module will wait 60 seconds for switching class and getting utc time.0:module will not wait. 
    u1_t confirmDuty;
    u1_t portPara;//3
    u1_t portTranparent;//5
    u2_t resetHours;
}PACKED MT_Dtu_RunPara;

typedef struct s_MT_DTUdynpara
{
	u8_t batterybase:8;//0
	u8_t utctime:32;//1-4
	u8_t temperature:16; //5-6
	u8_t batterylevel:8; //7
}PACKED MT_Dtu_DynPara;

typedef struct u_dtusysparas
{
	SMT_OTA_FIXHeader otaFixHeader;
	MT_Dtu_RunPara 	  RunPara;
	s4_t 		  timeOffset;
	MT_Dtu_DynPara 	  DynPara;
        UMT_Period QueryTimeout;
	u1_t 	   rsvB[6];
}PACKED MT_Dtu_SysParas;

typedef union uDtuWorkMode
{
  u1_t byte;
  struct sdtuworkmode
  {
    u1_t HeartEn:1;
    u1_t QueryEn:1;
    u1_t Resv:6;
  } bits;
} MT_DtuWorkMode;
typedef struct LoraFuncBit
{
  u1_t ADREn:1;
  u1_t FixedDR:1;
  u1_t Resv:6;
}PACKED MT_LoraFuncBit;
typedef struct u_dtuspropertypara
{
	u1_t SN[8];	
	u1_t HWType;
	u1_t SWversion;
	u1_t ProtocolVersion;
	MT_DtuWorkMode WorkMode;
	MT_LoraFuncBit LoraFuncBit;
	u1_t RadioBaudrate; 
	u1_t HeartPeriod[2];
	u1_t DeviceCount;
	u1_t UploadPeriod[2];
	u1_t queryWarningPeriod[2];
	u1_t Resv1[5];		// added by zhaomingfei 	
} PACKED MT_Dtu_InfoPara;
typedef struct DevReg
{
	u2_t 	ManuId;
	u2_t 	DevType;
	u4_t	DevId;
	u4_t	FmVer;
	u1_t	ProtocVer;
	u1_t	Resv[7];
}PACKED MT_DevReg;
typedef struct s_quebasetime
{
  u1_t	  Running:1;
  u1_t	  Timeout:1;
  u1_t	  LoraTx:1;
  u1_t	  ExcuteNow:1;
  u1_t	  LTC:1;
  u1_t    RandomUp:1;
  u1_t    Enable:1;
  u1_t    Excuted:1;
  u1_t    EventNO;
  u1_t 	  type; //1= lora tx event; 2=query event
  u1_t    rsv;
  void *  event;
  s8_t 	  timeTickms;
  struct  s_quebasetime * next;
}  SMT_QUE_BASE_TIME;
typedef struct s_MT_Dtu_AppPara
{
	MT_Dtu_SysParas SysPara;
	u1_t            resv[4];
	MT_Dtu_InfoPara ProductInfo;
	MT_DevReg 	DevReg[3];	
}PACKED MT_Dtu_AppPara;
typedef struct sMT_EB_Hook
{
  u1_t (* Hook_BoardInit)(void);
  u1_t (* Hook_EXTIWKIRQ)(s8_t ms);
  u1_t (* Hook_EXTI3IRQ)(s8_t ms);
  u1_t (* Hook_WKIRQ)(u1_t pin,s8_t ms);
  u1_t (* Hook_EVENTLWS)(mt_ev_t ev ,u1_t port,u1_t * Buffer, u2_t len);
  u1_t (* Hook_APPparaChaged)(void);
  u2_t (* Hook_UartUserDeal)(unsigned char * ReceiveData,unsigned short lenth,unsigned char * OutPut);
  u1_t (* Hook_FactoryInit)(void);
  u1_t (* Hook_APPInit)(void);
  void (*Hook_DataAcq)(void *task,void *p);
  void (*Hook_DataCvt)(u1_t *data,u2_t len,void *p);
  void (*Hook_DataUp)(u1_t port,u1_t *data,u2_t len);
  void (*Hook_DataDn)(u1_t port,u1_t *data,u2_t len);
  void (*Hook_UserEvent)(u1_t eventid,void *p);
}MT_EB_Hook;
/**************************************************************************************************
***************************************************************************************************/
#endif
