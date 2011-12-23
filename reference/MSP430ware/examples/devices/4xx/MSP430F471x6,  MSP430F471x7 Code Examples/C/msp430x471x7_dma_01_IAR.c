//******************************************************************************
//   MSP430x471xx Demo - DMA0, Repeated Burst to-from RAM, Software Trigger
//
//   Description: A 16 word block from 1400-141fh is transfered to 1420h-143fh
//   using DMA0 in a burst block using software DMAREQ trigger.
//   After each transfer, source, destination and DMA size are
//   reset to inital software setting because DMA transfer mode 5 is used.
//   P5.1 is toggled during DMA transfer only for demonstration purposes.
//   ** RAM location 0x1400 - 0x143f used - make sure no compiler conflict **
//   ACLK = 32kHz, MCLK = SMCLK = default DCO 1.045MHz
//
//                MSP430x471xx
//             -----------------
//         /|\|              XIN|-
//          | |                 | 32kHz
//          --|RST          XOUT|-
//            |                 |
//            |             P5.1|-->LED
//
//   K. Venkat
//   Texas Instruments Inc.
//   May 2009
//   Built with CCE Version: 3.2.0 and IAR Embedded Workbench Version: 4.11B
//******************************************************************************
#include "msp430x471x7.h"

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  P5DIR |= BIT1;                            // P5.1  output
  DMA0SA = 0x1400;                          // Start block address
  DMA0DA = 0x1420;                          // Destination block address
  DMA0SZ = 0x0010;                          // Block size
  DMA0CTL = DMADT_5 + DMASRCINCR_3 + DMADSTINCR_3; // Rpt, inc
  DMA0CTL |= DMAEN;                         // Enable DMA0

  while(1)
  {
    P5OUT |= BIT1;                          // P5.1 = 1, LED on
    DMA0CTL |= DMAREQ;                      // Trigger block transfer
    P5OUT &= ~BIT1;                         // P5.1 = 0, LED off
  }
}
