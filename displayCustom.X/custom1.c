#include <xc.h>
#include <stdio.h>

#define _XTAL_FREQ 4000000

#pragma config FOSC = INTRC_CLKOUT
#pragma config WDTE = OFF
#pragma config PWRTE = ON
#pragma config IESO = OFF
#pragma config FCMEN = OFF

void configurarLCD();
void crearCaracter(unsigned char *pattern, unsigned char address);
void comando(unsigned char command);
void escribirDato(unsigned char data);

/*
 * RE0:PORTEbits.RE0
 * RE1:PORTEbits.RE1
 * RE2:E
 */

int main(void) {
    unsigned char pattern1[] = {0x00, 0x00, 0x0a, 0x1f, 0x1f, 0x0e, 0x04, 0x00};
    unsigned char pattern2[] = {0x1F, 0x0D, 0x0D, 0x0D, 0x0D, 0x05, 0x0A, 0x0F}; // micro fondo negro

    TRISC = 0xFE; // C0 como salida
    TRISD = 0xFF; // PUERTO D como entrada
    TRISE = 0xF8; // E0, E1 y E2 como salidas
    ANSEL = 0; // Terminales compartidas como digitales
    configurarLCD();
    comando(0x0C); // Enciende el LCD sin cursor.
    comando(0x38); // 8 BITS 2 RENGLONES
    comando(0x01); // Limpia el exhibidor y pone las direcciones a 0.
    crearCaracter(pattern1, 1);
    crearCaracter(pattern2, 2);
    while (1) {
        PORTCbits.RC0 = 1;
        comando(0x83); // apunta a la primer columna del primer renglón
        escribirDato(1);
        PORTCbits.RC0 = 0;
        comando(0x85); // apunta a la primer columna del primer renglón
        escribirDato(2);
    }

}

void configurarLCD() {
    int t, estado;
    PORTEbits.RE0 = 0; // indica comando.
    t = 0;
    PORTEbits.RE1 = 1; // indica leer del LCD.
    t = 0;
    do {
        PORTEbits.RE2 = 1; // ENABLE
        t = 0;
        estado = (PORTD & 0x80); // Lee bandera de ocupado (BF).
        PORTEbits.RE2 = 0; // ENABLE
        t = 0;
    } while (estado != 0); // Espera a que el LCD esté desocupado.
    PORTEbits.RE1 = 0; // indica escribir al LCD.
    TRISD = 0x00; // PUERTO D como salidas.
    PORTD = 0x38; // Ordena usar bus de 8 bits con 2 renglones
    PORTEbits.RE2 = 1; // ENABLE
    t = 0;
    PORTEbits.RE2 = 0; // ENABLE
    t = 0;
    TRISD = 0xFF; // Puerto D como entrada
}

void crearCaracter(unsigned char *pattern, unsigned char address) {
    unsigned char i;
    /* establece la direccion a usar en la memoria 
     * de generacion de caractereres CGRAM */
    comando(0x40 + (address * 8)); // posision 
    for (i = 0; i < 8; i++) {
        escribirDato(pattern[i]);
    }
}

void comando(unsigned char command) {
    int t, estado;
    PORTEbits.RE0 = 0; // indica comando.
    t = 0;
    PORTEbits.RE1 = 1; // indica leer del LCD.
    t = 0;
    do {
        PORTEbits.RE2 = 1; // ENABLE
        t = 0;
        estado = (PORTD & 0x80); // Lee bandera de ocupado (BF)
        PORTEbits.RE2 = 0; // ENABLE
        t = 0;
    } while (estado != 0); // Espera a que el LCD esté desocupado.
    PORTEbits.RE1 = 0; // indica escribir al LCD.
    TRISD = 0x00; // PUERTO D como salida
    PORTD = command;
    PORTEbits.RE2 = 1; // ENABLE
    t = 0;
    PORTEbits.RE2 = 0; // ENABLE
    t = 0;
    TRISD = 0xFF; // Puerto D como entrada
}

void escribirDato(unsigned char data) {
    int t, estado;
    PORTEbits.RE0 = 0; // indica comando.
    t = 0;
    PORTEbits.RE1 = 1; // indica leer del LCD.
    t = 0;
    do {
        PORTEbits.RE2 = 1; // ENABLE
        t = 0;
        estado = (PORTD & 0x80); // Lee bandera de ocupado (BF)
        PORTEbits.RE2 = 0; // ENABLE
        t = 0;
    } while (estado != 0); // Espera a que el LCD esté desocupado.   
    /*
     * el procesador entrega un dato al exhibidor.
     * el uso del dato depende si en la operacion anterior
     * se establevió una direccion para la DDRAM o CGRAM
     */
    PORTEbits.RE0 = 1; // escribir un dato
    PORTEbits.RE1 = 0;
    TRISD = 0x00; // PUERTO D como salida
    PORTD = data; // mandamos el dato
    PORTEbits.RE2 = 1; // ENABLE
    t = 0;
    PORTEbits.RE2 = 0; // ENABLE
    t = 0;
    TRISD = 0xFF; // Puerto D como entrada
}

