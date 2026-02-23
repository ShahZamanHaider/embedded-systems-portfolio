# ESP32 Camera Blob Detection (XIAO ESP32-S3 / AI Thinker)

A lightweight **ESP32 camera project** for real-time **blob detection** using grayscale frames.  
Supports **XIAO ESP32-S3** and **AI Thinker** camera modules. The system detects bright regions (blobs) in the camera feed and prints their coordinates along with the frame processing time to the serial monitor.

---

## 📖 Overview

This project demonstrates:

- Capturing grayscale frames from an ESP32 camera
- Detecting high-intensity blobs in the image
- Reporting the number of blobs and their coordinates
- Maintaining high FPS while managing memory on ESP32

It is designed for applications such as:

- Motion detection
- Object tracking
- Machine vision experiments
- Robotics and IoT visual sensing

---

## ⚙️ Supported Hardware

- **ESP32 Module:** XIAO ESP32-S3 or AI Thinker ESP32-CAM  
- **Camera Module:** OV2640 (on the above ESP32 boards)  
- **Optional:** PSRAM-equipped ESP32 for higher frame buffer count

---

## 🔌 Pin Configuration

### XIAO ESP32-S3

| Signal | GPIO |
|--------|------|
| XCLK   | 10   |
| SIOD   | 40   |
| SIOC   | 39   |
| Y9     | 48   |
| Y8     | 47   |
| Y7     | 38   |
| Y6     | 21   |
| Y5     | 14   |
| Y4     | 13   |
| Y3     | 34   |
| Y2     | 17   |
| VSYNC  | 11   |
| HREF   | 9    |
| PCLK   | 8    |
| PWDN   | -1   |
| RESET  | -1   |

### AI Thinker ESP32 (Optional)

| Signal | GPIO |
|--------|------|
| XCLK   | 0    |
| SIOD   | 26   |
| SIOC   | 27   |
| Y9     | 35   |
| Y8     | 34   |
| Y7     | 39   |
| Y6     | 36   |
| Y5     | 21   |
| Y4     | 19   |
| Y3     | 18   |
| Y2     | 5    |
| VSYNC  | 25   |
| HREF   | 23   |
| PCLK   | 22   |
| PWDN   | 32   |
| RESET  | -1   |

> ⚠️ Only one camera model should be defined at a time using the `#define CAMERA_MODEL_XIAO_ESP32S3` or `#define CAMERA_MODEL_AI_THINKER` directive.

---

## 🧠 How It Works

1. The camera captures **grayscale frames**.
2. The code scans the frame with a pixel step (default: 4x4) to detect **bright blobs**.
3. Coordinates of detected blobs are stored in a `Blob` structure array.
4. The number of blobs and their coordinates are printed to the serial monitor.
5. The system calculates **frame time** (ms) to monitor FPS performance.
6. Maximum blobs (`maxBlobs`) can be configured to balance memory usage and detection accuracy.

---

▶️ Usage Instructions

1. Connect the camera to the ESP32 as per the selected model.
2. Update #define CAMERA_MODEL_XIAO_ESP32S3 or #define CAMERA_MODEL_AI_THINKER in the code.
3. Upload the sketch to your ESP32 using Arduino IDE or PlatformIO.
4. Open Serial Monitor at 115200 baud.
5. Observe detected blobs and frame timing output in real time.

---

