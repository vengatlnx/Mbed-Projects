#include "mbed.h"
#define SDA P0_27
#define SCL P0_28
#define E2P 0xA0

I2C e2p(SDA, SCL);

static void i2c_scane_bus(I2C e2p)
{
	printf("I2C Start\r\n");
	for (int i = 0; i <= 0xFF; i++) {
	e2p.start();
	if (e2p.write(i))
		printf("0x%02X ", i);
	e2p.stop();
	}
	printf("\r\nI2C End\r\n");
}

uint8_t eeprom_read(int addr)
{
	uint8_t data;

	e2p.start();
	if (!e2p.write(E2P))
	goto dev_fail;
	if (!e2p.write(addr))
	goto data_fail;
	e2p.start();
	if (!e2p.write(E2P | 1))
	goto dev_fail;
	data = e2p.read(0);
	e2p.stop();

	return data;

 dev_fail:
	e2p.stop();
	printf("Dev Not Found!\r\n");
	return 1;

 data_fail:
	e2p.stop();
	printf("Data read failed!\r\n");
	return 2;
}

void eeprom_write(int addr, uint8_t data)
{
	e2p.start();
	if (!e2p.write(E2P))
	goto dev_fail;
	if (!e2p.write(addr))
	goto data_fail;
	if (!e2p.write(data))
	goto data_fail;
	e2p.stop();
	return;

 dev_fail:
	e2p.stop();
	printf("Dev Not Found!\r\n");
	return;

 data_fail:
	e2p.stop();
	printf("Data read failed!\r\n");
	return;
}

void eeprom_tx(int addr, uint8_t src[], int src_len)
{
	int i;

	for (i = 0; i < src_len; i++) {
	eeprom_write(addr + i, src[i]);
	wait_ms(5);
	}
}

void eeprom_rx(int addr, uint8_t dest[], int dest_len)
{
	int i;

	for (i = 0; i < dest_len; i++) {
	dest[i] = eeprom_read(addr + i);
	}
}

int main()
{
	uint8_t src[] = "Hello World";
	uint8_t dest[30] = "NULL";

	printf("Dest: [%s]\r\n", dest);
	eeprom_tx(10, src, sizeof(src));
	eeprom_rx(10, dest, sizeof(src));
	printf("Dest: [%s]\r\n", dest);

	return 0;
}
