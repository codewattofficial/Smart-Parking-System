# 🚗 Smart Parking System using Arduino UNO

An automated Smart Parking System built using **Arduino UNO**, **HC-SR04 Ultrasonic Sensors**, an **SG90 Servo Motor**, and a **16x2 LCD Display**. This system automatically manages vehicle entry/exit, controls the barrier gate, tracks available slots in real-time, and provides interactive feedback on the display.

## 📌 Features

* **Automated Entry Gate**:

  * Detects incoming vehicles instantly using an ultrasonic sensor.

  * Displays `"Welcome!"` and lifts the servo motor barrier gate.

  * Holds the gate open for a configured duration, then smoothly closes the barrier.

  * Automatically updates the internal parking counter (`Parked +1`, `Free -1`).

  * Rejects entry with a `"Parking Full"` alert when all slots are occupied.

* **Smart Exit Tracking**:

  * Detects exiting vehicles via a secondary exit ultrasonic sensor.

  * Displays `"Thanks! Visit Again"` while the vehicle passes.

  * Updates counters dynamically when the car departs (`Parked -1`, `Total Exited +1`).

* **Debounce & Noise Filtering**:

  * Uses confirmation counts (`CONFIRM_COUNT`) to prevent false triggers caused by sensor noise or glitches.

* **Real-Time LCD Monitoring**:

  * Live display showing total **Parked Cars**, **Free Slots**, and **Total Exited Vehicles**.

## 🛠 Hardware Required

| Component | Quantity | Details | 
 | ----- | ----- | ----- | 
| **Arduino UNO R3** | 1 | Microcontroller board | 
| **HC-SR04 Ultrasonic Sensor** | 2 | Entry & Exit detection | 
| **SG90 Micro Servo Motor** | 1 | Barrier gate mechanism | 
| **16x2 LCD Display** | 1 | Standard Parallel LCD (Non-I2C) | 
| **10k Potentiometer** | 1 | LCD Contrast Adjustment | 
| **Breadboard & Jumpers** | \-- | Circuit wiring | 

## 🔌 Pin Wiring Connections

### 1. Servo Motor (Entry Barrier)

| Servo Pin | Arduino Pin | 
 | ----- | ----- | 
| **VCC** | 5V | 
| **GND** | GND | 
| **Signal** | Digital Pin 3 | 

### 2. Ultrasonic Sensors

| Sensor | VCC | GND | Trig Pin | Echo Pin | 
 | ----- | ----- | ----- | ----- | ----- | 
| **Entry Sensor** | 5V | GND | Pin 8 | Pin 9 | 
| **Exit Sensor** | 5V | GND | Pin 10 | Pin 11 | 

### 3. 16x2 LCD Display (Parallel Mode)

| LCD Pin | Connected To | 
 | ----- | ----- | 
| **VSS (Pin 1)** | GND | 
| **VDD (Pin 2)** | 5V | 
| **V0 (Pin 3)** | Potentiometer Wiper (Middle Pin) | 
| **RS (Pin 4)** | Digital Pin 2 | 
| **RW (Pin 5)** | GND | 
| **E (Pin 6)** | Digital Pin 4 | 
| **D4 (Pin 11)** | Digital Pin 5 | 
| **D5 (Pin 12)** | Digital Pin 6 | 
| **D6 (Pin 13)** | Digital Pin 7 | 
| **D7 (Pin 14)** | Digital Pin 12 | 
| **A (Pin 15)** | 5V (Backlight +) | 
| **K (Pin 16)** | GND (Backlight -) | 

## ⚙️ Configuration Parameters

Inside the code, you can customize the following variables to fit your physical prototype setup:

```
const int TOTAL_SLOTS = 4;           // Total available parking capacity
const int HOLD_TIME   = 2500;        // Gate open delay in milliseconds (2.5 sec)
const int DETECTION_DISTANCE = 10;   // Distance threshold in cm to detect a car
```

## 🚀 How to Run the Project

1. Clone or download this repository:

   ```
   git clone https://github.com/your-username/smart-parking-system.git
   ```

2. Open the code in the **Arduino IDE**.

3. Select board: **Tools > Board > Arduino AVR Boards > Arduino Uno**.

4. Select the correct COM Port under **Tools > Port**.

5. Make sure the built-in libraries (`LiquidCrystal` and `Servo`) are available.

6. Click **Upload** to compile and transfer the code to your Arduino.

## 📜 License

This project is open-source under the **MIT License**. Feel free to modify and expand it for your academic, DIY, or personal tech projects!