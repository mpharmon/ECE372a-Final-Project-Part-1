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

// Defines Front Left Sensor Buffer
#define IR_FRONTLEFT_BUF   ADC1BUF0

// Defines Front Right Sensor Buffer
#define IR_FRONTRIGHT_BUF  ADC1BUF1

// Defines Center Left Sensor Buffer
#define IR_CENTERLEFT_BUF  ADC1BUF2

// Defines Center Right Sensor Buffer
#define IR_CENTERRIGHT_BUF ADC1BUF3

// Defines at What ADC Value The Sensor is considered not over the black tape
#define IR_TRIGGER         1

// Initializes IR Sensor
void IRSensor_Init();

// Shortcut that Checks all Front Sensors
bool IRSensor_CheckFront();

// Shortcut that Checks Front Center Sensors
bool IRSensor_CheckCenter();

// Checks if Front Left Sensor is Less Than Trigger Value
bool IRSensor_CheckLeftFront();

// Checks if Front Right Sensor is Less Than Trigger Value
bool IRSensor_CheckRightFront();

// Checks if Center Right Sensor is Less Than Trigger Value
bool IRSensor_CheckCenterRight();

// Checks if Center Left Sensor is Less Than Trigger Value
bool IRSensor_CheckCenterLeft();

#endif // IRSENSOR_H_