#include <Arduino.h>
#include <RadioLib.h>
#include <SPI.h> // ДОБАВЛЕНО: Подключаем библиотеку для работы с SPI

// ДОБАВЛЕНО: Пины для шины SPI (замени на те, что используются на твоей плате)
#define SPI_SCK   18 // 35 // D18
#define SPI_MISO  19 // 38 // D19
#define SPI_MOSI  23 // 36 // D23

// SX1268 has the following connections:
#define DIO1_PIN 21 // 42 // D21
#define NRST_PIN 22 // 39 // D22
#define BUSY_PIN 17 // 27 // D17

#define PIN_NSS 5 // 34 // D5

void sendRadioPacket(const char* packet);

static SX1268 radio = NULL;

// save transmission states between loops
int transmissionState = RADIOLIB_ERR_NONE;

// flag to indicate transmission or reception state
bool transmitFlag = false;

// flag to indicate that a packet was sent or received
volatile bool operationDone = false;

// this function is called when a complete packet
// is transmitted or received by the module
#if defined(ESP8266) || defined(ESP32)
  ICACHE_RAM_ATTR
#endif
void setFlag(void) {
  // we sent or received a packet, set the flag
  operationDone = true;
}


void taskKeyboardInput( void *pvParameters ) {
    while (1) {
        if (Serial.available() > 0) {
            // Read the incoming byte
            char incomingChar;
            
            while (Serial.available() > 0) {
              incomingChar = Serial.read(); 
            }

            // Echo it back
            Serial.print("You pressed: ");
            Serial.println(incomingChar);
        
            sendRadioPacket(&incomingChar);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void sendRadioPacket(const char* packet) {
    int state = radio.transmit(packet);

  if (state == RADIOLIB_ERR_NONE) {
    // the packet was successfully transmitted
    Serial.println(F("success!"));

  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    // the supplied packet was longer than 256 bytes
    Serial.println(F("too long!"));

  } else if (state == RADIOLIB_ERR_TX_TIMEOUT) {
    // timeout occured while transmitting packet
    Serial.println(F("timeout!"));

  } else {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }
}

void setupRadioModule() {
  radio = new Module(PIN_NSS, DIO1_PIN, NRST_PIN, BUSY_PIN);

    // initialize SX1268 at 434 MHz
    Serial.print(F("[SX1268] Initializing ... "));

    ConfigLoRa_t config;
    
    config.frequency = 434;
    
    int state = radio.begin(config);

    if (state == RADIOLIB_ERR_NONE) {
        Serial.println(F("success!"));
    } else {
        Serial.print(F("failed, code "));
        Serial.println(state);
    }

    // set the function that will be called
    // when new packet is received
    // radio.setDio1Action(setFlag);
}

void setupSpi() {
    SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, PIN_NSS);
}

void setup() {
    Serial.begin(115200);

    setupSpi();
    setupRadioModule();

    xTaskCreate(taskKeyboardInput, "TaskKeyboardInput", 4 * 1024, NULL, 1, NULL);
}

void loop() {
    // Empty loop since we're using FreeRTOS tasks
}
























// /*
//   RadioLib SX126x Ping-Pong Example
//   This example is intended to run on two SX126x radios,
//   and send packets between the two.
// */

// // include the library
// #include <RadioLib.h>
// #include <SPI.h> // ДОБАВЛЕНО: Подключаем библиотеку для работы с SPI

// // uncomment the following only on one
// // of the nodes to initiate the pings
// //#define INITIATING_NODE

// // // ДОБАВЛЕНО: Пины для шины SPI (замени на те, что используются на твоей плате)
// // #define SPI_SCK   GPIO_PIN_18 // 35 // D18
// // #define SPI_MISO  GPIO_PIN_19 // 38 // D19
// // #define SPI_MOSI  GPIO_PIN_23 // 36 // D23

// // // SX1268 has the following connections:
// // #define DIO1_PIN GPIO_PIN_21 // 42 // D21
// // #define NRST_PIN GPIO_PIN_22 // 39 // D22
// // #define BUSY_PIN GPIO_PIN_17 // 27 // D17

// // #define PIN_NSS GPIO_PIN_5 // 34 // D5
// // ДОБАВЛЕНО: Пины для шины SPI (замени на те, что используются на твоей плате)
// #define SPI_SCK   18 // 35 // D18
// #define SPI_MISO  19 // 38 // D19
// #define SPI_MOSI  23 // 36 // D23

// // SX1268 has the following connections:
// #define DIO1_PIN 21 // 42 // D21
// #define NRST_PIN 22 // 39 // D22
// #define BUSY_PIN 17 // 27 // D17

// #define PIN_NSS 5 // 34 // D5


// SX1268 radio = NULL;

// // save transmission states between loops
// int transmissionState = RADIOLIB_ERR_NONE;

// // flag to indicate transmission or reception state
// bool transmitFlag = false;

// // flag to indicate that a packet was sent or received
// volatile bool operationDone = false;

// // this function is called when a complete packet
// // is transmitted or received by the module
// #if defined(ESP8266) || defined(ESP32)
//   ICACHE_RAM_ATTR
// #endif
// void setFlag(void) {
//   // we sent or received a packet, set the flag
//   operationDone = true;
// }

// void setup() {
//   Serial.begin(115200);

//   // ДОБАВЛЕНО: Инициализация кастомной шины SPI
//   // Формат: SPI.begin(SCK, MISO, MOSI, SS)
//   SPI.begin(SPI_SCK, SPI_MISO, SPI_MOSI, PIN_NSS);

//   radio = new Module(PIN_NSS, DIO1_PIN, NRST_PIN, BUSY_PIN);

//   // initialize SX1268 at 434 MHz
//   Serial.print(F("[SX1268] Initializing ... "));
  
//   ConfigLoRa_t config;
//   config.frequency = 434;
//   int state = radio.begin(config);
  
//   if (state == RADIOLIB_ERR_NONE) {
//     Serial.println(F("success!"));
//   } else {
//     Serial.print(F("failed, code "));
//     Serial.println(state);
//     while (true) { delay(10); }
//   }

//   // set the function that will be called
//   // when new packet is received
//   radio.setDio1Action(setFlag);

//   #if defined(INITIATING_NODE)
//     // send the first packet on this node
//     Serial.print(F("[SX1268] Sending first packet ... "));
//     transmissionState = radio.startTransmit("Hello World!");
//     transmitFlag = true;
//   #else
//     // start listening for LoRa packets on this node
//     Serial.print(F("[SX1268] Starting to listen ... "));
//     state = radio.startReceive();
//     if (state == RADIOLIB_ERR_NONE) {
//       Serial.println(F("success!"));
//     } else {
//       Serial.print(F("failed, code "));
//       Serial.println(state);
//       while (true) { delay(10); }
//     }
//   #endif
// }

// void loop() {
//   // check if the previous operation finished
//   if(operationDone) {
//     // reset flag
//     operationDone = false;

//     if(transmitFlag) {
//       // the previous operation was transmission, listen for response
//       // print the result
//       if (transmissionState == RADIOLIB_ERR_NONE) {
//         // packet was successfully sent
//         Serial.println(F("transmission finished!"));
//       } else {
//         Serial.print(F("failed, code "));
//         Serial.println(transmissionState);
//       }

//       // listen for response
//       radio.startReceive();
//       transmitFlag = false;

//     } else {
//       // the previous operation was reception
//       // print data and send another packet
//       String str;
//       int state = radio.readData(str);

//       if (state == RADIOLIB_ERR_NONE) {
//         // packet was successfully received
//         Serial.println(F("[SX1268] Received packet!"));

//         // print data of the packet
//         Serial.print(F("[SX1268] Data:\t\t"));
//         Serial.println(str);

//         // print RSSI (Received Signal Strength Indicator)
//         Serial.print(F("[SX1268] RSSI:\t\t"));
//         Serial.print(radio.getRSSI());
//         Serial.println(F(" dBm"));

//         // print SNR (Signal-to-Noise Ratio)
//         Serial.print(F("[SX1268] SNR:\t\t"));
//         Serial.print(radio.getSNR());
//         Serial.println(F(" dB"));
//       }

//       // wait a second before transmitting again
//       delay(1000);

//       // send another one
//       Serial.print(F("[SX1268] Sending another packet ... "));
//       transmissionState = radio.startTransmit("Hello World!");
//       transmitFlag = true;
//     }
//   }
// }