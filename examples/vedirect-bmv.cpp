// An example of interfacing a smart battery monitor with Signal K.

#include <Arduino.h>

#include "ReactESP.h"
#include "sensesp/signalk/signalk_output.h"
#include "sensesp_app.h"
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
      "electrical.batteries.house.voltage", "/Signal K/House Battery Voltage"));
  vedi->parser.data.channel_1_battery_current.connect_to(new SKOutputFloat(
      "electrical.batteries.house.current", "/Signal K/House Battery Current"));
  vedi->parser.data.state_of_charge.connect_to(
      new SKOutputFloat("electrical.batteries.house.capacity.stateOfCharge",
                        "/Signal K/House Battery State Of Charge"));

  vedi->parser.data.instantaneous_power.connect_to(new SKOutputFloat(
      "electrical.batteries.house.power", "/Signal K/House Battery Power"));

  vedi->parser.data.consumed_energy.connect_to(
      new SKOutputFloat("electrical.batteries.house.consumedEnergy",
                        "/Signal K/House Battery Consumed Energy"));

  vedi->parser.data.time_to_go.connect_to(
      new SKOutputFloat("electrical.batteries.house.timeToGo",
                        "/Signal K/House Battery Time To Go"));

  vedi->parser.data.depth_of_deepest_discharge.connect_to(
      new SKOutputFloat("electrical.batteries.house.depthOfDeepestDischarge",
                        "/Signal K/House Battery Depth Of Deepest Discharge"));
  vedi->parser.data.depth_of_last_discharge.connect_to(
      new SKOutputFloat("electrical.batteries.house.depthOfLastDischarge",
                        "/Signal K/House Battery Depth Of Last Discharge"));
  vedi->parser.data.depth_of_average_discharge.connect_to(
      new SKOutputFloat("electrical.batteries.house.depthOfAverageDischarge",
                        "/Signal K/House Battery Depth Of Average Discharge"));
  vedi->parser.data.number_of_charge_cycles.connect_to(
      new SKOutputInt("electrical.batteries.house.numberOfChargeCycles",
                      "/Signal K/House Battery Number Of Charge Cycles"));
  vedi->parser.data.number_of_full_discharges.connect_to(
      new SKOutputInt("electrical.batteries.house.numberOfFullDischarges",
                      "/Signal K/House Battery Number Of Full Discharges"));
  vedi->parser.data.cumulative_energy_drawn.connect_to(
      new SKOutputFloat("electrical.batteries.house.cumulativeEnergyDrawn",
                        "/Signal K/House Battery Cumulative Energy Drawn"));
  vedi->parser.data.minimum_main_voltage.connect_to(
      new SKOutputFloat("electrical.batteries.house.minimumVoltage",
                        "/Signal K/House Battery Minimum Voltage"));
  vedi->parser.data.maximum_main_voltage.connect_to(
      new SKOutputFloat("electrical.batteries.house.maximumVoltage",
                        "/Signal K/House Battery Maximum Voltage"));
  vedi->parser.data.seconds_since_last_full_charge.connect_to(new SKOutputFloat(
      "electrical.batteries.house.secondsSinceLastFullCharge",
      "/Signal K/House Battery Seconds Since Last Full Charge"));

  vedi->parser.data.auxiliary_voltage.connect_to(new SKOutputFloat(
      "electrical.batteries.start.voltage", "/Signal K/Start Battery Voltage"));
  vedi->parser.data.minimum_auxiliary_voltage.connect_to(
      new SKOutputFloat("electrical.batteries.start.minimumVoltage",
                        "/Signal K/Start Battery Minimum Voltage"));
  vedi->parser.data.maximum_auxiliary_voltage.connect_to(
      new SKOutputFloat("electrical.batteries.start.maximumVoltage",
                        "/Signal K/Start Battery Maximum Voltage"));

  sensesp_app->start();
}

void loop() { app.tick(); }
