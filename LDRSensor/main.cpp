#include "mbed.h"

AnalogIn ain(P0_23);
PwmOut led(P3_25);

int main(void){
    while (1) {
	led = ain;
    }
}
