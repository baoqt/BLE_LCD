/*
 * File:   LCD.c
 * Author: BAO
 *
 * Created on May 28, 2019, 12:56
 */
// CONFIG1
#pragma config FEXTOSC = OFF    // FEXTOSC External Oscillator mode Selection bits->Oscillator not enabled
#pragma config RSTOSC = HFINT1    // Power-up default value for COSC bits->HFINTOSC
#pragma config CLKOUTEN = OFF    // Clock Out Enable bit->CLKOUT function is disabled; I/O or oscillator function on OSC2
#pragma config CSWEN = ON    // Clock Switch Enable bit->Writing to NOSC and NDIV is allowed
#pragma config FCMEN = ON    // Fail-Safe Clock Monitor Enable->Fail-Safe Clock Monitor is enabled

// CONFIG2
#pragma config MCLRE = ON    // Master Clear Enable bit->MCLR/VPP pin function is MCLR; Weak pull-up enabled
#pragma config PWRTE = OFF    // Power-up Timer Enable bit->PWRT disabled
#pragma config WDTE = OFF    // Watchdog Timer Enable bits->WDT disabled; SWDTEN is ignored
#pragma config LPBOREN = OFF    // Low-power BOR enable bit->ULPBOR disabled
#pragma config BOREN = ON    // Brown-out Reset Enable bits->Brown-out Reset enabled, SBOREN bit ignored
#pragma config BORV = LOW    // Brown-out Reset Voltage selection bit->Brown-out voltage (Vbor) set to 2.45V
#pragma config PPS1WAY = ON    // PPSLOCK bit One-Way Set Enable bit->The PPSLOCK bit can be cleared and set only once; PPS registers remain locked after one clear/set cycle
#pragma config STVREN = ON    // Stack Overflow/Underflow Reset Enable bit->Stack Overflow or Underflow will cause a Reset
#pragma config DEBUG = OFF    // Debugger enable bit->Background debugger disabled

// CONFIG3
#pragma config WRT = OFF    // User NVM self-write protection bits->Write protection off
#pragma config LVP = ON    // Low Voltage Programming Enable bit->Low Voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored.

// CONFIG4
#pragma config CP = OFF    // User NVM Program Memory Code Protection bit->User NVM code protection disabled
#pragma config CPD = OFF    // Data NVM Memory Code Protection bit->Data NVM code protection disabled

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#include "LCD.h"

uint8_t packetIndex = 0;

void main(void) {
    OSCCON1 = 0x60; // HFINTOSC   
    OSCFRQ = 0x03;  // HFFRQ 4_MHz; 

    TRISA = 0b11011001;     // RA1, RA2 outputs (LCD)
                            // RA4 input, RA5 output (ACD)
    TRISC = 0b11001000;     // RC0, RC1, RC2 outputs RC5 activity LED(LCD)
                            // RC4 outputs (ADC)
    ANSELA = 0b00000000;    // Disable analog input
    ANSELC = 0b00000000;
    
    GIE = 1;                // Enable global interrupts
    PEIE = 1;               // Enable peripheral interrupts
    TMR0IE = 1;             // Enable overflow interrupt
    
    T0EN = 1;               // Enable timer0
    TMR0 = 0;               // 8 bit timer
    T0CON1 ^= 0b01000000;   // FOSC/4, 1:1 prescaler
    TMR0H = 0b10111111;     // Clock period
    
    LATA1 = 1;              // LCD data high
    LATA2 = 1;              // LCD clock high
    LATC0 = 0;              // LCD command mode
    LATC1 = 1;              // LCD reset N
    LATC2 = 1;              // LCD chip select N
    
    LATC4 = 1;              // ADC clock high
    LATA5 = 1;              // ADC off
    
    LCD_init();
    LCD_clear();
    
    while (1)
    {
        // main loop
    }
    
    return;
}

void __interrupt() displayClock(void)
{
    if (TMR0IF && TMR0IE && transmitFlag) 
    {
        if (packetIndex < 8)
        {
            LATA2 = ~LATA2;     // Toggle clock
            LATC5 = ~LATC5;
            
            if (LATA2 == 0)     // Clock low
            {                   // Push bit onto line
                LATA1 = (packet & 0b10000000) && 1;
            }                   // LATA1 : LCD data line
            else                // Clock high
            {
                packet <<= 1;   // Shift for next bit
                packetIndex++;
            }
        }
        else                    // Transmission finished
        {
            packetIndex = 0;
            transmitFlag = 0;
            LATC5 = 0;
        }
        
        TMR0IF = 0;             // Reset interrupt flag
    }
    
    return;
}