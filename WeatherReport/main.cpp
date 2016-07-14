#include "mbed.h"
#include "EthernetInterface.h"
#include "MbedJSONValue.h"
#include <string>
#include "ST7567.h"

using namespace std;
ST7567 lcd(P1_24, P1_20, P1_25, P1_22, P1_21); // mosi, sclk, reset, A0, nCS
char buffer[1024];

int establish_connection(EthernetInterface eth)
{
	eth.init();	
	lcd.locate(0, 5);
	
	if (eth.connect() >= 0) {
		lcd.printf("IP Address: %s\r\n", eth.getIPAddress());
		wait(2);
		return 1;
	} else {
		lcd.printf("Err: failed to connect");
		return 0;
	}
}

void fetch_data(TCPSocketConnection sock)
{
	int ret;
	char http_cmd[] = "GET /data/2.5/weather?q=Chennai,IN&APPID={APPKEY} \r\n"; //Add your app id key
	
	sock.connect("api.openweathermap.org", 80);
    sock.send_all(http_cmd, sizeof(http_cmd)-1);

    while ((ret = sock.receive(buffer, sizeof(buffer) - 1)) > 0) {
            buffer[ret] = '\0';			
    }
}

int main() {
	EthernetInterface eth;
	TCPSocketConnection sock;
	MbedJSONValue v;
	
	lcd.set_contrast(0x3F);
	lcd.cls();
	
	if (establish_connection(eth)) {
		fetch_data(sock);
		
		parse(v, buffer);
		
		lcd.cls();
		lcd.locate(0,5);
		lcd.printf("%s\r\n", (int)v["name"].get<string>().c_str());
		
		lcd.locate(0,15);
		lcd.printf("temp = %d\r\n", (int)v["main"]["temp"].get<double>());
		
		lcd.locate(0,25);
		lcd.printf("pressure = %d\r\n", (int)v["main"]["pressure"].get<double>());
		
		lcd.locate(0,35);
		lcd.printf("humidity = %d\r\n", (int)v["main"]["humidity"].get<double>());
	}
	
	sock.close();    
	eth.disconnect();
    
	while(1) {}
	return 0;
}
