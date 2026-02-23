#include "esp_camera.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "driver/rtc_io.h"
#include "FS.h"
#include "SD_MMC.h"
#include "EEPROM.h"
#include <Preferences.h>
#include <iostream>
#define EEPROM_SIZE 1
#include "driver/gpio.h"
#include "SPI.h"

#define BUILTIN_LED 4

unsigned int pictureCount = 0;
int ConfigContar;
const int led_BRIGHTNESS = 5;  // adjust according to your choice from range of 0-255, (0 is off state).


#include "esp_sleep.h"

#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22


void configESPCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA;
  config.jpeg_quality = 10;
  config.fb_count = 1;  // Fixed the typo from "onfig" to "config"

  if (esp_camera_init(&config) != ESP_OK) {
    Serial.println("Camera initialization failed");
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  if (s != NULL) {
    s->set_brightness(s, 2);
    s->set_contrast(s, -30);
    s->set_saturation(s, -2);
    s->set_special_effect(s, 0);
    s->set_whitebal(s, 0);
    s->set_awb_gain(s, 0);
    s->set_wb_mode(s, 0);
    s->set_exposure_ctrl(s, 1);
    s->set_aec2(s, 0);
    s->set_ae_level(s, 0);
    s->set_aec_value(s, 300);
    s->set_gain_ctrl(s, 1);
    s->set_agc_gain(s, 0);
    s->set_gainceiling(s, (gainceiling_t)0);
    s->set_bpc(s, 0);
    s->set_wpc(s, 1);
    s->set_raw_gma(s, 1);
    s->set_lenc(s, 1);
    s->set_hmirror(s, 0);
    s->set_vflip(s, 0);
    s->set_dcw(s, 1);
    s->set_colorbar(s, 0);
  }
}


void initMicroSDCard() {
  Serial.println("Mounting MicroSD Card");

  //SD Card is assigned to true so that it cannot make conflict with LED which is on GPIO 4
  //(By deffault SD card is atatched to GPIO4 for data transfer , it is converted into true so that GPIO 4 should be free, so that we can perfom LED operations on it)

  if (!SD_MMC.begin("/sdcard", true)) {
    Serial.println("MicroSD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No MicroSD Card found");
    return;
  }
}

void takeNewPhoto(String path, const int Brightness) {

  //Turn On LED
  analogWrite(BUILTIN_LED, Brightness);
  delay(500);

  camera_fb_t *fb = esp_camera_fb_get();
  Serial.println("Image Captured Successfuly");

  //Turn off LED after capturing picture
  analogWrite(BUILTIN_LED, 0);
  delay(500);
  Serial.println("LED brightness: " + String(Brightness));

  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  fs::FS &fs = SD_MMC;
  File file = fs.open(path.c_str(), FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file in write mode");
  } else {
    file.write(fb->buf, fb->len);
    Serial.printf("Saved file to path: %s\n", path.c_str());
  }
  file.close();
  esp_camera_fb_return(fb);
}


void setup() {
  Serial.begin(9600);
  delay(10);

  esp_sleep_enable_timer_wakeup(15 * 1000000);
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  //Turn BUILTIN LED to OFF state initially
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);

  EEPROM.begin(EEPROM_SIZE);
  pictureCount = EEPROM.read(0);

  Serial.println("Initializing the camera module...");
  configESPCamera();
  Serial.println("Camera OK!");

  Serial.println("Initializing the MicroSD card module...");
  initMicroSDCard();

  String path = "/photo_RGB" + String(pictureCount) + ".jpg";

  takeNewPhoto(path, led_BRIGHTNESS);  //Takes two inputs one is path where captured photos will be stored and second is Brightness value of your choice

  pictureCount++;
  EEPROM.write(0, pictureCount);
  EEPROM.commit();

  esp_deep_sleep_start();
}

void loop() {
  // Nothing to do in loop
}
