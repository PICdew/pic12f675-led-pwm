/*
 PIC12F675 PWM LED generator

 Driven by 10kHz interrupt

 Author: Stanislav Petr <glux@glux.org>

*/

#include <htc.h>
#define _XTAL_FREQ   4000000    
__CONFIG(FOSC_INTRCIO & WDTE_OFF & PWRTE_ON & MCLRE_OFF & BOREN_ON & CP_OFF & CPD_OFF);

unsigned char INTCYCLE = 0;

void InitPWM(void) {
  // Timer0 Registers
  // Prescaler = 1
  // TMR0 Preset = 156
  // Freq = 10000.00 Hz,  Period = 0.000100 seconds
  T0CS = 0;  // bit 5  TMR0 Clock Source Select bit...0 = Internal Clock (CLKO) 1 = Transition on T0CKI pin
  T0SE = 0;  // bit 4 TMR0 Source Edge Select bit 0 = low/high 1 = high/low
  PSA = 1;   // bit 3  Prescaler Assignment bit...1 = Prescaler is assigned to the WDT
  PS2 = 0;   // bits 2-0  PS2:PS0: Prescaler Rate Select bits
  PS1 = 0;
  PS0 = 0;
  TMR0 = 156; // preset for timer register

  // Interrupt Registers
  INTCON = 0; // Clear the interrpt control register
  T0IE = 1;   // Timer0 Overflow Interrupt Enable bit
  T0IF = 0;   // Clear timer 0 interrupt flag
  GIE = 1;    // Global interrupt enable
}

void interrupt ISR(void) {
  if(T0IF) { // If Timer0 interrupt
    TMR0 = 156; // preset for next interrupt run
    INTCYCLE++;
    switch (INTCYCLE) {
      case 1:
        GPIO = 0b00000001;
        break;
      case 2:
        GPIO = 0b00000010;
        break;
      case 3:
        GPIO = 0b00000100;
        break;
      case 4:
        GPIO = 0b00001000;
        break;
      case 5:
        GPIO = 0b00010000;
        break;
      case 6:
        GPIO = 0b00100000;
        break;
      case 7:
      case 8:
      case 9:
        GPIO = 0b00000000;
        break;
      default:
        INTCYCLE=0;
    }
    T0IF = 0; // Clear the interrupt
  }
}

void main() {
  ANSEL = 0x00; // Set ports as digital I/O, not analog input
  ADCON0 = 0x00; // Shut off the A/D Converter
  CMCON = 0x07; // Shut off the Comparator
  VRCON = 0x00; // Shut off the Voltage Reference
  TRISIO = 0b00000000; // All GPIO are output
  GPIO = 0x00; // Make all pins 0

  InitPWM(); // Initialize PWM

  while (1) {
  }
}