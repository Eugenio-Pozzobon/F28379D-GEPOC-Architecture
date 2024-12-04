/*
 * GPIO setup file
*/
#include "F2837xD_device.h"     // F2837xD Headerfile Include File
#include "F2837xD_Examples.h"   // F2837xD Examples Include File
#include <DSP_setup_gpios.h>

#ifdef CPU1

// setup gpios for CPU 1/2 or CLAs
void DSP_setupGpios(void){

    GPIO_SetupPinMux(LED_START_SELFTEST_GPIO, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(LED_START_SELFTEST_GPIO, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(DEBUG_LOG_TIMMING, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(DEBUG_LOG_TIMMING, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(DEBUG_MPC_CPU2, GPIO_MUX_CPU2, 0);
    GPIO_SetupPinOptions(DEBUG_MPC_CPU2, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(INIBIDOR_CPU2, GPIO_MUX_CPU2, 0);
    GPIO_SetupPinOptions(INIBIDOR_CPU2, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(DEBUG_TIMING_COM_CPU1, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(DEBUG_TIMING_COM_CPU1, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(DEBUG_TIMING_COM_CPU2, GPIO_MUX_CPU2, 0);
    GPIO_SetupPinOptions(DEBUG_TIMING_COM_CPU2, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(DEBUG_TIMING_CLA_CPU2, GPIO_MUX_CPU2CLA, 0);
    GPIO_SetupPinOptions(DEBUG_TIMING_CLA_CPU2, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(DEBUG_TIMING_CLA_CPU1, GPIO_MUX_CPU1CLA, 0);
    GPIO_SetupPinOptions(DEBUG_TIMING_CLA_CPU1, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(DEBUG_TIMING_AFD_CPU1, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(DEBUG_TIMING_AFD_CPU1, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(DEBUG_TIMING_AFD_CPU2, GPIO_MUX_CPU2, 0);
    GPIO_SetupPinOptions(DEBUG_TIMING_AFD_CPU2, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(LED_LEFT_CPU1, GPIO_MUX_CPU1, 0);
    GPIO_SetupPinOptions(LED_LEFT_CPU1, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(LED_RIGHT_CPU2,GPIO_MUX_CPU2,0);
    GPIO_SetupPinOptions(LED_RIGHT_CPU2, GPIO_OUTPUT, GPIO_PUSHPULL);

    GPIO_SetupPinMux(BUTTON_SELF_TEST,GPIO_MUX_CPU1,0);
    GPIO_SetupPinOptions(BUTTON_SELF_TEST, GPIO_INPUT, GPIO_PULLUP);
//
//    GPIO_SetupPinMux(ANALOG_SWITCH_01, GPIO_MUX_CPU1CLA, 0);
//    GPIO_SetupPinOptions(ANALOG_SWITCH_01, GPIO_OUTPUT, GPIO_PUSHPULL);
//
//    GPIO_SetupPinMux(ANALOG_SWITCH_02, GPIO_MUX_CPU1CLA, 0);
//    GPIO_SetupPinOptions(ANALOG_SWITCH_02, GPIO_OUTPUT, GPIO_PUSHPULL);
//
//    GPIO_SetupPinMux(ANALOG_SWITCH_03, GPIO_MUX_CPU1CLA, 0);
//    GPIO_SetupPinOptions(ANALOG_SWITCH_03, GPIO_OUTPUT, GPIO_PUSHPULL);

}

#endif // CPU1


// togle the state of a selected pin
void GPIO_TogglePin(Uint16 gpioNumber) {

    volatile uint32_t *gpioDataReg;
    uint32_t pinMask;

    gpioDataReg = (volatile uint32_t *)&GpioDataRegs + (gpioNumber / 32) * GPY_DATA_OFFSET;
    pinMask = 1UL << (gpioNumber % 32);

    // Toggle the pin state
    if (*gpioDataReg & pinMask) {
        gpioDataReg[GPYCLEAR] = pinMask; // Set the pin low
    } else {
        gpioDataReg[GPYSET] = pinMask;   // Set the pin high
    }

}

