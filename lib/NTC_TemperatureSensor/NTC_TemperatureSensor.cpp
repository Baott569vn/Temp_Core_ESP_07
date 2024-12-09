#include "NTC_TemperatureSensor.h"

// Constructor
NTC_TemperatureSensor::NTC_TemperatureSensor(int pin)
{
  sensorPin = pin;
  pinMode(sensorPin, INPUT);
  targetTemperature = NAN; // Khởi tạo giá trị mục tiêu là NAN
  tolerance = 0.0;         // Khởi tạo sai số bằng 0
}

// Hàm tính nhiệt độ từ giá trị ADC
double NTC_TemperatureSensor::calculateTemperature(int rawADC)
{
  if (rawADC <= 10 || rawADC >= 1014)
  {
    return NAN;
  }

  double resistance = SERIES_RESISTOR * (ADC_MAX / rawADC - 1);
  if (resistance > 1e6 || resistance < 10)
  {
    return NAN;
  }

  double tempKelvin = 1.0 / (log(resistance / NOMINAL_RESISTANCE) / BETA_COEFFICIENT +
                             1.0 / (NOMINAL_TEMPERATURE + 273.15));
  return tempKelvin - 273.15; // Trả về nhiệt độ tính bằng °C
}

// Hàm đọc nhiệt độ trực tiếp
double NTC_TemperatureSensor::readTemperature()
{
  int rawADC = readADC();
  return calculateTemperature(rawADC);
}

// Hàm đọc giá trị ADC từ cảm biến
int NTC_TemperatureSensor::readADC()
{
  return analogRead(sensorPin);
}

// Thiết lập nhiệt độ mục tiêu và sai số
void NTC_TemperatureSensor::setTargetTemperature(double target, double toleranceValue)
{
  targetTemperature = target;
  tolerance = toleranceValue;
}

// Kiểm tra nếu nhiệt độ nằm trong phạm vi mục tiêu
bool NTC_TemperatureSensor::isTemperatureInRange()
{
  double currentTemp = readTemperature();
  if (isnan(currentTemp))
  {
    return false; // Nếu nhiệt độ không hợp lệ, trả về false
  }

  // Kiểm tra nếu nhiệt độ nằm trong phạm vi sai số
  if (abs(currentTemp - targetTemperature) <= tolerance)
  {
    return false; // Nếu nhiệt độ trong phạm vi sai số, trả về false
  }

  // Nếu nhiệt độ nhỏ hơn mức nhiệt độ mục tiêu, trả về true
  return currentTemp < targetTemperature;
}
