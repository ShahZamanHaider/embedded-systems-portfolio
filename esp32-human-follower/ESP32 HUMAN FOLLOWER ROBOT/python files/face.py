# //============================================================================//
# /*!
#  *      ___        ___       _   _                _ _            _           
#  *     |  _|  /\  |_  |     | | | |              (_) |        /\| |/\        
#  *     | |   /  \   | |_   _| |_| |__   ___  _ __ _| |_ _   _ \ ` ' / ______ 
#  *     | |  / /\ \  | | | | | __| '_ \ / _ \| '__| | __| | | |_     _|______|
#  *     | | / ____ \ | | |_| | |_| | | | (_) | |  | | |_| |_| |/ , . \        
#  *     | |/_/    \_\| |\__,_|\__|_| |_|\___/|_|  |_|\__|\__, |\/|_|\/        
#  *     |___|      |___|                                  __/ |               
#  *                                                      |___/                
#  *
#  * @author  Shah Zaman Haider
#  * @date    April 6, 2024
#  * @file    esp32tst.ino
#  * @version v1.0.0 -- CHANGELOG
#  *              | v1.0.0 -> main face detection model which detects face and proccesses commands.
#  *
#  * @details This program is written in Python language using OpenCV framework.
#  *     
#  * 
#  * @note    IMPORTANT:
#  *              - Replace IP addresses
#  */




import cv2
import mediapipe as mp
import os
import sys
import threading
from esp32tst import cam
import requests

# Global flag to indicate face detection status
face_detected = False
a, b = 0, 0

 #ESP32 WROOVER URL
esp32_ip = '192.168.211.18'

def send_command(command):
    # Sends a command to the ESP32 via HTTP GET request
    url = f'http://{esp32_ip}/{command}'
    try:
        response = requests.get(url)
        print(f"Command {command} sent, response: {response.text}")
    except requests.exceptions.RequestException as e:
        print(f"Error sending command {command}: {e}")



def stop():
   send_command("stop")

def left():
    send_command("left")

def Right():
    send_command("right")

def forward():
    send_command("forward")



def restart_script():
    # Restart the script
    os.execv(sys.executable, ['python'] + [sys.argv[0]])


def check_face_detection():
    def restart_if_no_detection():
        if not face_detected:
            print("Face detection not started, restarting script...")
            restart_script()
            
    timer = threading.Timer(2.0, restart_if_no_detection)
    timer.start()
    timer = threading.Timer(2.0, restart_if_no_detection)
    timer.start()


def obj_data(img, mp_face, width, height):
    global face_detected,a ,b

    image_input = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
    results = mp_face.process(image_input)
    print("Face detection started")
    if results.detections:
        face_detected = True  # Set the flag when a face is detected
        print("Detecting face")
        
        for detection in results.detections:
            bbox = detection.location_data.relative_bounding_box
            x, y, w, h = int(bbox.xmin * width), int(bbox.ymin * height), int(bbox.width * width), int(bbox.height * height)
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 0, 255), 2)
            cx=int(x+x+w)//2
            cy=int(y+y+h)//2
           
            cv2.circle(img,(cx,cy),5,(0,0,255),-1)
           
            a = int(cx)//62
            cv2.putText(img,str(a),(580,60),5,cv2.FONT_HERSHEY_PLAIN,(0,255,0),2)
            
            b =int(cy -h)//10
#             print(b)
            cv2.putText(img,str(b),(30,60),5,cv2.FONT_HERSHEY_PLAIN,(0,255,0),2)

            if b > 12:
                 forward()
                 
            else:
                 stop()
            if a > 6:
                
                 Right()
                 stop()
               
            if a < 4:
                 
                 left()
                 stop()



        else:print("Face detection not started, Something went wrong")
        a, b = 0, 0

    return img


def main():
    global face_detected

    mp_face = mp.solutions.face_detection.FaceDetection(model_selection=1, min_detection_confidence=0.5)

    width, height = 640, 480

    # Start the face detection check timer
    threading.Thread(target=check_face_detection).start()

    while True:
        frame = cam()
        if frame is None:
            print("Failed to capture frame")
            continue
        
        frame = cv2.flip(frame, 1)
        frame = cv2.resize(frame, (width, height))
        frame = obj_data(frame, mp_face, width, height)  # Process frame for face detection

        cv2.imshow("FRAME", frame) #show the face detection window
        if cv2.waitKey(5) & 0xFF == 27:
            break


        
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main() #if detection stoped it restarts the detection
