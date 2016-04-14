#include <xc.h>
#include "IRSensor.h"
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
  // Initialize Motor(s))
  Motor_Init();
  // Initialize IRSensor(s))
  IRSensor_Init();
  // LCD Initialize
  LCD_Init();
  while(1){
    LCD_MoveCursor(1,1);
    LCD_PrintString("LF:%3.1fRF:%3.1f",IR_FRONTLEFT_BUF/1023.0,IR_FRONTRIGHT_BUF/1023.0);
    LCD_MoveCursor(1,2);
    LCD_PrintString("CL:%3.1fCR:%3.1f",IR_CENTERLEFT_BUF/1023.0,IR_CENTERRIGHT_BUF/1023.0);
    switch(state){
      case START:
        Motor_Set1Forward();
        Motor_Set1Forward();
        Motor_Set1DutyCycle(0.0);
        Motor_Set1DutyCycle(0.0);
        if(IRSensor_CheckCenter()){
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
            state = TURN_AROUND;
          }else if(lineCount == 4){
            state = END;
          }
        };
        if(IRSensor_CheckLeftFront()){state = TURN_LEFT;break;};
        if(IRSensor_CheckRightFront()){state = TURN_RIGHT;break;};
        break;
      case ADJUST_RIGHT:
        Motor_Set1DutyCycle(1.1 * STANDARD_DUTY_CYCLE);
        delayMs(200);
        Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE);
        Motor_Set2DutyCycle(1.1 * STANDARD_DUTY_CYCLE);
        delayMs(200);
        Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
        state = TRAVEL;
        break;
      case ADJUST_LEFT:
        Motor_Set2DutyCycle(1.1 * STANDARD_DUTY_CYCLE);
        delayMs(200);
        Motor_Set2DutyCycle(STANDARD_DUTY_CYCLE);
        Motor_Set1DutyCycle(1.1 * STANDARD_DUTY_CYCLE);
        delayMs(200);
        Motor_Set1DutyCycle(STANDARD_DUTY_CYCLE);
        state = TRAVEL;
        break;
      case TURN_AROUND:
        while(IRSensor_CheckCenterLeft()){
          Motor_Set1Backward();
          Motor_Set2Forward();
        };
        delayMs(250);
        state = TRAVEL;
        break;
      case END:
        Motor_Disable();
        break;
    };
  };
}
