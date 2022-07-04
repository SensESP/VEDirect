#ifndef _VEDIRECT_H_
#define _VEDIRECT_H_

#include "system/vedirect_parser.h"

#include "sensors/sensor.h"


class VEDirectInput : public Sensor {
 public:
  VEDirectInput(Stream* rx_stream);
  virtual void enable() override final;
  VEDirect::Parser parser;
 private:
  Stream* rx_stream_;
};

#endif
