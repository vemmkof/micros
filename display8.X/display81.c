#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000
#pragma config FOSC = INTRC_CLKOUT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config IESO = OFF
#pragma config FCMEN = OFF

void confBusLCD(void);
void comando(unsigned char com);
void putch(char dat);

int main(void) {
    char r_uno[] = "_::8 bits::__";
    char r_dos[] = "Micros - VEMM";
    int n;

    TRISE = 0xF8; // RE0, RE1 y RE2 como salidas.
    ANSEL = 0; // Terminales compartidas como digitales.
    confBusLCD(); // Configura el LCD para usar un bus de 8 bits.
    comando(0x0C); // Enciende el LCD sin cursor.
    comando(0x38); // 28
    comando(0x01); // Limpia el exhibidor y pone las direcciones a 0.
    comando(0x80); // Apunta a la primer columna del primer renglón.
    printf("%s", r_uno);
    comando(0xC0); // Apunta a la primera columna del segundo renglón.
    printf("%s", r_dos);
    while (1) {
        for (n = 0; n < 11; n++) {
            __delay_ms(1000);
            comando(0x18);
        }
        for (n = 0; n < 11; n++) {
            __delay_ms(1000);
            comando(0x1C);
        }
    }
}

void confBusLCD(void) {
    int t, estado;

    PORTEbits.RE0 = 0;
    t = 0; // RS indica comando.
    PORTEbits.RE1 = 1;
    t = 0; // RW indica leer del LCD.
    do {
        PORTEbits.RE2 = 1;
        t = 0; // Sube el habilitador E.
        estado = (PORTD & 0x80); // Lee bandera de ocupado (BF).
        PORTEbits.RE2 = 0;
        t = 0; // Baja el habilitador E.
    } while (estado != 0); // Espera a que el LCD esté desocupado.

    PORTEbits.RE1 = 0; // RW indica escribir al LCD.
    TRISD = 0x00; // RD7 a RD0 como salidas.
    PORTD = 0x30; // Ordena usar bus de 8 bits.
    PORTEbits.RE2 = 1;
    t = 0; // Sube el habilitador E.
    PORTEbits.RE2 = 0;
    t = 0; // Baja el habilitador E.
    TRISD = 0xFF; // Puerto D como entrada.
}

void comando(unsigned char com) {
    int t, estado;

    PORTEbits.RE0 = 0;
    t = 0; // RS indica comando.
    PORTEbits.RE1 = 1;
    t = 0; // RW indica leer del LCD.
    do {
        PORTEbits.RE2 = 1;
        t = 0; // Sube el habilitador E.
        estado = (PORTD & 0x80); // Lee bandera de ocupado (BF).
        PORTEbits.RE2 = 0;
        t = 0; // Baja el habilitador E.
        PORTEbits.RE2 = 1;
        t = 0; // Sube el habilitador E.
        PORTEbits.RE2 = 0;
        t = 0; // Baja el habilitador E.
    } while (estado != 0); // Espera a que el LCD esté desocupado.
    PORTEbits.RE1 = 0; // RW indica escribir al LCD.
    TRISD = 0x00; // RD7 a RD0 como salidas.
    PORTD = com;
    PORTEbits.RE2 = 1;
    t = 0; // Sube el habilitador E.
    PORTEbits.RE2 = 0;
    t = 0; // Baja el habilitador E.
    TRISD = 0xFF; // Puerto D como entrada.
}

void putch(char dat) {
    int t, estado;

    PORTEbits.RE0 = 0;
    t = 0; // RS indica comando.
    PORTEbits.RE1 = 1;
    t = 0; // RW indica leer del LCD.
    do {
        PORTEbits.RE2 = 1;
        t = 0; // Sube el habilitador E.
        estado = (PORTD & 0x80); // Lee bandera de ocupado (BF).
        PORTEbits.RE2 = 0;
        t = 0; // Baja el habilitador E.
        PORTEbits.RE2 = 1;
        t = 0; // Sube el habilitador E.
        PORTEbits.RE2 = 0;
        t = 0; // Baja el habilitador E.
    } while (estado != 0); // Espera a que el LCD esté desocupado.
    PORTEbits.RE0 = 1;
    t = 0; // RS indica dato.
    PORTEbits.RE1 = 0; // RW indica escribir al LCD.
    TRISD = 0x00; // RD7 a RD0 como salidas.
    PORTD = dat;
    PORTEbits.RE2 = 1;
    t = 0; // Sube el habilitador E.
    PORTEbits.RE2 = 0;
    t = 0; // Baja el habilitador E.
    TRISD = 0xFF; // Puerto D como entrada.
}
