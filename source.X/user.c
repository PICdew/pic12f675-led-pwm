/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

#include "user.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/

void InitApp(void)
{
  // Initialize ports
  OPTION_REG = 0b00000000;
  CMCON = 0b00000111; // Shut off the Comparator
  VRCON = 0b00000000; // Shut off the Voltage Reference
  GPIO = 0b00000000; // Make all pins 0
  ANSEL = 0b01111000; // AN3 analog input, AD clock from internal oscillator
  TRISIO = 0b00111011; // GP2 is output, all others are input
  WPU = 0b00100000; // Enable pull up on GP5

  // ADCON0 =
  IOCB = 0;


  // Initialize Timer 0
  T0CS = 0;  // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
  T0SE = 0;  // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
  PSA = 1;   // bit 3  Prescaler Assignment bit...1 = Prescaler is assigned to the WDT
  PS2 = 0;   // bits 2-0  PS2:PS0: Prescaler Rate Select bits
  PS1 = 0;
  PS0 = 0;
  TMR0 = 0x00;

  // Enable interrupts
  INTCON = 0b10100000; // Clear the interrpt control register
}

//Function ADC_Read
unsigned int ADC_Read(unsigned char channel)
{
  unsigned char ADCL=0;
  unsigned char ADCH=0;
  unsigned int Value = 0;

  ADCON0bits.CHS = channel;
  ADCON0bits.ADFM = 1;
  _delay(200);
  ADCON0bits.ADON=1;  //switch on the adc module
  _delay(200);
  ADCON0bits.GO_nDONE=1;  //Start conversion
  while(ADCON0bits.GO_nDONE); //wait for the conversion to finish
  ADCON0bits.ADON=0;  //switch off adc
  ADCH=ADRESH;
  ADCL=ADRESL;
  Value = (int)(ADCH*256) + ADCL;
  return Value;
}