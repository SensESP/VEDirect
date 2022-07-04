#include <Arduino.h>

#include "ReactESP.h"
#include "sensesp_app.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp_app_builder.h"
#include "sevedirect/sensors/vedirect.h"

// SDA and SCL pins on SH-ESP32
constexpr int kSDAPin = 16;
constexpr int kSCLPin = 17;

// ON SH-ESP32, we're using SCL and SDA pins for I/O because they've got a bit
// of EMC protection.
constexpr int kTXPin = kSCLPin;
constexpr int kRXPin = kSDAPin;

using namespace sensesp;

reactesp::ReactESP app;

void setup() {
// Some initialization boilerplate when in debug mode...
#ifndef SERIAL_DEBUG_DISABLED
  SetupSerialDebug(115200);
#endif

  SensESPAppBuilder builder;

  sensesp_app = builder.set_hostname("vedirect-bmv")->get_app();

  // initialize Serial1 on the opto_in pin

  Serial1.begin(19200, SERIAL_8N1, kRXPin, kTXPin, false);


  // Uncomment these lines to output VE.Direct input to the Serial port for
  // debugging purposes.

  // app.onAvailable(Serial1, []() {
  //   Serial.write(Serial1.read());
  // });

  VEDirectInput* vedi = new VEDirectInput(&Serial1);

  vedi->parser.data.channel_1_battery_voltage.connect_to(new SKOutputFloat(
      "electrical.batteries.house.voltage", "/battery/house/voltage"));
  vedi->parser.data.channel_1_battery_current.connect_to(new SKOutputFloat(
      "electrical.batteries.house.current", "/battery/house/current"));
  vedi->parser.data.state_of_charge.connect_to(
      new SKOutputFloat("electrical.batteries.house.capacity.stateOfCharge",
                        "/battery/house/stateOfCharge"));

  vedi->parser.data.instantaneous_power.connect_to(new SKOutputFloat(
      "electrical.batteries.house.power", "/battery/house/power"));

  vedi->parser.data.consumed_energy.connect_to(
      new SKOutputFloat("electrical.batteries.house.consumedEnergy",
                        "/battery/house/consumedEnergy"));

  vedi->parser.data.time_to_go.connect_to(new SKOutputFloat(
      "electrical.batteries.house.timeToGo", "/battery/house/timeToGo"));

  vedi->parser.data.depth_of_deepest_discharge.connect_to(
      new SKOutputFloat("electrical.batteries.house.depthOfDeepestDischarge",
                        "/battery/house/depthOfDeepestDischarge"));
  vedi->parser.data.depth_of_last_discharge.connect_to(
      new SKOutputFloat("electrical.batteries.house.depthOfLastDischarge",
                        "/battery/house/depthOfLastDischarge"));
  vedi->parser.data.depth_of_average_discharge.connect_to(
      new SKOutputFloat("electrical.batteries.house.depthOfAverageDischarge",
                        "/battery/house/depthOfAverageDischarge"));
  vedi->parser.data.number_of_charge_cycles.connect_to(
      new SKOutputInt("electrical.batteries.house.numberOfChargeCycles",
                      "/battery/house/numberOfChargeCycles"));
  vedi->parser.data.number_of_full_discharges.connect_to(
      new SKOutputInt("electrical.batteries.house.numberOfFullDischarges",
                      "/battery/house/numberOfFullDischarges"));
  vedi->parser.data.cumulative_energy_drawn.connect_to(
      new SKOutputFloat("electrical.batteries.house.cumulativeEnergyDrawn",
                        "/battery/house/cumulativeEnergyDrawn"));
  vedi->parser.data.minimum_main_voltage.connect_to(
      new SKOutputFloat("electrical.batteries.house.minimumVoltage",
                        "/battery/house/minimumVoltage"));
  vedi->parser.data.maximum_main_voltage.connect_to(
      new SKOutputFloat("electrical.batteries.house.maximumVoltage",
                        "/battery/house/maximumVoltage"));
  vedi->parser.data.seconds_since_last_full_charge.connect_to(
      new SKOutputFloat("electrical.batteries.house.secondsSinceLastFullCharge",
                        "/battery/house/secondsSinceLastFullCharge"));

  vedi->parser.data.auxiliary_voltage.connect_to(new SKOutputFloat(
      "electrical.batteries.start.voltage", "/battery/start/voltage"));
  vedi->parser.data.minimum_auxiliary_voltage.connect_to(
      new SKOutputFloat("electrical.batteries.start.minimumVoltage",
                        "/battery/start/minimumVoltage"));
  vedi->parser.data.maximum_auxiliary_voltage.connect_to(
      new SKOutputFloat("electrical.batteries.start.maximumVoltage",
                        "/battery/start/maximumVoltage"));

  sensesp_app->start();
}

void loop() { app.tick(); }
