///////////////////////////////////////////////////////////////////////////////
////                          can-pic24_dspic33.h                          ////
////                                                                       ////
////  Driver for ECAN peripheral for Microchip's PIC24 and dsPIC33 devices ////
////  with an ECAN peripheral.                                             ////
////                                                                       ////
///////////////////////////////////////////////////////////////////////////////
////           (C) Copyright 1996,2019 Custom Computer Services            ////
////  This source code may only be used by licensed users of the CCS C     ////
////  compiler.  This source code may only be distributed to other         ////
////  licensed users of the CCS C compiler.  No other use, reproduction or ////
////  distribution is permitted without written permission.  Derivative    ////
////  programs created using this software in object code form are not     ////
////  restricted in any way.                                               ////
////                        http://www.ccsinfo.com                         ////
///////////////////////////////////////////////////////////////////////////////

/*
   By default this driver used Transmit/Receive Buffer 0 as a transmit buffer
   and all remaining as receive buffer.
   
   CAN PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN_TX_BUFFERS
      The number of buffers used for transmitting messages 0-8, 0 - no transmit
      buffers.  Default size is 1.
   
   CAN_RX_BUFFERS
      The number of buffers used for receiving messages 0-32, 0 - no receive
      buffers.  CAN_TX_BUFFERS + CAN_RX_BUFFERS must equal 4, 6, 8, 12, 16, 24
      or 32.  Default size if 32 - CAN_TX_BUFFERS.
   
   CAN_CLOCK_DIVISOR
      The amount the system clock is divided by before presented to the ECAN
      peripheral 1 or 2.  Default divisor is 2, not used on PIC24HJ and 
      dsPIC33FJ divisor is always 2 on those devices and CAN_CLOCK_DIVISOR
      should be defined as 2 for them.
      
   CAN_BAUD_RATE
      The bit rate that the can_init() function will set the ECAN peripheral
      to.  Default is 125 kbps.
   
   CAN_DEFAULT_SAMPLE_POINT
      The sample point used by the can_set_baud() function when setting the
      ECAN bit rate.  Default is 875 for 87.5% of the bit time, the sample
      point the CANopen protocol uses.
   
   OPTIONAL CAN FILTER PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN_USE_FILTERS
      Used to enable can_init() to setup CAN filters, FALSE - disabled or 
      TRUE - enabled.  Default is disabled, can_init() sets up ECAN peripheral
      to receive all messages.
   
   CAN_USER_FILT_x
      Used to set the Id of the filter, x being the filter (0-15) to assign
      the Id to.
   
   CAN_USER_FILT_x_TYPE
      Used to set the filter type, x being the filter (0-15) to assign the type
      to.  The type can be one of the following:
         CAN_FILTER_TYPE_SID - SID Filter Id
         CAN_FILTER_TYPE_EID - EID Filter Id
   
   CAN_USER_FILT_x_BUFFER
      Used to set the buffer the filter is assigned to, x being the filter
      (0-15) to assign to object.  See CAN_FILTER_BUFFER type for buffers that 
      filter can be assigned to.
   
   CAN_USER_FILT_x_MASK
      Use to set the mask used with filter, x being the filter (0-15).  See
      CAN_FILTER_MASK type for masks that filter can use.
   
   CAN_USER_MASK_y
      Used to set the Id of the mask, y being the mask (0-2) to assign the Id
      to.
   
   CAN_USER_MASK_y_ID_TYPE
      Used to set the mask type, y being the mask (0-2), to assign the type
      to.  The type can be one of the following:
         CAN_MASK_ID_TYPE_SID - SID mask Id
         CAN_MASK_ID_TYPE_EID - EID mask Id
   
   CAN_USER_MASK_y_FILTER_TYPE
      Used to set if filter accepts both SID and EID messages, or only SID or
      EID messages.  The type can be one of the following:
         CAN_FILTER_MASK_TYPE_EITHER - match both SID and EID messages
         CAN_FILTER_MASK_TYPE_SID_OR_EID - only match SID or EID messages, 
                                           determined by CAN_USER_FILT_x_TYPE
   
   OPTIONAL CAN PERIPHERAL PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN_TX_DMA_CHANNEL
      Sets the DMA channel to use for moving CAN messages from RAM to
      peripheral.  Default is channel 0.
   
   CAN_RX_DMA_CHANNEL
      Sets the DMA channel to use for moving CAN messages from peripheral to
      RAM.  Default is channel 1.
   
   CAN_BRG_SAM
      Sets how many time the CAN bus is sampled at the sample point, 0-once, 
      1-three times.  Default is 0.
   
   CAN_BRG_WAKE_FILTER
      Sets if CAN bus line filter is used for wake-up, 0-not used, 1-used.
      Default is 0.
   
   CAN_ENABLE_CAN_CAPTURE
      Enables/disables input capture based on CAN message receive, 0-disable, 
      1-enable.  Default is 0.
   
   CAN_CONTINUE_IN_IDLE
      Sets if CAN peripheral operates when in Idle mode, TRUE-continues in Idle 
      mode, FALSE-discontinued in Idle mode.  Default is FALSE.
      
-------------------------------------------------------------------------------

   OPTIONAL CAN2 PREPROCESSORS
   ----------------------------------------------------------------------------
   
   USE_CAN2_PERIPHERAL
      If device has a CAN2 peripheral this sets whether to the CAN2 functions
      will be available for use.  Default is FALSE, if device doesn't have a
      CAN2 peripheral define is forced to FALSE.
   
   If USE_CAN2_PERIPHERAL == TRUE
      CAN2_TX_BUFFERS
         The number of buffers used for transmitting messages 0-8, 0 - no 
         transmit buffers.  Default size if 1.
      
      CAN2_RX_BUFFERS
         The number of buffers used for receiving messages 0-32, 0 - no receive
         buffers.  CAN2_TX_BUFFERS + CAN2_RX_BUFFERS must equal 4, 6, 8, 12, 
         16, 24 or 32.  Default size if 32 - CAN2_TX_BUFFERS.
      
      CAN2_CLOCK_DIVISOR
         The amount the system clock is divided by before presented to the ECAN
         peripheral 1 or 2.  Default divisor is 2, not used on PIC24HJ and 
         dsPIC33FJ divisor is always 2 on those devices and CAN2_CLOCK_DIVISOR
         should be defined as 2 for them.
         
      CAN2_BAUD_RATE
         The bit rate that the can2_init() function will set the ECAN2 
         peripheral to.  Default is 125 kbps.
      
      CAN2_DEFAULT_SAMPLE_POINT
         The sample point used by the can2_set_baud() function when setting the
         ECAN2 bit rate.  Default is 875 for 87.5% of the bit time, the sample
         point the CANopen protocol uses.
      
   OPTIONAL CAN2 FILTER PREPROCESSORS:
   ----------------------------------------------------------------------------
   
   CAN2_USE_FILTERS
      Used to enable can2_init() to setup CAN2 filters, FALSE - disabled or
      TRUE - enabled.  Default is disabled, can2_init() sets up ECAN2
      peripheral to receive all messages.
   
   CAN2_USER_FILT_x
      Used to set the Id of the filter, x being the filter (0-15) to assign
      the Id to.
   
   CAN2_USER_FILT_x_TYPE
      Used to set the filter type, x being the filter (0-15) to assign the type
      to.  The type can be one of the following:
         CAN_FILTER_TYPE_SID - SID Filter Id
         CAN_FILTER_TYPE_EID - EID Filter Id
   
   CAN2_USER_FILT_x_BUFFER
      Used to set the buffer the filter is assigned to, x being the filter
      (0-15) to assign to object.  See CAN_FILTER_BUFFER type for buffers that 
      filter can be assigned to.
   
   CAN2_USER_FILT_x_MASK
      Use to set the mask used with filter, x being the filter (0-15).  See
      CAN_FILTER_MASK type for masks that filter can use.
   
   CAN2_USER_MASK_y
      Used to set the Id of the mask, y being the mask (0-2) to assign the Id
      to.
   
   CAN2_USER_MASK_y_ID_TYPE
      Used to set the mask byte, y being the mask (0-2), to assign the type
      to.  The type can be one of the following:
         CAN_MASK_ID_TYPE_SID - SID mask Id
         CAN_MASK_ID_TYPE_EID - EID mask Id
   
   CAN2_USER_MASK_y_FILTER_TYPE
      Used to set if filter accepts both SID and EID messages, or only SID or
      EID messages.  The type can be one of the following:
         CAN_FILTER_MASK_TYPE_EITHER - match both SID and EID messages
         CAN_FILTER_MASK_TYPE_SID_OR_EID - only match SID or EID messages, 
                                           determined by CAN_USER_FILT_x_TYPE
   
   OPTIONAL CAN2 PERIPHERAL PREPROCESSORS
   ----------------------------------------------------------------------------
   
   CAN2_TX_DMA_CHANNEL
      Sets the DMA channel to use for moving CAN messages from RAM to
      peripheral.  Default is channel 2.
   
   CAN2_RX_DMA_CHANNEL
      Sets the DMA channel to use for moving CAN messages from peripheral to
      RAM.  Default is channel 3.
   
   CAN2_BRG_SAM
      Sets how many time the CAN bus is sampled at the sample point, 0-once, 
      1-three times.  Default is 0.
   
   CAN2_BRG_WAKE_FILTER
      Sets if CAN bus line filter is used for wake-up, 0-not used, 1-used.
      Default is 0.
   
   CAN2_ENABLE_CAN_CAPTURE
      Enables/disables input capture based on CAN message receive, 0-disable, 
      1-enable.  Default is 0.
   
   CAN2_CONTINUE_IN_IDLE
      Sets if CAN peripheral operates when in Idle mode, TRUE-continues in Idle 
      mode, FALSE-discontinued in Idle mode.  Default is FALSE.
-------------------------------------------------------------------------------
   
*/

#ifndef __CAN_PIC24_DSPIC33_H__
#define __CAN_PIC24_DSPIC33_H__

#include <stdint.h>

// CAN Error Codes used by this driver
// 0 is OK, non-zero is some kind of error
typedef enum 
{
   CAN_EC_OK = 0,
   CAN_EC_BUFFER_RX_EMPTY,    //can_getd failed because all RX buffers are empty, or specified buffer is empty
   CAN_EC_BUFFER_TX_FULL,     //can_putd failed because all TX buffers are full, or specified buffer is full
   CAN_EC_BUFFER_NOT_TX,      //can_putd failed because buffer is not a TX buffer
   CAN_EC_BUFFER_IS_RTR,      //can_putd failed because buffer is setup as an RTR TX Buffer
   CAN_EC_BUFFER_NO_RTR,      //CAN Buffer can not be set to respond to RTR requests
   CAN_EC_BAUD_NOT_DIVISIBLE, //can_set_baud failed because clock not divisible by baud rate
   CAN_EC_BAUD_INVALID,       //can_set_baud failed because baud rate not achievable
} can_ec_t;

// CAN TX Message Header type
typedef struct
{
   uint32_t Id;               //ID to send message with
   uint8_t Length;            //Number of data bytes to send
   int1 ext;                  //TRUE - send message with extended ID, FALSE - send message with standard ID
   int1 rtr;                  //TRUE - send RTR (request), FALSE - not an RTR (request)
   uint8_t Priority:2;        //0-3, use to determine which messages will be sent fist.  The high the priority the sooner the ECAN peripheral will send the message
} CAN_TX_HEADER;

// CAN RX Message Header type
typedef struct
{
   uint32_t Id;               //ID of received message
   uint8_t Length;            //Number of data bytes received
   uint8_t Filter;            //Filter message was received with
   uint8_t Buffer;            //Buffer message was retreived from
   int1 err_ovfl;             //TRUE - Buffer overflowed, FALSE - Buffer didn't overflow
   int1 ext;                  //TRUE - received extended ID message, FALSE - received standard ID message
   int1 rtr;                  //TRUE - received RTR (request), FALSE - not a RTR (request)
} CAN_RX_HEADER;

// CAN_OP_MODE type
typedef enum 
{
   CAN_OP_NORMAL,             //Normal operation mode
   CAN_OP_DISABLE,            //Disable mode
   CAN_OP_LOOPBACK,           //Loopback mode
   CAN_OP_LISTEN,             //Listen only mode
   CAN_OP_CONFIG,             //Configuration mode
   CAN_OP_LISTEN_ALL=7        //Listen all messages mode
} CAN_OP_MODE;

// CAN_FILTER_MASK type
typedef enum
{
   CAN_FILTER_MASK_0,
   CAN_FILTER_MASK_1,
   CAN_FILTER_MASK_2
} CAN_FILTER_MASK;

// CAN_MASK_ID_TYPE type
typedef enum {
   CAN_MASK_ID_TYPE_SID,   //Mask Id is a SID Id - only SID bits of Mask are written, EID bits are set to 0
   CAN_MASK_ID_TYPE_EID    //Mask Id is an EID Id - SID and EID bits of Mask are written
} CAN_MASK_ID_TYPE;

// CAN_FILTER_MASK_TYPE type
typedef enum {
   CAN_FILTER_MASK_TYPE_EITHER,     //match either SID or EID messages, ignores EXIDE bit of filter
   CAN_FILTER_MASK_TYPE_SID_OR_EID  //only match SID or EID messages, EXIDE bit of filter determines type of messages received
} CAN_FILTER_MASK_TYPE;

// CAN_FILTER type
typedef enum {CAN_FILTER_0, CAN_FILTER_1, CAN_FILTER_2, CAN_FILTER_3, 
              CAN_FILTER_4, CAN_FILTER_5, CAN_FILTER_6, CAN_FILTER_7,
              CAN_FILTER_8, CAN_FILTER_9, CAN_FILTER_10, CAN_FILTER_11,
              CAN_FILTER_12, CAN_FILTER_13, CAN_FILTER_14, CAN_FILTER_15
} CAN_FILTER;

// CAN_FILTER_TYPE type
typedef enum {
   CAN_FILTER_TYPE_SID,    //Filter Id is a SID Id - only SID bits of filter are written, EID bits are set to 0
   CAN_FILTER_TYPE_EID     //Filter Id is an EID Id - SID and EID bits of Mask are written
} CAN_FILTER_TYPE;

// CAN_BUFFER type
typedef enum { CAN_BUFFER_0, CAN_BUFFER_1, CAN_BUFFER_2, CAN_BUFFER_3, CAN_BUFFER_4, CAN_BUFFER_5, CAN_BUFFER_6, CAN_BUFFER_7,
               CAN_BUFFER_8, CAN_BUFFER_9, CAN_BUFFER_10, CAN_BUFFER_11, CAN_BUFFER_12, CAN_BUFFER_13, CAN_BUFFER_14, CAN_BUFFER_15,
               CAN_BUFFER_16, CAN_BUFFER_17, CAN_BUFFER_18, CAN_BUFFER_19, CAN_BUFFER_20, CAN_BUFFER_21, CAN_BUFFER_22, CAN_BUFFER_23,
               CAN_BUFFER_24, CAN_BUFFER_25, CAN_BUFFER_26, CAN_BUFFER_27, CAN_BUFFER_28, CAN_BUFFER_29, CAN_BUFFER_30, CAN_BUFFER_31
} CAN_BUFFER;

// CAN_FILTER_BUFFER type
typedef enum {CAN_FILTER_BUFFER_0, CAN_FILTER_BUFFER_1, CAN_FILTER_BUFFER_2, CAN_FILTER_BUFFER_3,
              CAN_FILTER_BUFFER_4, CAN_FILTER_BUFFER_5, CAN_FILTER_BUFFER_6, CAN_FILTER_BUFFER_7,
              CAN_FILTER_BUFFER_8, CAN_FILTER_BUFFER_9, CAN_FILTER_BUFFER_10, CAN_FILTER_BUFFER_11,
              CAN_FILTER_BUFFER_12, CAN_FILTER_BUFFER_13, CAN_FILTER_BUFFER_14, CAN_FILTER_BUFFER_FIFO              
} CAN_FILTER_BUFFER;

// CAN_INTERRUPT type
typedef enum
{
   CAN_INTERRUPT_TX = 0x01,         //TX Buffer Interrupt
   CAN_INTERRUPT_RX = 0x02,         //RX Buffer Interrupt
   CAN_INTERRUPT_RXOV = 0x04,       //RX Buffer Overflow Interrupt
   CAN_INTERRUPT_FIFO = 0x08,       //FIFO Almost Full Interrupt
   CAN_INTERRUPT_ERR = 0x20,        //Error Interrupt
   CAN_INTERRUPT_WAKE = 0x40,       //Bus Wake-up Activity Interrupt
   CAN_INTERRUPT_INVALID = 0x80     //Invalid Message Interrupt
} CAN_INTERRUPT;

#define CAN_TX_BUFFER_ANY     0xFF

#define CAN_RX_BUFFER_ANY     0xFF
#define CAN_RX_BUFFER_FIFO    0xFE

#define CAN_ABORT_ALL         0xFF

//////////////////////////////// CAN Prototypes ///////////////////////////////

// Initialize the CAN peripheral.  After initializing the peripheral it will be
// put into 'Mode'.
void can_init(CAN_OP_MODE Mode=CAN_OP_NORMAL);

// Sets the CAN bit rate of the device.  'Clock' is the CAN clock speed that is
// presented to the ECAN peripheral.  'Rate' is the bit rate to use.  The math
// requires that clock be evenly divisible by the bit rate.
can_ec_t can_set_baud(uint32_t Clock, uint32_t Rate);

// Sets the operational mode of the ECAN peripheral.  'Mode' is the operational
// mode to set.  'Update' specifies whether to update a global variable used to
// save the operation mode, which is used by some functions to return the
// operational mode back to it's previous operating mode.
void can_set_mode(CAN_OP_MODE Mode, int1 Update=TRUE);

// Sets up the specified Filter Mask's Id, Id Type and mask type.  'Mask' is 
// the Filter Mask to setup (0-2), 'Id' is the mask to assign to the filter
// mask, 'iType' is the Id type of the filter mask, see CAN_MASK_ID_TYPE above
// for what it can be set to and 'fType' sets the filter mask type, see 
// CAN_FILTER_MASK_TYPE above for types it can be set to.
void can_set_mask_id(CAN_FILTER_MASK Mask, uint32_t Id, CAN_MASK_ID_TYPE iType, CAN_FILTER_MASK_TYPE fType);

// Sets up the specified Filter's Id, and Type.  'Filter' is the filter to
// setup (0-15), 'Id' is the Id to assign to the filter and 'Type' is the Id
// type of the filter, see CAN_FILTER_TYPE above for type it can be set to.
void can_set_filter_id(CAN_FILTER Filter, uint32_t Id, CAN_FILTER_TYPE Type);

// Sets the number of DMA buffers to use with the ECAN peripheral.  'size' is
// number of DMA buffers to set, valid sizes are 4, 6, 8, 12, 16, 24, and 32.
void can_set_buffer_size(uint8_t size);

// Sets the start of the FIFO RX buffer.  'buffer', is the buffer were the FIFO
// RX buffer starts.  By default the can_enable_b_receiver() function and the
// can_enable_b_transfer() function, which is called by the can_init()
// function, sets the FIFO RX buffer to the first available RX buffer.  This is
// used in case were a filter is setup to send valid messages it receives to a
// specific buffer, so the FIFO start can be set to after that buffer.
void can_set_fifo_start_buffer(CAN_BUFFER Buffer);

// Used to enable specified filter, assigned it a specific buffer and set which
// mask to use with the filter.  'Filter' is the filter to enabled (0-15), 
// 'Buffer' is buffer to load matched messages to (0-14 and 
// CAN_FILTER_BUFFER_FIFO) and 'Mask' is the filer mask to use with filter 
// (0-2).
void can_enable_filter(CAN_FILTER Filter, CAN_FILTER_BUFFER Buffer, CAN_FILTER_MASK Mask);

// Used to disable a specified filter.  'Filter is the filter to disable 
// (0-15).
void can_disable_filter(CAN_FILTER Filter);

// Used to load a message to transmit into a TX buffer.  'Header' is a pointer
// to CAN_TX_HEADER type with info to send message with, Id, Data Length, etc.,
// 'Data' is pointer to the payload to send with message, and 'Buffer' is an 
// optional parameter specifying the TX buffer to load the message into.  By 
// default it loads the messages into the first available TX buffer.
can_ec_t can_putd(CAN_TX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY);

// Used to retrieve a received message from a RX buffer.  'Header' is a pointer
// to a CAN_RX_HEADER type variable to save the received message's header info
// to, Id, Data length, etc., 'Data' is a pointer to an array to save the
// received messages's payload to, and 'Buffer' is an optional parameter
// specifying the RX buffer to retrieve message from.  By default it retrieves
// the messages from the RX FIFO buffer.
can_ec_t can_getd(CAN_RX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer = CAN_RX_BUFFER_FIFO);

// Used to determine if there is a least one message to retrieve from a RX
// buffer.  Returns TRUE if there is a message to retrieve and FALSE if there
// is no message to retrieve.  'Buffer' is an optional parameter specifying the
// RX buffer to check, by default it checks the RX FIFO buffer.
int1 can_kbhit(CAN_BUFFER Buffer = CAN_RX_BUFFER_FIFO);

// Used to determine if the specified TX buffer is free to accept a new message
// to transmit.  Returns TRUE if buffer is free and FALSE if not.  'Buffer' is
// an optional parameter for specifying the TX buffer to check.  Default is
// CAN_TX_BUFFER_ANY, which causes to check all buffers and returns TRUE if at
// least one TX buffer is free.
int1 can_tbe(CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY);

// Used to determine if the specified TX buffer is empty, has no message to
// send.  Returns TRUE if TX buffer is empty, FALSE if it has a message to 
// send.  'Buffer' is an optional parameter for specifying the TX buffer to
// check.  Default is CAN_TX_BUFFER_ANY, which means all TX buffers must be
// empty for it to return TRUE.  If value other then CAN_TX_BUFFER_ANY is used
// then this function is equivalent to can_tbe().
int1 can_tx_empty(CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY);

// Used to abort pending transmissions for the specified buffer.  'Buffer' is
// an optional parameter specifying the TX Buffer to abort transmission for.  
// Default is CAN_ABORT_ALL which causes it to abort all pending transmissions
// for all TX buffers.
void can_abort(CAN_BUFFER Buffer = CAN_ABORT_ALL);

// Used to enable one of the TX/RX buffers as a TX buffer.  'Buffer' the TX/RX
// buffer to make a TX buffer (0-7).  This function is called by can_init()
// function based on the CAN_TX_BUFFERS define, can be used to change a TX/RX 
// buffer to a TX buffer at run-time.  This function also sets the start of the
// RX FIFO buffer to the first available buffer.
void can_enable_b_transfer(CAN_BUFFER Buffer);

// Used to enable one of the TX/RX buffers as a RX buffer.  'Buffer' the TX/RX
// buffer to make a RX buffer (0-7).  By default all TX/RX buffers are RX 
// buffers, can be used to make a buffer that was previously made TX buffer
// a RX buffer at run-time.  This function also sets the start of the RX FIFO
// buffer to the first available buffer.
void can_enable_b_receiver(CAN_BUFFER Buffer);

// Used to enable RTR messages on a TX buffer.
can_ec_t can_enable_rtr(CAN_BUFFER Buffer);

// Used to disable RTR messages on a TX buffer.  'Buffer' is the buffer to
// disable RTR messages on.
can_ec_t can_disable_rtr(CAN_BUFFER Buffer);

// Used to load a message into a TX buffer that will be transmitted when a
// remote transmission request is received.  'Header' is a pointer to a
// CAN_TX_HEADER structure containing info about the message to transmit, Id,
// Data length, etc. and 'Data' is a pointer to an array contain payload of
// message to transmit and 'Buffer' is the buffer to load the message into.
can_ec_t can_load_rtr(CAN_BUFFER Buffer, CAN_TX_HEADER *Header, uint8_t *Data);

// Used to enable one or more of the CAN interrupts.  'Setting' is the CAN
// interrupt to enable, multiple interrupts can be or'ed together to enable
// multiple CAN interrupts.
void can_enable_interrupts(CAN_INTERRUPT Setting);

// Used to disable one or more of the CAN interrupts.  'Setting' is the CAN
// interrupt to disable, multiple interrupts can be or'ed together to disable
// multiple CAN interrupts.
void can_disable_interrupts(CAN_INTERRUPT Setting);

// Used to determine which CAN interrupts are active.  Returns CAN_INTERRUPT 
// type, if an interrupt is active the corresponding bit will be set.  See
// CAN_INTERRUPT type to determine which bits corresponds to which interrupt.
CAN_INTERRUPT can_interrupts_active(void);

// Used to determine if specified CAN interrupt is active.  Returns TRUE if
// active, FALSE if not active.  'Interrupt' is the interrupt to test for.
int1 can_interrupt_active(CAN_INTERRUPT Interrupt);

// Used to clear specified CAN interrupt flag.  'Interrupt' is the interrupt
// to clear.
void can_clear_interrupt(CAN_INTERRUPT Interrupt);

/////////////////////////////// CAN2 Prototype ////////////////////////////////

#if getenv("SFR_VALID:C2CTRL1") == FALSE
 #if defined(USE_CAN2_PERIPHERAL) && (USE_CAN2_PERIPHERAL == TRUE)
  #undef USE_CAN2_PERIPHERAL
 #endif
#endif

#ifndef USE_CAN2_PERIPHERAL
 #define USE_CAN2_PERIPHERAL        FALSE
#endif

#if USE_CAN2_PERIPHERAL == TRUE
 // Initialize the CAN2 peripheral.  After initializing the peripheral it will be
 // put into 'Mode'.
 void can2_init(CAN_OP_MODE Mode=CAN_OP_NORMAL);

 // Sets the CAN2 bit rate of the device.  'Clock' is the CAN clock speed that is
 // presented to the ECAN2 peripheral.  'Rate' is the bit rate to use.  The math
 // requires that clock be evenly divisible by the bit rate.
 can_ec_t can2_set_baud(uint32_t Clock, uint32_t Rate);

 // Sets the operational mode of the ECAN2 peripheral.  'Mode' is the operational
 // mode to set.  'Update' specifies whether to update a global variable used to
 // save the operation mode, which is used by some functions to return the
 //operational mode back to it's previous operating mode.
 void can2_set_mode(CAN_OP_MODE Mode, int1 Update=TRUE);

 // Sets up the specified Filter Mask's Id, Id Type and mask type.  'Mask' is 
 // the Filter Mask to setup (0-2), 'Id' is the mask to assign to the filter
 // mask, 'iType' is the Id type of the filter mask, see CAN_MASK_ID_TYPE above
 // for what it can be set to and 'fType' sets the filter mask type, see 
 // CAN_FILTER_MASK_TYPE above for types it can be set to.
 void can2_set_mask_id(CAN_FILTER_MASK Mask, uint32_t Id, CAN_MASK_ID_TYPE iType, CAN_FILTER_MASK_TYPE fType);

 // Sets up the specified Filter's Id, and Type.  'Filter' is the filter to
 // setup (0-15), 'Id' is the Id to assign to the filter and 'Type' is the Id
 // type of the filter, see CAN_FILTER_TYPE above for type it can be set to.
 void can2_set_filter_id(CAN_FILTER Filter, uint32_t Id, CAN_FILTER_TYPE Type);

 // Sets the number of DMA buffers to use with the ECAN2 peripheral.  'size' is
 // number of DMA buffers to set, valid sizes are 4, 6, 8, 12, 16, 24, and 32.
 void can2_set_buffer_size(uint8_t size);

 // Sets the start of the FIFO RX buffer.  'buffer', is the buffer were the FIFO
 // RX buffer starts.  By default the can2_enable_b_receiver() function and the
 // can2_enable_b_transfer() function, which is called by the can2_init()
 // function, sets the FIFO RX buffer to the first available RX buffer.  This is
 // used in case were a filter is setup to send valid messages it receives to a
 // specific buffer, so the FIFO start can be set to after that buffer.
 void can2_set_fifo_start_buffer(CAN_BUFFER buffer);

 // Used to enable specified filter, assigned it a specific buffer and set which
 // mask to use with the filter.  'Filter' is the filter to enabled (0-15), 
 // 'Buffer' is buffer to load matched messages to (0-14 and 
 // CAN_FILTER_BUFFER_FIFO) and 'Mask' is the filer mask to use with filter 
 // (0-2).
 void can2_enable_filter(CAN_FILTER Filter, CAN_FILTER_BUFFER Buffer, CAN_FILTER_MASK Mask);

 // Used to disable a specified filter.  'Filter is the filter to disable 
 // (0-15).
 void can2_disable_filter(CAN_FILTER Filter);

 // Used to load a message to transmit into a TX buffer.  'Header' is a pointer
 // to CAN_TX_HEADER type with info to send message with, Id, Data Length, etc.,
 // 'Data' is pointer to the payload to send with message, and 'Buffer' is an 
 // optional parameter specifying the TX buffer to load the message into.  By 
 // default it loads the messages into the first available TX buffer.
 can_ec_t can2_putd(CAN_TX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY);

 // Used to retrieve a received message from a RX buffer.  'Header' is a pointer
 // to a CAN_RX_HEADER type variable to save the received message's header info
 // to, Id, Data length, etc., 'Data' is a pointer to an array to save the
 // received messages's payload to, and 'Buffer' is an optional parameter
 // specifying the RX buffer to retrieve message from.  By default it retrieves
 // the messages from the RX FIFO buffer.
 can_ec_t can2_getd(CAN_RX_HEADER *Header, uint8_t *Data, CAN_BUFFER Buffer = CAN_RX_BUFFER_FIFO);

 // Used to determine if there is a least one message to retrieve from a RX
 // buffer.  Returns TRUE if there is a message to retrieve and FALSE if there
 // is no message to retrieve.  'Buffer' is an optional parameter specifying the
 // RX buffer to check, by default it checks the RX FIFO buffer.
 int1 can2_kbhit(CAN_BUFFER Buffer = CAN_RX_BUFFER_FIFO);
 
 // Used to determine if the specified TX buffer is free to accept a new message
 // to transmit.  Returns TRUE if buffer is free and FALSE if not.  'Buffer' is
 // an optional parameter for specifying the TX buffer to check.  Default is
 // CAN_TX_BUFFER_ANY, which causes to check all buffers and returns TRUE if at
 // least one TX buffer is free.
 int1 can2_tbe(CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY);

 // Used to determine if the specified TX buffer is empty, has no message to
 // send.  Returns TRUE if TX buffer is empty, FALSE if it has a message to 
 // send.  'Buffer' is an optional parameter for specifying the TX buffer to
 // check.  Default is CAN_TX_BUFFER_ANY, which means all TX buffers must be
 // empty for it to return TRUE.  If value other then CAN_TX_BUFFER_ANY is used
 // then this function is equivalent to can_tbe().
 int1 can2_tx_empty(CAN_BUFFER Buffer = CAN_TX_BUFFER_ANY);

 // Used to abort pending transmissions for the specified buffer.  'Buffer' is
 // an optional parameter specifying the TX Buffer to abort transmission for.  
 // Default is CAN_ABORT_ALL which causes it to abort all pending transmissions
 // for all TX buffers.
 void can2_abort(CAN_BUFFER Buffer = CAN_ABORT_ALL);

 // Used to enable one of the TX/RX buffers as a TX buffer.  'Buffer' the TX/RX
 // buffer to make a TX buffer (0-7).  This function is called by can_init()
 // function based on the CAN2_TX_BUFFERS define, can be used to change a TX/RX 
 // buffer to a TX buffer at run-time.  This function also sets the start of the
 // RX FIFO buffer to the first available buffer.
 void can2_enable_b_transfer(CAN_BUFFER Buffer);

 // Used to enable one of the TX/RX buffers as a RX buffer.  'Buffer' the TX/RX
 // buffer to make a RX buffer (0-7).  By default all TX/RX buffers are RX 
 // buffers, can be used to make a buffer that was previously made TX buffer
 // a RX buffer at run-time.  This function also sets the start of the RX FIFO
 // buffer to the first available buffer.
 void can2_enable_b_receiver(CAN_BUFFER Buffer);

 // Used to enable RTR messages on a TX buffer.  'Buffer' is the buffer to 
 // enable RTR messages on.
 can_ec_t can2_enable_rtr(CAN_BUFFER Buffer);

 // Used to disable RTR messages on a TX buffer.  'Buffer' is the buffer to
 // disable RTR messages on.
 can_ec_t can2_disable_rtr(CAN_BUFFER Buffer);

 // Used to load a message into a TX buffer that will be transmitted when a
 // remote transmission request is received.  'Buffer' is the buffer to load the
 // message into, 'Header' is a pointer to a CAN_TX_HEADER structure containing
 // info about the message to transmit, Id, Data length, etc. and 'Data' is a
 // pointer to an array contain payload of message to transmit.
can_ec_t can2_load_rtr(CAN_BUFFER Buffer, CAN_TX_HEADER *Header, uint8_t *Data);

 // Used to enable one or more of the CAN interrupts.  'Setting' is the CAN
 // interrupt to enable, multiple interrupts can be or'ed together to enable
 // multiple CAN interrupts.
 void can2_enable_interrupts(CAN_INTERRUPT Setting);

 // Used to disable one or more of the CAN interrupts.  'Setting' is the CAN
 // interrupt to disable, multiple interrupts can be or'ed together to disable
 // multiple CAN interrupts.
 void can2_disable_interrupts(CAN_INTERRUPT Setting);

 // Used to determine which CAN interrupts are active.  Returns CAN_INTERRUPT 
 // type, if an interrupt is active the corresponding bit will be set.  See
 // CAN_INTERRUPT type to determine which bits corresponds to which interrupt.
 CAN_INTERRUPT can2_interrupts_active(void);

 // Used to determine if specified CAN interrupt is active.  Returns TRUE if
 // active, FALSE if not active.  'Interrupt' is the interrupt to test for.
 int1 can2_interrupt_active(CAN_INTERRUPT Interrupt);

 // Used to clear specified CAN interrupt flag.  'Interrupt' is the interrupt 
 // to clear.
 void can2_clear_interrupt(CAN_INTERRUPT Interrupt);

#endif
#endif   //__CAN_PIC24_DSPIC33_H__
