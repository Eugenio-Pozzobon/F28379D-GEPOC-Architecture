/*
 * This file contain the ePWM setup function.
 * It is called by the CPU1 to configure the ePWM
 * The ePWM is configured to trigger the SOC A.
 * The Header file contatins the counting flags
 *
*/

#ifndef HEADERS_DSP_SETUP_EPWM_H_
#define HEADERS_DSP_SETUP_EPWM_H_

#define PWM1_PERIOD            399
#define PWM1_DUTY_CYCLE        200

void EPWM_initEpwm(void);

#endif /* HEADERS_DSP_SETUP_EPWM_H_ */
