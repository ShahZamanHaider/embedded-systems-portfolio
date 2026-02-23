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
#  *              | v1.0.0 -> ESP32 communication with face detection model.
#  *
#  * @details This program is written in Python language using OpenCV framework.
#  *     
#  * 
#  * @note    IMPORTANT:
#  *              - Replace IP addresses
#  */




import urllib.request
import numpy as np
import cv2

def cam():
    #ESP32 CAM URL
    url = 'http://192.168.211.233/1024x768.jpg'

    try:
        # Use urllib to get the image from the IP camera
        imgResp = urllib.request.urlopen(url, timeout=5)  # Added timeout for reliability
        
        # Numpy to convert into an array
        imgNp = np.array(bytearray(imgResp.read()), dtype=np.uint8)
        
        # Decode the array to OpenCV usable format
        image1 = cv2.imdecode(imgNp, -1)

        # Check if the image was successfully decoded
        if image1 is None:
            print("Failed to decode image")
            return None

        return image1

    except Exception as e:
        print(f"Error capturing image from IP camera: {e}")
        return None
