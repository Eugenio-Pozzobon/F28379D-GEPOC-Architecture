/* dotp.h */
// https://e2e.ti.com/support/microcontrollers/c2000-microcontrollers-group/c2000/f/c2000-microcontrollers-forum/599096/ccs-tms320f28069-where-is-the-library-to-implement-the-convolution-of-2-real-vectors
// default vector length
#define DEFAULT_VECTOR_SIZE  4

// assembly function definition
extern float dotp(float *p, float *q, unsigned int N);
