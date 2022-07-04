#ifndef _VEDIRECT_H_
#define _VEDIRECT_H_

#include "sensesp/sensors/sensor.h"
#include "sevedirect/system/vedirect_parser.h"

using namespace sensesp;

namespace sensesp {

class VEDirectInput : public Sensor {
 public:
  VEDirectInput(Stream* rx_stream);
  virtual void start() override final;
  VEDirect::Parser parser;

 private:
  Stream* rx_stream_;
};

}  // namespace sensesp

#endif
