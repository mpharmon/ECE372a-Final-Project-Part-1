#include <xc.h>

#ifndef IR_SENSOR_H_
#define IR_SENSOR_H_

#define IR_FRONTLEFT_BUF   ADC1BUF0

#define IR_FRONTRIGHT_BUF  ADC1BUF1

#define IR_CENTERLEFT_BUF  ADC1BUF2

#define IR_CENTERRIGHT_BUF ADC1BUF3

// Defines at What ADC Value The Sensor is considered not over the black tape
#define IR_TRIGGER         1

void IRSensor_Init();

// Shortcut that Checks all Front Sensors
unsigned short IRSensor_CheckFront()

unsigned short IRSensor_CheckLeftFront();

unsigned short IRSensor_CheckRightFront();

unsigned short IRSensor_CheckCenterRight();

unsigned short IRSensor_CheckCenterLeft();

#endif // IRSENSOR_H_