#INCLUDE "p16f887.inc"    
    ;lo que configuramos lo saca port
    __CONFIG _CONFIG1, _WDT_OFF
    ORG 0
    BSF STATUS, RP0
    BCF TRISD, 0; configurar la direccion
    BCF STATUS, RP0
CICLO
    ;pata 1 es salida
    ;pata 2 es entrada
    ;;;;;;;;BTFSS PORTD, 0
    ;;;;;;;;BSF PORTD, 0
    ;;;;;;;;BTFSC PORTD, 0
    ;;;;;;;;BCF PORTD, 0    
    BTFSC PORTD, 1
    BSF PORTD, 0
    BTFSS PORTD, 1
    BCF PORTD, 0
    GOTO CICLO
    END    