#ifndef MENU_H
#define MENU_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <algorithm>
#include <functional> // Include functional header for std::function and std::bind

#include "GUI_OLED.h"
#include "RotaryEncoder.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

GUI_OLED gui;

// Định nghĩa chân kết nối cho encoder (quay và nút nhấn)
#define CLK_PIN 14 // Chân CLK của Encoder
#define DT_PIN 16  // Chân DT của Encoder
#define SW_PIN 12  // Chân nút nhấn của Encoder
// Khởi tạo đối tượng encoder, giá trị từ 0 đến 100
RotaryEncoder encoder(CLK_PIN, DT_PIN, SW_PIN, 0, 100);

static byte slecMainList = 0;
static byte slecSubList = 0;
bool flagMenu = false;
bool flagPress = false; // Cờ để kiểm soát trạng thái nhấn nút

class Menu
{
private:
    // Using a vector of std::function to hold function pointers
    std::vector<std::function<void()>> caseList;

public:
    Menu();
    void begin(Stream &outputErrOledConn);
    void handleEncoder();

    void MainMenu();
    void printSubMenu();
    void sub1();
    void sub2();
    void sub3();
    void sub4();
    void sub5();
    void sub6();
    void sub7();
    void sub8();
    void sub9();
};

/////////////////////////////////////////////////////////
Menu::Menu()
{
    // Initialize the function list in the constructor
    caseList = {
        std::bind(&Menu::sub1, this),
        std::bind(&Menu::sub2, this),
        std::bind(&Menu::sub3, this),
        std::bind(&Menu::sub4, this),
        std::bind(&Menu::sub5, this),
        std::bind(&Menu::sub6, this),
        std::bind(&Menu::sub7, this),
        std::bind(&Menu::sub8, this),
        std::bind(&Menu::sub9, this)};
}

void Menu::begin(Stream &outputErrOledConn)
{
    encoder.begin();

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
    {
        outputErrOledConn.println(F("SSD1306 allocation failed"));
    }

    display.clearDisplay();

    delay(100);

    gui.addMainItem("Setting Temp");
    gui.addMainItem("Timer Temp");
    gui.addMainItem("Graph Temp");

    gui.addMainItem("Connect Wifi");
    gui.addMainItem("Graph Wifi");

    gui.addMainItem("Wed Server");
    gui.addMainItem("Run Macro");
    gui.addMainItem("Connect FireBare");
    gui.addMainItem("About CoreCraft");

    // Đi ngang từ trái qua phải
    for (byte x = 0; x <= SCREEN_WIDTH; x += 20)
    {
        display.clearDisplay();
        display.fillRect(0, 0, x, SCREEN_HEIGHT, WHITE); // Vẽ một hình chữ nhật từ trái qua phải
        display.display();
    }

    // Xóa từ trái qua phải
    for (byte x = 0; x <= SCREEN_WIDTH; x += 20)
    {
        display.clearDisplay();
        display.fillRect(x, 0, SCREEN_WIDTH - x, SCREEN_HEIGHT, WHITE); // Xóa dần từ trái qua phải
        display.display();
    }
}

void Menu::handleEncoder()
{
    encoder.loop();
    delay(2);
}

void Menu::MainMenu()
{
    if (!flagMenu)
    {
        byte currentSelection = abs((encoder.setCounters(0, gui.getMainItem().size() - 1), encoder.getCounter())) % gui.getMainItem().size();
        byte scrollOffset = constrain(currentSelection - 7, 0, max(0, (int)gui.getMainItem().size() - 8));

        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);

        for (byte i = scrollOffset; i < scrollOffset + 8 && i < gui.getMainItem().size(); ++i)
        {
            display.setTextColor(i == currentSelection ? SSD1306_BLACK : SSD1306_WHITE, SSD1306_WHITE);
            display.println(gui.getMainItem()[i]);
        }

        display.display();

        (encoder.isPressed()) ? (slecMainList = currentSelection, flagMenu = true, encoder.resetCounter()) : void();
    }
    else
    {                                                                         // sử dụng toán tử điều kiện (ternary operator)
        flagPress = !encoder.isPressed() ? true : flagPress;                  // Đặt cờ khi nút không được nhấn
        (slecMainList < caseList.size()) ? caseList[slecMainList]() : void(); // Gọi hàm từ caseList nếu điều kiện thỏa mãn
    }
}

void Menu::printSubMenu()
{
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);

    byte currentSelection = abs((encoder.setCounters(0, gui.getsubItem().size() - 1), encoder.getCounter())) % gui.getsubItem().size();
    byte scrollOffset = constrain(currentSelection - 7, 0, max(0, (int)gui.getsubItem().size() - 8));

    // Hiển thị các mục con trong submenu
    for (byte i = scrollOffset; i < scrollOffset + 8 && i < gui.getsubItem().size(); ++i)
    {
        display.setTextColor(i == currentSelection ? SSD1306_BLACK : SSD1306_WHITE, SSD1306_WHITE);
        display.println(gui.getsubItem()[i]); // In các mục con
    }

    display.display();

    slecSubList = currentSelection;

    // Kiểm tra nếu nút "Back" được nhấn
    (slecSubList == 0 && encoder.isPressed() && flagPress) ? (encoder.resetCounter(), flagMenu = false, MainMenu()) : void();
}

void Menu::sub1()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    gui.addSubItem("Edit Temp");
    gui.addSubItem("Current Temp: ");
    // In các mục con
    printSubMenu();
}

void Menu::sub2()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    // In các mục con
    printSubMenu();
}
void Menu::sub3()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    // In các mục con
    printSubMenu();
}
void Menu::sub4()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    // In các mục con
    printSubMenu();
}
void Menu::sub5()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    // In các mục con
    printSubMenu();
}
void Menu::sub6()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    // In các mục con
    printSubMenu();
}
void Menu::sub7()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    // In các mục con
    printSubMenu();
}
void Menu::sub8()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    // In các mục con
    printSubMenu();
}
void Menu::sub9()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    // In các mục con
    printSubMenu();
}

#endif
