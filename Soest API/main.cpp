#include <Arduino.h>
#include "message_receiver.hpp"

void setup() {
  
}

void loop() {
  message_receiver receiver = message_receiver();
  for(;;){
    receiver.listen_for_message();
  }
}