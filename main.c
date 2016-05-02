/*  
 *  Class: ECE372a
 * 
 *  Group: 209
 * 
 *  Lab: Final Lab Part 1
 * 
 *  File: main.c
 * 
 *  Member: Michael Harmon (Software)
 *          Abdul Rana (Hardware)
 *          James Beulke (System Integration)
 *          Ali Hassan (Quality Control)
 */

#include <xc.h>
#include <stdbool.h>
#include "config.h"
#include "interrupt.h"
#include "IRSensor.h"
#include "../Motor.X/Motor.h"
#include "../LCD.X/lcd.h"

typedef enum{
  START = 0,
  TURN_LEFT = 1,
  TURN_RIGHT = 2,
  TRAVEL = 3,
  ADJUST_RIGHT = 4,
  ADJUST_LEFT = 5,
  TURN_AROUND = 6,
  END = 7
}STATE;

void printDebug(STATE state){
  LCD_MoveCursor(1,1);
  char stringToPrint[16];
  sprintf(stringToPrint,"%4d%4d%4d%4d",IRSensor_GetLeftFront(), IRSensor_GetCenterLeft(), IRSensor_GetCenterRight(), IRSensor_GetRightFront());
  LCD_PrintString(stringToPrint);
  LCD_MoveCursor(1,2);
  sprintf(stringToPrint,"OC:%5d:%5d %1d",OC2RS,OC4RS,state);
  LCD_PrintString(stringToPrint);
};

void main(void){
  SYSTEMConfigPerformance(10000000);
  STATE state = START;
  unsigned short int lineCount = 0;
  bool ignoreTurns = false;
  // Initialize Motor(s))
  Motor_Init();
  // Initialize IRSensor(s)
  IRSensor_Init();
  // LCD Initialize
  LCD_Init();
  while(1){
    printDebug(state);
    switch(state){
      case START:
        Motor_Disable();
        if(IRSensor_CheckCenter(IR_TRIGGER_TRAVEL))state = TRAVEL;
        break;
      case TRAVEL:
        Motor_Enable();
        if(IRSensor_CheckLeftFront(200)){
          state = TURN_LEFT;break;
        }else if(IRSensor_CheckRightFront(200)){
          state = TURN_RIGHT;break;
        }else if(IRSensor_GetCenterLeft() < 300 || IRSensor_GetCenterRight < 300){
          if(IRSensor_GetCenterLeft() > IRSensor_GetCenterRight()){
            Motor_Set1DutyCycle(0);
            Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
          }else{
            Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE);
            Motor_Set2DutyCycle(0);
          };
        }else if(IRSensor_GetCenterLeft() != IRSensor_GetCenterRight()){
          float biasValue = 1.0 + abs(IRSensor_GetCenterLeft() - IRSensor_GetCenterRight()) / 2000.0;
          if(IRSensor_GetCenterLeft() > IRSensor_GetCenterRight()){
            Motor_Set1DutyCycle(biasValue * STANDARD_DUTY_CYCLE);
            Motor_Set2DutyCycle(1.0/biasValue * STANDARD_DUTY_CYCLE);
          }else{
            Motor_Set1DutyCycle(1.0/biasValue * STANDARD_DUTY_CYCLE);
            Motor_Set2DutyCycle(biasValue * STANDARD_DUTY_CYCLE);
          };
        }else{
          Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE);
          Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
        };
        break;
      case TURN_LEFT:
        if(IRSensor_CheckCenter(IR_TRIGGER_TRAVEL)){
          Motor_Disable();
          state = TRAVEL;
        }else{
          Motor_Set1DutyCycle(0.0);
          Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE * MOTOR_BIAS_TURN);
          Motor_Enable();
        };
        break;
      case TURN_RIGHT:
        if(IRSensor_CheckCenter(IR_TRIGGER_TRAVEL)){
          Motor_Disable();
          state = TRAVEL;
        }else{
          Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE * MOTOR_BIAS_TURN);
          Motor_Set2DutyCycle(0.0);
          Motor_Enable();
        };
        break;
    };
  };
};

void oldmain(void){
  SYSTEMConfigPerformance(10000000);
  STATE state = START;
  unsigned short int lineCount = 0;
  bool ignoreTurns = false;
  // Initialize Motor(s))
  Motor_Init();
  // Initialize IRSensor(s)
  IRSensor_Init();
  // LCD Initialize
  LCD_Init();
  Motor_Enable();
  while(1){
    printDebug(state);
    switch(state){
      case START:
        Motor_Disable();
        if(IRSensor_CheckCenter(IR_TRIGGER_TRAVEL))state = TRAVEL;
        break;
      case TURN_LEFT:
        if(IRSensor_CheckCenter(IR_TRIGGER_TRAVEL)){
          Motor_Disable();
          state = TRAVEL;
        }else{
          Motor_Set1DutyCycle(0.0);
          Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE * MOTOR_BIAS_TURN);
          Motor_Enable();
        };
        break;
      case TURN_RIGHT:
        if(IRSensor_CheckCenter(IR_TRIGGER_TRAVEL)){
          Motor_Disable();
          state = TRAVEL;
        }else{
          Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE * MOTOR_BIAS_TURN);
          Motor_Set2DutyCycle(0.0);
          Motor_Enable();
        };
        break;
      case TRAVEL:
        Motor_Set1Forward();
        Motor_Set2Forward();
        Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE);
        Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
        Motor_Enable();
        if(!IRSensor_CheckCenterRight(IR_TRIGGER_ADJUST) && IRSensor_CheckCenterLeft(IR_TRIGGER_ADJUST)){
          Motor_Disable();
          state = ADJUST_RIGHT;break;
        }else if(!IRSensor_CheckCenterLeft(IR_TRIGGER_ADJUST) && IRSensor_CheckCenterRight(IR_TRIGGER_ADJUST)){
          Motor_Disable();
          state = ADJUST_LEFT;break;};
        //  if(IRSensor_CheckFront()){
        //  lineCount++;
        //  if(lineCount == 1){// Entering Extra Credit 'T' Intersection
        //    state = TURN_LEFT;
        //  }else if(lineCount == 2){// Exiting Extra Credit 'T' Intersection
        //    state = TURN_RIGHT;
        //  }else if(lineCount == 5){// End of First Time Through
        //    state = TURN_AROUND;
        //  };
        //};
        if(!ignoreTurns && IRSensor_CheckLeftFront(IR_TRIGGER_TURN)){Motor_Disable();state = TURN_LEFT;break;};
        if(!ignoreTurns && IRSensor_CheckRightFront(IR_TRIGGER_TURN)){Motor_Disable();state = TURN_RIGHT;break;};//
        break;
      case ADJUST_LEFT:
        Motor_Set1DutyCycle(MOTOR_BIAS_ADJUST * STANDARD_DUTY_CYCLE);
        Motor_Set2DutyCycle(0.75 * MOTOR_BIAS_ADJUST * STANDARD_DUTY_CYCLE);
        Motor_Enable();
        if(IRSensor_CheckLeftFront(IR_TRIGGER_TURN)){
          state = TURN_LEFT;
          break;
        }else if(IRSensor_CheckRightFront(IR_TRIGGER_TURN)){
          state = TURN_RIGHT;
          break;
        }else if(IRSensor_CheckCenter(IR_TRIGGER_TRAVEL)){
          Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE);
          Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
          state = TRAVEL;
          break;
        }else{
          break;
        };
      case ADJUST_RIGHT:
        Motor_Set1DutyCycle(0.75 * MOTOR_BIAS_ADJUST * STANDARD_DUTY_CYCLE);
        Motor_Set2DutyCycle(MOTOR_BIAS_ADJUST * STANDARD_DUTY_CYCLE);
        Motor_Enable();
        if(IRSensor_CheckLeftFront(IR_TRIGGER_TURN)){
          state = TURN_LEFT;
          break;
        }else if(IRSensor_CheckRightFront(IR_TRIGGER_TURN)){
          state = TURN_RIGHT;
          break;
        }else if(IRSensor_CheckCenter(IR_TRIGGER_TRAVEL)){
          Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE);
          Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
          state = TRAVEL;
          break;
        }else{
          break;
        };
      //case TURN_AROUND:
      //  while(IRSensor_CheckCenterLeft(IR_TRIGGER_TRAVEL)){
      //    Motor_Set1Backward();
      //    Motor_Set2Forward();
      //  };
      //  delayMs(IR_DELAY_TURNAROUND);
      //  state = TRAVEL;
      //  ignoreTurns = true;
      //  break;
      //case END:
      //  Motor_Disable();
      //  break;
    };
  };
};