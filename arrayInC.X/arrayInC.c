#include <xc.h>

void main(void) {
    TRISD = 0xC0; // 1100 0000
    FSR = 64;
    while (1) {
        FSR++;
        FSR = FSR | 0x40;
        FSR = FSR & 0x7F;
        PORTD = FSR;
    };

}
