/**
 * ESP32 and LoRa Communication - Receiving Data
 * 
 * This code receives data from a LoRa module (E32), processes it, and prints the received data
 * to the Serial monitor. The data structure includes analog sensor values and MPU9255 sensor data.
 */

#include <Wire.h>
#include "LoRa_E32.h"

// Pin definitions for LoRa E32 module
#define AUX_PIN 4
#define M0_PIN 32
#define M1_PIN 33
#define RX 27
#define TX 35

// Create a HardwareSerial object for Serial1
HardwareSerial mySerial(1);

// Initialize LoRa_E32 object with specified parameters
LoRa_E32 e32(TX, RX, &mySerial, UART_BPS_RATE_19200);

// Structure to hold received data
struct Signal {
  byte analog_1[4];
  byte analog_2[4];
  byte analog_3[4];
  byte analog_4[4];
  byte accX[4];
  byte accY[4];
  byte accZ[4];
  byte gyroX[4];
  byte gyroY[4];
  byte gyroZ[4];
  // uint8_t padding[25]; // space filling the remaining 26 bytes
} data;

void setup() {
  // Set pin modes for LoRa module
  pinMode(M0_PIN, OUTPUT);
  pinMode(M1_PIN, OUTPUT);
  digitalWrite(M0_PIN, LOW);
  digitalWrite(M1_PIN, LOW);

  // Initialize serial communication
  Serial.begin(19200);

  // Delay for setup
  delay(500);

  // Initialize LoRa module
  e32.begin();

  // Delay for module setup
  delay(500);
}

void loop() {
  // Check if data is available to receive
  ResponseStructContainer rsc = e32.receiveMessage(sizeof(Signal));

  // If data is received correctly
  if (rsc.status.code == 1) { 
    // Copy received data into the Signal structure
    data = *(Signal*)rsc.data;
    rsc.close();

    // Print received data to Serial
    Serial.print("Data: ");

    Serial.print("Analog 1: ");
    Serial.print(*(int*)(data.analog_1));  Serial.print(" ");
    Serial.print("Analog 2: ");
    Serial.print(*(int*)(data.analog_2));  Serial.print(" ");
    Serial.print("Analog 3: ");
    Serial.print(*(int*)(data.analog_3));  Serial.print(" ");
    Serial.print("Analog 4: ");
    Serial.print(*(int*)(data.analog_4));  Serial.print(" ");

    Serial.print("AX: ");
    Serial.print(*(float*)(data.accX)); Serial.print(" ");
    Serial.print("AY: ");
    Serial.print(*(float*)(data.accY)); Serial.print(" ");
    Serial.print("AZ: ");
    Serial.print(*(float*)(data.accZ)); Serial.print(" ");

    Serial.print("GX: ");
    Serial.print(*(float*)(data.gyroX)); Serial.print(" ");
    Serial.print("GY: ");
    Serial.print(*(float*)(data.gyroY)); Serial.print(" ");
    Serial.print("GZ: ");
    Serial.println(*(float*)(data.gyroZ));
  } 
  else 
  {
    // If there is an error in receiving data
    Serial.println("Data reception error");
  }
}
