#ifndef __VAL2PWM_H__
#define __VAL2PWM_H__

#include <stdint.h>

extern const uint8_t sLumLut[256];

#define val2pwm(v) sLumLut[v]

#endif // __VAL2PWM_H__
