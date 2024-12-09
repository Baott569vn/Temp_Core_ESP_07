// esptool --port COM9 --baud 115200 write_flash --flash_size 1MB 0x00000 "E:\DATA\ESP8266\ESP_07_TempCore\.pio\build\esp07\firmware.bin"
// pio device monitor
// platformIO run
// pio run --target clean

#include "Arduino.h"
#include <RotaryEncoder.h>

#include "Menu.h"

Menu menu;

unsigned long previousMillis_main_Task1 = 0; // Định nghĩa giá trị của biến
const long interval = 3000;                   // 500ms

void setup()
{
    Serial.begin(115200);
    menu.begin(Serial);

    pinMode(13, OUTPUT);
}

void loop()
{
    unsigned long currentMillis = millis(); // Lấy thời gian hiện tại
    if (currentMillis - previousMillis_main_Task1 >= interval)
    {
        previousMillis_main_Task1 = currentMillis; // Cập nhật thời gian hiện tại
        menu.temperature();
    }
    menu.handleEncoder();
    menu.MainMenu();
    // menu.printSubMenu();
}
