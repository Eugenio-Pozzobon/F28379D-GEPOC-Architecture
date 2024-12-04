/*
 * This file setups the program alocation inside the CPU LS memory, starting the tasks.
 *
*/

#if (!defined(_STANDALONE) & defined(_FLASH) & defined(CPU1))
#define SAVE_LOG
#endif

#include <DSP_setup_cla.h>
#include "F28x_Project.h"
#include "F2837xD_Cla_defines.h"
#include "F2837xD_cla.h"
#include "stdint.h"

extern int allow_control;
extern int allow_log_next_sample;
extern void rmrac();

// CLA_configClaMemory - Configure CLA memory sections
void CLA_configClaMemory(void){


#ifdef _FLASH
    extern uint32_t Cla1funcsRunStart, Cla1funcsLoadStart, Cla1funcsLoadSize;
    extern uint32_t Cla1ConstRunStart, Cla1ConstLoadStart, Cla1ConstLoadSize;
#endif //_FLASH

    EALLOW;

#ifdef _FLASH
    // Copy over code from FLASH to RAM
    // https://e2e.ti.com/support/microcontrollers/c2000-microcontrollers-group/c2000/f/c2000-microcontrollers-forum/1090113/tms320f28377d-ep-tms320f28377d-ep?tisearch=e2e-sitesearch&keymatch=_Cla1funcsLoadSize#
    memcpy((uint32_t *)&Cla1funcsRunStart, (uint32_t *)&Cla1funcsLoadStart,
            (uint32_t)&Cla1funcsLoadSize);
    memcpy((uint32_t *)&Cla1ConstRunStart, (uint32_t *)&Cla1ConstLoadStart,
            (uint32_t)&Cla1ConstLoadSize);
#endif //_FLASH

#ifdef CPU2
    // Enable CPU2 clocking at the sys clock level
    CpuSysRegs.PCLKCR0.bit.CLA1 = 1;
#endif

    // Initialize and wait for CLA1ToCPUMsgRAM
    MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

    // Initialize and wait for CPUToCLA1MsgRAM
    MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

    // Select LS4RAM and LS5RAM to be the programming space for the CLA
    // First configure the CLA to be the master for LS4 and LS5 and then
    // set the space to be a program block
    MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1;

    MemCfgRegs.LSxMSEL.bit.MSEL_LS0 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS0 = 0;

    MemCfgRegs.LSxMSEL.bit.MSEL_LS1 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS1 = 0;

    MemCfgRegs.LSxMSEL.bit.MSEL_LS2 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS2 = 0;

    MemCfgRegs.LSxMSEL.bit.MSEL_LS3 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS3 = 0;

    EDIS;
}

// CLA_initCpu1Cla1 - Initialize CLA1 task vectors and end of task interrupts
void CLA_initCpuXCla1(void){

    // Compute all CLA task vectors
    // On Type-1 CLAs the MVECT registers accept full 16-bit task addresses as
    // opposed to offsets used on older Type-0 CLAs
    EALLOW;
    Cla1Regs.MVECT1 = (uint16_t)(&Cla1Task1);
    Cla1Regs.MVECT2 = (uint16_t)(&Cla1Task2);
    Cla1Regs.MVECT3 = (uint16_t)(&Cla1Task3);
    Cla1Regs.MVECT4 = (uint16_t)(&Cla1Task4);
    Cla1Regs.MVECT5 = (uint16_t)(&Cla1Task5);
    Cla1Regs.MVECT6 = (uint16_t)(&Cla1Task6);
    Cla1Regs.MVECT7 = (uint16_t)(&Cla1Task7);
    Cla1Regs.MVECT8 = (uint16_t)(&Cla1Task8);

    // Enable the IACK instruction to start a task on CLA in software
    // for all 8 CLA tasks. Also, globally enable all 8 tasks (or a
    // subset of tasks) by writing to their respective bits in the
    // MIER register

    // Cla1Regs.MCTL.bit.IACKE = 1;
    Cla1Regs.MIER.all = 0x00FF;

    // Configure the vectors for the end-of-task interrupt for all 8 tasks
    PieVectTable.CLA1_1_INT = &cla1Isr1;
    PieVectTable.CLA1_2_INT = &cla1Isr2;
    PieVectTable.CLA1_3_INT = &cla1Isr3;
    PieVectTable.CLA1_4_INT = &cla1Isr4;
    PieVectTable.CLA1_5_INT = &cla1Isr5;
    PieVectTable.CLA1_6_INT = &cla1Isr6;
    PieVectTable.CLA1_7_INT = &cla1Isr7;
    PieVectTable.CLA1_8_INT = &cla1Isr8;

    // Set the source of trigger for each task
    DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK1 = 1; // 1 -> ADC A INTerrupt

    // Enable CLA interrupts at the group and subgroup levels
    PieCtrlRegs.PIEIER11.all = 0xFFFF;
    IER |= (M_INT11);
    EDIS;
}

__interrupt void cla1Isr1 (){
    // Clear the PIEACK bits so another interrupt can be taken
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP11;
    allow_control = 1;
#ifdef SAVE_LOG
    allow_log_next_sample = 1;
#endif
}

__interrupt void cla1Isr2 (){
    asm(" ESTOP0");
}

__interrupt void cla1Isr3 (){
    asm(" ESTOP0");
}

__interrupt void cla1Isr4 (){
    asm(" ESTOP0");
}

__interrupt void cla1Isr5 (){
    asm(" ESTOP0");
}

__interrupt void cla1Isr6 (){
    asm(" ESTOP0");
}

__interrupt void cla1Isr7 (){
    asm(" ESTOP0");
}

__interrupt void cla1Isr8 (){
    asm(" ESTOP0");
}
// END OF FILE
