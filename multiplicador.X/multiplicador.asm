#include "p16f887.inc"
    ORG 0
    NOP
    BSF PCLATH, 3
    CALL SR
FIN   
    NOP
    GOTO FIN    
    ORG 0800
SR
    NOP
    RETURN
    NOP
    NOP
    NOP
    NOP
    NOP
    END


