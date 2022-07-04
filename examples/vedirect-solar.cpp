// An example of interfacing a solar charge controller with Signal K.

#include <Arduino.h>

#include "ReactESP.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp_app.h"
#include "sensesp_app_builder.h"
#include "sevedirect/sensors/vedirect.h"

// NOTE: IMPORTANT! Victron MPPT chargers use a 5V VE.Direct interface, so
// a logic level converter or a voltage divider MUST be used to interface the
// devices. You _will_ damage the ESP32 device is you fail to do so!

constexpr int kTXPin = 13;
constexpr int kRXPin = 12;

// Provide an ID for the charge controller, to be used in the Signal K
// and the configuration paths
#define SOLAR_CHARGE_CONTROLLER_ID "1"

using namespace sensesp;

reactesp::ReactESP app;

void setup() {
// Some initialization boilerplate when in debug mode...
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  SensESPAppBuilder builder;

  sensesp_app = builder.set_hostname("vedirect-solar")->get_app();

  // initialize Serial1 on the opto_in pin

  Serial1.begin(19200, SERIAL_8N1, kRXPin, kTXPin, false);

  // Uncomment these lines to output VE.Direct input to the Serial port for
  // debugging purposes.

  // app.onAvailable(Serial1, []() {
  //   Serial.write(Serial1.read());
  // });

  VEDirectInput* vedi = new VEDirectInput(&Serial1);

  vedi->parser.data.channel_1_battery_voltage.connect_to(new SKOutputFloat(
      "electrical.solar." SOLAR_CHARGE_CONTROLLER_ID ".voltage",
      "/Signal K/Solar Charger " SOLAR_CHARGE_CONTROLLER_ID " Voltage"));
  vedi->parser.data.channel_1_battery_current.connect_to(new SKOutputFloat(
      "electrical.solar." SOLAR_CHARGE_CONTROLLER_ID ".current",
      "/Signal K/Solar Charger " SOLAR_CHARGE_CONTROLLER_ID " Current"));
  vedi->parser.data.panel_voltage.connect_to(new SKOutputFloat(
      "electrical.solar." SOLAR_CHARGE_CONTROLLER_ID ".panelVoltage",
      "/Signal K/Solar Charger " SOLAR_CHARGE_CONTROLLER_ID " Panel Voltage"));
  vedi->parser.data.panel_power.connect_to(new SKOutputFloat(
      "electrical.solar." SOLAR_CHARGE_CONTROLLER_ID ".panelPower",
      "/Signal K/Solar Charger " SOLAR_CHARGE_CONTROLLER_ID " Panel Power"));
  vedi->parser.data.yield_total.connect_to(new SKOutputFloat(
      "electrical.solar." SOLAR_CHARGE_CONTROLLER_ID ".yieldTotal",
      "/Signal K/Solar Charger " SOLAR_CHARGE_CONTROLLER_ID " Yield Total"));
  vedi->parser.data.yield_today.connect_to(new SKOutputFloat(
      "electrical.solar." SOLAR_CHARGE_CONTROLLER_ID ".yieldToday",
      "/Signal K/Solar Charger " SOLAR_CHARGE_CONTROLLER_ID " Yield Today"));
  vedi->parser.data.maximum_power_today.connect_to(new SKOutputFloat(
      "electrical.solar." SOLAR_CHARGE_CONTROLLER_ID ".maxPowerToday",
      "/Signal K/Solar Charger " SOLAR_CHARGE_CONTROLLER_ID
      " Max Power Today"));

  sensesp_app->start();
}

void loop() { app.tick(); }
