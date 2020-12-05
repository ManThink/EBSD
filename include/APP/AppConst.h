#ifndef _APPCONST_H
#define	_APPCONST_H

#define BLLPARA_START	0
#define BLLPARA_LEN 2048
#define QUERY_RANDOM_LEN 128
#define UP_LORA_RANDOM_LEN 255
#define PERIOD_FOREVER 0x1FA0

#define BLLPARA_HDR 0x55
#define BLLPARA_TAIL   0xAA
#define UPPARA_STARTTAG  0xAA
#define UPPARA_TAIL    0x20
#define MAX_QUERYPARA	18 
#define MAX_UPTEMPLT	8

#define MAX_REQUERY 3
#define MAX_TIMEOUTRESET 15

#define MAX_UPTEMPLATE  4
#define MAX_DNWRDTU     4

#define UPCH_LORA  0
#define UPCH_SW 1

#define CVT_NONE 0

#define RdDevAck	0
#define WrDevAck	1
#define RdDtu	2
#define WrDtu	3

#define  UPMASKBITS	128

#define SECTION_LEN 224		//设置参数超过256个字节时，需要使用段+偏移地址的方式读写参数。参数在flash中的实际位置=section*SECTION_LEN+reg_addr;

#define	DNDES_DEV 0 //下行数据流向DTU下面的设备
#define DNDES_DTU	1	//下行数据流向DTU

#define UPDES_LTC	0
#define UPDES_LTU	1
#define UPDES_SWTX	2

#define TIME_UNIT_SECOND 0
#define TIME_UNIT_MINUTE 1
#define TIME_UNIT_HOUR   2
#define TIME_UNIT_DAY    3

#define OPERATOR_MASK   0x1F
#define OPERATOR_MULTI  0 //乘
#define OPERATOR_DIV	1 //除
#define OPERATOR_ADD	2 //加
#define OPERATOR_SUB	3 //减
#define OPERATOR_AND	4 //与
#define OPERATOR_OR		5 //或
#define OPERATOR_EXP	6 //指数次方
#define OPERATOR_NOT	7
#define OPERATOR_LMOV	8
#define OPERATOR_RMOV	9
#define OPERATOR_SEARCH 10
#define OPERATOR_NOTEQUAL 	11
#define OPERATOR_ISEQUAL    	12
#define	OPERATOR_SMALLER	13
#define OPERATOR_GREATER	14
#define OPERATOR_INRANGE	15
#define OPERATOR_OUTRANGE 	16
#define OPERATOR_DIC		17
#define OPERATOR_MOD		18
#define OPERATOR_XOR		19
#define OPERATOR_NXOR   20
#define OPERATOR_SET    21
#define OPERATOR_NULL		63

#define COND_COPY 0
#define COND_CVT  1
#define COND_QUERY 2
#define COND_UP   3

#define OP_ANYWAY	7
#define OP_AIM_BUFFER	0
#define OP_AIM_DYN_LEN  1

#define RULE_OP_ANYWAY 0
#define RULE_OP_ONTIME  1
#define RULE_OP_TIMEOUT 2

#define RULE_OP_IDX_TIMEOUT 	(0x80)
#define RULE_OP_IDX_ALL			(0x40)
#define RULE_OP_IDX_MASK		(0x3F)

#define COPY_PARA_RSV_TIMEOUT		(0x08)


#define PORT_SWINTER		220
#define PORT_SWINT_SLAVETX  219
#define PORT_SWEX			215
#define PORT_SUCESS			211
#define PORT_FAILED			212
#define PORT_MP_FUOTA       213
#define PORT_MP_PARA        214
#define PORT_MP_GBROAD		215
#define PORT_FUOTA 			201

#define IF_UART1 0
#define IF_UART2 1
#define IF_SPI   2
#define IF_I2C   3
#define IF_GPIO  4
#define IF_AD    5

 #endif