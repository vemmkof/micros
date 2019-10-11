#include <xc.h>
#include <stdio.h>
#pragma config FOSC = INTRC_CLKOUT
#pragma config WDTE = OFF
#define _XTAL_FREQ 4000000 
#define MEM1_WRITE 0xAE // 1010 111 write
#define MEM1_READ 0xAF // 1010 111 read

// I2C PIC
void initI2C(void);
void waitMSSP(void);
void setACK(void);
void setNACK(void);

// I2C MEM
void pageWrite(unsigned char cadena[], unsigned char tam);
void randomRead(unsigned char tam);

// USART
void initUSART(void);
void writeUSART(unsigned char dato);
void writeStringUSART(unsigned char dato[], unsigned char tam);

void main() {
    // MENSAJES
    unsigned char data[20] = {"24LC512 W/PIC16F887"};
    unsigned char msg[] = "SI2C\n"; // start 

    // INICIAR
    initUSART();
    writeStringUSART(msg, 5);
    initI2C();
    __delay_ms(500);

    // ESCRIBIR
    msg[0] = 'W'; // write
    writeStringUSART(msg, 5);
    pageWrite(data, 19);
    __delay_ms(500);

    // LEER
    msg[0] = 'R'; // read
    while (1) {
        writeStringUSART(msg, 5);
        randomRead(19);
        writeUSART('\n');
        __delay_ms(2000);
    }
}

/****   I2C PIC ****/

void initI2C() {
    // Set up I2C lines by setting as input 
    TRISC3 = 1;
    TRISC4 = 1;
    // 2: Enables the serial port and configures the SDA and SCL pins as the source of the serial port pins
    // 8: I2C Master mode, clock = FOSC/(4 * (SSPADD+1))
    SSPCON = 0x28;
    // reloj de comunicacion a 100KHz
    SSPADD = 9; // 0b1001;
    // deshabilita control de rapidez de respuesta
    SSPSTAT = 80;
}

void waitMSSP() {
    /*
     * The MSSP module in I 2 C mode, fully implements all
     * master and slave functions (including general call
     * support) and provides interrupts on Start and Stop bits in
     * hardware, to determine a FREE BUS
     */
    while (!SSPIF) {
        // SSPIF of the PIR1 register, 
        // is set on the falling edge of the ninth SCL pulse 
        // (interrupt is generated, if enabled)
        NOP();
    };
    // Flag bit SSPIF of the PIR1 register must be cleared in software
    SSPIF = 0;
}

void setACK() {
    ACKDT = 0; // Acknowledge data 1: NACK, 0: ACK 
    ACKEN = 1; // Enable ACK to send 
}

void setNACK() {
    ACKDT = 1; // Acknowledge data 1: NACK, 0: ACK 
    ACKEN = 1; // Enable ACK to send 
}

/****   I2C MEM ****/

void pageWrite(unsigned char data[], unsigned char tam) { // page write

    // START (S)
    SEN = 1; // Send start bit 
    waitMSSP();

    // CONTROL BYTE RW = 0
    SSPBUF = MEM1_WRITE; // write command  to mem1
    setACK();
    waitMSSP();

    // ADRESS
    SSPBUF = 0x00; // high address
    setACK();
    waitMSSP();
    SSPBUF = 0x00; // low address
    setACK();
    waitMSSP();

    // DATA
    for (unsigned char i = 0; i < tam; i++) {
        SSPBUF = data[i]; // send byte per byte
        setACK();
        waitMSSP();
    }
    // como no es una pagina completa, 128 bytes,
    // lo que se escribe se omite el ack
    // setACK(); 
    PEN = 1; // Send stop bit     
    waitMSSP();
}

void randomRead(unsigned char tam) {
    unsigned char i;
    for (i = 0; i < tam; i++) { // random read
        // START
        SEN = 1; //Send start bit 
        waitMSSP();

        // CONTROL BYTE
        SSPBUF = MEM1_WRITE; //Send Slave address write command 
        setACK();
        waitMSSP();

        // ADDRESS
        SSPBUF = 0x00; // high address 
        setACK();
        waitMSSP();
        SSPBUF = i; // low address 
        setACK();
        waitMSSP();

        // START
        SEN = 1; // send start bit         
        waitMSSP();

        // CONTROL BYTE
        SSPBUF = MEM1_READ; // read command 
        setACK();
        waitMSSP();

        // DATA BYTE        
        RCEN = 1; // Enable receive 
        waitMSSP();

        // NO ACK
        setNACK();

        // STOP
        PEN = 1; // Stop condition 
        waitMSSP();

        // send data to PC
        writeUSART(SSPBUF);
        NOP();
    }
}

/****   USART PIC ****/

void initUSART(void) {
    SPBRG = 25; // BR = 9600
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    TXSTAbits.TXEN = 1;
    TXSTAbits.BRGH = 1;
}

void writeUSART(unsigned char dato) {
    while (TXIF == 0) {
        NOP();
    }
    TXREG = dato;
}

void writeStringUSART(unsigned char dato[], unsigned char tam) {
    for (unsigned char c = 0; c < tam; c++) {
        writeUSART(dato[c]);
    }
}