//============================================================================//
/*!
 *      ___        ___       _   _                _ _            _           
 *     |  _|  /\  |_  |     | | | |              (_) |        /\| |/\        
 *     | |   /  \   | |_   _| |_| |__   ___  _ __ _| |_ _   _ \ ` ' / ______ 
 *     | |  / /\ \  | | | | | __| '_ \ / _ \| '__| | __| | | |_     _|______|
 *     | | / ____ \ | | |_| | |_| | | | (_) | |  | | |_| |_| |/ , . \        
 *     | |/_/    \_\| |\__,_|\__|_| |_|\___/|_|  |_|\__|\__, |\/|_|\/        
 *     |___|      |___|                                  __/ |               
 *                                                      |___/                
 *
 * @author  Shah Zaman Haider
 * @date    April 6, 2024
 * @file    motoresp32.ino
 * @version v1.0.0 -- CHANGELOG
 *              | v1.0.0 -> Added functionality for motor controls and face detection comamnds
 *
 * @details This program is written in C++ language using Arduino framework for
 *          ESP32 platform.
 * 
 * @note    IMPORTANT:
 *              - Select board "DOIT ESP32 DEVKIT V1"
 *              - And PORT
 */




#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "OPPO A96";
const char* password = "shahzaman";

WebServer server(80);  // HTTP server on port 80

// Motor A connections
int enA = 18;
int in1 = 12;
int in2 = 13;
// Motor B connections
int enB = 19;
int in3 = 25;
int in4 = 26;


void setup() {
  Serial.begin(115200);

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
  digitalWrite(in3, 0);
  digitalWrite(in4, 0);


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");


  // Define routes
  server.on("/forward", HTTP_GET, []() {
    server.send(200, "text/plain", "Moving Forward");
    forward();
  });

  server.on("/stop", HTTP_GET, []() {
    server.send(200, "text/plain", "Stopping");
    stop();
  });

  server.on("/left", HTTP_GET, []() {
    server.send(200, "text/plain", "Turning Left");
    left();
  });

  server.on("/right", HTTP_GET, []() {
    server.send(200, "text/plain", "Turning Right");
    right();
  });

  // Start server
  server.begin();
}

void loop() {
  server.handleClient();  // Handle client requests
}


void forward() {

  // Motor A forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 200);  // Speed of motor A

  // Motor B forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 200);  // Speed of motor B

  Serial.println("Forward");
}

void stop() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  Serial.println("Stop");
}

void left() {

  // Motor A backward
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, 200);  // Speed of motor A

  // Motor B forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 200);  // Speed of motor B
  Serial.println("Left");
}

void right() {

  // Motor A forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 200);  // Speed of motor A

  // Motor B backward
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, 200);  // Speed of motor B
  Serial.println("Right");
}