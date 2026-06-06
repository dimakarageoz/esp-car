#include <Arduino.h>
#include "RadioLib.h"

void setup_radio_reciver(SX1268 *radio, void (*setFlag)(void)) {
    // initialize SX1268 at 434 MHz
    Serial.print(F("[SX1268] Initializing ... "));

    ConfigLoRa_t config;
    
    config.frequency = 434;
    
    int state = radio->begin(config);

    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
    }

    // set the function that will be called
    // when new packet is received
    radio->setDio1Action(setFlag);
}