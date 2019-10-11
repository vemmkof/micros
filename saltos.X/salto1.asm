#INCLUDE "p16f887.inc"
    __CONFIG _CONFIG1, _WDT_OFF;0X2007
    ORG 0
    NOP
    NOP
    NOP
    CALL SSR
CICLO
    NOP
    NOP
    NOP    
    GOTO CICLO
    NOP
    NOP
    ORG 0800
SSR    
    NOP
    NOP
    RETURN
    END