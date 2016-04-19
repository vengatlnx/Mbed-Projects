#include "mbed.h"
#include "ST7567.h"

ST7567 lcd(P1_24, P1_20, P1_25, P1_22, P1_21); // mosi, sclk, reset, A0, nCS

int main() {
    lcd.set_contrast(0x3F);
    lcd.cls();
    lcd.locate(0, 1);
    lcd.printf("HelloWorld!");
}
