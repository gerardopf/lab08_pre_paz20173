/*
 * lab08_pre
 * File:   lab08_pre_pgr.c
 * Author: Gerardo Paz - 20173
 * Potenci�metro en RA0
 * Leds en puerto C
 * Created on April 5, 2022, 8:52 PM
 */

#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)
// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>         // registros del PIC
#include <stdio.h>
#include <stdlib.h>

/*------------ CONSTANTES -----------------*/
#define _XTAL_FREQ 4000000  // oscilador 

/*------------- VARIABLES GLOBALES ------------------*/

/*--------------- FUNCIONES DE INTERRUPCIONES -----------------*/

/*---------------- FUNCIONES PRINCIPALES ---------------*/

void setup(void){
    ANSEL = 1;      // entrada anal�gica en A y E
    
    TRISA = 1;      // A in
    TRISC = 0;      // C out
    
    PORTA = 0;
    PORTC = 0;      // limpiar
    return;
}

void clk(void){
    OSCCONbits.IRCF = 0b0110;   // Tiempo
    OSCCONbits.SCS = 1;         // oscilador interno
    return;
}

void setup_adc(void){
    ADCON1bits.ADFM = 0;    // justificado izquierda
    ADCON1bits.VCFG0 = 0;   // VDD
    ADCON1bits.VCFG1 = 0;   // VSS
    
    ADCON0bits.ADCS = 0b01; // Fosc/8
    ADCON0bits.CHS = 0;     // AN0
    ADCON0bits.ADON = 1;    // habilitar m�dulo ADC
    __delay_us(50);         // tiempo para que cargue el capacitor
    return;
}

void setup_int(void){
    INTCONbits.GIE = 1;     // globales
    INTCONbits.PEIE = 1;    // perif�ricas
    PIE1bits.ADIE = 1;      // ADC
    PIR1bits.ADIF = 0;      // ADC bandera
    return;
}

/*------------ C�DIGO PRINCIPAL ---------------*/
void __interrupt() isr (void){
    if(ADIF){
        PIR1bits.ADIF = 0;      // limpiar bandera
        PORTC = ADRESH;         // mostrar valor
    }
    return;
}

void main(void){
    setup();
    clk();
    setup_adc();
    setup_int();
    ADCON0bits.GO = 1;  // iniciar conversi�n
    while(1){   // principal loop
        if(ADCON0bits.GO == 0){
            __delay_us(50);     
            ADCON0bits.GO = 1;  // comenzar otra conversi�n
        }   
    }
}

