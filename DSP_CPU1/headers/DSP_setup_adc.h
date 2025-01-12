/*
 * This file contain the ADC setup function.
 * It is called by the CPU1 to configure the ADCs
 * The ePWM trigger the ADC, that schedule an ISR to the CLA Task 1
 *
*/

// ADC defines
#define REFERENCE_INTERNAL     0 //internal reference (12-bit only)
#define REFERENCE_EXTERNAL     1 //external reference

#define ADC_ACQPS   19 // 19 sysclk = 100ns
#define ADC_PRESCALE    6   // ADCCLK divider to /4
#define ADC_TRIG_SOURCE 5   // EPWM1A

void ADC_initAdcs(void);
