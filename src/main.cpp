// esptool --port COM9 --baud 115200 write_flash --flash_size 1MB 0x00000 "E:\DATA\ESP8266\ESP_07_TempCore\.pio\build\esp07\firmware.bin"
// pio device monitor
// platformIO run
// pio run --target clean

#include "Arduino.h"
#include <RotaryEncoder.h>

#include "Menu.h"

Menu menu;

void setup()
{
    Serial.begin(115200);
    menu.begin(Serial);
}

void loop()
{
    menu.handleEncoder();
    menu.MainMenu();
    // menu.printSubMenu();
}
