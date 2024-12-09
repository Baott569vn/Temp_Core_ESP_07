#ifndef MENU_H
#define MENU_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <algorithm>
#include <functional> // Include functional header for std::function and std::bind
#include <cmath>
#include <EEPROM.h>

#include "GUI_OLED.h"
#include "RotaryEncoder.h"

#include <NTC_TemperatureSensor.h>

// Khai báo đối tượng cảm biến NTC, sử dụng chân A0
NTC_TemperatureSensor tempSensor(A0);

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
bool _isPressed = false;

bool flagSet = false;
byte modeSlecSub = 0;

double setTemp = 0.0;  // Lấy giá trị từ EEPROM, chia 10 để chuyển thành số thực
double setError = 0.0; // Tương tự
double temp;           // nhiet do hienj tai

#define MAX_POINTS 128                    // Định nghĩa số lượng điểm tối đa
double temperatureData[MAX_POINTS] = {0}; // Lưu trữ nhiệt độ
int timeStep = 0;                         // Dùng để theo dõi vị trí cập nhật

// Định nghĩa các biến
bool PID_Temp = false;
bool timer_Temp = false;
static unsigned long previousMillis = 0; // Lưu thời điểm bật chân 13
unsigned long setTimerTemp = 0;          // Thời gian bật (ms)

class Menu
{
private:
    // Using a vector of std::function to hold function pointers
    std::vector<std::function<void()>> caseList;
    void _PID_Temp();
    void _timer_Temp();

public:
    Menu();
    void begin(Stream &outputErrOledConn);
    void handleEncoder();
    double temperature();

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
}

double Menu::temperature()
{
    tempSensor.setTargetTemperature(setTemp, setError);
    temp = tempSensor.readTemperature();
    return temp;
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
    display.setTextSize(1);
    display.setCursor(0, 0);

    byte currentSelection;
    if (!flagSet)
    {
        currentSelection = abs((encoder.setCounters(0, gui.getsubItem().size() - 1), encoder.getCounter())) % gui.getsubItem().size();
        slecSubList = currentSelection;
    }
    byte scrollOffset = constrain(currentSelection - 7, 0, max(0, (int)gui.getsubItem().size() - 8));
    // Hiển thị các mục con trong submenu
    for (byte i = scrollOffset; i < scrollOffset + 8 && i < gui.getsubItem().size(); ++i)
    {
        display.setTextColor(i == currentSelection ? SSD1306_BLACK : SSD1306_WHITE, SSD1306_WHITE);
        display.println(gui.getsubItem()[i]); // In các mục con
    }
    display.setTextColor(SSD1306_WHITE);
    display.display();

    // Kiểm tra nếu nút "Back" được nhấn
    (slecSubList == 0 && encoder.isPressed() && flagPress && !flagSet) ? (encoder.resetCounter(), flagMenu = false, MainMenu()) : void();
    // Xử lý nhấn encoder để thay đổi chế độ
    if (encoder.isPressed() && slecSubList != 0)
        modeSlecSub = (modeSlecSub + 1) % 3, flagSet = modeSlecSub ? true : false;
}

void Menu::sub1()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    gui.addSubItem("Set Temp  :");
    gui.addSubItem("Set Error :");
    // Thiết lập nhiệt độ mục tiêu và sai số cho phép

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 30);
    display.print("Temp:");
    (std::isnan(temp)) ? display.println("ERROR") : display.println(temp, 2);

    display.setTextSize(1);
    display.setCursor(66, 8);
    display.println(setTemp, 1); // Hiển thị `setTemp` với 1 chữ số thập phân
    display.setCursor(66, 16);
    display.println(setError, 1); // Hiển thị `setError` với 1 chữ số thập phân

    // In các mục con
    printSubMenu();

    if (flagSet)
    {
        if (slecSubList == 1)
        {
            encoder.setCounters(0, 500); // Giới hạn từ 0-500
            setTemp = encoder.getCounter();
        }
        else if (slecSubList == 2)
        {
            encoder.setCounters(0, 100); // Giới hạn từ 0-100
            setError = encoder.getCounter() * 0.1;
        }
    }
    // Serial.println("Set temp: " + String(setTemp));
    // Serial.println("Set Error: " + String(setError));
}

void Menu::sub2()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    gui.addSubItem("Set Timer OFF:");

    // Hiển thị nhiệt độ
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 32);
    display.print("Temp:");
    display.println(std::isnan(temp) ? "ERROR" : String(temp, 2));

    // Hiển thị thời gian dưới dạng phút
    display.setCursor(0, 48);
    display.print("Time:");
    display.println(setTimerTemp / (60 * 1000)); // Hiển thị `setTimerTemp` dưới dạng phút

    // In các mục con
    printSubMenu();

    // Cập nhật hẹn giờ nếu `Set Timer OFF` được chọn
    if (flagSet && slecSubList == 1)
    {
        encoder.setCounters(0, 500);                     // Giới hạn từ 0 đến 500 phút
        setTimerTemp = encoder.getCounter() * 60 * 1000; // Cập nhật hẹn giờ (mili giây)
    }
}
void Menu::sub3()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");
    gui.addSubItem("Run PID");
    gui.addSubItem("Run Time");
    gui.addSubItem("OFF ALL");

    // Cập nhật dữ liệu nhiệt độ
    if (timeStep < MAX_POINTS)
    {
        temperatureData[timeStep] = temp;
        timeStep++;
    }
    else
    {
        // Dịch dữ liệu qua trái khi mảng đầy
        for (int i = 0; i < MAX_POINTS - 1; i++)
        {
            temperatureData[i] = temperatureData[i + 1];
        }
        temperatureData[MAX_POINTS - 1] = temp;
    }

    // Vẽ đồ thị nhiệt độ
    display.clearDisplay();

    for (int x = 0; x < timeStep - 1; x++)
    {
        int y1 = SCREEN_HEIGHT - map(temperatureData[x], 25, 100, 0, SCREEN_HEIGHT); // 25 là điểm thấp nhất
        int y2 = SCREEN_HEIGHT - map(temperatureData[x + 1], 25, 100, 0, SCREEN_HEIGHT);
        display.drawLine(x, y1, x + 1, y2, SSD1306_WHITE); // Vẽ đường nối giữa 2 điểm
    }

    display.setTextSize(1);
    display.setCursor(66, 8);
    display.setTextColor((PID_Temp == true) ? SSD1306_BLACK : SSD1306_WHITE);
    display.print(PID_Temp ? "ON" : "OFF");

    display.setCursor(66, 16);
    display.setTextColor((timer_Temp == true) ? SSD1306_BLACK : SSD1306_WHITE);
    display.println(timer_Temp ? "ON" : "OFF");

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(68, 30);
    display.println(std::isnan(temp) ? "ERROR" : String(temp, 2)); // Hiển thị nhiệt độ

    display.setCursor(68, 42);
    display.println(setTimerTemp / (60 * 1000));
    display.setCursor(68, 50);
    display.println(setTemp);

    display.setCursor(0, 42);
    display.println(tempSensor.readADC());
    display.setCursor(0, 50);
    display.println(setError, 1); // Hiển thị `setError` với 1 chữ số thập phân
    display.println();
    // In các mục con
    printSubMenu();

    if (flagSet)
    {
        if (slecSubList == 1)
        {
            PID_Temp = true;    // Đổi trạng thái PID
            timer_Temp = false; // Tắt chế độ hẹn giờ
            _PID_Temp();
        }
        else if (slecSubList == 2)
        {
            timer_Temp = true; // Đổi trạng thái hẹn giờ
            PID_Temp = false;  // Tắt chế độ PID
            _timer_Temp();     // Gọi hàm xử lý hẹn giờ
        }
        else if (slecSubList == 3)
        {
            PID_Temp = false;   // Tắt chế độ PID
            timer_Temp = false; // Tắt chế độ hẹn giờ
            _PID_Temp();
            _timer_Temp(); // Gọi hàm xử lý hẹn giờ
        }
    }
}

void Menu::sub4()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");

    display.clearDisplay();
    // In các mục con
    printSubMenu();
}
void Menu::sub5()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");

    display.clearDisplay();
    // In các mục con
    printSubMenu();
}
void Menu::sub6()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");

    display.clearDisplay();
    // In các mục con
    printSubMenu();
}
void Menu::sub7()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");

    display.clearDisplay();
    // In các mục con
    printSubMenu();
}
void Menu::sub8()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");

    display.clearDisplay();
    // In các mục con
    printSubMenu();
}
void Menu::sub9()
{
    gui.clearSubItems(); // Xóa các mục con cũ
    gui.addSubItem("Back");

    display.clearDisplay();
    // In các mục con
    printSubMenu();
}

#endif

void Menu::_PID_Temp()
{
    if (PID_Temp)
    {
        digitalWrite(13, tempSensor.isTemperatureInRange());
    }
    else
    {
        digitalWrite(13, 0);
    }
    // Serial.println(tempSensor.isTemperatureInRange());
}

static bool flag_getMillis = false;
void Menu::_timer_Temp()
{
    if (timer_Temp)
    {
        previousMillis = (flag_getMillis == false) ? millis() : previousMillis;
        // Nếu nhiệt độ không nằm trong phạm vi, bật chân 13 và lưu thời gian
        digitalWrite(13, tempSensor.isTemperatureInRange());
        flag_getMillis = true;
        // Kiểm tra nếu đã hết thời gian hẹn giờ
        if (millis() - previousMillis >= setTimerTemp)
        {
            digitalWrite(13, LOW); // Tắt chân 13
            timer_Temp = false;    // Tắt trạng thái `timer_Temp` để không lặp lại
            flag_getMillis = false;
        }
    }
    else
    {
        digitalWrite(13, LOW); // Đảm bảo chân 13 luôn tắt nếu `timer_Temp` không bật
    }
}