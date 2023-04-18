#include "pwm.h"

void pwm1Init(double freq){
    ANSELCbits.ANSC2 = 0;
    TRISCbits.TRISC2 = 0;
    
    CCP1CON = 0x0F;
    T2CON = 0x01;
    
    PR2 = _XTAL_FREQ / (4.*freq*4.) - 1.;
}

void pwm1DutyCycle(double DC){
    unsigned int CCPR;
    double _PR2 = PR2;
    
    if(DC < 5) DC = 5;
    if(DC > 95) DC = 95;
    
    CCPR = (100.-DC)*(_PR2+1.)*4./100.;
 
    CCPR1L = (unsigned char)(CCPR >> 2);
    CCP1CONbits.DC1B1 = CCPR & 0x02;
    CCP1CONbits.DC1B0 = CCPR & 0x01;
}

void pwm1Start(void){
    T2CONbits.TMR2ON = 1;
}

void pwm1Stop(void){
    T2CONbits.TMR2ON = 0;
}