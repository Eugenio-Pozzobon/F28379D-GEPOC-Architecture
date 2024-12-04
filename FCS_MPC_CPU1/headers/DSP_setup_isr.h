#ifndef HEADERS_DSP_SETUP_ISR_H_
#define HEADERS_DSP_SETUP_ISR_H_


#ifdef _FLASH
#pragma CODE_SECTION(cpu_timer0_isr,"isrfunc")
#endif

__interrupt void cpu_timer0_isr(void);

#endif /* HEADERS_DSP_SETUP_ISR_H_ */
