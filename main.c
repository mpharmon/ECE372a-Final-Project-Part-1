#include <xc.h>

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

}
