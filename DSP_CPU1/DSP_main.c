/*
 * This file contain the routine target to the F28379D
 * There is compiler directives to separate the code target to CPU1 and CPU2.
 *
*/

#include "F28x_Project.h"
#include "F2837xD_Cla_defines.h"
#include "F2837xD_cla.h"
#include "stdint.h"

#include "math.h"
#include "fpu_types.h"

#include <DSP_setup_cla.h>
#include <DSP_setup_gpios.h>
#include "DSP_setup_isr.h"

#ifdef CPU1
#include <DSP_setup_adc.h>
#include <DSP_setup_epwm.h>
#endif // CPU1

#ifdef CPU1
#include "F2837xD_Ipc_drivers.h"
#endif // CPU1

// variables that are used to load data from FLASH
#ifdef _FLASH
extern uint16_t ramconsts_loadstart;
extern uint16_t ramconsts_loadsize;
extern uint16_t ramconsts_runstart;
#endif // _FLASH


#ifdef _FLASH
extern uint16_t isrfuncLoadStart;
extern uint16_t isrfuncLoadEnd;
extern uint16_t isrfuncRunStart;
extern uint16_t isrfuncLoadSize;
#endif // _FLASH


int globalResult;


void main(void){

    // initialize PLL, WatchDog, enable Peripheral Clocks
    InitSysCtrl();

#ifdef CPU1
    // Give memory access to GSRAM to CPU2
    while( !(MemCfgRegs.GSxMSEL.bit.MSEL_GS8 &
                MemCfgRegs.GSxMSEL.bit.MSEL_GS9 &
                MemCfgRegs.GSxMSEL.bit.MSEL_GS10 &
                MemCfgRegs.GSxMSEL.bit.MSEL_GS11 &
                MemCfgRegs.GSxMSEL.bit.MSEL_GS12 &
                MemCfgRegs.GSxMSEL.bit.MSEL_GS13 &
                MemCfgRegs.GSxMSEL.bit.MSEL_GS14 &
                MemCfgRegs.GSxMSEL.bit.MSEL_GS15)){


            EALLOW;
            MemCfgRegs.GSxMSEL.bit.MSEL_GS8 = 1;
            MemCfgRegs.GSxMSEL.bit.MSEL_GS9 = 1;
            MemCfgRegs.GSxMSEL.bit.MSEL_GS10 = 1;
            MemCfgRegs.GSxMSEL.bit.MSEL_GS11 = 1;
            MemCfgRegs.GSxMSEL.bit.MSEL_GS12 = 1;
            MemCfgRegs.GSxMSEL.bit.MSEL_GS13 = 1;
            MemCfgRegs.GSxMSEL.bit.MSEL_GS14 = 1;
            MemCfgRegs.GSxMSEL.bit.MSEL_GS15 = 1;
            EDIS;
    }
#endif

    // copy consts parameters from flash to ram
    // https://e2e.ti.com/support/microcontrollers/c2000-microcontrollers-group/c2000/f/c2000-microcontrollers-forum/1121371/tms320f28035-tms320f28035/4158636?tisearch=e2e-sitesearch&keymatch=_RamfuncsLoadSize%2520undefined#4158636
#ifdef _FLASH
#ifdef SAVE_LOG
    InitFlash();
#endif
    memcpy(&ramconsts_runstart, &ramconsts_loadstart, (Uint32)&ramconsts_loadsize);
#endif // _FLASH


#ifdef CPU1

    InitGpio();

    //  send boot command to allow the CPU02 begin execution
    //  https://e2e.ti.com/support/microcontrollers/c2000-microcontrollers-group/c2000/f/c2000-microcontrollers-forum/1211117/tms320f28379d-cpu2-boot-status-signalling-to-cpu1-ipcbootsts/4570806?tisearch=e2e-sitesearch&keymatch=_standalone#4570806
    //  https://e2e.ti.com/support/microcontrollers/c2000-microcontrollers-group/c2000/f/c2000-microcontrollers-forum/1104997/tms320f28379d-how-to-make-dual-cpu-run-from-flash?tisearch=e2e-sitesearch&keymatch=_standalone#
    #ifdef _STANDALONE
    #ifdef _FLASH
        IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_FLASH);
    #else
        IPCBootCPU2(C1C2_BROM_BOOTMODE_BOOT_FROM_RAM);
    #endif
    #endif


    DSP_setupGpios();

    // enable PWM1 and GPIOs
    CpuSysRegs.PCLKCR2.bit.EPWM1 = 1;
    InitEPwm1Gpio();



#endif // CPU1

    // Disable CPU interrupts
    DINT;

    // Initialize the PIE control registers to their default state.
    // The default state is all PIE interrupts disabled and flagsm are cleared.
    InitPieCtrl();

    // Disable CPU interrupts and clear all CPU interrupt flags:
    IER = 0x0000;
    IFR = 0x0000;

    // Initialize the PIE vector table with pointers to the shell ISRs
    // This will populate the entire table, even if the interrupt is not used.
    // This is useful for debug purposes.
    InitPieVectTable();

#ifdef CPU1
    // Configure the ADC to start sampling at the EPWM trigger
    ADC_initAdcs();

    // Configure EPWM to trigger ADCs every 4us
    EPWM_initEpwm();
#endif // CPU1

#ifdef _FLASH
    // Copy ISR routine to a specified RAM location to determine the size
    memcpy(&isrfuncRunStart, &isrfuncLoadStart, (uint32_t)&isrfuncLoadSize);
#endif // _FLASH


    EALLOW;
    PieVectTable.TIMER0_INT = &cpu_timer0_isr;

#if (defined(SELF_TEST) & defined(CPU1))
    PieVectTable.EPWM3_INT = &epwm3_int_isr;
    PieVectTable.XINT1_INT = &xint1_isr;
#endif
    EDIS;

    InitCpuTimers();   // initialize the Cpu Timers

    // Configure CPU-Timers:
    ConfigCpuTimer(&CpuTimer0, 200, 1000000); // 1s

    // To ensure precise timing, use write-only instructions to write
    CpuTimer0Regs.TCR.all = 0x4000;

    // PIE Channel Mapping, see pg 102 of the techinical reference manual
    IER |= M_INT1; // enable XINT1, timer0, and WAKEINT
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1; // enable timer0

    // Configure CLA to trigger task1 with the ADC EOC
    CLA_configClaMemory();
    CLA_initCpuXCla1();

    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

#ifdef CPU1
    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1; // Turn on the EPWM
    EPwm1Regs.TBCTL.bit.CTRMODE = 0; //unfreeze, and enter up count mode
    EDIS;
#endif // CPU1




    for(;;){

    }
}

// end of file
