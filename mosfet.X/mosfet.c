#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000
#pragma config FOSC = INTRC_CLKOUT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config IESO = OFF
#pragma config FCMEN = OFF

int main(void) {
    TRISD = 0xFD; // RD1 como salida.
    ANSEL = 0; // Terminales compartidas como digitales.
    while (1) {
        if (PORTDbits.RD0 == 1) {
            PORTDbits.RD1 = 1;
        } else {
            PORTDbits.RD1 = 0;
        }
    }
}