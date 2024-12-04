/*
 * Arc Fault Detector Flash api storage file
 * UFSM Team
 *
 * This file contain a code to log the processed features into the Flash memory. There is several flash parts that are allocated to store this values.
 * WARNING: This code may be used only for debugging and development programming on the Flash memory. It is not tested within CPU2.
 *
*/
#include <DSP_setup_log.h>
#include "F2837xD_device.h"     // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"   // F2837xD Examples Include File
#include "F021_F2837xD_C28X.h"
#include "DSP_setup_gpios.h"


#pragma DATA_SECTION(log,"log")
float32 log[LOG_SAMPLES];

uint32 current_log_index;
Fapi_StatusType oReturnCheck;

int allow_log_next_sample;
int logToggle;
float value;

void DSP_setup_flash_log(){

    // Gain pump semaphore
    SeizeFlashPump();

    EALLOW;

    oReturnCheck = Fapi_initializeAPI(F021_CPU0_BASE_ADDRESS,200);
    if(oReturnCheck == Fapi_Status_Success){
        oReturnCheck = Fapi_setActiveFlashBank(Fapi_FlashBank0);
    }

    // Erase a Sector, 2000 per sector
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32 *)0x0084000);
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32 *)0x0086000);
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32 *)0x0088000);
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32 *)0x0090000);
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32 *)0x0098000);
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32 *)0x00A0000);
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32 *)0x00A0000);
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32 *)0x00A8000);
    oReturnCheck = Fapi_issueAsyncCommandWithAddress(Fapi_EraseSector, (uint32 *)0x00B0000);

    while (Fapi_checkFsmForReady() != Fapi_Status_FsmReady){}

    if(oReturnCheck != Fapi_Status_Success){
        // Do blank check. Check: https://www.tij.co.jp/jp/lit/ug/spnu629/spnu629.pdf
        // Verify that the sector is erased.
        // oReturnCheck = Fapi_doBlankCheck((uint32 *)0x0084000, Bzero_16KSector_u32length, &oFlashStatusWord);
    }

    EDIS;
}



void DSP_log(float *buffer){

    // para salvar o log: tools > save memory > binary > 0x84000 > words = 192000
    if (logToggle != 1){
        return;
    }

    if(current_log_index >= (uint32) LOG_SAMPLES){
        ESTOP0;
        logToggle = 0;
        return;
    }

    EALLOW;

    oReturnCheck = Fapi_issueProgrammingCommand(
            (uint32*) &log[current_log_index],
            (uint16*) buffer, 8, 0, 0, Fapi_DataOnly);

    EDIS;

    current_log_index += 4;

}

// Currents
extern float ialfagrid, ibetagrid;
extern float ialfaconv, ibetaconv;
extern float iaconv, ibconv, icconv;
extern float iagrid, ibgrid, icgrid;

// Voltages
extern float valfagrid, vbetagrid;
extern float valfaconv, vbetaconv;
extern float vaconv, vbconv, vcconv;
extern float vabgrid, vbcgrid, vcagrid;
extern float valfagrid_fase, valfagrid_quad;
extern float vbetagrid_fase, vbetagrid_quad;


#define BUFFER_SIZE 4
int log_select;

void DSP_log_variables(){

    if (logToggle != 1){
        return;
    }

    GPIO_WritePin(DEBUG_LOG_TIMMING, 1);


    log_select = log_select>=20? 0: log_select+1;
//
    if (log_select == 0) {

        float buffer_00[BUFFER_SIZE] = {valfagrid, vbetagrid, ialfagrid, ibetagrid};
        DSP_log(buffer_00);

    } else if (log_select == 1) {

        float buffer_07[BUFFER_SIZE] = {iaconv, ibconv, icconv, iagrid};
        DSP_log(buffer_07);

    } else if (log_select == 2) {
    // Buffer 08 - 11
        float buffer_08[BUFFER_SIZE] = {ibgrid, icgrid, vaconv, vbconv};
        DSP_log(buffer_08);

        float buffer_09[BUFFER_SIZE] = {vcconv, vabgrid, vbcgrid, vcagrid};
        DSP_log(buffer_09);


    } else if (log_select == 3) {
        float buffer_28[4] = {valfaconv, vbetaconv, ialfaconv, ibetaconv};
        DSP_log(buffer_28);


    }

    GPIO_WritePin(DEBUG_LOG_TIMMING, 0);

}


