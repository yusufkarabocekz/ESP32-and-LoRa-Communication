/**
 * ESP32 and LoRa Communication with MPU9255 and Analog Sensors
 * 
 * This code reads data from the MPU9255 sensor and four analog sensors, processes the data,
 * and sends it via LoRa using the E32 module. The data is transmitted in a structured format.
 */

#include <Wire.h>
#include <MPU9255.h>
#include <LoRa_E32.h>

// Pin definitions
#define RX 16
#define TX 17
#define AUX_PIN 23
#define M0 32
#define M1 33

// Constants
#define G 9.81 // 1g ~ 9.81 m/s^2
#define MAGNETOMETER_CAL 0.06 // Magnetometer calibration

// Analog pins
const int analogPin_1 = 27;
const int analogPin_2 = 14;
const int analogPin_3 = 13;
const int analogPin_4 = 12;

// Analog values
int analogValue_1 = 0;
int analogValue_2 = 0;
int analogValue_3 = 0;
int analogValue_4 = 0;

// MPU9255 instance
MPU9255 mpu;

// Serial1 instance for LoRa
HardwareSerial mySerial(1); 
LoRa_E32 e32(TX, RX, &mySerial, UART_BPS_RATE_19200);

// Structure for sensor data
struct Signal {
  byte analog_1[2];
  byte analog_2[2];
  byte analog_3[2];
  byte analog_4[2];
  byte accX[4];
  byte accY[4];
  byte accZ[4];
  byte gyroX[4];
  byte gyroY[4];
  byte gyroZ[4];
};

// Data instance
Signal data;

// Function to process acceleration data
double process_acceleration(int input, scales sensor_scale) {
  double output = 1;
  if (sensor_scale == scale_2g)
    output = input / 16384.0 * G;
  else if (sensor_scale == scale_4g)
    output = input / 8192.0 * G;
  else if (sensor_scale == scale_8g)
    output = input / 4096.0 * G;
  else if (sensor_scale == scale_16g)
    output = input / 2048.0 * G;
  return output;
}

// Function to process angular velocity data
double process_angular_velocity(int16_t input, scales sensor_scale) {
  if (sensor_scale == scale_250dps)
    return input / 131.0;
  else if (sensor_scale == scale_500dps)
    return input / 65.5;
  else if (sensor_scale == scale_1000dps)
    return input / 32.8;
  else if (sensor_scale == scale_2000dps)
    return input / 16.4;
  return 0;
}

void setup() {
  // Pin modes for LoRa module
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);
  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  // Initialize serial communication
  Serial.begin(19200);
  mySerial.begin(19200, SERIAL_8N1, 27, 35); // RX = 27, TX = 35 Serial1
  Wire.begin();

  // Initialize MPU9255
  mpu.init();

  // Initialize LoRa module
  e32.begin();

  // Delay for module setup
  delay(500);
}

void loop() {
  // Read sensor data from MPU9255
  mpu.read_acc();
  mpu.read_gyro();
  mpu.read_mag();

  // Read analog sensor values
  analogValue_1 = analogRead(analogPin_1);
  analogValue_2 = analogRead(analogPin_2);
  analogValue_3 = analogRead(analogPin_3);
  analogValue_4 = analogRead(analogPin_4);

  // Print sensor data to Serial
  Serial.print("Sent Data: ");
  Serial.print("Analog Value 1: "); Serial.print(analogValue_1); Serial.print(" ");
  Serial.print("Analog Value 2: "); Serial.print(analogValue_2); Serial.print(" ");
  Serial.print("Analog Value 3: "); Serial.print(analogValue_3); Serial.print(" ");
  Serial.print("Analog Value 4: "); Serial.print(analogValue_4); Serial.print(" ");

  Serial.print("AccelX: "); Serial.print(process_acceleration(mpu.ax, scale_2g)); Serial.print(" ");
  Serial.print("AccelY: "); Serial.print(process_acceleration(mpu.ay, scale_2g)); Serial.print(" ");
  Serial.print("AccelZ: "); Serial.print(process_acceleration(mpu.az, scale_2g)); Serial.print(" ");

  Serial.print("GyroX: "); Serial.print(process_angular_velocity(mpu.gx, scale_250dps)); Serial.print(" ");
  Serial.print("GyroY: "); Serial.print(process_angular_velocity(mpu.gy, scale_250dps)); Serial.print(" ");
  Serial.print("GyroZ: "); Serial.print(process_angular_velocity(mpu.gz, scale_250dps)); Serial.println(" ");

  // Store sensor data in structure
  *(int*)(data.analog_1) = analogValue_1;
  *(int*)(data.analog_2) = analogValue_2;
  *(int*)(data.analog_3) = analogValue_3;
  *(int*)(data.analog_4) = analogValue_4;
  *(float*)(data.accX) = process_acceleration(mpu.ax, scale_2g);
  *(float*)(data.accY) = process_acceleration(mpu.ay, scale_2g);
  *(float*)(data.accZ) = process_acceleration(mpu.az, scale_2g);
  *(float*)(data.gyroX) = process_angular_velocity(mpu.gx, scale_250dps);
  *(float*)(data.gyroY) = process_angular_velocity(mpu.gy, scale_250dps);
  *(float*)(data.gyroZ) = process_angular_velocity(mpu.gz, scale_250dps);

  // Delay to manage transmission frequency
  delay(25);

  // Send data via LoRa
  ResponseStatus rs = e32.sendFixedMessage(0, 2, 23, &data, sizeof(Signal));
  Serial.println(rs.getResponseDescription());
}
