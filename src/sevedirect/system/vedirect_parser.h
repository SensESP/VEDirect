#ifndef _VEDIRECT_PARSER_H_
#define _VEDIRECT_PARSER_H_

#include <forward_list>
#include <functional>
#include <map>

#include "Arduino.h"
#include "sensesp.h"
#include "sensesp/system/observablevalue.h"

using namespace sensesp;

const int max_fields_in_block = 22;
const int label_max_length = 9;
const int value_max_length = 33;
typedef char label_str[label_max_length];
typedef char value_str[value_max_length];

namespace VEDirect {

struct Field {
  String label;
  String value;
};

bool FieldParser_mV(const String value, ObservableValue<float>* observable);
bool FieldParser_permille(const String value,
                          ObservableValue<float>* observable);
bool FieldParser_W(const String value, ObservableValue<float>* observable);
bool FieldParser_mA(const String value, ObservableValue<float>* observable);
bool FieldParser_mAh(const String value, ObservableValue<float>* observable);
bool FieldParser_degC(const String value, ObservableValue<float>* observable);
bool FieldParser_minutes(const String value,
                         ObservableValue<float>* observable);
bool FieldParser_seconds(const String value,
                         ObservableValue<float>* observable);
bool FieldParser_kWh(const String value, ObservableValue<float>* observable);
bool FieldParser_Vac(const String value, ObservableValue<float>* observable);
bool FieldParser_Iac(const String value, ObservableValue<float>* observable);
bool FieldParser_VA(const String value, ObservableValue<float>* observable);
bool FieldParser_int(const String value, ObservableValue<int>* observable);
bool FieldParser_ONOFF(const String value, ObservableValue<bool>* observable);
bool FieldParser_uint16(const String value, ObservableValue<int>* observable);
bool FieldParser_uint24(const String value, ObservableValue<int>* observable);
bool FieldParser_str(const String value, ObservableValue<String>* observable);

struct VEDirectData {
  ObservableValue<float> channel_1_battery_voltage;
  ObservableValue<float> channel_2_battery_voltage;
  ObservableValue<float> channel_3_battery_voltage;
  ObservableValue<float> auxiliary_voltage;
  ObservableValue<float> mid_point_voltage;
  ObservableValue<float> mid_point_deviation;
  ObservableValue<float> panel_voltage;
  ObservableValue<float> panel_power;
  ObservableValue<float> channel_1_battery_current;
  ObservableValue<float> channel_2_battery_current;
  ObservableValue<float> channel_3_battery_current;
  ObservableValue<float> load_current;
  ObservableValue<bool> load_output_state;
  ObservableValue<float> battery_temperature;
  ObservableValue<float> instantaneous_power;
  ObservableValue<float> consumed_energy;
  ObservableValue<float> state_of_charge;
  ObservableValue<float> time_to_go;
  ObservableValue<bool> alarm_condition_active;
  ObservableValue<bool> relay_state;
  ObservableValue<int> alarm_reason;
  ObservableValue<int> off_reason;
  ObservableValue<float> depth_of_deepest_discharge;
  ObservableValue<float> depth_of_last_discharge;
  ObservableValue<float> depth_of_average_discharge;
  ObservableValue<int> number_of_charge_cycles;
  ObservableValue<int> number_of_full_discharges;
  ObservableValue<float> cumulative_energy_drawn;
  ObservableValue<float> minimum_main_voltage;
  ObservableValue<float> maximum_main_voltage;
  ObservableValue<float> seconds_since_last_full_charge;
  ObservableValue<int> number_of_automatic_synchronizations;
  ObservableValue<int> number_of_low_main_voltage_alarms;
  ObservableValue<int> number_of_high_main_voltage_alarms;
  ObservableValue<int> number_of_low_auxiliary_voltage_alarms;
  ObservableValue<int> number_of_high_auxiliary_voltage_alarms;
  ObservableValue<float> minimum_auxiliary_voltage;
  ObservableValue<float> maximum_auxiliary_voltage;
  ObservableValue<float> amount_of_discharged_energy;
  ObservableValue<float> amount_of_charged_energy;
  ObservableValue<float> yield_total;
  ObservableValue<float> yield_today;
  ObservableValue<float> maximum_power_today;
  ObservableValue<float> yield_yesterday;
  ObservableValue<float> maximum_power_yesterday;
  ObservableValue<int> error_code;
  ObservableValue<int> state_of_operation;
  ObservableValue<String> model_description;
  ObservableValue<int> firmware_version;
  ObservableValue<int> firmware_version_24;
  ObservableValue<int> product_id;
  ObservableValue<int> serial_number;
  ObservableValue<int> day_sequence_number;
  ObservableValue<int> device_mode;
  ObservableValue<float> ac_output_voltage;
  ObservableValue<float> ac_output_current;
  ObservableValue<float> ac_output_apparent_power;
  ObservableValue<int> warning_reason;
  ObservableValue<int> tracker_operation_mode;
};

class Parser {
 public:
  Parser();
  void handle(int c);
  VEDirectData data;

 protected:
  void (Parser::*current_state)(char);
  void state_start(char c);
  void state_label(char c);
  void state_tab(char c);
  void state_value(char c);
  void state_checksum_value(char c);
  void state_received_field(char c);
  void state_received_block(char c);
  void state_newline(char c);
  void state_error(char c);

  label_str label_buffer;
  value_str value_buffer;
  // current offset of the buffer
  int cur_offset;

  // number of fields received in the current block so far
  int num_fields_in_block = 0;
  // checksum
  unsigned int checksum = 0;
  std::map<String, std::function<bool(const String)>> field_parsers;
  void add_all_field_parsers();

  std::forward_list<Field> field_list;

  void parse_fields();
};

}  // namespace VEDirect

#endif
