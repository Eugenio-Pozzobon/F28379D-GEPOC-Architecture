/*
 * The epwm triggers the sampling process.
*/

#include <DSP_setup_epwm.h>
#include "F2837xD_device.h"     // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"   // F2837xD Examples Include File


void EPWM_initEpwm(void){

    EALLOW;

    // Turn off the EPWM clock
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;

    // Disable SOC on A group
    EPwm1Regs.ETSEL.bit.SOCAEN = 0;

    // Select SOC on up-count mode
    EPwm1Regs.ETSEL.bit.SOCASEL = 4;

    // Generate pulse on 1st event
    EPwm1Regs.ETPS.bit.SOCAPRD = 1;

    // Set compare A value
    EPwm1Regs.CMPA.bit.CMPA = PWM1_DUTY_CYCLE;

    // Set period (counts)
    EPwm1Regs.TBPRD = PWM1_PERIOD;

    // freeze counter
    EPwm1Regs.TBCTL.bit.CTRMODE = 3;

    // enable SOCA
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;
    EPwm1Regs.TBCTL.bit.HSPCLKDIV = 0;

    // Clear PWM1A on Zero
    EPwm1Regs.AQCTLA.bit.CAU = AQ_SET;
    EPwm1Regs.AQCTLA.bit.PRD = AQ_CLEAR;

    EDIS;

}
