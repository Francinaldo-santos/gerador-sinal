/*
 * File:   main.c
 * Gerador de sinal
 * 
 * PWM com frequencia e duty cycle ajustavel
 * Onda quadrada (PWM DC 25%)
 * 
 * Frequencia ajustavel por botao, 5 ate 40KHz
 * 
 * Mostrar no display LCD
 * Tipo de onda
 *  -> PWM
 *  -> Quadrada
 *
 */

#include "config.h"
#include "ADC.h"
#include "pwm.h"
#include "LCD4bits.h"
#include <stdio.h>

#define _XTAL_FREQ 8000000
#define ADCCH 24

unsigned int leFreq(char ch);
void escreveLcd(unsigned int valor);
void lcdEnterMode(char linha, char coluna, char *string);

void main(void) {
    // Cristal Oscilador Interno
    OSCCON = 0b01100011;
    
    unsigned int leitura_adc;
    unsigned int duty_cycle;
    unsigned int freq;
    char screen[16];
    char modo = 1;
    
    ANSELD = 0;
    
    // Botoes
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB4 = 1;
    ANSELBbits.ANSB2 = 0;
    ANSELBbits.ANSB4 = 0;
    
    lcdInit();
    adcInit();
    
    while(1){
        if(!PORTBbits.RB2){
            while(!PORTBbits.RB2) __delay_ms(20);
            modo++;
            if(modo > 2) modo = 1;
        }
        
        if(modo == 1){
            lcdStringCursor(1,1,"<-    PWM     ->");
        }
        else if(modo == 2){
            lcdStringCursor(1,1,"<- RETANGULAR ->");
        }
        
        lcdStringCursor(2,2,"CONFIRMAR(B2)");
        
        // Confirma modo
        if(!PORTBbits.RB4){  
            while(!PORTBbits.RB4) __delay_ms(20);
            
            // PWM
            if(modo == 1){              
                
                lcdEnterMode(1,7,"PWM");
                
                while(1){
                    // Definir Frequencia
                    freq = leFreq(ADCCH);
                    escreveLcd(freq);
                    
                    // B1 Cancela e volta ao menu
                    if(!PORTBbits.RB2){
                        while(!PORTBbits.RB2) __delay_ms(20);
                        lcdClean();
                        break;
                    }

                    // B2 Confirma frequencia
                    if(!PORTBbits.RB4){
                        while(!PORTBbits.RB4) __delay_ms(20);
                        
                        // Inicia PWM com freq
                        pwm1Init(freq*1000);
                        pwm1DutyCycle(2);
                        pwm1Start();
                        
                        lcdClean();
                        
                        // PWM Ativado
                        while(1){     
                            // Ajusta Duty Cycle
                            leitura_adc = adcRead(ADCCH);
                            duty_cycle = (float) leitura_adc * 100./1023.;
                            
                            // Limita os níveis
                            if(duty_cycle < 5) duty_cycle = 5;
                            if(duty_cycle > 95) duty_cycle = 95;
                            
                            sprintf(screen, "DC %03u %%", duty_cycle);
                            lcdStringCursor(1,5,screen);
                            lcdStringCursor(2,4, "STOP(1)(2)");
                            
                            pwm1DutyCycle(duty_cycle);
                            
                            // Cancela e volta ao menu
                            if(!PORTBbits.RB2){
                                while(!PORTBbits.RB2) __delay_ms(20);
                                pwm1Stop();
                                lcdClean();
                                break;
                            }
                            if(!PORTBbits.RB4){
                                while(!PORTBbits.RB4) __delay_ms(20);
                                pwm1Stop();
                                lcdClean();
                                break;
                            }
                        }
                        break;
                    }
                    __delay_ms(50);
                }   
            }

            // Quadrada
            else if (modo == 2){
                
                lcdEnterMode(1,4,"RETANGULAR");

                while(1){
                    // Definir Frequencia
                    freq = leFreq(ADCCH);
                    escreveLcd(freq);
                    
                    // B1 Cancela e volta ao menu
                    if(!PORTBbits.RB2){
                        while(!PORTBbits.RB2) __delay_ms(20);
                        lcdClean();
                        break;
                    }

                    // B2 Confirma frequencia
                    if(!PORTBbits.RB4){
                        while(!PORTBbits.RB4) __delay_ms(20);
                        
                        // Inicia PWM com freq
                        pwm1Init(freq*1000);
                        pwm1DutyCycle(20);
                        pwm1Start();
                        
                        lcdClean();
                        lcdStringCursor(1,4,"RETANGULAR");
                        lcdStringCursor(2,4, "STOP(1)(2)");
                        
                        // B1 E B2 Cancela e volta ao menu
                        while(1){
                            if(!PORTBbits.RB2){
                                while(!PORTBbits.RB2) __delay_ms(20);
                                pwm1Stop();
                                lcdClean();
                                break;
                            }
                            if(!PORTBbits.RB4){
                                while(!PORTBbits.RB4) __delay_ms(20);
                                pwm1Stop();
                                lcdClean();
                                break;
                            }
                        }
                        break;
                    }
                    __delay_ms(50);
                } 
            }
        }
        __delay_ms(50);
    }
}

unsigned int leFreq(char ch){
    unsigned int leitura_adc;
    unsigned int freq;  
    leitura_adc = adcRead(ch);
    freq = (float) leitura_adc * 40./1023.;
    if(freq<5) freq = 5;
    return freq;
}

void escreveLcd(unsigned int valor){
    char screen[16]; 
    sprintf(screen, "FREQ %02u KHz", valor);
    lcdStringCursor(1,3,screen);
    lcdStringCursor(2,1,"STOP(1) START(2)");
}

void lcdEnterMode(char linha, char coluna, char *string){
    lcdClean();
    lcdStringCursor(linha, coluna, string);
    __delay_ms(1500);
    lcdClean();
}