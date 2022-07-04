
#include "vedirect.h"

#include "sensesp.h"

VEDirectInput::VEDirectInput(Stream* rx_stream)
    : Sensor(), rx_stream_{rx_stream} {}


void VEDirectInput::enable() {
  // enable reading the serial port
  app.onAvailable(*rx_stream_, [this]() {
    while (rx_stream_->available()) {
      parser.handle(rx_stream_->read());
    }
  });
}
