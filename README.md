### **BLE-Based Gyroscope Controlled Motor System**

This project demonstrates a **Bluetooth Low Energy (BLE)**-based system to control motor movements using gyroscope data streamed from a mobile device via the **Adafruit Bluefruit Connect app**. The system parses gyroscope data packets, validates them, and uses the parsed values to control a motorized platform's movements.

---

## **Features**

- **BLE Communication**:
  - Implements a robust BLE communication protocol using the Adafruit Bluefruit LE UART Friend module.
  - Reads, validates, and parses gyroscope data packets in real-time.

- **Integration with Bluefruit Connect App**:
  - Utilizes the **Bluefruit Connect app** (available for iOS and Android) to stream gyroscope readings from the phone.
  - Leverages the phone's gyroscope to dynamically control motorized movements.

- **Gyroscope Data Handling**:
  - Parses the gyroscope data format specified by Adafruit:
    - Prefix: `!G`
    - Data format: `[‘!’] [‘G’] [float x] [float y] [float z] [CRC]`
    - Each packet contains X, Y, and Z axis values as 32-bit floats, followed by a single-byte checksum (CRC).
  - Validates the checksum (CRC) to ensure data integrity.

- **Motor Control**:
  - Controls motor actions (forward, backward, left, right, stop) based on gyroscope data.
  - Integrates Pulse Width Modulation (PWM) for precise motor speed control.

- **Debugging and Logging**:
  - Outputs raw BLE data in hexadecimal format for debugging.
  - Logs parsed gyroscope data (`X`, `Y`, `Z`) and motor actions via UART.

---

## **Raw Data Format**

This project is designed to handle BLE data packets sent in the following format, as defined by the **Adafruit Bluefruit Connect app** documentation:

- **Prefix**:
  - `!` (0x21): Start of packet
  - `G` (0x47): Identifies gyroscope data

- **Payload**:
  - `float x` (4 bytes): X-axis value in radians/second
  - `float y` (4 bytes): Y-axis value in radians/second
  - `float z` (4 bytes): Z-axis value in radians/second

- **Checksum**:
  - 1-byte CRC: Computed as the inverted sum of all previous bytes in the packet.

**Example BLE Packet**:
```
Raw Hexadecimal: 21 47 96 E9 71 3D 93 A8 18 3D C7 A8 08 3B 28
Parsed Values:
  X: 0.91
  Y: 0.76
  Z: -0.53
CRC: 0x28 (valid)
```

---

## **System Workflow**

1. **Bluefruit Connect App as Controller**:
   - The phone streams gyroscope data (`X`, `Y`, `Z`) via BLE to the microcontroller.
   - Gyroscope readings are captured when the app's "Controller" mode is active.

2. **BLE Data Reception**:
   - The system continuously receives data packets over BLE.
   - Data packets are validated and checked for integrity using CRC.

3. **Gyroscope Parsing**:
   - Extracts the gyroscope's `X`, `Y`, and `Z` axis values as 32-bit floating-point numbers.

4. **Motor Control**:
   - Based on the gyroscope values:
     - Moves forward/backward depending on the Y-axis tilt.
     - Turns left/right based on the X-axis tilt.
     - Stops if the X and Y values are within a neutral threshold.

5. **Debugging**:
   - Prints raw BLE data and parsed float values to the UART console for monitoring.

---

## **Hardware Requirements**

- **Microcontroller**: MSP432 LaunchPad
- **BLE Module**: Adafruit Bluefruit LE UART Friend
- **Motors**: Two DC motors with PWM control
- **Power Supply**: Appropriate power source for the motors and microcontroller
- **Additional Components**:
  - Motor driver (e.g., L293D or equivalent)
  - Breadboard and jumper wires for connections
- **Mobile Device**: Smartphone with the **Bluefruit Connect app** installed

---

## **Software Components**

### **1. BLE_UART**
- Handles BLE initialization, data transmission, and reception.
- Implements CRC validation and gyroscope packet parsing.

### **2. GyroParser**
- Validates BLE packet structure and checksum.
- Extracts gyroscope values (`X`, `Y`, `Z`) for motor control.

### **3. Motor**
- Provides functions for motor initialization and movement:
  - Forward
  - Backward
  - Left
  - Right
  - Stop
- Configures PWM for precise speed control.

### **4. Main**
- Integrates BLE data handling and motor control.
- Logs debugging information via UART.

---

## **Usage**

1. **Install Bluefruit Connect App**:
   - Download the **Bluefruit Connect app** from the [App Store](https://apps.apple.com/) or [Google Play](https://play.google.com/).

2. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/BLE-Gyroscope-Motor-Control.git
   cd BLE-Gyroscope-Motor-Control
   ```

3. **Flash the Code**:
   - Use your preferred IDE (e.g., Code Composer Studio) to flash the code to the MSP432 LaunchPad.

4. **Connect BLE Module**:
   - Pair your smartphone with the Adafruit BLE UART Friend module via the Bluefruit Connect app.

5. **Stream Gyroscope Data**:
   - In the app, navigate to the **Controller** mode and activate the gyroscope.
   - Tilt your phone to control the motorized platform.

6. **Monitor Debugging Logs**:
   - Use a serial terminal to view raw BLE data and parsed gyroscope values.

---

## **Example BLE Data Workflow**

1. **Raw BLE Data**:
   ```
   21 47 96 E9 71 3D 93 A8 18 3D C7 A8 08 3B 28
   ```
2. **Parsed Floats**:
   ```
   X: 0.91, Y: 0.76, Z: -0.53
   ```
3. **Motor Action**:
   ```
   Motor: Moving Forward
   ```

---

## **Contributing**

Contributions are welcome! Please fork the repository and submit a pull request with your changes.

---

Let me know if you'd like additional edits or instructions! 🚀
