#include "vedirect_parser.h"

#include <cstring>

namespace VEDirect {

bool FieldParser_mV(const String value, ObservableValue<float>* observable) {
  float val_ = value.toFloat() / 1000;  // mV to V
  observable->set(val_);
  return true;
}

bool FieldParser_permille(const String value,
                          ObservableValue<float>* observable) {
  float val_ = value.toFloat() / 1000;  // permille to ratio
  observable->set(val_);
  return true;
}

bool FieldParser_W(const String value, ObservableValue<float>* observable) {
  float val_ = value.toFloat();
  observable->set(val_);
  return true;
}

bool FieldParser_mA(const String value, ObservableValue<float>* observable) {
  float val_ = value.toFloat() / 1000;  // mA to A
  observable->set(val_);
  return true;
}

bool FieldParser_mAh(const String value, ObservableValue<float>* observable) {
  float val_ = value.toFloat() / 1000 * 3600;  // mAh to Coulomb
  observable->set(val_);
  return true;
}

bool FieldParser_degC(const String value, ObservableValue<float>* observable) {
  float val_ = value.toFloat() + 273.15;  // degC to K
  observable->set(val_);
  return true;
}

bool FieldParser_minutes(const String value,
                         ObservableValue<float>* observable) {
  float val_ = value.toFloat() * 60;  // minutes to seconds
  observable->set(val_);
  return true;
}

bool FieldParser_seconds(const String value,
                         ObservableValue<float>* observable) {
  float val_ = value.toFloat();  // minutes to seconds
  observable->set(val_);
  return true;
}

bool FieldParser_kWh(const String value, ObservableValue<float>* observable) {
  float val_ = value.toFloat() * 0.01 * 3.6e6;  // 0.01 kWh to J
  observable->set(val_);
  return true;
}

bool FieldParser_Vac(const String value, ObservableValue<float>* observable) {
  float val_ = value.toFloat() * 0.01;  // 0.01 V to V
  observable->set(val_);
  return true;
}

bool FieldParser_Iac(const String value, ObservableValue<float>* observable) {
  float val_ = value.toFloat() * 0.1;  // 0.1 A to A
  observable->set(val_);
  return true;
}

bool FieldParser_VA(const String value, ObservableValue<float>* observable) {
  float val_ = value.toFloat();
  observable->set(val_);
  return true;
}

bool FieldParser_int(const String value, ObservableValue<float>* observable) {
  float val_ = value.toInt();
  observable->set(val_);
  return true;
}

bool FieldParser_int(const String value, ObservableValue<int>* observable) {
  float val_ = value.toInt();
  observable->set(val_);
  return true;
}

bool FieldParser_ONOFF(const String value, ObservableValue<bool>* observable) {
  bool ok = false;
  bool val_;

  String val_upper = value;
  val_upper.toUpperCase();

  if (val_upper == "ON") {
    val_ = true;
    ok = true;
  } else if (val_upper == "OFF") {
    val_ = false;
    ok = true;
  }
  if (ok) {
    observable->set(val_);
  }
  return ok;
}

bool FieldParser_uint16(const String value, ObservableValue<int>* observable) {
  float val_ = value.toInt();
  observable->set(val_);
  return true;
}

bool FieldParser_uint24(const String value, ObservableValue<int>* observable) {
  float val_ = value.toInt();
  observable->set(val_);
  return true;
}

bool FieldParser_str(const String value, ObservableValue<String>* observable) {
  observable->set(value);
  return true;
}

Parser::Parser() {
  add_all_field_parsers();
  current_state = &Parser::state_start;
}

void Parser::add_all_field_parsers() {
  field_parsers["V"] = [this](const String value) -> bool {
    return FieldParser_mV(value, &(this->data.channel_1_battery_voltage));
  };
  field_parsers["V2"] = [this](const String value) -> bool {
    return FieldParser_mV(value, &(this->data.channel_2_battery_voltage));
  };
  field_parsers["V3"] = [this](const String value) -> bool {
    return FieldParser_mV(value, &(this->data.channel_3_battery_voltage));
  };
  field_parsers["VS"] = [this](const String value) -> bool {
    return FieldParser_mV(value, &(this->data.auxiliary_voltage));
  };
  field_parsers["VM"] = [this](const String value) -> bool {
    return FieldParser_mV(value, &(this->data.mid_point_voltage));
  };
  field_parsers["DM"] = [this](const String value) -> bool {
    return FieldParser_permille(value, &(this->data.mid_point_deviation));
  };
  field_parsers["VPV"] = [this](const String value) -> bool {
    return FieldParser_mV(value, &(this->data.panel_voltage));
  };
  field_parsers["PPV"] = [this](const String value) -> bool {
    return FieldParser_W(value, &(this->data.panel_power));
  };
  field_parsers["I"] = [this](const String value) -> bool {
    return FieldParser_mA(value, &(this->data.channel_1_battery_current));
  };
  field_parsers["I2"] = [this](const String value) -> bool {
    return FieldParser_mA(value, &(this->data.channel_2_battery_current));
  };
  field_parsers["I3"] = [this](const String value) -> bool {
    return FieldParser_mA(value, &(this->data.channel_3_battery_current));
  };
  field_parsers["IL"] = [this](const String value) -> bool {
    return FieldParser_mA(value, &(this->data.load_current));
  };
  field_parsers["LOAD"] = [this](const String value) -> bool {
    return FieldParser_ONOFF(value, &(this->data.load_output_state));
  };
  field_parsers["T"] = [this](const String value) -> bool {
    return FieldParser_degC(value, &(this->data.battery_temperature));
  };
  field_parsers["P"] = [this](const String value) -> bool {
    return FieldParser_W(value, &(this->data.instantaneous_power));
  };
  field_parsers["CE"] = [this](const String value) -> bool {
    return FieldParser_mAh(value, &(this->data.consumed_energy));
  };
  field_parsers["SOC"] = [this](const String value) -> bool {
    return FieldParser_permille(value, &(this->data.state_of_charge));
  };
  field_parsers["TTG"] = [this](const String value) -> bool {
    return FieldParser_minutes(value, &(this->data.time_to_go));
  };
  field_parsers["Alarm"] = [this](const String value) -> bool {
    return FieldParser_ONOFF(value, &(this->data.alarm_condition_active));
  };
  field_parsers["Relay"] = [this](const String value) -> bool {
    return FieldParser_ONOFF(value, &(this->data.relay_state));
  };
  field_parsers["AR"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.alarm_reason));
  };
  field_parsers["OR"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.off_reason));
  };
  field_parsers["H1"] = [this](const String value) -> bool {
    return FieldParser_mAh(value, &(this->data.depth_of_deepest_discharge));
  };
  field_parsers["H2"] = [this](const String value) -> bool {
    return FieldParser_mAh(value, &(this->data.depth_of_last_discharge));
  };
  field_parsers["H3"] = [this](const String value) -> bool {
    return FieldParser_mAh(value, &(this->data.depth_of_average_discharge));
  };
  field_parsers["H4"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.number_of_charge_cycles));
  };
  field_parsers["H5"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.number_of_full_discharges));
  };
  field_parsers["H6"] = [this](const String value) -> bool {
    return FieldParser_mAh(value, &(this->data.cumulative_energy_drawn));
  };
  field_parsers["H7"] = [this](const String value) -> bool {
    return FieldParser_mV(value, &(this->data.minimum_main_voltage));
  };
  field_parsers["H8"] = [this](const String value) -> bool {
    return FieldParser_mV(value, &(this->data.maximum_main_voltage));
  };
  field_parsers["H9"] = [this](const String value) -> bool {
    return FieldParser_seconds(value,
                               &(this->data.seconds_since_last_full_charge));
  };
  field_parsers["H10"] = [this](const String value) -> bool {
    return FieldParser_int(value,
                           &(this->data.number_of_automatic_synchronizations));
  };
  field_parsers["H11"] = [this](const String value) -> bool {
    return FieldParser_int(value,
                           &(this->data.number_of_low_main_voltage_alarms));
  };
  field_parsers["H12"] = [this](const String value) -> bool {
    return FieldParser_int(value,
                           &(this->data.number_of_high_main_voltage_alarms));
  };
  field_parsers["H13"] = [this](const String value) -> bool {
    return FieldParser_int(
        value, &(this->data.number_of_low_auxiliary_voltage_alarms));
  };
  field_parsers["H14"] = [this](const String value) -> bool {
    return FieldParser_int(
        value, &(this->data.number_of_high_auxiliary_voltage_alarms));
  };
  field_parsers["H15"] = [this](const String value) -> bool {
    return FieldParser_mV(value, &(this->data.minimum_auxiliary_voltage));
  };
  field_parsers["H16"] = [this](const String value) -> bool {
    return FieldParser_mV(value, &(this->data.maximum_auxiliary_voltage));
  };
  field_parsers["H17"] = [this](const String value) -> bool {
    return FieldParser_kWh(value, &(this->data.amount_of_discharged_energy));
  };
  field_parsers["H18"] = [this](const String value) -> bool {
    return FieldParser_kWh(value, &(this->data.amount_of_charged_energy));
  };
  field_parsers["H19"] = [this](const String value) -> bool {
    return FieldParser_kWh(value, &(this->data.yield_total));
  };
  field_parsers["H20"] = [this](const String value) -> bool {
    return FieldParser_kWh(value, &(this->data.yield_today));
  };
  field_parsers["H21"] = [this](const String value) -> bool {
    return FieldParser_W(value, &(this->data.maximum_power_today));
  };
  field_parsers["H22"] = [this](const String value) -> bool {
    return FieldParser_kWh(value, &(this->data.yield_yesterday));
  };
  field_parsers["H23"] = [this](const String value) -> bool {
    return FieldParser_W(value, &(this->data.maximum_power_yesterday));
  };
  field_parsers["ERR"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.error_code));
  };
  field_parsers["CS"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.state_of_operation));
  };
  field_parsers["BMV"] = [this](const String value) -> bool {
    return FieldParser_str(value, &(this->data.model_description));
  };
  field_parsers["FW"] = [this](const String value) -> bool {
    return FieldParser_uint16(value, &(this->data.firmware_version));
  };
  field_parsers["FWE"] = [this](const String value) -> bool {
    return FieldParser_uint24(value, &(this->data.firmware_version_24));
  };
  field_parsers["PID"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.product_id));
  };
  field_parsers["SER#"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.serial_number));
  };
  field_parsers["HSDS"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.day_sequence_number));
  };
  field_parsers["MODE"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.device_mode));
  };
  field_parsers["AC_OUT_V"] = [this](const String value) -> bool {
    return FieldParser_Vac(value, &(this->data.ac_output_voltage));
  };
  field_parsers["AC_OUT_I"] = [this](const String value) -> bool {
    return FieldParser_Iac(value, &(this->data.ac_output_current));
  };
  field_parsers["AC_OUT_S"] = [this](const String value) -> bool {
    return FieldParser_VA(value, &(this->data.ac_output_apparent_power));
  };
  field_parsers["WARN"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.warning_reason));
  };
  field_parsers["MPPT"] = [this](const String value) -> bool {
    return FieldParser_int(value, &(this->data.tracker_operation_mode));
  };
}

void Parser::handle(int c) {
  Serial.write(c);
  checksum = (checksum + c) & 255;
  (this->*(current_state))(c);
}

void Parser::state_start(char c) {
  cur_offset = 0;
  current_state = &Parser::state_label;
  (this->*(current_state))(c);
}

void Parser::state_label(char c) {
  if (c == '\t') {
    // done reading the label
    label_buffer[cur_offset++] = 0;
    current_state = &Parser::state_tab;
    (this->*(current_state))(c);
    return;
  }
  if (cur_offset == label_max_length) {
    // read too much already
    current_state = &Parser::state_error;
    (this->*(current_state))(c);
    return;
  }
  // process the next character of the label
  label_buffer[cur_offset++] = c;
}

void Parser::state_tab(char c) {
  cur_offset = 0;
  if (strcmp(label_buffer, "Checksum") == 0) {
    // the checksum byte needs special treatment
    current_state = &Parser::state_checksum_value;
  } else {
    current_state = &Parser::state_value;
  }
  // we already know we're at a tab, so we can ingest it by doing nothing
}

void Parser::state_value(char c) {
  if (c == 0x0d) {
    // beginning of newline -- we got a complete field
    value_buffer[cur_offset++] = 0;
    current_state = &Parser::state_received_field;
    (this->*(current_state))(c);
    return;
  }
  if (cur_offset == value_max_length) {
    // read too much already
    current_state = &Parser::state_error;
    (this->*(current_state))(c);
    return;
  }
  // process the next character of the value
  value_buffer[cur_offset++] = c;
}

void Parser::state_checksum_value(char c) {
  debugD("Checksum: %d (%d) vs %d", checksum, checksum % 256, c);
  if (checksum == 0) {
    // checksum matches, we've received the whole block
    current_state = &Parser::state_received_block;
    (this->*(current_state))(c);
  } else {
    // else the checksum didn't match
    current_state = &Parser::state_newline;
  }
  checksum = 0;
}

void Parser::state_received_field(char c) {
  debugD("Received field: %s, %s", label_buffer, value_buffer);
  num_fields_in_block += 1;
  if (num_fields_in_block > max_fields_in_block) {
    current_state = &Parser::state_error;
    (this->*(current_state))(c);
    return;
  }
  Field f = {.label = label_buffer, .value = value_buffer};
  field_list.push_front(f);
  current_state = &Parser::state_newline;
  (this->*(current_state))(c);
}

void Parser::state_received_block(char c) {
  debugD("Received block");
  for (auto const& field : field_list) {
    // check if the field is present
    if (field_parsers.find(field.label) == field_parsers.end()) {
      // not found, ignore
      debugD("Could not find parser for field %s", field.label.c_str());
    } else {
      // found
      field_parsers[field.label](field.value);
    }
  }
  field_list.clear();
  num_fields_in_block = 0;
  current_state = &Parser::state_newline;
}

void Parser::state_newline(char c) {
  if (c != 0xd && c != 0xa) {
    cur_offset = 0;
    current_state = &Parser::state_label;
    (this->*(current_state))(c);
  }
}

void Parser::state_error(char c) {
  if (c == 0xd || c == 0xa) {
    num_fields_in_block = 0;
    field_list.clear();
    current_state = &Parser::state_newline;
    (this->*(current_state))(c);
  }
}

}  // namespace VEDirect
