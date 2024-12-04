/*
 * This file contain the GPIO setup function.
 * It is called by the CPU1 to configure the GPIOs
 * Constant pinout are defined here
 *
*/

void DSP_setupGpios(void);
void GPIO_TogglePin(Uint16 gpioNumber);

#define DEBUG_MPC_CPU1  44
#define INIBIDOR_CPU1    39

#define DEBUG_MPC_CPU2  45
#define INIBIDOR_CPU2    40

#define DEBUG_TIMING_CLA_CPU1   48 // pin 126
#define DEBUG_TIMING_CLA_CPU2   49 // pin 128

#define DEBUG_TIMING_COM_CPU1   65 // pin 130
#define DEBUG_TIMING_COM_CPU2   67 // pin 132
#define DEBUG_TIMING_AFD_CPU1   36 // pin 122
#define DEBUG_TIMING_AFD_CPU2   38 // pin 124

#define LED_LEFT_CPU1   31
#define LED_RIGHT_CPU2   34

#define BUTTON_SELF_TEST   94 // pin 165 used to interrupt and start self test

#define DEBUG_LOG_TIMMING   49

#define LED_START_SELFTEST_GPIO 163

//DI13    GPIO-14/PWM-8A
//DI14    GPIO-12/PWM-7A
//DI15    GPIO-15/PWM-8B
//DI16    GPIO-13/PWM-7B
//DI17    GPIO-34
//DI18    GPIO-39
//DI19    GPIO-40
//DI20    GPIO-41
//DI21    GPIO-44
//DI22    GPIO-45
//DI23    GPIO-48
//DI24    GPIO-49
