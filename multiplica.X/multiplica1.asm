;#INCLUDE "p16f887.inc"    
;    UDATA 020
;MDO RES 1
;MDR RES 1
;ANS RES 1
;EME RES 1
;ENE RES 1
;ROT RES 1
;    ORG 0
;    MOVLW .10
;    MOVWF MDO
;    MOVLW .8
;    MOVWF MDR
;    MOVLW .4
;    MOVWF EME
;    MOVLW .3
;    MOVWF ROT
;LOOP1;EME -- MDR
;    MOVLW .4
;    MOVWF ENE
;LOOP2;ENE -- MDO
;    
;    ;BTFSC MDO, ENE;SKIP IF CLEAR
;    ;BTFSC MDR, ENE;
;    ;BTFSS ;SKIP IF SET
;    DECFSZ ENE, F
;    GOTO LOOP2
;    
;    DECFSZ EME, F
;    GOTO LOOP1
;FIN
;    NOP
;    GOTO FIN
;    END
#INCLUDE "p16f887.inc"    
    UDATA 020
MDO RES 1
MDR RES 1
ANS RES 1
    ORG 0
    MOVLW .10
    MOVWF MDO
    MOVLW .8
    MOVWF MDR
    CLRF ANS
    MOVF MDO, W
LOOP
    ADDWF ANS, F
    DECFSZ MDR, F
    GOTO LOOP
FIN
    NOP
    GOTO FIN
    END

