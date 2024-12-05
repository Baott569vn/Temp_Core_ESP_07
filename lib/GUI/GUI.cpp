#include "GUI.h"

// Hàm khởi tạo
GUI::GUI() {}

void GUI::GUI_begin()
{
    // Khởi tạo nếu cần, hiện tại để trống.
}

// Tìm nút dựa trên tên
GUI::Node *GUI::findNode(Node *node, const String &itemName)
{
    if (!node)
        return nullptr;
    if (node->subNodes.find(itemName) != node->subNodes.end())
    {
        return &(node->subNodes[itemName]);
    }
    return nullptr;
}

// Thêm mục chính
void GUI::addMainItem(const String &mainItem)
{
    if (std::find(mainItems.begin(), mainItems.end(), mainItem) == mainItems.end())
    {
        mainItems.push_back(mainItem); // Thêm mục chính nếu chưa tồn tại
    }
}

// Thêm mục con
void GUI::addSubItem(const String &parentItem, const String &subItem)
{
    Node *parentNode = findNode(&root, parentItem);

    if (!parentNode)
    {
        parentNode = &root.subNodes[parentItem]; // Tạo mục cha nếu chưa tồn tại
    }

    if (std::find(parentNode->items.begin(), parentNode->items.end(), subItem) == parentNode->items.end())
    {
        parentNode->items.push_back(subItem); // Thêm mục con nếu chưa tồn tại
    }
}

// Hiển thị danh sách chính
void GUI::GUI_list(Stream &output)
{
    output.println("Main List:");
    for (const auto &item : mainItems)
    {
        output.println(item);
    }
}

// Hiển thị danh sách mục con
void GUI::GUI_listSub(const String &parentItem, Stream &output)
{
    Node *parentNode = findNode(&root, parentItem);

    if (parentNode)
    {
        if (!parentNode->items.empty())
        {
            output.println("Sub List:");
            for (const auto &subItem : parentNode->items)
            {
                output.println(subItem);
            }
        }
        else
        {
            output.println("No sub-items available.");
        }
    }
    else
    {
        output.println("Parent item not found in the list.");
    }
}

std::vector<String> GUI::getSubItems(const String &parentItem)
{
    Node *parentNode = findNode(&root, parentItem);
    if (parentNode)
    {
        return parentNode->items;
    }
    return {}; // Trả về danh sách rỗng nếu không tìm thấy
}

size_t GUI::getSubItemCount(const String &mainItem)
{
    Node *parentNode = findNode(&root, mainItem);

    if (parentNode)
    {
        return parentNode->items.size(); // Đếm số phần tử trong danh sách mục con
    }
    else
    {
        return 0; // Nếu không tìm thấy mục chính, trả về 0
    }
}
