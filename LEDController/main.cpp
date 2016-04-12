#include "mbed.h"

Serial serial(P0_2, P0_3); //RxD, TxD usb
PwmOut led(P3_25);

char command[3];

int main(void) {
    while(1) {
	// serial receive data from 0-9
	if (serial.readable()) {
	    serial.gets(command, 3);
	    float value = atof(command);

	    if (value == 10) {
		led = 1.0f;	        //led on
	    } else {
		led = (float)value/100; //glow led
	    }
	}
    }
    return 0;
}
