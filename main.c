#include <xc.h>
#include "IRSensor.h"
#include "../Motor.X/Motor.h"
#include "../delay.X/delay.h"

#define STANDARD_DUTY_CYCLE 50.0

typedef enum STATE{
  START,
  TURN_LEFT,
  TURN_RIGHT,
  TRAVEL,
  ADJUST_RIGHT,
  ADJUST_LEFT,
  END
}STATE;

volatile STATE state = START;

void main(void){
  unsigned short int lineCount = 0;
  // Initialize Motor(s))
  Motor_Init();
  // Initialize IRSensor(s))
  IRSensor_Init();
  while(1){
    switch(state){
      case START:
        Motor_Set1Forward();
        Motor_Set1Forward();
        Motor_Set1DutyCycle(0.0);
        Motor_Set1DutyCycle(0.0);
        if(IRSensor_CheckFront()){
          state = TRAVEL;
        };
        break;
      case TURN_LEFT:
        delayMs(300);
        while(!IRSensor_CheckCenterLeft()){
          Motor_Set1Backward();
          Motor_Set1DutyCycle(0.5 * STANDARD_DUTY_CYCLE);
          Motor_Set2Forward();
          Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
        };
        state = TRAVEL;
        break;
      case TURN_RIGHT:
        delayMs(300);
        while(!IRSensor_CheckCenterRight()){
          Motor_Set2Backward();
          Motor_Set2DutyCycle(0.5 * STANDARD_DUTY_CYCLE);
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
          if(lineCount == 3){
            Motor_Set1Forward();
            Motor_Set2Backward();
          };
        };
        if(IRSensor_CheckLeftFront()){state = TURN_LEFT;break;};
        if(IRSensor_CheckRightFront()){state = TURN_RIGHT;break;};
        break;
      case ADJUST_RIGHT:
        Motor_Set1DutyCycle(1.1 * STANDARD_DUTY_CYCLE);
        delayMs(300);
        state = TRAVEL;
        break;
      case ADJUST_LEFT:
        Motor_Set2DutyCycle(1.1 * STANDARD_DUTY_CYCLE);
        state = TRAVEL;
        break;
    };
  };
}
