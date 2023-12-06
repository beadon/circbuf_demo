#include <main.h>
#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t can_header;
    uint8_t data[8];
    uint8_t errors;
    uint8_t counter;
} can_rx_frame_t;

#include "include/utils/circbuf.h"
#include "src/circbuf.c"

#use rs232(baud=115200,parity=N,xmit=PIN_C7,rcv=PIN_C6,bits=8,stream=RS232_U1,UART1,RECEIVE_BUFFER=2,errors)

#use rtos(timer=0,minor_cycle=1ms)

#task(rate=1000ms,max=1ms,enabled=TRUE)
int16_t application();

CIRCBUF_DEF(can_rx_frame_t, my_circ_buf, 32)


//!can_rx_frame_t my_circ_buf_circbuf_data[32]; 
//!circbuf_t my_circ_buf = { 
//!    my_circ_buf_circbuf_data, 
//!    0, 
//!    0, 
//!    32, 
//!    sizeof(can_rx_frame_t) 
//!}; 
//!int my_circ_buf_push_refd(can_rx_frame_t *pt) { 
//!    return __circbuf_push(&my_circ_buf, pt); 
//!}
//!int my_circ_buf_pop_refd(can_rx_frame_t *pt) { 
//!    return __circbuf_pop(&my_circ_buf, pt, 0); 
//!} 
//!int my_circ_buf_peek_refd(can_rx_frame_t *pt) { 
//!    return __circbuf_pop(&my_circ_buf, pt, 1); 
//!};
//!


int16_t application()
{
    can_rx_frame_t in_frame;
    can_rx_frame_t out_frame;
    
    in_frame.can_header = 1;
    out_frame.can_header = 0; //overwrite me

    if (my_circ_buf_push_refd(&in_frame)) {
        printf("Out of space in CB\n");
        return -1;
    }

    if (my_circ_buf_pop_refd(&out_frame)) {
        printf("CB is empty\n");
        return -1;
    }

    // here is the data
    printf("Push: 0x%x\n", in_frame.can_header);
    printf("Pop:  0x%x\n", out_frame.can_header);
    return 0;
}


void main()
{
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1|RTCC_8_BIT);      //16.0 us overflow
   while(TRUE)
   {
      rtos_run ( );
   }
}
