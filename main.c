/*
 PIC12F675 PWM LED generator

 Driven by 10kHz interrupt

 Author: Stanislav Petr <glux@glux.org>

          |----o----|
     +5V -|         |- GND
          |         |
   Start -|GP5   GP0|- GP0
          |         |
   Volt. -|AN3   GP1|- GP1
          |         |
         -|GP3   GP2|- PWM OUT
          |---------|

*/

#include <htc.h>
#define _XTAL_FREQ   4000000    
__CONFIG(FOSC_INTRCIO & WDTE_OFF & PWRTE_ON & MCLRE_OFF & BOREN_ON & CP_OFF & CPD_OFF);

unsigned char PWMCycle = 0;

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
  TMR0 = 0xE7; // preset for timer register (156)

  // Interrupt Registers
  INTCON = 0; // Clear the interrpt control register
  T0IE = 1;   // Timer0 Overflow Interrupt Enable bit
  T0IF = 0;   // Clear timer 0 interrupt flag
  GIE = 1;    // Global interrupt enable
}

void interrupt ISR(void) {
  if(T0IF) { // If Timer0 interrupt
    PWMCycle++;
    if(PWMCycle==10 && GP5==0) {
      GP2 = 1;
      PWMCycle=0;
    } else {
      GP2 = 0;
    }
    TMR0 = 0xE7; // preset for next interrupt run
    T0IF = 0; // Clear the interrupt
  }
}

void main() {
  OPTION_REG = 0b00000000;
  TRISIO = 0b00111011; // GP2 is output, all others are input
//  ANSEL = 0b00011000; // Set port AN3 as analog I/O, TAD = 2uS (@4MHz clock)
//  ADCON0 = 0b00000000; // Turn on the A/D Converter for AN3
  CMCON = 0x07; // Shut off the Comparator
  VRCON = 0x00; // Shut off the Voltage Reference
  GPIO = 0b00000000; // Make all pins 0
  WPU = 0b00100000; // Enable pull up on GP5
  IOCB = 0;
  InitPWM(); // Initialize PWM

  while (1) {
  }
}