#ifndef GUI_OLED_H
#define GUI_OLED_H

#include <Arduino.h>
#include <vector>
#include <algorithm>

class GUI_OLED
{
private:
    std::vector<String> mainList_Arr; // Giữ lại std::vector
    std::vector<String> subList_Arr;  // Giữ lại std::vector

public:
    GUI_OLED();
    // void begin(unsigned int &baudR); // Nếu cần, bỏ comment và triển khai hàm này

    // Thêm các Item vào main và sub list
    void addMainItem(const String &menuList);
    void addSubItem(const String &subList);

    // Gọi các Item vào main và sub list (tùy vị trí nạp nếu cần)
    /*
        Stream giúp bạn tái sử dụng hàm với các đối tượng như:
        Serial (cổng USB).
        WiFiClient (kết nối qua WiFi).
        EthernetClient (kết nối qua Ethernet)
    */
    void printMainItem(Stream &outputList); // In item main list
    void printSubItem(Stream &outputList);  // In item sub list

    // Trả về danh sách dưới dạng vector (tùy chọn nếu vẫn muốn sử dụng vector)
    std::vector<String> getMainItem(); // Vector main list
    std::vector<String> getsubItem();  // Vector sub list

    void clearSubItems();
};

#endif // GUI_OLED_H
