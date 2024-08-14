
#include "vedirect.h"

#include "sensesp.h"

VEDirectInput::VEDirectInput(Stream* rx_stream)
    : rx_stream_{rx_stream} {
  // enable reading the serial port
  ReactESP::app->onAvailable(*rx_stream_, [this]() {
    while (rx_stream_->available()) {
      parser.handle(rx_stream_->read());
    }
  });
}
