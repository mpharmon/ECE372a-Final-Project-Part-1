#include <xc.h>
#include "IRSensor.h"
#include "../Motor.X/Motor.h"
#include "../delay.X/delay.h"

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
  while(1){
    switch(state){
      case START:
        Motor_Set1Forward();
        Motor_Set1Forward();
        Motor_Set1DutyCycle(0.0);
        Motor_Set1DutyCycle(0.0);
        if(IRSensor_CheckLeftFront() && IRSensor_CheckRightFront() && IRSensor_CheckCenterLeft() && IRSensor_CheckCenterRight()){
          state = TRAVEL;
        };
        break;
      case TURN_LEFT:
        delayMs(300);
        while(!IRSensor_CheckRightFront()){
          Motor_Set1Backward();
          Motor_Set1DutyCycle(25.0);
          Motor_Set2Forward();
          Motor_Set2DutyCycle(50.0);
        };
        state = TRAVEL;
        break;
      case TURN_RIGHT:
        delayMs(300);
        while(!IRSensor_CheckLeftFront()){
          Motor_Set1Backward();
          Motor_Set1DutyCycle(25.0);
          Motor_Set2Forward();
          Motor_Set2DutyCycle(50.0);
        }
        state = TRAVEL;
        break;
      case TRAVEL:
        Motor_Set1Forward();
        Motor_Set2Forward();
        Motor_Set1DutyCycle(50.0);
        Motor_Set2DutyCycle(50.0);
        if(!IRSensor_CheckCenterLeft()){state = ADJUST_RIGHT;break;};
        if(!IRSensor_CheckCenterRight()){state = ADJUST_LEFT;break;};
        if(!IRSensor_CheckLeftFront() && IRSensor_CheckRightFront){
          lineCount++;
          if(lineCount == 3){
            Motor_Set1Forward();
            Motor_Set2Backward();
          };
        };
        if(IRSensor_CheckLeftFront()){};
        if(IRSensor_CheckRightFront()){};
        break;
      case ADJUST_RIGHT:
        state = TRAVEL;
        break;
      case ADJUST_LEFT:
        state = TRAVEL;
        break;
    };
  };
}
