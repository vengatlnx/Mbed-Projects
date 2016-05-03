#include "mbed.h"

BusOut seg(P2_11, P0_20, P2_2, P0_27, P2_0, P2_1, P0_4, P0_5, P4_28); //GPIO pins

int main(void){

    int digits[] = { 0x3F, /* 0 */
		     0x06, /* 1 */
		     0x5B, /* 2 */
		     0x4F, /* 3 */
		     0x66, /* 4 */
		     0x6D, /* 5 */
		     0x7D, /* 6 */
		     0x07, /* 7 */
		     0x7F, /* 8 */
		     0x67 /* 9 */
    };
    
    while (1) {
	for(int i = 0; i < 10; i++) {
	    seg = digits[i];
            wait(0.5);
        }
    }
}
