# ESP32 and LoRa Communication with MPU9255 and Analog Sensors

This project demonstrates how to read data from an MPU9255 sensor and multiple analog sensors using an ESP32 microcontroller, and transmit the data wirelessly using the LoRa E32 module. The data is sent in a structured format and received by another ESP32, which then prints the data to the Serial monitor.

## Components Used

- ESP32 Dev Kit
- LoRa E32 Module
- MPU9255 Sensor
- Analog Sensors
- Jumper Wires
- Breadboard

  ![image](https://github.com/user-attachments/assets/3128a5dc-4fbc-4622-93ac-db98be26b10e)


## Pin Configuration

### Transmitter ESP32

- **M0_PIN**: GPIO 32
- **M1_PIN**: GPIO 33
- **AUX_PIN**: GPIO 23
- **RX**: GPIO 16
- **TX**: GPIO 17
- **Analog Pin 1**: GPIO 27
- **Analog Pin 2**: GPIO 14
- **Analog Pin 3**: GPIO 13
- **Analog Pin 4**: GPIO 12

### Receiver ESP32

- **M0_PIN**: GPIO 32
- **M1_PIN**: GPIO 33
- **AUX_PIN**: GPIO 4
- **RX**: GPIO 27
- **TX**: GPIO 35

## Installation and Setup

1. **Install Arduino IDE:**
   Download and install the Arduino IDE from the [official website](https://www.arduino.cc/en/software).

2. **Install ESP32 Board:**
   Follow the instructions to install the ESP32 board package:
   - Open Arduino IDE.
   - Go to `File` -> `Preferences`.
   - In the "Additional Board Manager URLs" field, add this URL: `https://dl.espressif.com/dl/package_esp32_index.json`.
   - Go to `Tools` -> `Board` -> `Board Manager` and search for "ESP32". Install the package by Espressif Systems.

3. **Install Required Libraries:**
   Install the following libraries via Arduino Library Manager (`Sketch` -> `Include Library` -> `Manage Libraries...`):
   - `MPU9255`
   - `LoRa_E32`

4. **Connect Components:**
   Connect the ESP32, LoRa module, MPU9255 sensor, and analog sensors as per the pin configuration mentioned above.

5. **Upload Code:**
   - Open `transmitter.ino` in Arduino IDE and upload it to the transmitter ESP32.
   - Open `receiver.ino` in Arduino IDE and upload it to the receiver ESP32.

## Code Explanation

### Transmitter Code

The transmitter code initializes the MPU9255 sensor and reads data from it, along with data from four analog sensors. This data is processed and sent via the LoRa E32 module.

Key sections include:
- **Initialization:** Setting pin modes and initializing Serial, Wire, MPU9255, and LoRa modules.
- **Data Reading:** Reading accelerometer, gyroscope, and magnetometer data from MPU9255, and analog data from the sensors.
- **Data Processing:** Converting raw sensor data to meaningful values.
- **Data Transmission:** Sending the structured data via LoRa.

### Receiver Code

The receiver code receives the structured data from the LoRa E32 module and prints it to the Serial monitor.

Key sections include:
- **Initialization:** Setting pin modes and initializing Serial and LoRa modules.
- **Data Reception:** Receiving data and storing it in the `Signal` structure.
- **Data Display:** Printing the received data to the Serial monitor.

## Usage

1. **Power Up:**
   Power up both the transmitter and receiver ESP32 boards.
   
2. **Monitor Serial Output:**
   Open the Serial Monitor from the Arduino IDE for both transmitter and receiver boards (set baud rate to 19200). The transmitter will send sensor data, and the receiver will display the received data.

3. **Verify Communication:**
   Ensure that the data sent by the transmitter matches the data received and displayed by the receiver.

## Troubleshooting

- Ensure all connections are secure.
- Check the baud rate settings in the code and Serial Monitor.
- Verify the LoRa module configuration and connections.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- [Arduino](https://www.arduino.cc/)
- [ESP32](https://www.espressif.com/)
- [MPU9255](https://www.invensense.com/products/motion-tracking/9-axis/mpu-9255/)
- [LoRa E32](https://www.electrodragon.com/product/e32-ttl-100-lora-ttl-100mw-wireless-module/)

Feel free to modify and expand this project as needed.
