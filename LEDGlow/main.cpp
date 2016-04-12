/*
 * Example program to glow led using pwm
 */

#include "mbed.h"

PwmOut led(P1_18);

int main(void) {
    float pulse;
    while (1) {
	for (pulse = 0.0f; pulse < 1.0f; pulse += 0.1f) {
	    led = pulse;
	    wait(0.1);
	}
	for (pulse = 1.0f; pulse > 0.0f; pulse -= 0.1f) {
	    led = pulse;
	    wait(0.1);
	}
    }
return 0;
}
