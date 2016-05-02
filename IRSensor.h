/*  
 *  Class: ECE372a
 * 
 *  Group: 209
 * 
 *  Lab: Final Lab Part 1
 * 
 *  File: IRSensor.h
 * 
 *  Member: Michael Harmon (Software)
 *          Abdul Rana (Hardware)
 *          James Beulke (System Integration)
 *          Ali Hassan (Quality Control)
 */

#include <xc.h>
#include <stdbool.h>

#ifndef IR_SENSOR_H_
#define IR_SENSOR_H_

// Defines at What ADC Value The Sensor is considered not over the black tape
#define IR_TRIGGER_ADJUST       525
#define IR_TRIGGER_TRAVEL       475
#define IR_TRIGGER_TURN         650

#define IR_SENSOR_DIFFERENTIAL  150

// Delay(ms) to be used during turn around
#define IR_DELAY_TURNAROUND     500

//Delay(ms) to be used during Turns
#define IR_DELAY_TURN           250

// The Biasing Value for Adjusting the Direction of the Robot
#define MOTOR_BIAS_ADJUST       1.2

// The Biasing Value for Turning the Robot.
#define MOTOR_BIAS_TURN         1.35

// Initializes IR Sensor
void IRSensor_Init();

// Shortcut that Checks all Front Sensors
bool IRSensor_CheckFront(int trigger);

// Shortcut that Checks Front Center Sensors
bool IRSensor_CheckCenter(int trigger);

unsigned int IRSensor_GetLeftFront();

// Checks if Front Left Sensor is Less Than Trigger Value
bool IRSensor_CheckLeftFront(int trigger);

unsigned int IRSensor_GetRightFront();

// Checks if Front Right Sensor is Less Than Trigger Value
bool IRSensor_CheckRightFront(int trigger);

unsigned int IRSensor_GetCenterRight();

// Checks if Center Right Sensor is Less Than Trigger Value
bool IRSensor_CheckCenterRight(int trigger);

unsigned int IRSensor_GetCenterLeft();

// Checks if Center Left Sensor is Less Than Trigger Value
bool IRSensor_CheckCenterLeft(int trigger);

#endif // IRSENSOR_H_