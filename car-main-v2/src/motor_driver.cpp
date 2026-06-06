#include <stdint.h>
#include <Arduino.h>
#include <math.h>

#include "motor_driver.h"

void setupMotorDriver(MotorDriver *motor) {
    // Setup pin_forward as output
    pinMode(motor->pin_forward, OUTPUT);
    
    // Setup PWM for forward channel with frequency and resolution from structure
    ledcSetup(motor->channel_forward, motor->frequency, motor->resolution);
    ledcAttachPin(motor->pin_forward, motor->channel_forward);
    
    // Setup pin_backward as output
    pinMode(motor->pin_backward, OUTPUT);
    
    // Setup PWM for backward channel with frequency and resolution from structure
    ledcSetup(motor->channel_backward, motor->frequency, motor->resolution);
    ledcAttachPin(motor->pin_backward, motor->channel_backward);
}

void moveForward(MotorDriver *motor, uint16_t duty) {
    // Check if input duty equals structure duty
    if (duty == motor->duty) {
        return;
    }
    
    // Update structure duty with new input value
    motor->duty = duty;
    
    // Set forward channel to duty value
    ledcWrite(motor->channel_forward, (pow(2, motor->resolution) - 1) * duty / 100.0);
    
    // Set backward channel to 0
    ledcWrite(motor->channel_backward, 0);
}

void moveBackward(MotorDriver *motor, uint16_t duty) {
    // Check if input duty equals structure duty
    if (duty == motor->duty) {
        return;
    }
    
    // Update structure duty with new input value
    motor->duty = duty;
    
    // Set backward channel to duty value
    ledcWrite(motor->channel_backward, (pow(2, motor->resolution) - 1) * duty / 100.0);
    
    // Set forward channel to 0
    ledcWrite(motor->channel_forward, 0);
}

void motorStop(MotorDriver *motor) {
    // Check if structure duty is already zero
    if (motor->duty == 0) {
        return;
    }
    
    // Set structure duty to zero
    motor->duty = 0;
    
    // Stop both forward and backward channels
    ledcWrite(motor->channel_forward, 0);
    ledcWrite(motor->channel_backward, 0);
}
