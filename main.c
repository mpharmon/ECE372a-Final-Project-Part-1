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
#include "config.h"
#include "IRSensor.h"
#include <stdbool.h>
#include "../Motor.X/Motor.h"
#include "../delay.X/delay.h"
#include "../LCD.X/lcd.h"

typedef enum STATE{
  START,
  TURN_LEFT,
  TURN_RIGHT,
  TRAVEL,
  ADJUST_RIGHT,
  ADJUST_LEFT,
  TURN_AROUND,
  END
}STATE;

void main(void){
  STATE state = START;
  unsigned short int lineCount = 0;
  bool ignoreTurns = false;
  // Initialize Motor(s))
  Motor_Init();
  // Initialize IRSensor(s))
  IRSensor_Init();
  // LCD Initialize
  LCD_Init();
  while(1){
    LCD_MoveCursor(1,1);
    char stringToPrint[16];
    sprintf(stringToPrint,"LF:%3.1fRF:%3.1f",(IR_FRONTLEFT_BUF/1023.0),(IR_FRONTRIGHT_BUF/1023.0));
    LCD_PrintString(stringToPrint);
    LCD_MoveCursor(1,2);
    sprintf(stringToPrint,"CL:%3.1fCR:%3.1f",(IR_CENTERLEFT_BUF/1023.0),(IR_CENTERRIGHT_BUF/1023.0));
    LCD_PrintString(stringToPrint);
    switch(state){
      case START:
        Motor_Enable();
        if(IRSensor_CheckCenter()){
          state = TRAVEL;
        };
        break;
      case TURN_LEFT:
        delayMs(IR_DELAY_TURN);
        while(!IRSensor_CheckCenterLeft()){
          Motor_Set1Backward();
          Motor_Set1DutyCycle(IR_BIAS_TURN * STANDARD_DUTY_CYCLE);
          Motor_Set2Forward();
          Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
        };
        state = TRAVEL;
        break;
      case TURN_RIGHT:
        delayMs(IR_DELAY_TURN);
        while(!IRSensor_CheckCenterRight()){
          Motor_Set2Backward();
          Motor_Set2DutyCycle(IR_BIAS_TURN * STANDARD_DUTY_CYCLE);
          Motor_Set1Forward();
          Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE);
        }
        state = TRAVEL;
        break;
      case TRAVEL:
        Motor_Enable();
        Motor_Set1Forward();
        Motor_Set2Forward();
        Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE);
        Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
        if(!IRSensor_CheckCenterLeft()){state = ADJUST_RIGHT;break;};
        if(!IRSensor_CheckCenterRight()){state = ADJUST_LEFT;break;};
        if(IRSensor_CheckFront()){
          lineCount++;
          if(lineCount == 1){// Entering Extra Credit 'T' Intersection
            state = TURN_LEFT;
          }else if(lineCount == 2){// Exiting Extra Credit 'T' Intersection
            state = TURN_RIGHT;
          }else if(lineCount == 5){// End of First Time Through
            state = TURN_AROUND;
          };
        };
        if(!ignoreTurns && IRSensor_CheckLeftFront()){state = TURN_LEFT;break;};
        if(!ignoreTurns && IRSensor_CheckRightFront()){state = TURN_RIGHT;break;};
        break;
      case ADJUST_RIGHT:
        Motor_Set1DutyCycle(IR_BIAS_ADJUST * STANDARD_DUTY_CYCLE);
        delayMs(IR_DELAY_ADJUSTMENT);
        Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE);
        Motor_Set2DutyCycle(IR_BIAS_ADJUST * STANDARD_DUTY_CYCLE);
        delayMs(IR_DELAY_ADJUSTMENT);
        Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
        state = TRAVEL;
        break;
      case ADJUST_LEFT:
        Motor_Set2DutyCycle(IR_BIAS_ADJUST * STANDARD_DUTY_CYCLE);
        delayMs(IR_DELAY_ADJUSTMENT);
        Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
        Motor_Set1DutyCycle(IR_BIAS_ADJUST * STANDARD_DUTY_CYCLE);
        delayMs(IR_DELAY_ADJUSTMENT);
        Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE);
        state = TRAVEL;
        break;
      case TURN_AROUND:
        while(IRSensor_CheckCenterLeft()){
          Motor_Set1Backward();
          Motor_Set2Forward();
        };
        delayMs(IR_DELAY_TURNAROUND);
        state = TRAVEL;
        ignoreTurns = true;
        break;
      case END:
        Motor_Disable();
        break;
    };
  };
}
