#include "motor_driver.h"

#define MOTOR_FREQUENCY 10000
#define MOTOR_RESOLUTION    8

static MotorDriver motor1 = {
    21,
    0,
    48,
    1,
    MOTOR_RESOLUTION,
    MOTOR_FREQUENCY,
    0
};

static MotorDriver motor2 = {
    39,
    2,
    40,
    3,
    MOTOR_RESOLUTION,
    MOTOR_FREQUENCY,
    0
};
static MotorDriver motor3 = {
    41,
    4,
    42,
    5,
    MOTOR_RESOLUTION,
    MOTOR_FREQUENCY,
    0
};
static MotorDriver motor4 = {
    2,
    6,
    1,
    7,
    MOTOR_RESOLUTION,
    MOTOR_FREQUENCY,
    0
};
