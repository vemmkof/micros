#include <xc.h>
#include <stdio.h>
#pragma config FOSC = INTRC_CLKOUT
#pragma config WDTE = OFF
#define _XTAL_FREQ 4000000

void initUSART(void);
void initADC(void);
void escribeUSART(unsigned char dato);

void main(void) {
    unsigned char msb, lsb;
    initUSART();
    initADC();
    while (1) {
        __delay_ms(5000);
        //iniciando al conversion
        ADCON0bits.GO = 1;
        // is conversion done?
        while (PIR1bits.ADIF != 1) {
            //        while (ADCON0bits.GO != 1) {
            NOP();
        }
        /*procesar ADRESH:ADRESL*/
        msb = (ADRESH & 0b00000011);
        escribeUSART(msb);
        escribeUSART(lsb);
        lsb = ADRESL;
        /************************/
        /*bajar manualmente el ADIF*/
        PIR1bits.ADIF = 0;
    }

}

void initUSART(void) {
    SPBRG = 25; // BR = 9600
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    TXSTAbits.TXEN = 1;
    TXSTAbits.BRGH = 1;
}

void initADC(void) {
    // port configuration
    TRISA = 0xFF; // puerto A como entrada
    ANSEL = 1; // entradas como análogica   
    /***********ADCON0**************/
    // ADC conversion clock source
    ADCON0bits.ADCS = 0b01; // FOSC/8 para TAD a 2us
    // channel selection
    ADCON0bits.CHS = 0b0001; // AN1: PUERTO A - PIN 1
    // enable ADC module    
    ADCON0bits.ADON = 1;
    /***********ADCON1**************/
    // results formatting 
    ADCON1bits.ADFM = 1; // justificado a la derecha
    // ADC voltage reference selection
    ADCON1bits.VCFG1 = 0; //--Vss is negative voltage reference
    ADCON1bits.VCFG0 = 0; //--Vdd is positive voltage reference
}

void escribeUSART(unsigned char dato) {
    while (PIR1bits.TXIF != 1) {
        NOP();
    }
    TXREG = dato;
}