#include <LPC21xx.h>
#include "types.h"
#include "defines.h"
#include "can_defines.h"
#include "delay.h"
typedef struct CAN_Frame
{
u32 ID;
struct BitField
{
u32 RTR : 1;
u32 DLC : 4;
}BFV;
u32 Data1,Data2;
}CANF;
void Init_CAN1(void)
{
PINSEL1|=0x00040000;
SETBIT(C1MOD,RM_BIT);
CLRBIT(AFMR,AccOFF_BIT);
SETBIT(AFMR,AccBP_BIT);
C1BTR=BTR_LVAL;
CLRBIT(C1MOD,RM_BIT);
}
void CAN1_Tx(CANF txFrame)
{
while(READBIT(C1GSR,TBS1_BIT)==0);
C1TID1=txFrame.ID;
C1TFI1=(txFrame.BFV.RTR<<RTR_BIT)|
(txFrame.BFV.DLC<<DLC_BITS);
if(txFrame.BFV.RTR!=1)
{
C1TDA1= txFrame.Data1;
C1TDB1= txFrame.Data2;
}
C1CMR|=1<<STB1_BIT|1<<TR_BIT;
while(READBIT(C1GSR,TCS1_BIT)==0);
}
void CAN1_Rx(CANF *rxFrame)
{
while(READBIT(C1GSR,RBS_BIT)==0);
rxFrame->ID=C1RID;
rxFrame->BFV.RTR=(C1RFS>>RTR_BIT)&1;
rxFrame->BFV.DLC=(C1RFS>>DLC_BITS)&0x0f;
if(rxFrame->BFV.RTR==0)
{
rxFrame->Data1=C1RDA;
rxFrame->Data2=C1RDB;
}
SETBIT(C1CMR,RRB_BIT);
}


