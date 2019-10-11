#include <xc.h>
#include <stdio.h>
#pragma config FOSC = INTRC_CLKOUT
#pragma config WDTE = OFF
#define _XTAL_FREQ 4000000

void main(void) {
    //    unsigned char cadena[] = "hola mundo\n";
    unsigned char indice;

    //    SPBRG = 0; // BR = 230400
    //    SPBRG = 1; // BR = 115200
    //    SPBRG = 3; // BR = 57600
    //    SPBRG = 5; // BR = 38400   
    SPBRG = 6; // BR = 38400
    //    SPBRG = 12; // BR = 19200
    //    SPBRG = 25; // BR = 9600
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    TXSTAbits.TXEN = 1;
    TXSTAbits.BRGH = 1;
    while (1) {
        if (PIR1bits.RCIF == 1) {
            indice = RCREG + 1;
            while (PIR1bits.TXIF != 1);
            TXREG = indice;
        }
    }
}