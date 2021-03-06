//******************************************************************************
//  MSP430F552x Demo - USCI_B0 I2C Slave TX single bytes to MSP430 Master
//
//  Description: This demo connects two MSP430's via the I2C bus. The master
//  reads from the slave. This is the SLAVE code. The TX data begins at 0
//  and is incremented each time it is sent. An incoming start condition
//  is used as a trigger to increment the outgoing data. The master checks the
//  data it receives for validity. If it is incorrect, it stops communicating
//  and the P1.0 LED will stay on. The USCI_B0 TX interrupt is used to know
//  when to TX.
//  ACLK = n/a, MCLK = SMCLK = default DCO = ~1.045MHz
//
//                                /|\  /|\
//               MSP430F5529      10k  10k     MSP430F5529
//                   slave         |    |        master
//             -----------------   |    |   -----------------
//           -|XIN  P3.0/UCB0SDA|<-|----+->|P3.0/UCB0SDA  XIN|-
//            |                 |  |       |                 |
//           -|XOUT             |  |       |             XOUT|-
//            |     P3.1/UCB0SCL|<-+------>|P3.1/UCB0SCL     |
//            |                 |          |                 |
//
//   Bhargavi Nisarga
//   Texas Instruments Inc.
//   April 2009
//   Built with CCSv4 and IAR Embedded Workbench Version: 4.21
//******************************************************************************

#include <msp430f5529.h>

unsigned char TXData;
unsigned char i=0;

void main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P3SEL |= 0x03;                            // Assign I2C pins to USCI_B0
  UCB0CTL1 |= UCSWRST;                      // Enable SW reset
  UCB0CTL0 = UCMODE_3 + UCSYNC;             // I2C Slave, synchronous mode
  UCB0I2COA = 0x48;                         // Own Address is 048h
  UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation
  UCB0IE |= UCTXIE + UCSTTIE + UCSTPIE;     // Enable TX interrupt
                                            // Enable Start condition interrupt
  TXData = 0;                               // Used to hold TX data

  __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupts
  __no_operation();                         // For debugger
}

// USCI_B0 State ISR
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
{
  switch(__even_in_range(UCB0IV,12))
  {
  case  0: break;                           // Vector  0: No interrupts
  case  2: break;                           // Vector  2: ALIFG
  case  4: break;                           // Vector  4: NACKIFG
  case  6:                                  // Vector  6: STTIFG
     UCB0IFG &= ~UCSTTIFG;                  // Clear start condition int flag
     break;
  case  8:                                  // Vector  8: STPIFG
    TXData++;                               // Increment TXData
    UCB0IFG &= ~UCSTPIFG;                   // Clear stop condition int flag
    break;
  case 10: break;                           // Vector 10: RXIFG  
  case 12:                                  // Vector 12: TXIFG
    UCB0TXBUF = TXData;                     // TX data
    break;
  default: break; 
  }
}


