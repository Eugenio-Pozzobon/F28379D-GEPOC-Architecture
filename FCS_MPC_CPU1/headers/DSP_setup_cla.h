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


#define OFFSET_CA   2048

#define OFFSET_CHA1 2048-2039
#define OFFSET_CHA2 2048-2039
#define OFFSET_CHA3 2048-2043
#define OFFSET_CHA4 2048-2055
#define OFFSET_CHA5 2048-2053
#define OFFSET_CHB0 2048-2006
#define OFFSET_CHB1 2048-2045      // Se os valores estiverem muito diferentes destes aqui,
#define OFFSET_CHB2 2048-2047      // conferir se os pinos de A/D do DSP nao estao com mal contato

#define OFFSET_CHB3 2048-2050
#define OFFSET_CHB4 2048-2053
#define OFFSET_CHB5 2048-2053
#define OFFSET_CHD0 2048-2049
#define OFFSET_CHD1 2048-2049
#define OFFSET_CHC14 2048-2030
#define OFFSET_CHC15 2048-2033

//-----------------------------------------------------------------------------
extern float iagrid,ibgrid,icgrid;    // Correntes da rede (abc)
extern float iaconv,ibconv,icconv;    // Correntes do conversor (abc)
extern float vabgrid,vbcgrid,vcagrid; // Tensões da rede no ponto de conexão (abc)
extern float vagrid,vbgrid,vcgrid;    // Tensões de fase da rede (após transformação)
extern float vaconv,vbconv,vcconv;    // Tensão dos capacitores (abc)
extern float vcc;                     // Tensão do barramento CC

// Ganhos dos sensores de tens�o e corrente => SETUP 50kW
//#define ganho_corrente_AC     0.05812872023809    //fundo_escala_corrente_AC = 120 / 2048     -> R=12.6 ohms
//#define ganho_rogowski_AC     0.021989971318184   //fundo_escala_corrente_AC = 30 amperes RMS
//#define ganho_tensao_AC       0.330505580         // 486/1720,32   //0.1953125 //fundo_escala_tensao_AC   = 400 / 2048        -> R=120 ohms
//#define ganho_tensao_CC       0.478515625         //fundo_escala_tensao_CC   = 980/ 2048

// Ganhos dos sensores de tens�o e corrente
#define ESCALA_1000 0.48828  // fundo_escala_tensao_AC   = +-1000 / 4096

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

