#include "IRSensor.h"

void IRSensor_Init(){
  AD1CON1bits.FORM = 0;   // Unsigned Integer Output
  AD1CON1bits.SSRC = 7;   // Auto Converstion
  AD1CON1bits.ASAM = 1;   // Enable Auto Sample
  AD1CON2bits.VCFG = 0;   // Voltage Refrence (Vdd & Vss)
  AD1CON2bits.CSCNA = 1;  // Enable Scaning of Inputs (Needed Since We are Using Multiple)
  AD1CON2bits.SMPI = 15;  // Interrupt after 16 Sample and Converts
  AD1CON2bits.BUFM = 0;   // Use a Single 16 Byte Buffer
  AD1CON2bits.ALTS = 0;   // Only Use Mux A
  AD1CON3bits.ADRC = 0;   // Clock Derived From Peripheral Bus
  AD1CON3bits.SAMC = 1;   // Sample Every ADC Clock Cycle
  AD1CON3bits.ADCS = 0;   // ADC Clock Set (2 * Peripheral Bus Clock)
  AD1CHSbits.CH0NA = 0;   // Negative as V_REFL
  AD1CHSbits.CH0SA = 0;   // Positive Input as AN0
  // TODO: ADJUST INPUTS SO ONLY USING THE NEEDED 4
  AD1CSSL = 0xf;          // Scan All ANx Inputs
  AD1CON1bits.ON = 1;     // Turn ADC On
};

unsigned short IRSensor_CheckFront(){
  if(IRSensor_CheckLeftFront() && IRSensor_CheckRightFront() && IRSensor_CheckCenterLeft() && IRSensor_CheckCenterRight())return 1;
  else return 0;
}

unsigned short IRSensor_CheckLeftFront(){
  if(IR_FRONTLEFT_BUF < IR_TRIGGER)return 1;
  else return 0;
};

unsigned short IRSensor_CheckRightFront(){
  if(IR_FRONTRIGHT_BUF < IR_TRIGGER)return 1;
  else return 0;
};

unsigned short IRSensor_CheckCenterRight(){
  if(IR_CENTERRIGHT_BUF < IR_TRIGGER)return 1;
  else return 0;
};

unsigned short IRSensor_CheckCenterLeft(){
  if(IR_CENTERLEFT_BUF < IR_TRIGGER)return 1;
  else return 0;
};
