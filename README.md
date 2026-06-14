<h1 align="center">🚗 MULTI-ECU VEHICLE MONITORING USING CAN</h1>

## 📖 **Overview**

The **Multi-ECU Vehicle Monitoring System Using CAN** is an embedded systems project developed using the **LPC2129 ARM7 Microcontroller** and the **Controller Area Network (CAN) Protocol**.

This project demonstrates communication between multiple Electronic Control Units (ECUs) over a CAN Bus to monitor and display vehicle parameters such as:

- 🔋 Battery Status
- 🌡️ Engine Temperature
- 🚦 Indicator Status

The system provides real-time monitoring through an LCD display and showcases distributed embedded system communication.

---

## 🎯 Project Objective

To design and develop a multi-ECU vehicle monitoring system using the CAN protocol that acquires, decodes, and displays real-time data from multiple ECUs, enabling effective monitoring of key vehicle parameters through an embedded display.

---

## ✨ Features

✅ Real-time CAN Communication

✅ Multi-Node Architecture

✅ Engine Temperature Monitoring

✅ Battery Percentage Monitoring

✅ Indicator Control via CAN

✅ LCD-Based Status Display

✅ Interrupt-Based Event Handling

✅ ADC-Based Battery Simulation

---

## 🏗️ System Architecture

### 🖥️ Main Node

- Reads engine temperature from DS18B20 sensor
- Displays temperature on LCD
- Receives battery percentage from Battery Node
- Sends indicator commands to Indicator Node
- Displays battery and indicator status

### 🔋 Battery Node

- Monitors battery condition
- Uses potentiometer to simulate battery voltage
- Converts voltage into battery percentage (0–100%)
- Sends battery status over CAN Bus

### 🚦 Indicator Node

- Receives commands from Main Node
- Controls 8 LEDs
- Displays left and right indicator patterns
- Implements scrolling LED effects

---
---

## 🛠️ Hardware Requirements

- LPC2129 ARM7 Microcontroller
- MCP2551 CAN Transceiver
- DS18B20 Temperature Sensor
- LCD Display
- Potentiometer
- LEDs
- Push Buttons / Switches
- USB to UART Converter
- CAN Bus Connections

---

## 💻 Software Requirements

- Embedded C
- Keil uVision IDE
- Flash Magic
- Proteus (Optional for Simulation)

---

## 🔧 Technologies Used

- ARM7 LPC2129 Architecture
- Embedded C Programming
- CAN Protocol
- GPIO Interfacing
- ADC Interfacing
- External Interrupts
- LCD Interfacing
- DS18B20 Sensor Interfacing
- Real-Time Embedded Systems

---
### 🔄Block Diagram

<img width="900" height="790" alt="image" src="https://github.com/user-attachments/assets/016e1e7f-1210-497e-af4e-c12f8f6aef5b" />

----
## ⚙️ Implementation Sequence

### 1️⃣ LCD Verification

- Interface LCD with LPC2129
- Display characters, strings, and integers

### 2️⃣ Switch Interfacing

- Interface active-low switches
- Display switch status on LCD

### 3️⃣ Temperature Monitoring

- Interface DS18B20 Sensor
- Read and display engine temperature

### 4️⃣ ADC Testing

- Connect potentiometer to ADC channel
- Display ADC value on LCD

### 5️⃣ External Interrupt Testing

- Configure EINT0 and EINT1
- Verify interrupt generation

### 6️⃣ CAN Communication Testing

- Configure CAN Controller
- Verify CAN transmission and reception

### 7️⃣ Node Development

- Develop Main Node
- Develop Battery Node
- Develop Indicator Node

### 8️⃣ System Integration

- Connect all nodes through CAN Bus
- Test complete vehicle monitoring system

---

## 📂 Project Folder Structure

```text
Multi-ECU-Vehicle-Monitoring-Using-CAN/
│
├── Main_Node/
│ ├── main.c
│ ├── can.c
│ ├── lcd.c
│ └── ds18b20.c
│
├── Indicator_Node/
│ ├── indicator.c
│ └── can.c
│
├── Battery_Node/
│ ├── battery.c
│ └── adc.c
│
├── Docs/
│ ├── block_diagram.png
│ └── project_report.pdf
│
└── README.md
```
----

### Output

The LCD displays:

🌡️ Engine Temperature : XX °C

🔋 Battery Level : XX %

🚦 Indicator Status :
LEFT / RIGHT / OFF

## 📸 Project Output

### Dashboard Display

<p align="center">
  <img src="https://github.com/user-attachments/assets/b2afa677-492d-49e9-948f-1fc606788741" width="600">
</p>

### Real-Time Monitoring

<p align="center">
  <img src="https://github.com/user-attachments/assets/ac96fc39-1788-414c-a822-cf90625de8cb" width="600">
</p>

### Complete Setup

<p align="center">
  <img src="https://github.com/user-attachments/assets/a3cbfcad-5d63-4387-9ecf-ea75e6e74a50" width="600">
</p>

----

## 🚀 Applications

- Automotive Embedded Systems
- Vehicle Diagnostics
- ECU Communication Systems
- CAN-Based Industrial Automation
- Vehicle Health Monitoring
- Smart Transportation Systems
- Distributed Embedded Networks
----
## 🎓 Learning Outcomes

- Embedded C Programming
- CAN Protocol Implementation
- LPC2129 ARM7 Architecture
- ADC and GPIO Interfacing
- Interrupt Handling
- Sensor Integration
- Multi-ECU Communication
- Real-Time Embedded System Design

-----

## 👨‍💻 Project Presented By

Kavitha Varikuti
B.Tech – Electronics and Communication Engineering
Vector India Major Project
Embedded Systems | CAN | LPC2129


