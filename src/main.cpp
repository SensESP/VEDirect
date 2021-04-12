#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>

#include "sensesp_app.h"
#include "sensesp_app_builder.h"
#include "signalk/signalk_output.h"

// SDA and SCL pins on SH-ESP32
#define SDA_PIN 16
#define SCL_PIN 17

// Opto in pin on SH-ESP32
#define OPTO_IN_PIN 35

// OLED display width and height, in pixels
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// define temperature display units
#define TEMP_DISPLAY_FUNC KelvinToCelsius
//#define TEMP_DISPLAY_FUNC KelvinToFahrenheit

TwoWire* i2c;
Adafruit_SSD1306* display;

/// Clear a text row on an Adafruit graphics display
void ClearRow(int row) { display->fillRect(0, 8 * row, SCREEN_WIDTH, 8, 0); }

/**
 * @brief Send Engine Dynamic Parameter data
 *
 * Send engine temperature data using the Engine Dynamic Parameter PGN.
 * All unused fields that are sent with undefined value except the status
 * bit fields are sent as zero. Hopefully we're not resetting anybody's engine
 * warnings...
 */

ReactESP app([]() {
// Some initialization boilerplate when in debug mode...
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  SensESPAppBuilder builder;

  sensesp_app = builder.set_hostname("nmea1_test")
                    ->set_standard_sensors(NONE)
                    ->get_app();

  // initialize the display
  i2c = new TwoWire(0);
  i2c->begin(SDA_PIN, SCL_PIN);
  display = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, i2c, -1);
  if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    debugW("SSD1306 allocation failed");
  }
  delay(100);
  display->setRotation(2);
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->printf("Host: %s", sensesp_app->get_hostname().c_str());
  display->display();

  // initialize Serial1 on the opto_in pin

  Serial1.begin(4800, SERIAL_8N1, OPTO_IN_PIN, -1, true);

  app.onAvailable(Serial1, []() {
    Serial.write(Serial1.read());
  });

  sensesp_app->enable();
});
