#ifndef GUI_H
#define GUI_H

#include <Arduino.h>
#include <vector>
#include <map>
#include <algorithm>

class GUI
{
private:
  struct Node
  {
    std::vector<String> items;       // Danh sách mục con
    std::map<String, Node> subNodes; // Bản đồ các nút con
  };

  Node root;                     // Nút gốc
  Node *currentNode = &root;     // Con trỏ đến nút hiện tại
  std::vector<String> mainItems; // Danh sách các mục chính

  // Hàm tìm nút dựa trên tên
  Node *findNode(Node *node, const String &itemName);

public:
  GUI();
  void GUI_begin(); // Thêm khai báo
  // Phương thức truy xuất danh sách các mục chính
  const std::vector<String> &getMainItems() const { return mainItems; }

  // Getter để lấy danh sách chính và phụ
  std::vector<String> getSubItems(const String &parentItem);

  // Thêm mục chính hoặc mục con
  void addMainItem(const String &mainItem);
  void addSubItem(const String &parentItem, const String &subItem);

  // Hiển thị danh sách
  void GUI_list(Stream &output);                              // Xuất danh sách chính
  void GUI_listSub(const String &parentItem, Stream &output); // Xuất danh sách con
  // Trả về số lượng mục con của một mục chính
  size_t getSubItemCount(const String &mainItem);

};

#endif
