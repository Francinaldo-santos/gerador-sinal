/*
PIC18F45K22
Gera PWM pelo canal CCP1
*/

#ifndef _PWM_H_
#define	_PWM_H_

// Inicializa o CCP no modo PWM
// @param: freq -> frequÃªncia do PWM
void pwm1Init(double freq);

// Define o Duty Cycle do PWM
// @param: DC -> DutyCycle, valor de 0 a 100
void pwm1DutyCycle(double DC);

// Inicia o PWM
void pwm1Start(void);

// Para o PWM
void pwm1Stop(void);

#define _XTAL_FREQ 8000000
#include <xc.h>

#endif