#include "LCD4bits.h"

void lcdSetCursor(char linha, char coluna){
    char posicao;
    // Subtrai 1 pois endereÃ§o comeÃ§a em 0
    if(linha == 1) posicao = 0x80 + (coluna - 1);
    if(linha == 2) posicao = 0xC0 + (coluna - 1);
    
    lcdComando(0, posicao);
    
}

void lcdString(char *str){ 
    // Enquanto o caracter nÃ£o for nulo
    while(*str){
        lcdComando(1, *str);
        str++;
    }
}

void lcdStringCursor(char linha, char coluna, char *str){
    lcdSetCursor(linha, coluna);
    lcdString(str);
}

void lcdClean(void){
    lcdComando(0, 0x01);
    __delay_ms(1);
}

void lcdInit(void){
    ANSELCbits.ANSC4 = 0;
    ANSELCbits.ANSC5 = 0;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    
    DATA_TRIS = 0;
    DATA_PORT = 0;
    
    __delay_ms(60);
    
    DATA_PORT = 0x03;
    
    for(char i = 0; i<3; i++){
        CLOCK = 1;
        CLOCK = 0;
        __delay_ms(5);
    }
    
    DATA_PORT = 0x02;
    
    for(char i = 0; i<2; i++){
        CLOCK = 1;
        CLOCK = 0;
        __delay_ms(5);
    }
    
    DATA_PORT = 0;
    CLOCK = 1;
    CLOCK = 0;
    
    lcdComando(0, 0x28);
    __delay_ms(1);

    lcdComando(0, 0x0C);
    __delay_ms(1);

    lcdComando(0, 0x01);
    __delay_ms(1);

    lcdComando(0, 0x06);
    __delay_ms(1);
}

void lcdComando(unsigned char tipo, unsigned char com){
    RS = tipo;
    __delay_ms(1);
    CLOCK = 0;
    // Envia primeiro a parte alta
    D4 = com & 0x10 ? 1 : 0;
    D5 = com & 0x20 ? 1 : 0;
    D6 = com & 0x40 ? 1 : 0;
    D7 = com & 0x80 ? 1 : 0;
    CLOCK = 1;
    CLOCK = 0;
    __delay_ms(1);
    D4 = com & 0x01 ? 1 : 0;
    D5 = com & 0x02 ? 1 : 0;
    D6 = com & 0x04 ? 1 : 0;
    D7 = com & 0x08 ? 1 : 0;
    CLOCK = 1;
    CLOCK = 0;
    __delay_ms(1);
    RS = 0;
}
