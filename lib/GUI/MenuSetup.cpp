#include "MenuSetup.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RotaryEncoder.h>

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

// Vị trí của encoder và offset cho màn hình hiển thị
int encoderPosition = 0; // Vị trí của encoder, dùng để theo dõi vị trí hiện tại của menu
int displayOffset = 0;   // Vị trí bắt đầu hiển thị của danh sách mục menu (tương ứng với dòng đầu tiên)

unsigned long lastDebounceTime = 0; // Thời gian lần cuối cùng cập nhật trạng thái của encoder
unsigned long debounceDelay = 50;   // Độ trễ debounce (10 ms)

byte numColForOled = 8; // Số dòng tối đa mà màn hình OLED có thể hiển thị (8 dòng)

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
    for (int i = 0; i < sizeof(mainItems) / sizeof(mainItems[0]); ++i)
        gui.addMainItem(mainItems[i]);

    // Thêm mục con vào các mục menu chính
    gui.addSubItem("Temp Graph", "Back");
    gui.addSubItem("Temp Setting", "Edit");
    gui.addSubItem("Temp Setting", "Back");

    gui.addSubItem("Wifi Graph", "Back");
    gui.addSubItem("Wifi Connector", "Back");
    gui.addSubItem("Phone Connector", "Back");
    gui.addSubItem("Linux", "Back");
    gui.addSubItem("Macro", "Back");
    gui.addSubItem("Win 10", "Back");
    gui.addSubItem("Win 11", "Back");
    gui.addSubItem("Win 7", "Back");
}

/// @brief Hàm kiểm tra và xử lý các sự kiện từ encoder
void handleEncoder()
{
    unsigned long currentTime = millis(); // Lấy thời gian hiện tại
    // Kiểm tra xem đã qua đủ thời gian debounce chưa (10ms)
    if ((currentTime - lastDebounceTime) > debounceDelay)
    {
        encoder.loop();                 // Cập nhật trạng thái của encoder
        lastDebounceTime = currentTime; // Cập nhật lại thời gian lần cuối cập nhật encoder
    }
}

/// @brief Hàm xử lý và hiển thị menu lên màn hình OLED
/// @param gui Đối tượng GUI dùng để thao tác với menu
void Menu(GUI &gui)
{
    int mainItemsCount = gui.getMainItems().size(); // Lấy số lượng mục chính trong menu
    encoder.setCounters(0, mainItemsCount - 1);     // Cập nhật giới hạn cho bộ đếm encoder

    int newEncoderPosition = encoder.getCounter(); // Lấy giá trị bộ đếm của encoder

    // Nếu vị trí của encoder thay đổi, cập nhật giá trị và điều chỉnh offset
    if (encoderPosition != newEncoderPosition)
    {
        encoderPosition = newEncoderPosition;

        // Điều chỉnh offset dựa trên sự di chuyển của encoder
        // Nếu encoder di chuyển lên (giảm giá trị), giảm offset (cuộn lên)
        displayOffset = constrain(displayOffset + ((encoderPosition < displayOffset) ? -1 : (encoderPosition >= displayOffset + numColForOled ? 1 : 0)), 0, mainItemsCount - numColForOled);
    }

    // Hiển thị danh sách menu lên màn hình OLED
    display.clearDisplay();              // Xóa màn hình
    display.setTextSize(1);              // Đặt kích thước văn bản
    display.setTextColor(SSD1306_WHITE); // Đặt màu chữ là trắng
    display.setCursor(0, 0);             // Đặt con trỏ vẽ tại vị trí (0, 0)

    // Hiển thị các mục menu, tối đa `munColForOled` dòng (8 dòng)
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
