#include <xc.h>
#define _XTAL_FREQ 4000000
#pragma config FOSC = INTRC_CLKOUT
#pragma config WDTE = OFF

void interrupt rai(void) {
    INTCONbits.INTF = 0;
    asm("MOVLW 25");
    NOP();
}

void main(void) {
    ANSELHbits.ANS12 = 0;
    INTCONbits.GIE = 1;
    INTCONbits.INTE = 1;
    __delay_ms(1);
    while (1) {
        asm("MOVLW 20");
        NOP();
    }
}