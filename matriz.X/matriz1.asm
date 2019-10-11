#INCLUDE "p16f887.inc"
    __CONFIG _CONFIG1, 0xFFF7;WHATCHDOG APAGADO: _WDT_OFF
    UDATA 020
CONTADOR    
    RES 1
INDICE	    
    RES 1
    ORG 0
    CALL INIT
    MOVLW .64
    MOVWF CONTADOR
    MOVLW 040
    MOVWF FSR
    CLRF INDICE
LLENADO
    MOVF INDICE, W
    MOVWF INDF
    INCF FSR, F
    INCF INDICE, F
    DECFSZ CONTADOR
    GOTO LLENADO
    MOVLW 040
    MOVWF FSR
    CALL MOSTRAR
FIN
    NOP
    GOTO FIN
INIT
    BSF STATUS, RP0
    MOVLW 0C0
    MOVWF TRISD
    BCF STATUS, RP0
    RETURN
MOSTRAR
    MOVF INDF, W
    MOVWF PORTD
    INCF FSR, F
    BCF FSR, .7
    BSF FSR, .6
    GOTO MOSTRAR
    END