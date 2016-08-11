/*
 * Example program for SPI based 7segment
 */

#include "mbed.h"

#define SPI1

#if defined SPI0
#define MOSI P0_18
#define MISO P0_17
#define SCLK P0_15
#define LOAD P0_6
#elif defined SPI1
#define MOSI P0_9
#define MISO P0_8
#define SCLK P0_7
#define LOAD P0_6
#endif

#define TOT_DIGITS 10
uint8_t encode_digit[] = {
         0x3F, /* 0 */
         0x06, /* 1 */
         0x5B, /* 2 */
         0x4F, /* 3 */
         0x66, /* 4 */
         0x6D, /* 5 */
         0x7D, /* 6 */
         0x07, /* 7 */
         0x7F, /* 8 */
         0x67, /* 9 */
};
SPI sseg(MOSI, MISO, SCLK);
DigitalOut load(LOAD);

int main(void)
{
	int i = 0;

	while (1) {
		sseg.write(encode_digit[i]);
		sseg.write(encode_digit[i]);
		sseg.write(encode_digit[i]);
		sseg.write(encode_digit[i]);

		load = 1;
		load = 0;
		wait(1);

		i++;
		i %= TOT_DIGITS;
	}
}

