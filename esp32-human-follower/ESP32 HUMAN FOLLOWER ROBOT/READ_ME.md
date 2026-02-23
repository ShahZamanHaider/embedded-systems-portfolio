# Face Detection And Folowing Robot CAR

##
ESP32-CAM
ESP WROOM 32
1 PC or laptop

## Overview
This project building a face detection system using the ESP32-CAM module. Since the ESP32-CAM has limited processing power, the face detection might feel a bit slow. To enhance performance, you can attach an external antenna to the ESP32-CAM, which improves connectivity.
During testing, the system worked effectively up to a distance of 350 cm under good lighting conditions. Proper lighting is essential for consistent and accurate results.

Project work flow:

The ESP32-CAM captures images and sends them to a PC or laptop for processing.
The PC processes the images and identifies the detected face.
It then sends data back to the ESP32 WROOM-32 module, which controls the robot's motors to move in the direction of the detected face.
It also shows live video Streaming to your mobile

It has three pages webpage:

1: Manually Control Robot CAR
2: Automatic Control Robot Car using face detection
3: Change Video Streaming Resolution


### ESP32 Preferences URL
Add the following ESP32 board manager URL to the Arduino IDE:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

## Pre-requisites

### Software Installation

#### Python Setup:
- Download and install Python 3.8.10 from the [official website](https://www.python.org/downloads/release/python-3810/).
- Ensure Visual Studio Code or another IDE is installed to run Python scripts.
- Install pip by opening a terminal and running:  

python -m ensurepip --upgrade


#### Python Libraries:
Install the necessary libraries for face detection:


pip install urllib3==2.2.1
pip install numpy==1.24.4
pip install opencv-python==4.6.0.66
pip install mediapipe==0.10.2


## Configuration
Update the ESP32-CAM and ESP32 WRoover IP addresses in the Python files as indicated within the files themselves.

## Usage
1. Open the ESP32-CAM IP address in a web browser.
2. Click the `Automatic Control` button and enable face detection.
3. Go to `Advance Stream` options and minimize the window.
4. Run the Python script to begin face detection.