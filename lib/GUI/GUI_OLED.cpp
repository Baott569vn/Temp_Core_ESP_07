#include <string>
#include <set>
#include "GUI_OLED.h"

GUI_OLED::GUI_OLED()
{
    // Constructor có thể không cần gì ở đây, nếu không có gì cần khởi tạo
}

void GUI_OLED::addMainItem(const String &menuList)
{
    if (std::find(mainList_Arr.begin(), mainList_Arr.end(), menuList) == mainList_Arr.end())
    {
        mainList_Arr.push_back(menuList);
    }
}

void GUI_OLED::addSubItem(const String &subList)
{
    if (std::find(subList_Arr.begin(), subList_Arr.end(), subList) == subList_Arr.end())
    {
        subList_Arr.push_back(subList);
    }
}

void GUI_OLED::printMainItem(Stream &outputList)
{
    for (const auto &item : mainList_Arr)
    {
        outputList.println(item);
    }
}

void GUI_OLED::printSubItem(Stream &outputList)
{
    for (const auto &item : subList_Arr)
    {
        outputList.println(item);
    }
}

// Triển khai phương thức trả về danh sách main và sub dưới dạng vector
std::vector<String> GUI_OLED::getMainItem()
{
    return mainList_Arr; // Trả về vector chính xác
}

std::vector<String> GUI_OLED::getsubItem()
{
    return subList_Arr; // Trả về vector chính xác
}

void GUI_OLED::clearSubItems()
{
    subList_Arr.clear(); // Xóa tất cả các item trong subList_Arr
}