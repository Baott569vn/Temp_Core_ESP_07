#include "MenuSetup.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RotaryEncoder.h>
#include <string>

// Định nghĩa chân kết nối cho encoder (quay và nút nhấn)
#define CLK_PIN 14 // Chân CLK của Encoder
#define DT_PIN 16  // Chân DT của Encoder
#define SW_PIN 12  // Chân nút nhấn của Encoder

// Khởi tạo đối tượng encoder, giá trị từ 0 đến 100
RotaryEncoder encoder(CLK_PIN, DT_PIN, SW_PIN, 0, 100);

// Định nghĩa kích thước màn hình OLED
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Khởi tạo đối tượng màn hình OLED với kích thước đã định nghĩa
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void handleEncoder(GUI &gui);
void displayMenu(GUI &gui, int displayOffset, int numColForOled, int mainItemsCount);
void menuSub1(GUI &gui, int displayOffset, int numColForOled, int mainItemsCount, String nameSubList);
void menuSub2(GUI &gui, int displayOffset, int numColForOled, int mainItemsCount, String nameSubList);

// Vị trí của encoder và offset cho màn hình hiển thị
int encoderPosition = 0; // Vị trí của encoder, dùng để theo dõi vị trí hiện tại của menu
int _displayOffset = 0;  // Vị trí bắt đầu hiển thị của danh sách mục menu (tương ứng với dòng đầu tiên)

unsigned long lastDebounceTime = 0; // Thời gian lần cuối cùng cập nhật trạng thái của encoder
unsigned long debounceDelay = 50;   // Độ trễ debounce (10 ms)

byte numColForOled = 8; // Số dòng tối đa mà màn hình OLED có thể hiển thị (8 dòng)

static bool flagSubList = false;

/// @brief Hàm khởi tạo menu ban đầu, thiết lập các mục menu chính và mục con
/// @param gui Đối tượng GUI dùng để thao tác với menu
void setupMenu(GUI &gui)
{
    gui.GUI_begin();                           // Khởi tạo GUI
    encoder.begin();                           // Khởi tạo encoder
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Khởi tạo màn hình OLED với địa chỉ 0x3C

    // Thêm các mục chính vào menu
    const char *mainItems[] = {
        "Temp Graph", "Temp Setting", "Wifi Graph", "Wifi Connector",
        "Phone Connector", "Wed View", "Linux", "Macro", "Win 10",
        "Win 11", "Win 7"};

    // Lặp qua mảng và thêm các mục chính vào menu
    for (size_t i = 0; i < sizeof(mainItems) / sizeof(mainItems[0]); ++i)
        gui.addMainItem(mainItems[i]);

    // Thêm mục con vào các mục menu chính
    gui.addSubItem("Temp Graph", "Back");
    gui.addSubItem("Temp Graph", "Edit");
    gui.addSubItem("Temp Graph", "About");
    gui.addSubItem("Temp Graph", "Edit");


    gui.addSubItem("Temp Setting", "Back");
    gui.addSubItem("Temp Setting", "Edit");
    gui.addSubItem("Temp Setting", "Support");

    gui.addSubItem("Wifi Graph", "Back");
    gui.addSubItem("Wifi Connector", "Back");
    gui.addSubItem("Phone Connector", "Back");
    gui.addSubItem("Linux", "Back");
    gui.addSubItem("Macro", "Back");
    gui.addSubItem("Win 10", "Back");
    gui.addSubItem("Win 11", "Back");
    gui.addSubItem("Win 7", "Back");
}

int mainItemsCount;     // Lấy số lượng mục chính trong menu
int newEncoderPosition; // Lấy giá trị bộ đếm của encoder
int valueFrist;
/// @brief Hàm kiểm tra và xử lý các sự kiện từ encoder
void handleEncoder(GUI &gui)
{
    unsigned long currentTime = millis(); // Lấy thời gian hiện tại
    // Kiểm tra xem đã qua đủ thời gian debounce chưa (10ms)
    if ((currentTime - lastDebounceTime) > debounceDelay)
    {
        encoder.loop(); // Cập nhật trạng thái của encoder

        encoder.setCounters(0, mainItemsCount - 1); // Cập nhật giới hạn cho bộ đếm encoder

        newEncoderPosition = encoder.getCounter(); // Lấy giá trị bộ đếm của encoder

        // Nếu vị trí của encoder thay đổi, cập nhật giá trị và điều chỉnh offset
        if (encoderPosition != newEncoderPosition)
        {
            encoderPosition = newEncoderPosition;

            Serial.println(encoderPosition);
            // Điều chỉnh offset dựa trên sự di chuyển của encoder
            // Nếu encoder di chuyển lên (giảm giá trị), giảm offset (cuộn lên)
            _displayOffset = constrain(_displayOffset + ((encoderPosition < _displayOffset) ? -1 : (encoderPosition >= _displayOffset + numColForOled ? 1 : 0)), 0, mainItemsCount - numColForOled);

            Serial.println(String(">>>") + _displayOffset);
        }

        lastDebounceTime = currentTime; // Cập nhật lại thời gian lần cuối cập nhật encoder
    }
}

/// @brief Hàm xử lý và hiển thị menu lên màn hình OLED
/// @param gui Đối tượng GUI dùng để thao tác với menu
void Menu(GUI &gui)
{
    if (flagSubList == false)
    {
        mainItemsCount = gui.getMainItems().size(); // Lấy số lượng mục chính trong menu

        // Hiển thị danh sách menu lên màn hình OLED
        displayMenu(gui, _displayOffset, numColForOled, mainItemsCount);

        if (encoder.isPressed())
        {
            Serial.println("Pressed");
            encoder.resetCounter();
            _displayOffset = 0;
            flagSubList = true;
        }
    }

    if (flagSubList == true)
    {
        switch (_displayOffset)
        {
        case 0:
            mainItemsCount = gui.getSubItems("Temp Graph").size();
            menuSub1(gui, _displayOffset, numColForOled, mainItemsCount, "Temp Graph");
            break;

        case 1:
            mainItemsCount = gui.getSubItems("Temp Setting").size();
            menuSub2(gui, _displayOffset, numColForOled, mainItemsCount, "Temp Setting");
            break;

        default:
            break;
        }
    }

    void handleEncoder();
}

void displayMenu(GUI &gui, int displayOffset, int numColForOled, int mainItemsCount)
{
    // Hiển thị danh sách menu lên màn hình OLED
    display.clearDisplay();              // Xóa màn hình
    display.setTextSize(1);              // Đặt kích thước văn bản
    display.setTextColor(SSD1306_WHITE); // Đặt màu chữ là trắng
    display.setCursor(0, 0);             // Đặt con trỏ vẽ tại vị trí (0, 0)

    // Hiển thị các mục menu, tối đa numColForOled dòng (8 dòng)
    for (int i = 0; i < numColForOled; ++i)
    {
        int index = displayOffset + i; // Tính chỉ mục để hiển thị mục menu (với offset hiện tại)
        if (index >= mainItemsCount)
            break; // Nếu vượt quá số lượng mục menu, thoát khỏi vòng lặp
        // Nếu mục này là mục được chọn bởi encoder, thay đổi màu sắc để đánh dấu
        display.setTextColor(index == encoder.getCounter() ? SSD1306_BLACK : SSD1306_WHITE, SSD1306_WHITE);
        display.println(gui.getMainItems()[index]); // In tên của mục menu tại vị trí tương ứng
    }

    display.display(); // Cập nhật màn hình OLED sau khi vẽ xong
}

void menuSub1(GUI &gui, int displayOffset, int numColForOled, int mainItemsCount, String nameSubList)
{
    display.clearDisplay();              // Xóa màn hình
    display.setTextSize(1);              // Đặt kích thước văn bản
    display.setTextColor(SSD1306_WHITE); // Đặt màu chữ là trắng
    display.setCursor(0, 0);             // Đặt con trỏ vẽ tại vị trí (0, 0)

    // Lấy các mục con từ sublist
    std::vector<String> subListItems = gui.getSubItems(nameSubList); // Use vector here

    // Hiển thị tất cả các mục trong sublist
    for (int i = 0; i < numColForOled; ++i)
    {
        int index = displayOffset + i; // Tính chỉ mục để hiển thị mục menu
        if (index >= subListItems.size())
            break; // Nếu hết mục trong sublist thì thoát khỏi vòng lặp

        // Nếu mục này là mục được chọn bởi encoder, thay đổi màu sắc để đánh dấu
        display.setTextColor(index == encoder.getCounter() ? SSD1306_BLACK : SSD1306_WHITE, SSD1306_WHITE);
        display.println(subListItems[index]); // In tên của mục con tại vị trí tương ứng
    }

    display.display(); // Cập nhật màn hình OLED sau khi vẽ xong

    // Kiểm tra nếu nhấn nút "Back", quay lại menu chính
    if (encoder.isPressed())
    {
        String selectedItem = subListItems[encoder.getCounter()];
        if (selectedItem == "Back") // Nếu chọn mục "Back"
        {
            Serial.println("Back Pressed");
            flagSubList = false; // Quay lại menu chính
        }
        else
        {
            // Nếu chọn một mục khác, bạn có thể xử lý logic cho sublist con
            // Ví dụ: vào một sublist con mới, hoặc thực hiện hành động nào đó
            Serial.println("Selected item: " + selectedItem);
        }
    }
}

void menuSub2(GUI &gui, int displayOffset, int numColForOled, int mainItemsCount, String nameSubList)
{
    display.clearDisplay();              // Xóa màn hình
    display.setTextSize(1);              // Đặt kích thước văn bản
    display.setTextColor(SSD1306_WHITE); // Đặt màu chữ là trắng
    display.setCursor(0, 0);             // Đặt con trỏ vẽ tại vị trí (0, 0)

    // Lấy các mục con từ sublist
    std::vector<String> subListItems = gui.getSubItems(nameSubList); // Use vector here

    // Hiển thị tất cả các mục trong sublist
    for (int i = 0; i < numColForOled; ++i)
    {
        int index = displayOffset + i; // Tính chỉ mục để hiển thị mục menu
        if (index >= subListItems.size())
            break; // Nếu hết mục trong sublist thì thoát khỏi vòng lặp

        // Nếu mục này là mục được chọn bởi encoder, thay đổi màu sắc để đánh dấu
        display.setTextColor(index == encoder.getCounter() ? SSD1306_BLACK : SSD1306_WHITE, SSD1306_WHITE);
        display.println(subListItems[index]); // In tên của mục con tại vị trí tương ứng
    }

    display.display(); // Cập nhật màn hình OLED sau khi vẽ xong

    // Kiểm tra nếu nhấn nút "Back", quay lại menu chính
    if (encoder.isPressed())
    {
        String selectedItem = subListItems[encoder.getCounter()];
        if (selectedItem == "Back") // Nếu chọn mục "Back"
        {
            Serial.println("Back Pressed");
            flagSubList = false; // Quay lại menu chính
        }
        else
        {
            // Nếu chọn một mục khác, bạn có thể xử lý logic cho sublist con
            Serial.println("Selected item: " + selectedItem);
        }
    }
}