/*
 * Example program to on led when key press
 */

#include "mbed.h"

#define PWM_CH 6
#define CH0 P2_0
#define CH1 P2_1
#define CH2 P2_2
#define CH3 P2_3
#define CH4 P2_4
#define CH5 P2_5

InterruptIn key1(P2_6);
InterruptIn key2(P2_7);
PwmOut pwm[6] = {CH0, CH1, CH2, CH3, CH4, CH5};
int led;

void glow_led(void) {
	float pulse;
	for (pulse = 0.0f; pulse < 1.0f; pulse += 0.1f) {
	    pwm[led] = pulse;
	    wait(0.1);
	}
	for (pulse = 1.0f; pulse > 0.0f; pulse -= 0.1f) {
	    pwm[led] = pulse;
	    wait(0.1);
	}
}

void previous(void)
{
	pwm[led] = 0;
	if (led == 0)
		led = PWM_CH;
	led -= 1;
	wait(0.5);
}

void next(void)
{
	pwm[led] = 0;
	led += 1;
	led %= PWM_CH;
	wait(0.5);
}

int main(void) {
    key1.rise(&previous);
	key2.rise(&next);
	
    while (1) {
		glow_led();
    }
}
