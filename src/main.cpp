// esptool --port COM3 --baud 115200 write_flash --flash_size 1MB 0x00000 "E:\DATA\ESP8266\ESP_07_TempCore\.pio\build\esp07\firmware.bin"
// pio device monitor
// platformIO run

#include "Arduino.h"
#include <RotaryEncoder.h>

#include "GUI.h"
#include "MenuSetup.h"



GUI gui;

byte i;

void setup()
{
    Serial.begin(115200); // Khởi tạo Serial Monitor với tốc độ

    while (!Serial)
    {
        ; // Chờ Serial khởi động
    }
    // Gọi hàm cấu hình menu từ file MenuSetup.cpp
    setupMenu(gui);
    // Hiển thị menu lên Serial Monitor
}

void loop()
{
    Menu(gui);
    handleEncoder();
}
