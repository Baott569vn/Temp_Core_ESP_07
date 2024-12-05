#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <Arduino.h>

/**
 * @class RotaryEncoder
 * @brief Lớp quản lý bộ mã hóa quay (rotary encoder) với khả năng xử lý nút nhấn.
 */
class RotaryEncoder
{
public:
  /**
   * @brief Khởi tạo đối tượng RotaryEncoder.
   * @param clkPin Chân tín hiệu CLK của encoder.
   * @param dtPin Chân tín hiệu DT của encoder.
   * @param swPin Chân nút nhấn.
   * @param minCounter Giá trị nhỏ nhất của bộ đếm.
   * @param maxCounter Giá trị lớn nhất của bộ đếm.
   */
  RotaryEncoder(uint8_t clkPin, uint8_t dtPin, uint8_t swPin, int minCounter, int maxCounter);

  /**
   * @brief Cấu hình các chân và ISR, khởi tạo callback.
   * @param callback Con trỏ hàm được gọi khi giá trị bộ đếm thay đổi.
   */
  void begin(void (*callback)(int) = nullptr);

  /**
   * @brief Vòng lặp chính kiểm tra trạng thái nút nhấn.
   */
  void loop();

  /**
   * @brief Lấy giá trị hiện tại của bộ đếm.
   * @return Giá trị của bộ đếm.
   */
  int getCounter() const;

  /**
   * @brief Đặt lại bộ đếm về 0.
   */
  void resetCounter();

  /**
   * @brief Kiểm tra trạng thái nút nhấn.
   * @return `true` nếu nút được nhấn, `false` nếu không.
   */
  bool isPressed();

  /**
   * @brief Cập nhật giá trị của minCounter và maxCounter cùng lúc.
   * @param minCounter Giá trị minCounter mới.
   * @param maxCounter Giá trị maxCounter mới.
   */
  void setCounters(int minCounter, int maxCounter);

private:
  uint8_t _clkPin, _dtPin, _swPin;        ///< Các chân kết nối của encoder và nút nhấn.
  int _counter, _minCounter, _maxCounter; ///< Giá trị bộ đếm và giới hạn.
  volatile bool _buttonPressed;           ///< Trạng thái nút nhấn.

  unsigned long _lastPressTime;             ///< Thời gian nhấn nút gần nhất.
  const unsigned long _debounceDelay = 200; ///< Thời gian chống nhiễu (ms).

  static RotaryEncoder *_instance; ///< Con trỏ tĩnh để sử dụng trong ISR.

  void (*_callback)(int); ///< Con trỏ hàm callback.

  // ISR handlers
  static void IRAM_ATTR handleEncoderInterrupt();
  static void IRAM_ATTR handleButtonInterrupt();

  // Internal methods
  void IRAM_ATTR readEncoder();
  void IRAM_ATTR buttonPress();
};

#endif
