#include "mbed.h"
#include "GPS.h"

Serial pc(USBTX, USBRX);
GPS gps(NC, P2_1);

int main(void) {
    while(1) {
        if(gps.sample()) {
             pc.printf("I'm at %f, %f\r\n", gps.longitude, gps.latitude);
        } else {
             pc.printf("Oh Dear! No lock :(\r\n");
        }
    }
	return 0;
}
 
