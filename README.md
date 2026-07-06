# Arduino Alarm System

An embedded systems project featuring a custom-built alarm system controlled via an IR remote and an attached matrix keypad for user configuration, security access, and state management.

## 📝 Project Description
This project implements an intelligent home/space security alarm system using an Arduino board. The system monitors access points and triggers visual and acoustic alerts if an unauthorized entry or action is detected. Users can arm, disarm, and configure the alarm dynamically using multiple input interfaces.

### Core Features:
- **Dual Input Interface:** Supports access and configuration via a 4x4 Matrix Keypad and an Infrared (IR) Remote Control.
- **LCD Display with Smart Backlight:** Displays real-time status, menus, and warnings, featuring an automatic power-saving timeout (AFK mode).
- **Acoustic & Visual Alerts:** Uses a Piezo Buzzer to play distinct melodies for arming, disarming, incorrect password warnings, and active alarms, synchronized with status LEDs.
- **Servo Motor Integration:** Actuates a physical locking mechanism (servo door/latch) upon correct authentication.
- **Dynamic Configuration:** Supports real-time password verification, retry limits, and status checks.

---

## 🕹️ Control & Interface (How it works)

The system is interactively controlled via the 4x4 Matrix Keypad for administration tasks and user authentication[cite: 7]. Below is the accurate mapping of the system's functional keys based on the current firmware logic[cite: 7]:

### ⌨️ Keypad Mapping

| Key | Action Triggered | Function Description |
| :---: | :--- | :--- |
| **`#`** | **Password Reset** | Triggers the `passwordReset()` routine. It prompts the user for the old password before allowing a new 4-digit master code to be configured[cite: 7]. |
| **`A`** | **Ring Time Configuration** | Triggers the `ringTimeReset()` routine, allowing the user to change the duration (in seconds) that the alarm will sound when a sensor is tripped[cite: 7]. |
| **`B`** | **Ring Mode Configuration** | Triggers the `ringModeReset()` routine to toggle between different acoustic alarm patterns/melodies (Mode 1 or Mode 2)[cite: 7]. |
| **`C`** | **Wall Ring Toggling** | Triggers the `wallRingSet()` routine, enabling (`1`) or disabling (`0`) the continuous ultrasonic distance checking for wall-mount security[cite: 7]. |
| **`0-9`** | **Numeric Code Entry** | Used for standard 4-digit authentication to switch system states (Arms the system if disarmed, or Disarms it if armed/triggered)[cite: 7]. |

---

### 📺 IR Remote Mapping

The system utilizes specific directional arrows on the IR Remote for quick wireless arming and disarming actions:

- **`2`** - **Arm the System:** Sets the system state to ARMED, locking the mechanism (`armed = 1`) and executing the security locking protocol.
- **`3`** - **Disarm the System:** Sets the system state to DISARMED, unlocking the mechanism (`armed = 0`) and executing the system access protocol.

---

### 🔄 State Switching & Authentication Logic
- **Arming/Disarming:** When you type a sequence of 4 digits, the system automatically checks them against the saved password[cite: 7]. If correct, it toggles between **Armed** (locks the mechanism via the servo, turns on the red LED) and **Disarmed** (opens the lock via the servo, turns on the green LED)[cite: 7].
- **Configuration Security:** Pressing any utility key (`#`, `A`, `B`, `C`) will not instantly open a menu[cite: 7]. The system will prompt `Type password` first, ensuring that only authorized users can modify the alarm parameters[cite: 7].

---

## 🔌 Dependencies & Libraries
To compile and upload this project, you need to install the following libraries within your Arduino IDE (`Sketch` -> `Include Library` -> `Manage Libraries...`):

* **Keypad v3.1.1** by Mark Stanley, Alexander Brevig
* **LiquidCrystal v1.0.7** by Arduino, Adafruit
* **Servo v1.2.2** by Michael Margolis, Arduino
* **IRremote v4.4.1** by shirriff, z3t0, ArminJo

---

## 🚀 How to Set Up and Run

### 1. Hardware Connection
Connect your components to the Arduino pins as defined in the source code:
- **Keypad:** Map rows and columns to the respective digital pins.
- **IR Receiver:** Connect to the designated IR input pin.
- **LCD Display (Parallel):** Connect directly via digital pins using standard parallel wiring (RS, Enable, D4, D5, D6, D7) along with a potentiometer for contrast adjustment.
- **Buzzer & Servo:** Connect to PWM/Digital output pins.

### 2. Software Initialization
1. Clone or download this repository.
2. Open the `AlarmSystem.ino` file in the **Arduino IDE**.
3. Go to the Library Manager and install the required [Dependencies](#-dependencies--libraries).
4. Select your correct Arduino Board (e.g., Arduino Mega or Mega 2560) and Serial Port under the `Tools` menu.
5. Click **Verify** to compile the code, then click **Upload** to flash it onto your board.

## 🎬 Demo on how it works

A video demonstration of how the alarm system works is available on [YouTube](https://www.youtube.com/watch?v=ySTaATLWpZw&t=10s).