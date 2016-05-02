#include "mbed.h"

DigitalOut Red(P0_4);
DigitalOut Blue(P0_5);
DigitalOut Green(P2_11);

int main(void){
    Green = 1;
    Blue = 1;
    Red = 1;
    while (1) {
    	if (Green && Blue == 1) {
    	    Red = 0;
    	    wait(0.2);
    	    Red = 1;
    	}
    	if (Blue && Red == 1) {
    	    Green = 0;
    	    wait(0.2);
    	    Green = 1;
    	}
    	if (Green && Red == 1) {
    	    Blue = 0;
    	    wait(0.2);
    	    Blue = 1;
    	}
    }
}
