/*
 * This file contain the variable functions and definitions shared by CPUs and CLAs.
 * Both CPUs may call this functions
 * Also, there are the ISRs triggered by the CLA every time that an task ends.
 * This ISRs run in the CPU and are disabled for this project.
*/

#include "stdint.h"
#define ARRAY_SIZE 256


// CLA defines
#define WAITSTEP               asm(" RPT #255 || NOP")


// Function Prototypes

void CLA_configClaMemory(void);
void CLA_initCpuXCla1(void);

__interrupt void Cla1Task1();
__interrupt void Cla1Task2();
__interrupt void Cla1Task3();
__interrupt void Cla1Task4();
__interrupt void Cla1Task5();
__interrupt void Cla1Task6();
__interrupt void Cla1Task7();
__interrupt void Cla1Task8();

__interrupt void cla1Isr1();
__interrupt void cla1Isr2();
__interrupt void cla1Isr3();
__interrupt void cla1Isr4();
__interrupt void cla1Isr5();
__interrupt void cla1Isr6();
__interrupt void cla1Isr7();
__interrupt void cla1Isr8();

