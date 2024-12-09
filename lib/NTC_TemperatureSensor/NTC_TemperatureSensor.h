#ifndef NTC_TEMPERATURE_SENSOR_H
#define NTC_TEMPERATURE_SENSOR_H

#include <math.h>
#include <Arduino.h>

// Các hằng số cấu hình
#define ADC_REF_VOLTAGE 1.0        // Điện áp tham chiếu của ADC (ESP8266 hỗ trợ 0-1V)
#define ADC_MAX 1024.0             // Giá trị tối đa của ADC (10-bit ADC)
#define SERIES_RESISTOR 5000       // Điện trở nối tiếp với NTC (10kΩ * 5 = 50kΩ)
#define NOMINAL_RESISTANCE 10000   // Điện trở danh định của NTC tại 25°C
#define NOMINAL_TEMPERATURE 25     // Nhiệt độ danh định (25°C)
#define BETA_COEFFICIENT 3950      // Hệ số Beta của NTC

class NTC_TemperatureSensor {
  public:
    // Constructor
    NTC_TemperatureSensor(int pin);

    // Hàm để tính toán nhiệt độ từ giá trị ADC
    double calculateTemperature(int rawADC);

    // Hàm đọc giá trị nhiệt độ từ cảm biến
    double readTemperature();

    // Hàm đọc giá trị ADC từ cảm biến
    int readADC();

    // Thiết lập giá trị nhiệt độ mục tiêu
    void setTargetTemperature(double target, double tolerance);

    // Kiểm tra nếu nhiệt độ nằm trong phạm vi mục tiêu
    bool isTemperatureInRange();

  private:
    int sensorPin;          // Chân cảm biến (ví dụ: A0)
    double targetTemperature; // Nhiệt độ mục tiêu
    double tolerance;       // Sai số cho phép
};

#endif
