#include "types.h"                    // Includes user-defined data types such as u32

typedef struct CAN_Frame              // Defines a structure named CAN_Frame
{
    u32 ID;                           // Stores the CAN message identifier

    struct BitField                   // Defines a structure for CAN control fields
    {
        u32 RTR : 1;                  // RTR = Remote Transmission Request; occupies 1 bit
                                       // 0 → Data frame
                                       // 1 → Remote frame

        u32 DLC : 4;                  // DLC = Data Length Code; occupies 4 bits
                                       // Specifies the number of data bytes in the CAN frame

    }BFV;                             // BFV is the variable name for the BitField structure

    u32 Data1,Data2;                  // Stores the CAN message data

    //u8 Data[8];                     // Alternative: array for storing 8 CAN data bytes
                                       // Currently commented out

}CANF;                                // CANF is an alias for struct CAN_Frame


void Init_CAN1(void);                 // Function prototype for initializing CAN1

void CAN1_Tx(CANF txFrame);           // Function prototype for transmitting a CAN frame
                                       // txFrame contains ID, RTR, DLC and data

void CAN1_Rx(CANF *rxFrame);          // Function prototype for receiving a CAN frame
                                       // Pointer is used so received data can be stored in rxFrame