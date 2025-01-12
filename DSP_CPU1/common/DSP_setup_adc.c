/*
 * This file setups the ADC triggers by EOC and the sampling process within the 24 channels.
 *
*/

#include <DSP_setup_adc.h>
#include "F2837xD_device.h"     // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"   // F2837xD Examples Include File

// Configure all ADCs for both CPUs
void ADC_initAdcs(void){

    EALLOW;
    
    // see Table 11-12. ADC Timings in 12-bit Mode in the technical reference manual, pg 1581
    AdcaRegs.ADCCTL2.bit.PRESCALE = ADC_PRESCALE; 
    AdcbRegs.ADCCTL2.bit.PRESCALE = ADC_PRESCALE; 
    AdccRegs.ADCCTL2.bit.PRESCALE = ADC_PRESCALE;
    AdcdRegs.ADCCTL2.bit.PRESCALE = ADC_PRESCALE;

    AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

    // Set pulse positions to late
    // 0 -> ISR is triggered in the end of SH Window
    // 1 -> ISR is triggered in the end of SH + Tlat Window
    AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 0;
    AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 0;
    AdccRegs.ADCCTL1.bit.INTPULSEPOS = 0;
    AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 0;

    // power up the ADCs
    AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;
    DELAY_US(1000);

    // Select the channels to convert
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = 0;  // SOC0 will convert pin A0
    AdcaRegs.ADCSOC1CTL.bit.CHSEL = 1;  // SOC1 will convert pin A1
    AdcaRegs.ADCSOC2CTL.bit.CHSEL = 2;  // SOC1 will convert pin A2
    AdcaRegs.ADCSOC3CTL.bit.CHSEL = 3;  // SOC1 will convert pin A3
    AdcaRegs.ADCSOC4CTL.bit.CHSEL = 4;  // SOC1 will convert pin A4
    AdcaRegs.ADCSOC5CTL.bit.CHSEL = 5;  // SOC1 will convert pin A5

    AdcbRegs.ADCSOC0CTL.bit.CHSEL = 0;
    AdcbRegs.ADCSOC1CTL.bit.CHSEL = 1;
    AdcbRegs.ADCSOC2CTL.bit.CHSEL = 2;
    AdcbRegs.ADCSOC3CTL.bit.CHSEL = 3;
    AdcbRegs.ADCSOC4CTL.bit.CHSEL = 4;
    AdcbRegs.ADCSOC5CTL.bit.CHSEL = 5;

    AdccRegs.ADCSOC14CTL.bit.CHSEL = 14;
    AdccRegs.ADCSOC15CTL.bit.CHSEL = 15;
    AdccRegs.ADCSOC2CTL.bit.CHSEL = 2;
    AdccRegs.ADCSOC3CTL.bit.CHSEL = 3;
    AdccRegs.ADCSOC4CTL.bit.CHSEL = 4;
    AdccRegs.ADCSOC5CTL.bit.CHSEL = 5;

    AdcdRegs.ADCSOC0CTL.bit.CHSEL = 0;
    AdcdRegs.ADCSOC1CTL.bit.CHSEL = 1;
    AdcdRegs.ADCSOC2CTL.bit.CHSEL = 2;
    AdcdRegs.ADCSOC3CTL.bit.CHSEL = 3;
    AdcdRegs.ADCSOC4CTL.bit.CHSEL = 4;
    AdcdRegs.ADCSOC5CTL.bit.CHSEL = 5;


    // sample window is ADC_ACQPS + 1 SYSCLK cycles
    // see pg 1562 in the technical reference manual
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = ADC_ACQPS;
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = ADC_ACQPS;
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = ADC_ACQPS;
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = ADC_ACQPS;
    AdcaRegs.ADCSOC4CTL.bit.ACQPS = ADC_ACQPS;
    AdcaRegs.ADCSOC5CTL.bit.ACQPS = ADC_ACQPS;

    AdcbRegs.ADCSOC0CTL.bit.ACQPS = ADC_ACQPS;
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = ADC_ACQPS;
    AdcbRegs.ADCSOC2CTL.bit.ACQPS = ADC_ACQPS;
    AdcbRegs.ADCSOC3CTL.bit.ACQPS = ADC_ACQPS;
    AdcbRegs.ADCSOC4CTL.bit.ACQPS = ADC_ACQPS;
    AdcbRegs.ADCSOC5CTL.bit.ACQPS = ADC_ACQPS;

    AdccRegs.ADCSOC14CTL.bit.ACQPS = ADC_ACQPS;
    AdccRegs.ADCSOC15CTL.bit.ACQPS = ADC_ACQPS;
    AdccRegs.ADCSOC2CTL.bit.ACQPS = ADC_ACQPS;
    AdccRegs.ADCSOC3CTL.bit.ACQPS = ADC_ACQPS;
    AdccRegs.ADCSOC4CTL.bit.ACQPS = ADC_ACQPS;
    AdccRegs.ADCSOC5CTL.bit.ACQPS = ADC_ACQPS;

    AdcdRegs.ADCSOC0CTL.bit.ACQPS = ADC_ACQPS;
    AdcdRegs.ADCSOC1CTL.bit.ACQPS = ADC_ACQPS;
    AdcdRegs.ADCSOC2CTL.bit.ACQPS = ADC_ACQPS;
    AdcdRegs.ADCSOC3CTL.bit.ACQPS = ADC_ACQPS;
    AdcdRegs.ADCSOC4CTL.bit.ACQPS = ADC_ACQPS;
    AdcdRegs.ADCSOC5CTL.bit.ACQPS = ADC_ACQPS;

    // set the trigger source to be the EPWM 1A
    // see Table 11-32. ADCSOC0CTL Register Field Descriptions in the technical reference manual, pg 1637
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcaRegs.ADCSOC5CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;

    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcbRegs.ADCSOC3CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcbRegs.ADCSOC4CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcbRegs.ADCSOC5CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;

    AdccRegs.ADCSOC14CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdccRegs.ADCSOC15CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdccRegs.ADCSOC3CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdccRegs.ADCSOC4CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdccRegs.ADCSOC5CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;

    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcdRegs.ADCSOC2CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcdRegs.ADCSOC3CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcdRegs.ADCSOC4CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;
    AdcdRegs.ADCSOC5CTL.bit.TRIGSEL = ADC_TRIG_SOURCE;


    // ADCINTSEL1N2 NOT in Continuous Mode:
    // interrupts are generated until ADCINT1 flag is cleared by user.
    AdcaRegs.ADCINTSEL1N2.bit.INT1CONT = 0;
//    AdcbRegs.ADCINTSEL1N2.bit.INT1CONT = 0;
//    AdccRegs.ADCINTSEL1N2.bit.INT1CONT = 0;
//    AdcdRegs.ADCINTSEL1N2.bit.INT1CONT = 0;

    AdcaRegs.ADCINTSEL1N2.bit.INT2CONT = 0;
//    AdcbRegs.ADCINTSEL1N2.bit.INT2CONT = 0;
//    AdccRegs.ADCINTSEL1N2.bit.INT2CONT = 0;
//    AdcdRegs.ADCINTSEL1N2.bit.INT2CONT = 0;

    // enable interrupt of the ADCs
    AdcaRegs.ADCINTSEL1N2.bit.INT1E = 1;
//    AdcbRegs.ADCINTSEL1N2.bit.INT1E = 1;
//    AdccRegs.ADCINTSEL1N2.bit.INT1E = 1;
//    AdcdRegs.ADCINTSEL1N2.bit.INT1E = 1;

    // disable ADCINTSEL3N4
    AdcaRegs.ADCINTSEL3N4.all = 0;
//    AdcbRegs.ADCINTSEL3N4.all = 0;
//    AdccRegs.ADCINTSEL3N4.all = 0;
//    AdcdRegs.ADCINTSEL3N4.all = 0;

    // Not used
    // SOC Priority pg. 1340 do Technical Reference Manual
//    AdcaRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 6;
//    AdcbRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 6;
//    AdccRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 6;
//    AdcdRegs.ADCSOCPRICTL.bit.SOCPRIORITY = 6;

    // end of SOC5 will set INT1 flag
    AdcaRegs.ADCINTSEL1N2.bit.INT1SEL = 5;
//    AdcbRegs.ADCINTSEL1N2.bit.INT1SEL = 5;
//    AdccRegs.ADCINTSEL1N2.bit.INT1SEL = 5;
//    AdcdRegs.ADCINTSEL1N2.bit.INT1SEL = 5;

    // make sure INT1 flag is cleared
    // only ADCA trigger the CLA
    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;

    EDIS;
}

