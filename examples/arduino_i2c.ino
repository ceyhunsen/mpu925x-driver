/**
 * @file
 * @author Ceyhun Şen
 * @brief Example usage for MPU-925X driver on Arduino.
 * */

#include "mpu925x.h"
#include <Wire.h>

/**
 * @brief I2C read function.
 * */
uint8_t arduino_i2c_read(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
{
  Wire.begin();
  Wire.beginTransmission(slave_address);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(slave_address, size);
  for (uint16_t i = 0; i < size; i++) {
    if (Wire.available()) {
      buffer[i] = (uint8_t)Wire.read();
    }
    else {
      return 1;
    }
  }
  return 0;
}

/**
 * @brief I2C write function.
 * */
uint8_t arduino_i2c_write(mpu925x_t *mpu925x, uint8_t slave_address, uint8_t reg, uint8_t *buffer, uint8_t size)
{
  Wire.beginTransmission(slave_address);
  Wire.write(reg);
  for (uint16_t i = 0; i < size; i++) {
    Wire.write(buffer[i]);
  }
  Wire.endTransmission();
  return 0;
}

/**
 * @brief Delay function.
 * */
void arduino_delay_ms(mpu925x_t *mpu925x, uint32_t dly)
{
  delay(dly);
}

mpu925x_t mpu925x;

void setup() {
  // Pass the pointers to driver.
  mpu925x.master_specific.bus_read = arduino_i2c_read;
  mpu925x.master_specific.bus_write = arduino_i2c_write;
  mpu925x.master_specific.delay_ms = arduino_delay_ms;

  // Initialize driver and sensor.
  uint8_t return_value = mpu925x_init(&mpu925x, 0);

  // Print initialization value.
  Serial.begin(9600);
  if (!return_value) {
    Serial.println("MPU-925X sensor is ready!");
  }
  else {
    Serial.println("MPU-925X sensor is not ready! Check wiring and restart Arduino.");
    while (1);
  }

  // Optional calibration.
  Serial.println("Calibrating accelerometer, please hold still and wait...");
  mpu925x_accelerometer_offset_cancellation(&mpu925x, 255);
  Serial.println("Calibration complete.");

  Serial.println("Calibrating gyroscope, please hold still and wait...");
  mpu925x_gyroscope_offset_cancellation(&mpu925x, 255);
  Serial.println("Calibration complete.");
}

void loop() {
  // Read all sensor data and store it in mpu925x.
  mpu925x_get_all(&mpu925x);

  // Convert readed data to string.
  String acceleration = (String)mpu925x.sensor_data.acceleration[0] + "\t" + (String)mpu925x.sensor_data.acceleration[1] + "\t" + (String)mpu925x.sensor_data.acceleration[2];
  String rotation = (String)mpu925x.sensor_data.rotation[0] + "\t" + (String)mpu925x.sensor_data.rotation[1] + "\t" + (String)mpu925x.sensor_data.rotation[2];
  String magnetic_field = (String)mpu925x.sensor_data.magnetic_field[0] + "\t" + (String)mpu925x.sensor_data.magnetic_field[1] + "\t" + (String)mpu925x.sensor_data.magnetic_field[2];
  String temperature = (String)mpu925x.sensor_data.temperature;

  // Print sensor data.
  Serial.print("Acceleration: ");
  Serial.println(acceleration);
  Serial.print("Rotation: ");
  Serial.println(rotation);
  Serial.print("Magnetic field: ");
  Serial.println(magnetic_field);
  Serial.print("Temperature: ");
  Serial.println(temperature);
  Serial.println("");

  delay(1000);
}
