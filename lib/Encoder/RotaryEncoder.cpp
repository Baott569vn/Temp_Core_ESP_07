#include "RotaryEncoder.h"

// Khởi tạo con trỏ tĩnh
RotaryEncoder *RotaryEncoder::_instance = nullptr;

/**
 * @brief Hàm khởi tạo.
 */
RotaryEncoder::RotaryEncoder(uint8_t clkPin, uint8_t dtPin, uint8_t swPin, int minCounter, int maxCounter)
    : _clkPin(clkPin), _dtPin(dtPin), _swPin(swPin),
      _counter(0), _minCounter(minCounter), _maxCounter(maxCounter),
      _buttonPressed(false), _lastPressTime(0), _callback(nullptr) {}

/**
 * @brief Khởi tạo encoder và cấu hình ISR.
 */
void RotaryEncoder::begin(void (*callback)(int))
{
    _callback = callback;
    _instance = this;

    pinMode(_clkPin, INPUT);
    pinMode(_dtPin, INPUT);
    pinMode(_swPin, INPUT_PULLUP);

    attachInterrupt(digitalPinToInterrupt(_clkPin), handleEncoderInterrupt, CHANGE);
    attachInterrupt(digitalPinToInterrupt(_swPin), handleButtonInterrupt, FALLING);
}

/**
 * @brief Cập nhật giá trị của minCounter và maxCounter cùng lúc.
 * @param minCounter Giá trị minCounter mới.
 * @param maxCounter Giá trị maxCounter mới.
 */
void RotaryEncoder::setCounters(int minCounter, int maxCounter)
{
    _minCounter = minCounter;
    _maxCounter = maxCounter;
}

/**
 * @brief Vòng lặp chính kiểm tra nút nhấn.
 */
void RotaryEncoder::loop()
{
    if (_buttonPressed)
    {
        _buttonPressed = false; // Reset trạng thái nút nhấn.
    }
}

/**
 * @brief Lấy giá trị bộ đếm.
 */
int RotaryEncoder::getCounter() const
{
    return _counter;
}

/**
 * @brief Đặt lại giá trị bộ đếm về 0.
 */
void RotaryEncoder::resetCounter()
{
    _counter = 0;
}

/**
 * @brief Kiểm tra trạng thái nút nhấn và reset.
 */
bool RotaryEncoder::isPressed()
{
    bool state = _buttonPressed;

    // Sau khi nhận nhấn, reset trạng thái để không nhận lại nhấn xuống cho đến khi nhả ra
    if (_buttonPressed)
    {
        _buttonPressed = false;
        // Serial.println("isPressed");
    }

    return state;
}

/**
 * @brief ISR cho tín hiệu quay của encoder.
 */
void IRAM_ATTR RotaryEncoder::handleEncoderInterrupt()
{
    if (_instance)
    {
        _instance->readEncoder();
    }
}

/**
 * @brief ISR cho nút nhấn.
 */
void IRAM_ATTR RotaryEncoder::handleButtonInterrupt()
{
    if (_instance)
    {
        _instance->buttonPress();
    }
}

/**
 * @brief Đọc tín hiệu encoder và cập nhật bộ đếm.
 */
void IRAM_ATTR RotaryEncoder::readEncoder()
{
    static int lastState = LOW;   // Trạng thái trước của tín hiệu CLK.
    static int lastDTState = LOW; // Trạng thái trước của tín hiệu DT.

    int currentState = digitalRead(_clkPin);
    int currentDTState = digitalRead(_dtPin);

    if (currentState != lastState || currentDTState != lastDTState)
    {
        if (currentState == HIGH)
        { // Khi tín hiệu CLK thay đổi
            if (currentDTState == LOW)
            { // Quay thuận
                if (_counter < _maxCounter)
                {
                    _counter++;
                    if (_callback)
                        _callback(_counter);
                }
            }
            else
            { // Quay ngược
                if (_counter > _minCounter)
                {
                    _counter--;
                    if (_callback)
                        _callback(_counter);
                }
            }
        }
    }

    lastState = currentState;
    lastDTState = currentDTState;
}

/**
 * @brief Xử lý trạng thái nút nhấn với chống nhiễu.
 */
void IRAM_ATTR RotaryEncoder::buttonPress()
{
    unsigned long currentTime = millis();

    // Chỉ nhận nút nhấn khi đã qua thời gian debounce
    if (currentTime - _lastPressTime > _debounceDelay)
    {
        bool buttonState = digitalRead(_swPin) == LOW; // Nút nhấn khi trạng thái là LOW

        // Kiểm tra nếu nút nhấn chuyển từ HIGH sang LOW
        if (buttonState && !_buttonPressed)
        {
            _buttonPressed = true; // Ghi nhận nút nhấn
        }

        _lastPressTime = currentTime; // Cập nhật thời gian nhấn
    }
}