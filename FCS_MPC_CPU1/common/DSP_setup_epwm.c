/*
 * The epwm triggers the sampling process.
*/


#include <mpc.h>
#include "F2837xD_device.h"     // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"   // F2837xD Examples Include File
//#include <DSP_setup_epwm.h>

#define TB_SOFTWARE 0
#define DEADBAND    200


void EPWM_initEpwm(void){

    EALLOW;


    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;

    #ifdef CPU1

    // EPwm1Regs
    EPwm1Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm1Regs.TBPRD = PWM_PERIOD;       // Set timer period (max count)
    EPwm1Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
    EPwm1Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm1Regs.TBCTR = 0x0000;                  // Clear counter
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;   // Clock ratio to SYSCLKOUT
    EPwm1Regs.TBCTL.bit.CLKDIV = 0; // set epwm clock to 50 MHz

    EPwm1Regs.ETSEL.bit.SOCAEN = 1;                //Enable the ADC Start of Conversion A (EPWMxSOCA) Pulse
    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;  // Enable event (SOCA) time-base counter equal to zero or period
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;          // ePWM ADC Start-of-Conversion on 1st Event

    // EPwm2Regs
    EPwm2Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm2Regs.TBPRD = PWM_PERIOD;       // Set timer period
    EPwm2Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
    EPwm2Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm2Regs.TBCTR = 0x0000;                  // Clear counter
    EPwm2Regs.TBCTL.bit.HSPCLKDIV = 0;   // Clock ratio to SYSCLKOUT
    EPwm2Regs.TBCTL.bit.CLKDIV = 0;

    // EPwm3Regs
    EPwm3Regs.TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN; // Count up
    EPwm3Regs.TBPRD = PWM_PERIOD;       // Set timer period
    EPwm3Regs.TBCTL.bit.PHSEN = TB_DISABLE;    // Disable phase loading
    EPwm3Regs.TBPHS.bit.TBPHS = 0x0000;        // Phase is 0
    EPwm3Regs.TBCTR = 0x0000;                  // Clear counter
    EPwm3Regs.TBCTL.bit.HSPCLKDIV = 0;   // Clock ratio to SYSCLKOUT
    EPwm3Regs.TBCTL.bit.CLKDIV = 0;

#endif
    EDIS;
}
