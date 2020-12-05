#ifndef _CRCCAL_H
#define	_CRCCAL_H
#include "MP_Global.h"
#define CRC16 0
#define CCITT16 1
#define CRCSUM 2
#define NOCRC 0xF

typedef struct CrcPara
{
  u4_t	Para:16;
  u4_t	StartIdx:5;
  u4_t	EndIdx:5;
  u4_t	Mode:4;	 //0-Crc16,1-CCITT16,2-SUM8,F-NONE
  u4_t	Crclen:2;
}PACKED MT_CrcPara;
extern u4_t  CrcCal(u1_t *buf,u2_t lenth,MT_CrcPara *crcpara);
extern void SetCrc(u1_t *src,u1_t len,MT_CrcPara *crcpara,u1_t endian,u1_t direct,u1_t idx);
extern u4_t	CrcCal_hook(u1_t crcmode, u1_t * buf, u2_t lenth, u2_t poly);
#endif