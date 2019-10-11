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
    char fila_uno[] = "  IPN - ESCOM";
    char fila_dos[] = " Micros - VEMM";
    int n;

    TRISE = 0xF8; // RE0, RE1 y RE2 como salidas.
    ANSEL = 0; // Terminales compartidas como digitales.
    confBusLCD(); // Configura el LCD para usar un bus de 4 bits.
    comando(0x0C); // Enciende el LCD sin cursor.
    comando(0x28); // 2 renglones, caracteres de 5 x 8, nuevamente bus de 4 bits.
    comando(0x01); // Limpia el exhibidor y pone las direcciones a 0.
    comando(0x80); // Apunta a la cuarta columna del primer renglón.
    printf("%s", fila_uno);
    comando(0xC0); // Apunta a la primera columna del segundo renglón.
    printf("%s", fila_dos);
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

    // Asigné ceros a t para producir retardos pequeños.
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
    //    TRISD = 0x0F; // RD7 a RD3 como salidas.
    TRISD = 0x0F; // RD7 a RD4 como salidas.
    PORTD = 0x20; // Ordena usar bus de 4 bits.
    PORTEbits.RE2 = 1;
    t = 0; // Sube el habilitador E.
    PORTEbits.RE2 = 0;
    t = 0; // Baja el habilitador E.
    TRISD = 0xFF; // Puerto D como entrada.
}

void comando(unsigned char com) {
    int t, estado;

    // Asigné ceros a t para producir retardos pequeños.
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
    TRISD = 0x0F; // RD7 a RD3 como salidas.
    PORTD = com;
    PORTEbits.RE2 = 1;
    t = 0; // Sube el habilitador E.
    PORTEbits.RE2 = 0;
    t = 0; // Baja el habilitador E.
    PORTD = (com << 4);
    PORTEbits.RE2 = 1;
    t = 0; // Sube el habilitador E.
    PORTEbits.RE2 = 0;
    t = 0; // Baja el habilitador E.
    TRISD = 0xFF; // Puerto D como entrada.
}

void putch(char dat) {
    int t, estado;

    // Asigné ceros a t para producir retardos pequeños.
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
    t = 0; // RS i  ndica dato.
    PORTEbits.RE1 = 0; // RW indica escribir al LCD.
    TRISD = 0x0F; // RD7 a RD3 como salidas.
    PORTD = dat;
    PORTEbits.RE2 = 1;
    t = 0; // Sube el habilitador E.
    PORTEbits.RE2 = 0;
    t = 0; // Baja el habilitador E.
    PORTD = (dat << 4);
    PORTEbits.RE2 = 1;
    t = 0; // Sube el habilitador E.
    PORTEbits.RE2 = 0;
    t = 0; // Baja el habilitador E.
    TRISD = 0xFF; // Puerto D como entrada.
}
