#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <stdint.h>
#include <Arduino.h>

/**
 * @struct MotorDriver
 * @brief Structure to manage motor driver configuration and control
 */
typedef struct {
    uint8_t pin_forward;      /**< GPIO pin for forward direction */
    uint8_t channel_forward;  /**< PWM channel for forward direction */
    uint8_t pin_backward;     /**< GPIO pin for backward direction */
    uint8_t channel_backward; /**< PWM channel for backward direction */
    uint8_t resolution;       /**< PWM resolution (in bits) */
    uint32_t frequency;       /**< PWM frequency (in Hz) */
    uint16_t duty;            /**< PWM duty cycle (0-4095 for 12-bit resolution) */
} MotorDriver;

/**
 * @brief Initialize and setup the motor driver
 * @param motor Pointer to MotorDriver structure
 * 
 * This function configures:
 * - pin_forward as output with PWM on channel_forward
 * - pin_backward as output with PWM on channel_backward
 * - Uses resolution and frequency from the structure
 */
void setupMotorDriver(MotorDriver *motor);

/**
 * @brief Move motor forward with specified duty cycle
 * @param motor Pointer to MotorDriver structure
 * @param duty PWM duty cycle value
 * 
 * If the new duty matches the current structure duty, function returns without changes.
 * Otherwise, updates structure duty and sets forward channel to duty value,
 * backward channel to 0 (stop).
 */
void moveForward(MotorDriver *motor, uint16_t duty);

/**
 * @brief Move motor backward with specified duty cycle
 * @param motor Pointer to MotorDriver structure
 * @param duty PWM duty cycle value
 * 
 * If the new duty matches the current structure duty, function returns without changes.
 * Otherwise, updates structure duty and sets backward channel to duty value,
 * forward channel to 0 (stop).
 */
void moveBackward(MotorDriver *motor, uint16_t duty);

/**
 * @brief Stop the motor
 * @param motor Pointer to MotorDriver structure
 * 
 * If the structure duty is already 0, function returns without changes.
 * Otherwise, sets structure duty to 0 and stops both forward and backward channels.
 */
void motorStop(MotorDriver *motor);

#endif // MOTOR_DRIVER_H
