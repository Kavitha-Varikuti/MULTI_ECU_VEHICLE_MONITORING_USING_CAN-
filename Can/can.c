#include <LPC21xx.h>                    // Includes LPC21xx microcontroller register definitions
#include "types.h"                      // Includes user-defined data types such as u32
#include "defines.h"                    // Includes macros such as SETBIT(), CLRBIT(), READBIT()
#include "can_defines.h"                // Includes CAN-related bit definitions and constants
#include "delay.h"                      // Includes delay functions

typedef struct CAN_Frame                // Defines a structure named CAN_Frame for CAN messages
{
    u32 ID;                              // Stores the CAN message identifier

    struct BitField                     // Defines a structure for CAN control bit fields
    {
        u32 RTR : 1;                     // RTR bit: 1 bit, identifies data frame or remote frame
        u32 DLC : 4;                     // DLC field: 4 bits, specifies number of data bytes
    }BFV;                                // Creates a variable BFV for the BitField structure

    u32 Data1,Data2;                    // Stores CAN data bytes/words
}CANF;                                  // CANF becomes an alias for struct CAN_Frame


void Init_CAN1(void)                    // Function to initialize CAN1
{
    PINSEL1|=0x00040000;                // Configures the required LPC21xx pins for CAN1 function

    SETBIT(C1MOD,RM_BIT);               // Sets Reset Mode bit of CAN1 to enter reset/configuration mode

    CLRBIT(AFMR,AccOFF_BIT);            // Enables acceptance filter operation

    SETBIT(AFMR,AccBP_BIT);             // Bypasses acceptance filter so received messages are accepted

    C1BTR=BTR_LVAL;                     // Configures CAN1 bit timing and baud rate

    CLRBIT(C1MOD,RM_BIT);               // Clears Reset Mode bit and starts CAN1 operation
}


void CAN1_Tx(CANF txFrame)              // Function to transmit a CAN frame through CAN1
{
    while(READBIT(C1GSR,TBS1_BIT)==0);  // Waits until CAN1 transmit buffer 1 is available

    C1TID1=txFrame.ID;                  // Loads CAN message ID into Transmit Identifier Register

    C1TFI1=(txFrame.BFV.RTR<<RTR_BIT)|
           (txFrame.BFV.DLC<<DLC_BITS);
                                        // Places RTR and DLC values into Transmit Frame Information Register

    if(txFrame.BFV.RTR!=1)              // Checks whether the frame is a data frame
    {
        C1TDA1=txFrame.Data1;            // Loads first data word into CAN transmit data register

        C1TDB1=txFrame.Data2;            // Loads second data word into CAN transmit data register
    }

    C1CMR|=1<<STB1_BIT|1<<TR_BIT;       // Selects transmit buffer 1 and requests CAN transmission

    while(READBIT(C1GSR,TCS1_BIT)==0);  // Waits until CAN transmission is successfully completed
}


void CAN1_Rx(CANF *rxFrame)             // Function to receive a CAN frame through CAN1
{
    while(READBIT(C1GSR,RBS_BIT)==0);   // Waits until a CAN message is available in the receive buffer

    rxFrame->ID=C1RID;                  // Reads received CAN identifier into rxFrame

    rxFrame->BFV.RTR=(C1RFS>>RTR_BIT)&1;
                                        // Extracts RTR bit from Receive Frame Status register

    rxFrame->BFV.DLC=(C1RFS>>DLC_BITS)&0x0f;
                                        // Extracts the 4-bit DLC value from Receive Frame Status register

    if(rxFrame->BFV.RTR==0)              // Checks whether the received frame is a data frame
    {
        rxFrame->Data1=C1RDA;            // Reads first received data word

        rxFrame->Data2=C1RDB;            // Reads second received data word
    }

    SETBIT(C1CMR,RRB_BIT);              // Releases the CAN receive buffer after reading the message
}