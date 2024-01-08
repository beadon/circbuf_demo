#include "canbus.h"

#include "circbuf.h"  // this stays here, don't move me, preprocessors work hard
#include "circbuf.c"  // this stays here, don't move me, preprocessors work hard

//#include "util.h"  // revist safe array copying.

// CCS C requires different headers for send and receive
// Hence the need for 2 frame types
typedef struct
{
   CAN_RX_HEADER header;   // see the CAN oject definition in can-pic18_fd.h
                           //   length of data is in his header object
                           //   and is size uint8_t -- 1 byte
   uint8_t data[8];        // data storage for message
   can_ec_t errors;        // error codes for message
   uint32_t counter;        // tracking num. via STBoard.can_msg_rx counter.
} can_rx_frame_t;

typedef struct
{
   CAN_TX_HEADER header;    // see the CAN oject definition in can-pic18_fd.h
                            //   length of the data is in this header object
   uint8_t data[8];         // data storage message
   can_ec_t errors;        // error codes for message
   uint32_t counter;        // tracking num. via STBoard.can_msg_rx counter.
} can_tx_frame_t;

CIRCBUF_DEF(can_rx_frame_t, rx_ring_buf, 32 );  // circular buffer 32 in size
CIRCBUF_DEF(can_tx_frame_t, tx_ring_buf, 32 );  // circular buffer 32 in size

// This interrupt is triggered when a message is received to CAN
#INT_C1RX
void can_rx_isr()
{
   if ( !can_kbhit() ) { return; } // bail out, something grabbed our bytes before we could get to it.
   
   STBoard.can_msg_rx++;
   
   can_rx_frame_t temp_my_can_msg;
   uint8_t *pdata = &temp_my_can_msg.data;
   temp_my_can_msg.counter = STBoard.can_msg_rx;
   
   can_ec_t ret;
   ret = can_getd(&temp_my_can_msg.header, pdata, CAN_OBJECT_FIFO_1);
   temp_my_can_msg.errors = ret;
  
   /* WARNING- Compiler / Debugger Quirk 
    * The size of the stored data in data[i] is 2 bytes
    * the debugger only shows 1 byte
    */
   if (rx_ring_buf_push_refd(&temp_my_can_msg))
   {
      fprintf(RS232_U1,
         "[%8Ld]:CAN:"
         "ERROR: Out of space in RX Ring Buffer!"
         "\n\r",
         *STBoard.milliseconds
      );
      // Errors - find a better place for errors
    }
   // No Errors
}


void can_handle_err ( can_ec_t err )
{
     // Disable printf statement to not run during an ISR,
     // compiler correctly warns when enabling this.
     fprintf(RS232_U1,
       "[%8Ld]:CAN:"
       "Received message level ERROR [%d]:"
       "\r\n",
       *STBoard.milliseconds,
       err
     );
}


static inline char hex_nibble (uint8_t bin)
{
  return "0123456789ABCDEF"[bin];
}

// speedy pack routines

//!Big endian ("network order"):
//!


//!a[0] = v >> 24;
//!a[1] = v >> 16;
//!a[2] = v >>  8;
//!a[3] = v;

//!Little endian
//!a[0] = v;
//!a[1] = v >>  8;
//!a[2] = v >> 16;
//!a[3] = v >> 24;

/* WARNING: Quirk Compiler behavior
 * Right shifts (>>) of signed integers does not shift in the sign 
 * (so 0xF1>>4 becomes 0x0F instead of 0xFF).
 */

//!uint32_t pack (size_t size, uint8_t bytes[size])
//!{
//!  uint8_t tmp[sizeof(uint32_t)] = {0};
//!  memcpy(tmp, bytes, size);
//!
//!  return tmp[0] <<  0 |
//!         tmp[1] <<  8 |
//!         tmp[2] << 16 |
//!         tmp[3] << 24 ;
//!}

int16_t can_pack ( uint8_t mydata[], uint8_t size )
{

   can_tx_frame_t out_frame;
  
   out_frame.header.ext = FALSE;
   out_frame.header.Id = STBoard.can_address;
  
   // Trusting the caller too much, no bounds checking here ...
   memcpy(out_frame.data, mydata, size*8 );  // magic number 8 = bytes
//   COPY_ARRAY(out_frame.data, mydata, size);  // See util.h for safe implementation

   out_frame.header.Length = size;
 
   if ( tx_ring_buf_push_refd(&out_frame) )
   {
      fprintf(RS232_U1,"[%8Ld]:CAN:"
           "TX Buffer is Full\n\r",
           *STBoard.milliseconds
        );
      return -1;  // find a better place for errors
   } 
}

void can_setup()
{
   STBoard.can_msg_tx = 0;
   STBoard.can_msg_rx = 0;
   CIRCBUF_FLUSH(rx_ring_buf);
   CIRCBUF_FLUSH(tx_ring_buf);
   
   STBoard.can_address = 0xFF;
   
   can_init();  // their version
  
//   enable_interrupts(INT_CAN1);    // interrupt driven CAN messages
                                   // triggering on send AND receive
   can_enable_interrupts(CAN_INTERRUPT_RX);
   can_enable_fifo_interrupts(CAN_OBJECT_FIFO_1, CAN_FIFO_INTERRUPT_RXNE);
   enable_interrupts(INT_C1RX);
}

// TODO : Defined based on the battery type and other peripherals on the line
void can_set_filter ()
{


}

//!// send whatever's in buffer
int16_t can_tx() 
{

   int16_t total_msg;
   total_msg = tx_ring_buf.push_count - tx_ring_buf.pop_count;
   
   if (total_msg < 0)  // if we have looped around
      total_msg += (2 * tx_ring_buf.size);

   if (total_msg == 0)
      return -1; // Empty

   for ( uint8_t i = 0 ; i < total_msg ; i++ )
   {
      can_rx_frame_t temp_frame;
      if (tx_ring_buf_pop_refd(&temp_frame))
      {
        // Errors during sending
        fprintf(RS232_U1,"[%8Ld]:CAN:"
           "CAN Message Sending Error\n\r",
           *STBoard.milliseconds
        );
      }

      fprintf(RS232_U1,"[%8Ld]:CAN:"
           "Data:",
           *STBoard.milliseconds
      );
      for ( uint8_t j = 0 ; j < temp_frame.header.Length ; j++ )
      {
         fprintf(RS232_U1," %LX", temp_frame.data[j] );
      }
      fprintf(RS232_U1, "\r\n" );
      can_putd(&temp_frame.header,temp_frame.data);

      STBoard.can_msg_tx++;
   }
}
//!
//!void can_rx()
//!{
//!  //check every loop for a message sitting on CAN for us .. reimplement in ISR.
//!  can_getd(&can_rx_header, &can_rx_buffer);
//!  STBoard.can_msg_rx++;
//!}


can_rx_frame_t can_unpack ( CAN_RX_HEADER *header , uint8_t *Data )
{
   can_rx_frame_t frame;
   frame.header = header;

   return frame;
}


int16_t can_print_rx_buffer()
{
   int16_t total_msg;
   total_msg = rx_ring_buf.push_count - rx_ring_buf.pop_count;
   
   if (total_msg < 0)  // if we have looped around
      total_msg += (2 * rx_ring_buf.size);

   if (total_msg == 0)
      return -1; // Empty
         
   for ( uint8_t i = 0 ; i < total_msg ; i++ )
   {
      if ( can_print_rx_msg() )   // pops and prints the next message
      {
         // Errors during printing
        fprintf(RS232_U1,"[%8Ld]:CAN:"
           "CAN Message Printing Error\n\r",
           *STBoard.milliseconds
        );
        return -1; // stop printing
      }
   }
   return 0;
}

int16_t can_print_rx_msg()
{
   can_rx_frame_t temp_frame;

   if (rx_ring_buf_pop_refd(&temp_frame))
   {
        fprintf(RS232_U1,"[%8Ld]:CAN:"
           "RX Buffer is empty\n\r",
           *STBoard.milliseconds
        );
      return -1;  // find a better place for errors
   }

   if ( temp_frame.errors != CAN_EC_OK )  // error checking
   {
      can_handle_err(temp_frame.errors);
   }
   fprintf(RS232_U1,
      "[%8Ld]:CAN: Received msg num[%Ld] from [%LX]:",
      *STBoard.milliseconds, temp_frame.counter, temp_frame.header.Id );
   for ( uint8_t j = 0 ; j < temp_frame.header.Length ; j++ )
   {
      fprintf(RS232_U1," %LX", temp_frame.data[j] );
   }
   fprintf(RS232_U1, "\r\n" );
   return 0;
}
