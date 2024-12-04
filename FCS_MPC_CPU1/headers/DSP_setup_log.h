/*
 * Arc Fault Detector Self Test header file
 * UFSM Team
 *
 *
*/

// How many channels will store the data in the flash.
// If is 1, then CH1 will be stored.
// If is 2, channels CH1 and CH2 will be stored.
// The available space is filled alocating the wole space

#include "stdint.h"
#define LOG_SAMPLES 96000

void DSP_setup_flash_log(void);
void DSP_log(float *buffer);
void DSP_log_variables(void);

extern int allow_log_next_sample;
