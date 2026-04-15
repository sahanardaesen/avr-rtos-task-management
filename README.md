# Arduino FreeRTOS Emergency Controller

This project demonstrates a multi-tasking embedded system using **FreeRTOS** on an Arduino Uno. It features real-time sensor monitoring, inter-task communication, and a hardware-interrupt-based emergency shutdown system.

## ⚙️ Features
* **Real-Time Tasking:** 3 concurrent tasks (Sensor, Reporter, Watchdog).
* **Inter-Task Communication:** Data transfer via `Queues`.
* **Thread Safety:** Serial port access protected by `Mutex`.
* **Safety Mechanism:** Hardware Interrupt (ISR) for immediate emergency shutdown.
* **Watchdog Timer (WDT):** System health monitoring and auto-reset capability.

## 🛠️ Hardware Setup (Simulide)
* **Pin 2:** Emergency Stop Button (Input Pull-up)
* **Pin 3:** Emergency Indicator LED
* **Pin 13:** System Status LED
* **A0:** Potentiometer (Analog Sensor Input)

## 🚀 How to Run
1. Load the `src/main.cpp` into PlatformIO or Arduino IDE.
2. Install the `FreeRTOS` library.
3. Open `sim/circuit.simu` in Simulide.
4. Compile and upload the `.hex` file to the Arduino.

<img width="576" height="668" alt="image" src="https://github.com/user-attachments/assets/375c5130-ae6a-484b-bbd3-2f5543115acd" />
