/*
 * This file contain the ISRs to blink leds
 *
*/
#include "F2837xD_device.h"     // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"   // F2837xD Examples Include File
#include <DSP_setup_gpios.h>
#include <DSP_setup_isr.h>

// blink the F28379D board leds
__interrupt void cpu_timer0_isr(void){

   EALLOW;

   CpuTimer0.InterruptCount++;

#ifdef CPU1
   GPIO_TogglePin(31);
#endif

#ifdef CPU2
   GPIO_TogglePin(34);
#endif

   EDIS;

   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}
