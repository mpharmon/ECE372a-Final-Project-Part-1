/*  
 *  Class: ECE372a
 * 
 *  Group: 209
 * 
 *  Lab: Final Lab Part 1
 * 
 *  File: IRSensor.c
 * 
 *  Member: Michael Harmon (Software)
 *          Abdul Rana (Hardware)
 *          James Beulke (System Integration)
 *          Ali Hassan (Quality Control)
 */

#include "IRSensor.h"

void IRSensor_Init(){
  AD1CON1bits.FORM = 0;   // Unsigned Integer Output
  AD1CON1bits.SSRC = 7;   // Auto Conversion
  AD1CON1bits.ASAM = 1;   // Enable Auto Sample
  
  AD1CON2bits.VCFG = 0;   // Voltage Reference (Vdd & Vss)
  AD1CON2bits.CSCNA = 1;  // Enable Scanning of Inputs (Needed Since We are Using Multiple ANx's)
  AD1CON2bits.SMPI = 3;   // Interrupt after 4 Sample and Converts
  AD1CON2bits.BUFM = 1;   // Use a 2x8 Byte Buffer
  AD1CON2bits.ALTS = 0;   // Only Use Mux A
  
  AD1CON3bits.ADRC = 0;   // Clock Derived From Peripheral Bus
  AD1CON3bits.SAMC = 1;   // Sample Every 1 ADC Clock Cycle
  AD1CON3bits.ADCS = 0;   // ADC Clock Set (2 * Peripheral Bus Clock)
  
  AD1CHSbits.CH0NA = 0;   // Negative as V_REFL
  
  AD1CSSL = 0x000f;       // Scan AN0 to AN3
  
  AD1CON1bits.ON = 1;     // Turn ADC On
};

bool IRSensor_CheckFront(int trigger){
  if(IRSensor_CheckLeftFront(trigger) && IRSensor_CheckRightFront(trigger) && IRSensor_CheckCenterLeft(trigger) && IRSensor_CheckCenterRight(trigger))return true;
  else return false;
}

bool IRSensor_CheckCenter(int trigger){
  if(IRSensor_CheckCenterRight(trigger) && IRSensor_CheckCenterLeft(trigger))return true;
  else return false;
}

unsigned int IRSensor_GetLeftFront(){
  if(AD1CON2bits.BUFS)return ADC1BUF0;
  else return ADC1BUF8;
}

bool IRSensor_CheckLeftFront(int trigger){
  if(IRSensor_GetLeftFront() > trigger)return true;
  else return false;
};

unsigned int IRSensor_GetRightFront(){
  if(AD1CON2bits.BUFS)return ADC1BUF3;
  else return ADC1BUFB;
 }

bool IRSensor_CheckRightFront(int trigger){
  if(IRSensor_GetRightFront() > trigger)return true;
  else return false;
};

unsigned int IRSensor_GetCenterRight(){
  if(AD1CON2bits.BUFS)return ADC1BUF2;
  else return ADC1BUFA;
}

bool IRSensor_CheckCenterRight(int trigger){
  if(IRSensor_GetCenterRight() > trigger)return true;
  else return false;
};

unsigned int IRSensor_GetCenterLeft(){
  if(AD1CON2bits.BUFS)return ADC1BUF1;
  else return ADC1BUF9;
}

bool IRSensor_CheckCenterLeft(int trigger){
  if(IRSensor_GetCenterLeft() > trigger)return true;
  else return false;
};