/*
 * Example program to on led when key press
 */

#include "mbed.h"

DigitalIn key(P1_19);
DigitalOut led(P0_19);

int main(void) {
    while (1) {
	if (key == 0)
	    led = 1;
	else
	    led = 0;
    }
return 0;
}
