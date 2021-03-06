;******************************************************************************
;   MSP-FET430P430 Demo - DAC12_0, Output Voltage Ramp on VeREF+
;
;   Description: Using DAC12_0 and 2.5V ADC12REF reference with a gain of 1,
;   output positive ramp on VeREF+. Normal mode is LPM0 with CPU off. WDT used
;   to provide 61us interrupt used to wake up the CPU and update the DAC
;   with software. Uses internal 2.5V Vref. This code example shows how to set
;   the DAC output to the second DAC12_0 output on the FG devices.
;   ACLK = n/a, MCLK = SMCLK = default DCO
;
;                MSP430FG439
;             -----------------
;         /|\|              XIN|-
;          | |                 |
;          --|RST          XOUT|-
;            |                 |
;            |      VeREF+/DAC0|--> Ramp_positive
;            |                 |
;
;   M. Buccini / M. Mitchell
;   Texas Instruments Inc.
;   May 2005
;   Built with Code Composer Essentials Version: 1.0
;******************************************************************************
 .cdecls C,LIST,  "msp430xG43x.h"
;------------------------------------------------------------------------------
            .text                  ; Progam Start
;------------------------------------------------------------------------------
RESET       mov.w   #0A00h,SP               ; Initialize stack pointer
SetupWDT    mov.w   #WDT_MDLY_0_064,&WDTCTL   ; WDT 61us interval timer
            bis.b   #WDTIE,&IE1             ; Enable WDT interrupt
SetupADC12  mov.w   #REF2_5V+REFON,&ADC12CTL0 ; Internal 2.5V ref on
SetupDAC120 mov.w   #DAC12IR+DAC12AMP_5+DAC12ENC+DAC12OPS,&DAC12_0CTL
                                            ; Int ref gain 1
                                            ;						
Mainloop    bis.w   #CPUOFF+GIE,SR          ; Enter LPM0, interrupts enabled
            inc.w   &DAC12_0DAT             ; Positive ramp
            and.w   #0FFFh,&DAC12_0DAT      ;
            jmp     Mainloop                ;
                                            ;
;------------------------------------------------------------------------------
WDT_ISR  ;  Exit LPM0 on reti
;------------------------------------------------------------------------------
            bic.w   #CPUOFF,0(SP)           ; TOS = clear LPM0
            reti                            ;
                                            ;
;-----------------------------------------------------------------------------
;           Interrupt Vectors
;-----------------------------------------------------------------------------
            .sect   ".reset"                ; RESET Vector
            .short  RESET                   ;
            .sect   ".int10"                ; WDT Vector
            .short  WDT_ISR                 ;
            .end
