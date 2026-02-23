#include "WifiCam.hpp"
#include <StreamString.h>
#include <uri/UriBraces.h>
#include <HTTPClient.h>
#include "advanced_options.cpp"

bool isFaceDetectionEnabled = false;

const char FRONTPAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32-CAM</title>
<style>
  body {
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    margin: 0;
    padding: 0;
    background-color: #f0f0f0;
    color: #333;
    text-align: center;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    height: 100vh;
  }

  .button {
    border: none;
    color: white;
    padding: 15px 32px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
    margin: 4px 2px;
    transition-duration: 0.4s;
    cursor: pointer;
    border-radius: 5px;
    box-shadow: 0 2px 4px rgba(0,0,0,0.2);
  }

  .button1 { background-color: #008CBA; } /* Blue */
  .button2 { background-color: #f44336; } /* Red */
  .button3 { background-color: #555555; } /* Gray */

  .button:hover { box-shadow: 0 4px 8px rgba(0,0,0,0.2); }

  .button1:hover { background-color: #005f73; } /* Darker Blue */
  .button2:hover { background-color: #da190b; } /* Darker Red */
  .button3:hover { background-color: #333333; } /* Darker Gray */

  #advancedOptions { display: none; } /* Initially hide advanced options */

  table, th, td {
    border: solid 1px #000000;
    border-collapse: collapse;
  }
  th, td { padding: 0.4rem; }
  a { text-decoration: none; }
  footer { margin-top: 1rem; }

</style>
</head>
<body>

<h1>ESP32-CAM Control Page</h1>

<a href="/automatic_control" class="button button1">Automatic Control</a>
<a href="/manual_control" class="button button2">Manual Control</a>
<a href="/advanced-options" class="button button3">Advance Stream Options</a>

<script>

</script>

</body>
</html>
)rawliteral";



/////////////////////////////////////////////////////////////////

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>ESP32-CAM Robot</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body {
      font-family: Arial, sans-serif;
      margin: 0;
      padding: 20px;
      background-color: #f0f0f0;
    }
    #content {
      text-align: center;
      max-width: 800px;
      margin: auto;
      display: flex;
      flex-direction: column;
      align-items: center;
    }
    #homeButton {
      position: absolute;
      top: 10px;
      left: 10px;
      background-color: #4CAF50; 
      color: white;
      padding: 10px 20px;
      text-decoration: none;
      font-size: 16px;
      border-radius: 5px;
    }
    .button {
      background-color: #2f4468;
      border: none;
      color: white;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 18px;
      margin: 6px 3px;
      cursor: pointer;
    }
    img {
      width: auto;
      max-width: 100%;
      height: auto;
      margin-top: 20px;
    }
    .controls {
      margin-top: 20px;
    }
  </style>
</head>
<body>
  <a id="homeButton" href="/">Home</a>
  <div id="content">
    <h1>ESP32-CAM Robot MANUAL Mode</h1>
    
    <div class="controls">
      <div><button class="button" onmousedown="toggleCheckbox('forward');" ontouchstart="toggleCheckbox('forward');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Forward</button></div>
      <div>
        <button class="button" onmousedown="toggleCheckbox('left');" ontouchstart="toggleCheckbox('left');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Left</button>
        <button class="button" onmousedown="toggleCheckbox('stop');" ontouchstart="toggleCheckbox('stop');">Stop</button>
        <button class="button" onmousedown="toggleCheckbox('right');" ontouchstart="toggleCheckbox('right');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Right</button>
      </div>
      <div><button class="button" onmousedown="toggleCheckbox('backward');" ontouchstart="toggleCheckbox('backward');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Backward</button></div>
    </div>
  </div>
  <script>
    function toggleCheckbox(command) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/action?go=" + command, true);
      xhr.send();
    }
  </script>
</body>
</html>
)rawliteral";




//////////////////////////////////////////////////////////////////////

const char index_html2[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32-CAM Robot Control</title>
<style>
  body {
    font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
    margin: 0;
    padding: 0;
    background-color: #f0f0f0; /* Light grey background */
    color: #333; /* Dark text for contrast */
    text-align: center;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
    height: 100vh;
  }

  h1 {
    font-size: 1.8em;
    margin-bottom: 1.5rem;
    color: #333; /* Dark text for readability */
  }

  .toggle-switch {
    position: relative;
    width: 120px;
    height: 50px;
    display: inline-block;
  }

  .toggle-switch input {
    display: none;
  }

  .slider {
    position: absolute;
    top: 0;
    bottom: 0;
    left: 0;
    right: 0;
    background-color: #ccc;
    border-radius: 34px;
    cursor: pointer;
    transition: background-color 0.2s ease-in-out;
  }

  .slider:before {
    content: "";
    position: absolute;
    height: 46px;
    width: 46px;
    left: 2px;
    bottom: 2px;
    background-color: white;
    border-radius: 50%;
    transition: transform 0.2s ease-in-out;
  }

  input:checked + .slider {
    background-color: darkred; /* Dark red background for 'ON' state */
  }

  input:checked + .slider:before {
    transform: translateX(70px);
  }

  .slider:after {
    content: 'OFF';
    color: white;
    display: block;
    position: absolute;
    transform: translate(-50%, -50%);
    top: 50%;
    left: 75%;
    font-size: 16px;
    font-weight: bold;
  }

  input:checked + .slider:after {
    content: 'ON';
    left: 25%;
  }
</style>
</head>
<body>

<h1>ESP32-CAM Robot AUTOMATIC Mode</h1>

<label class="toggle-switch">
  <input type="checkbox" id="toggleButton" onchange="toggleFaceDetection()">
  <span class="slider"></span>
</label>

<script>
  function toggleFaceDetection() {
    var checkBox = document.getElementById("toggleButton");
    var url = checkBox.checked ? '/enable-face-detection' : '/disable-face-detection';
    fetch(url)
      .then(response => console.log("Toggle face detection"))
      .catch(error => console.error("Error toggling face detection:", error));
  }
</script>

</body>
</html>
)rawliteral";





//////////////////////////////END OF HTML////////////////////////////////////




static void
serveStill(bool wantBmp) {
  auto frame = esp32cam::capture();
  if (frame == nullptr) {
    Serial.println("capture() failure");
    server.send(500, "text/plain", "still capture error\n");
    return;
  }
  Serial.printf("capture() success: %dx%d %zub\n", frame->getWidth(), frame->getHeight(),
                frame->size());

  if (wantBmp) {
    if (!frame->toBmp()) {
      Serial.println("toBmp() failure");
      server.send(500, "text/plain", "convert to BMP error\n");
      return;
    }
    Serial.printf("toBmp() success: %dx%d %zub\n", frame->getWidth(), frame->getHeight(),
                  frame->size());
  }

  server.setContentLength(frame->size());
  server.send(200, wantBmp ? "image/bmp" : "image/jpeg");
  WiFiClient client = server.client();
  frame->writeTo(client);
}



static void
serveMjpeg() {
  Serial.println("MJPEG streaming begin");
  WiFiClient client = server.client();
  auto startTime = millis();
  int nFrames = esp32cam::Camera.streamMjpeg(client);
  auto duration = millis() - startTime;
  Serial.printf("MJPEG streaming end: %dfrm %0.2ffps\n", nFrames, 1000.0 * nFrames / duration);
}

void addRequestHandlers() {

  // Home page handler
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", FRONTPAGE);
  });

  // <---------------------------------------------------------->

  // Manual Control page handler
  server.on("/manual_control", HTTP_GET, []() {
    server.send(200, "text/html", index_html);  // Make sure `index_html` contains your HTML content as a string
  });

  // <---------------------------------------------------------->


  // Action handler
  server.on("/action", HTTP_GET, []() {
    String action = server.arg("go");  // Retrieve value of the "go" query parameter
    HTTPClient http;

    String url = "http://192.168.211.18/action?" + action;

    http.begin(url);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();


    if (action == "forward") {
       server.send(200, "text/plain", "forward");
      Serial.println("Moving forward");
    } else if (action == "left") {
       server.send(200, "text/plain", "left");
      Serial.println("Turning left");
    } else if (action == "right") {
       server.send(200, "text/plain", "right");
      Serial.println("Turning right");
    } else if (action == "stop") {
       server.send(200, "text/plain", "stop");
      Serial.println("Stopping");
    } else {
      Serial.println("Unknown action: ");
    }

  });

  // <---------------------------------------------------------->

  // Automatic Control page handler
  server.on("/automatic_control", HTTP_GET, []() {
    server.send(200, "text/html", index_html2);  // Make sure `index_html2` contains your HTML content as a string
  });

  // <---------------------------------------------------------->

  server.on("/advanced-options", HTTP_GET, []() {
    if (isFaceDetectionEnabled) {
      server.send(200, "text/html", advanced_options);
    } else {
      // Inform the user that face detection needs to be enabled first
      server.send(200, "text/plain", "Please enable face detection to access Advanced Options.");
    }
  });

  // <---------------------------------------------------------->

  // Enable Face Detection handler
  server.on("/enable-face-detection", HTTP_GET, []() {
    isFaceDetectionEnabled = true;
    Serial.println("Face detection enabled");
    server.send(200, "text/plain", "Face detection enabled");
  });

  // <---------------------------------------------------------->

  // Disable Face Detection handler
  server.on("/disable-face-detection", HTTP_GET, []() {
    Serial.println("Face detection disabled");
    server.send(200, "text/plain", "Face detection disabled");
  });

  // <---------------------------------------------------------->

  server.on("/robots.txt", HTTP_GET, [] {
    server.send(200, "text/html", "User-Agent: *\nDisallow: /\n");
  });


  // <---------------------------------------------------------->

  server.on("/resolutions.csv", HTTP_GET, [] {
    StreamString b;
    for (const auto& r : esp32cam::Camera.listResolutions()) {
      b.println(r);
    }
    server.send(200, "text/csv", b);
  });

  // <---------------------------------------------------------->

  server.on(UriBraces("/{}x{}.{}"), HTTP_GET, [] {
    long width = server.pathArg(0).toInt();
    long height = server.pathArg(1).toInt();
    String format = server.pathArg(2);
    if (width == 0 || height == 0 || !(format == "bmp" || format == "jpg" || format == "mjpeg")) {
      server.send(404);
      return;
    }

    auto r = esp32cam::Camera.listResolutions().find(width, height);
    if (!r.isValid()) {
      server.send(404, "text/plain", "non-existent resolution\n");
      return;
    }
    if (r.getWidth() != width || r.getHeight() != height) {
      server.sendHeader("Location",
                        String("/") + r.getWidth() + "x" + r.getHeight() + "." + format);
      server.send(302);
      return;
    }

    if (!esp32cam::Camera.changeResolution(r)) {
      Serial.printf("changeResolution(%ld,%ld) failure\n", width, height);
      server.send(500, "text/plain", "changeResolution error\n");
    }
    Serial.printf("changeResolution(%ld,%ld) success\n", width, height);

    if (format == "bmp") {
      serveStill(true);
    } else if (format == "jpg") {
      serveStill(false);
    } else if (format == "mjpeg") {
      serveMjpeg();
    }
  });

  ////////////////////////////END OF HANDLERS////////////////////////////////
}
