#include <SPI.h>
#include <Arduino.h>
#include "RadioLib.h"

#include "motor_driver.h"
#include "motor_inst.h"
#include "radio_reciver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

// Queue for single-character car commands
static QueueHandle_t commandQueue = NULL;

#define SPI_MOSI_PIN 4
#define SPI_MISO_PIN 5
#define SPI_CLK_PIN  6
#define SPI_CS_PIN   10

#define RF_MODULE_DIO1_PIN 9
#define RF_MODULE_NRST_PIN 12
#define RF_MODULE_BUSY_PIN 11

#define MOTOR_SPEED_PERCENT 75

static SX1268 radio = NULL;

volatile bool receivedFlag = false;

ICACHE_RAM_ATTR void setFlag(void) {
  // we sent or received a packet, set the flag
  receivedFlag = true;
}

void carCommandExec(char cmdChar) {
  switch (cmdChar) {
    case 'W':
    case 'w':
      moveForward(&motor1, MOTOR_SPEED_PERCENT);
      moveForward(&motor2, MOTOR_SPEED_PERCENT);
      moveForward(&motor3, MOTOR_SPEED_PERCENT);
      moveForward(&motor4, MOTOR_SPEED_PERCENT);
      break;
      
    case 'S':
    case 's':
      moveBackward(&motor1, MOTOR_SPEED_PERCENT);
      moveBackward(&motor2, MOTOR_SPEED_PERCENT);
      moveBackward(&motor3, MOTOR_SPEED_PERCENT);
      moveBackward(&motor4, MOTOR_SPEED_PERCENT);
      break;
    case 'A':
    case 'a':
      moveForward(&motor1, MOTOR_SPEED_PERCENT);
      moveBackward(&motor2, MOTOR_SPEED_PERCENT);
      moveBackward(&motor3, MOTOR_SPEED_PERCENT);
      moveForward(&motor4, MOTOR_SPEED_PERCENT);
      break;
    case 'D':
    case 'd':
      moveBackward(&motor1, MOTOR_SPEED_PERCENT);
      moveForward(&motor2, MOTOR_SPEED_PERCENT);
      moveForward(&motor3, MOTOR_SPEED_PERCENT);
      moveBackward(&motor4, MOTOR_SPEED_PERCENT);
      break;
    
    default:
      break;
  }
}

void motorTestTask(void *pvParameters) {
  char commans[] = {'W', 'S', 'A', 'D'};
  int current = 0;

  while (true) {
    char cmd = commans[current];

    Serial.print(F("Current test command: "));
    Serial.println(cmd);

    carCommandExec(cmd);

    vTaskDelay(pdMS_TO_TICKS(3000));

    current = (current + 1) % 4;
  }
}

// FreeRTOS task: polls receivedFlag every 10 ms and reads radio when set
void radioTask(void *pvParameters) {

  while (true) {
    if (receivedFlag) {
      String str;
      int state = radio.receive(str);
      if (state == RADIOLIB_ERR_NONE) {
        // Send each character of the received string into the command queue
        if (commandQueue != NULL) {
          xQueueSend(commandQueue, &str[0], 0);
        }
      } else {
        Serial.print(F("[SX1268] Failed to receive packet, code "));
        Serial.println(state);
      }
      receivedFlag = false;
    }
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

// Task to consume single-character commands from the queue
void carCommandTask(void *pvParameters) {
  char cmdChar = 0;

  while (true) {
    // Wait for command with 100ms timeout
    BaseType_t result = xQueueReceive(commandQueue, &cmdChar, pdMS_TO_TICKS(100));
    
    if (result == pdPASS) {
      // Command received - process it
      Serial.print(F("[CarCommand] Received char: "));
      Serial.println(cmdChar);

      carCommandExec(cmdChar);
      // Loop back to wait for next command with 100ms timeout
      // If new command arrives, timer resets
      
    } else {
      // Timeout: no command received within 100ms, stop all motors
      motorStop(&motor1);
      motorStop(&motor2);
      motorStop(&motor3);
      motorStop(&motor4);
      
      // Wait indefinitely for next command
      // xQueueReceive(commandQueue, &cmdChar, portMAX_DELAY);
    }
  }
}

void setup_spi() {
    SPI.begin(SPI_CLK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_CS_PIN);
}

void setup_radio_reciver() {
  radio = new Module(SPI_CS_PIN, RF_MODULE_DIO1_PIN, RF_MODULE_NRST_PIN, RF_MODULE_BUSY_PIN);

  setup_radio_reciver(&radio, setFlag);
}

void setup_motors() {
  setupMotorDriver(&motor1);
  setupMotorDriver(&motor2);
  setupMotorDriver(&motor3);
  setupMotorDriver(&motor4);
}

void setup() {
  Serial.begin(115200);
  
  setup_spi();
  setup_radio_reciver();
  setup_motors();
  
  // Create a 1-element queue for command characters
  commandQueue = xQueueCreate(1, sizeof(char));

  // Create FreeRTOS tasks
  xTaskCreatePinnedToCore(radioTask, "RadioTask", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(carCommandTask, "CarCommand", 2048, NULL, 1, NULL, 1);

  #ifdef MOTOR_TEST
  xTaskCreatePinnedToCore(motorTestTask, "TestTask", 2048, NULL, 1, NULL, 1);
  #endif
}

void loop() {

}