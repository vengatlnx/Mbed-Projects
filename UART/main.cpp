/*
 * Echo World
 */

#include "mbed.h"

#define UART3

#if defined UART1
#define RX P0_16  // Pin in Connector 25
#define TX P0_15  // Pin... 23
#elif defined UART2
#define RX P0_11  // Pin... 26
#define TX P0_10  // Pin... 24
#elif defined UART3
#define RX P0_1   // Pin... 17
#define TX P0_0   // Pin... 15
#endif

Serial uart(TX, RX);

int main(void)
{
	int ch;

	uart.puts("Echo World:\r\n");
	while (1) {
		ch = uart.getc();
		uart.putc(ch);
	}
}
