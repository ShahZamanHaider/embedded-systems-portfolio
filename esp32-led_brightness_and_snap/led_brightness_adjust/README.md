# ESP32 Camera Photo Capture with MicroSD

A compact ESP32 project to **capture photos with a camera module** and store them on a **MicroSD card**, with optional LED illumination control. Supports deep sleep for low-power operation.

---

## ⚙️ Features

- Captures **JPEG images** at SVGA resolution
- Uses **XIAO ESP32-S3 / AI Thinker camera**
- Saves images directly to **MicroSD card**
- Configurable **LED brightness** for illumination
- Stores **picture count in EEPROM** for sequential filenames
- Supports **deep sleep** for power saving

---

## 🔌 Pin & Hardware Setup

- **Built-in LED:** GPIO 4 (for illumination during capture)
- **Camera Pins (AI Thinker example):**  
  - XCLK: 0, SIOD: 26, SIOC: 27  
  - D0–D7: 5,18,19,21,36,39,34,35  
  - VSYNC: 25, HREF: 23, PCLK: 22, PWDN: 32, RESET: -1
- **MicroSD:** Uses SD_MMC interface on ESP32 (no conflict with LED)

> Adjust pins if using XIAO ESP32-S3 or other boards.

---

## ⚙️ How It Works

1. **Initialize camera** with SVGA JPEG settings.
2. **Set sensor parameters** (brightness, contrast, white balance, etc.).
3. **Mount MicroSD card** and check availability.
4. **Turn on LED**, capture photo, **save to SD card**, and turn off LED.
5. Increment **EEPROM counter** for sequential file naming.
6. Enter **deep sleep** for power efficiency.

---

## ▶️ Usage Instructions

1. Connect your ESP32 camera and MicroSD card.
2. Adjust `#define` pins for your specific board if needed.
3. Set `led_BRIGHTNESS` value (0–255) as required.
4. Upload the sketch using **Arduino IDE** or **PlatformIO**.
5. Open **Serial Monitor** at 9600 baud to view status messages.
6. Each cycle captures a photo and stores it on the SD card automatically before deep sleep.

---

## 💡 Applications

- Time-lapse photography  
- IoT surveillance and monitoring  
- Low-power photo capture devices  
- Robotics visual logging  

---

## 👨‍💻 Author

Shah Zaman Haider  
Robotics & IoT Systems Engineer  
ESP32 Embedded Vision Specialist  

---

## 📜 License

Open for educational and personal project use. Modify and use freely with attribution.
