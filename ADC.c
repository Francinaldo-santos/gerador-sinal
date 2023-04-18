#include "ADC.h"

union {
    char leitura[2];
    unsigned int result;
} adc;

void adcInit(void){
    ADCON1 = 0x00;
    ADCON2 = 0xAA;
}

unsigned int adcRead(char ch){
    if(ch == 1){
        TRISAbits.TRISA0 = 1;
        ANSELAbits.ANSA0 = 1;
        ADCON0 = 0;
    }
    
    if(ch == 24){
        TRISDbits.TRISD4 = 1;
        ANSELDbits.ANSD4 = 1;
        ADCON0 = 0x60;
    }
    
    if(ch == 25){
        TRISDbits.TRISD5 = 1;
        ANSELDbits.ANSD5 = 1;
        ADCON0 = 0x64;
    }
    
    ADCON0bits.ADON = 1;
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);

    adc.leitura[1] = ADRESH;
    adc.leitura[0] = ADRESL;
    
    ADCON0bits.ADON = 0;
    return adc.result;
}