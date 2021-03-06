//******************************************************************************
//  MSP430FR57x Demo - Timer1_A3, Toggle P1.0, CCR0 Cont Mode ISR, 32KHz ACLK 
//
//  Description: Toggle P1.0 using software and TA1_0 ISR. Timer1_A is
//  configured for continuous mode, thus the timer overflows when TAR counts
//  to CCR0. In this example, CCR0 is loaded with 50000.
//  ACLK = TACLK = 32768Hz, MCLK = SMCLK  = default DCO = ~666KHz
//
//           MSP430FR5739
//         ---------------
//     /|\|               |
//      | |               |
//      --|RST            |
//        |               |
//        |           P1.0|-->LED
//
//   Priya Thanigai
//   Texas Instruments Inc.
//   August 2010
//   Built with IAR Embedded Workbench Version: 5.10 & Code Composer Studio V4.0
//******************************************************************************

#include "msp430fr5739.h"


void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT  
  // XT1 Setup   
  PJSEL0 |= BIT4 + BIT5;   
  CSCTL0_H = 0xA5;
  CSCTL2 = SELA_0 + SELS_3 + SELM_3;        // set ACLK = XT1; MCLK = DCO
  CSCTL3 = DIVA_0 + DIVS_0 + DIVM_0;        // set all dividers 
  CSCTL4 |= XT1DRIVE_0; 
  CSCTL4 &= ~XT1OFF;
  
  do
  {
    CSCTL5 &= ~XT1OFFG;
                                            // Clear XT1 fault flag
    SFRIFG1 &= ~OFIFG; 
  }while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
  
 
  P1DIR |= BIT0;                            // LED interrupt
  P1OUT |= BIT0;
  
  TA1CCTL0 = CCIE;                          // TACCR0 interrupt enabled
  TA1CCR0 = 50000;
  TA1CTL = TASSEL_1 + MC_2;                 // ACLK, continuous mode

  __bis_SR_register(LPM3_bits + GIE);       // Enter LPM3 w/ interrupt
}

// Timer A1 interrupt service routine
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
{
  P1OUT ^= BIT0;
  TA0CCR0 += 50000;                         // Add Offset to TACCR0
}