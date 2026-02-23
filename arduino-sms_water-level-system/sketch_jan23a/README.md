# GSM Water Tank Monitoring & Alert System (SIM900A + Arduino)

A simple Arduino-based water tank monitoring system that uses a **SIM900A GSM module** to automatically send SMS alerts and make phone calls when the tank becomes **FULL** or **EMPTY**.

The system is designed for remote monitoring where internet connectivity is not available. It relies entirely on GSM communication to notify users through messages and calls.

---

## 📖 Overview

This project monitors two water level sensors connected to an Arduino:

- **Tank Empty Sensor**
- **Tank Full Sensor**

When a sensor is triggered, the system:

1. Sends SMS notifications to multiple phone numbers.
2. Automatically makes phone calls for immediate alerts.
3. Allows manual call termination using a reset button.

The goal is to provide a low-cost and reliable notification system for homes, farms, or industrial water tanks.

---

## ⚙️ Hardware Requirements

- Arduino (Uno / Nano / compatible board)
- SIM900A GSM Module
- Active SIM Card
- Water Level Sensors (2)
- Push Button (Reset / Hang-up)
- Jumper wires
- Power supply suitable for SIM900A

---

## 🔌 Pin Configuration

| Component | Arduino Pin |
|-----------|-------------|
| SIM900A TX/RX | 2, 3 (SoftwareSerial) |
| Tank Full Sensor | 7 |
| Tank Empty Sensor | 8 |
| Reset Button | 9 |

---

## 🚀 Features

- Automatic SMS alerts when tank is FULL or EMPTY
- Automatic phone call notifications
- Multiple contact numbers support
- GSM-based communication (no WiFi required)
- Manual call hang-up using reset button
- Serial monitor status feedback

---

## 📂 How It Works

### 1. Sensor Monitoring
The Arduino continuously reads two digital sensors:

- If **Empty sensor = HIGH** → Tank Empty alert
- If **Full sensor = HIGH** → Tank Full alert

---

### 2. SMS Notification
When triggered, the system sends SMS messages to three predefined phone numbers using AT commands through the SIM900A module.

Example messages:
