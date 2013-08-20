/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#if defined(__XC)
    #include <xc.h>         /* XC8 General Include File */
#elif defined(HI_TECH_C)
    #include <htc.h>        /* HiTech General Include File */
#endif

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

/* Baseline devices don't have interrupts. Unfortunately the baseline detection 
 * macro is named _PIC12 */

#ifndef _PIC12

unsigned char PWMCycle = 0;
unsigned int Battery_Value;
bool Battery_OK;

void interrupt isr(void)
{
  if(T0IF) { // If Timer0 interrupt
    PWMCycle++;
    if(PWMCycle>=10 && GP5==1 && Battery_Value>512) {
      GP2 = 1;
      PWMCycle=0;
    } else {
      GP2 = 0;
    }
    TMR0 = 0xE7; // preset for next interrupt run
    T0IF = 0; // Clear the interrupt
  }
}
#endif


