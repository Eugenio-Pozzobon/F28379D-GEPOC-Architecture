/*
 * This file contain the GPIO setup function.
 * It is called by the CPU1 to configure the GPIOs
 * Constant pinout are defined here
 *
*/

void DSP_setupGpios(void);
void GPIO_TogglePin(Uint16 gpioNumber);


#define GLOBAL_DETECTION_PIN_CPU1    59 // pin 110
#define GLOBAL_DETECTION_PIN_CPU2    60 // pin 125

#define DEBUG_TIMING_CLA_CPU1   61 // pin 126
#define DEBUG_TIMING_CLA_CPU2   63 // pin 128
#define DEBUG_TIMING_COM_CPU1   65 // pin 130
#define DEBUG_TIMING_COM_CPU2   67 // pin 132

#define DEBUG_TIMING_AFD_CPU1   36 // pin 122
#define DEBUG_TIMING_AFD_CPU2   38 // pin 124

#define LED_LEFT_CPU1   31
#define LED_RIGHT_CPU2   34

#define BUTTON_SELF_TEST   94 // pin 165 used to interrupt and start self test

#define GLOBAL_DETECTION_PIN   92

#define LED_START_SELFTEST_GPIO 163
#define START_SELFTEST_GPIO 162
#define RESULT_SELFTEST_GPIO 161
