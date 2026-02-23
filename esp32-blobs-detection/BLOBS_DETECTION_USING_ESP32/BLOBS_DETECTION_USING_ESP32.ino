#include "esp_camera.h"


//#define CAMERA_MODEL_AI_THINKER
 #define CAMERA_MODEL_XIAO_ESP32S3

// CAMERA PINS CONFIGURATION 
#if defined(CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#elif defined(CAMERA_MODEL_XIAO_ESP32S3)
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM    10
#define SIOD_GPIO_NUM    40
#define SIOC_GPIO_NUM    39
#define Y9_GPIO_NUM      48
#define Y8_GPIO_NUM      47
#define Y7_GPIO_NUM      38
#define Y6_GPIO_NUM      21
#define Y5_GPIO_NUM      14
#define Y4_GPIO_NUM      13
#define Y3_GPIO_NUM      34
#define Y2_GPIO_NUM      17
#define VSYNC_GPIO_NUM   11
#define HREF_GPIO_NUM     9
#define PCLK_GPIO_NUM     8
#endif

struct Blob {
  int x;
  int y;
};

//---------------------------IMP NOTE--------------------------

// Set maxBlobs by keeping memory of ESP32 in mind becuase the more the number of blobs detected at a time.
// The more memory will be consumed and it can result to crash of code.

const int maxBlobs = 20; 
unsigned long lastFrameTime = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initializing camera...");

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;

  if(psramFound()){
    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 12;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QQVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  Serial.println("Camera initialized.");
}

void loop() {
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb || fb->format != PIXFORMAT_GRAYSCALE) {
    Serial.println("Camera capture failed");
    return;
  }

  const uint8_t* image = fb->buf;
  int width = fb->width;
  int height = fb->height;

  Blob blobs[maxBlobs];
  int blobCount = 0;

  for (int y = 0; y < height; y += 4) {
    for (int x = 0; x < width; x += 4) {
      int index = y * width + x;
      if (image[index] > 200) { 
        if (blobCount < maxBlobs) {
          blobs[blobCount++] = {x, y};
        }
        x += 3; // few pixels skiped to reduce noise
      }
    }
  }

  unsigned long now = millis();
  unsigned long frameTime = now - lastFrameTime;
  lastFrameTime = now;

  Serial.printf("\nDetected %d blobs | Frame time: %lu ms\n", blobCount, frameTime);
  for (int i = 0; i < blobCount; i++) {
    Serial.printf("Blob %d: X=%d Y=%d\n", i + 1, blobs[i].x, blobs[i].y);
  }

  esp_camera_fb_return(fb);
  delay(10); // around 33 FPS max used
}
