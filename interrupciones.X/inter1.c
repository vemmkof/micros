#include <xc.h>
void interrupt rai (void) {
    INTCONbits.INTF = 0;
    NOP();
}

void main(void) {
    ANSELHbits.ANS12 = 0;
    INTCONbits.GIE = 1;
    INTCONbits.INTE = 1;
    while (1) {
        NOP();
    }
}