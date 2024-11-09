/*
 * File:   main.c
 * Author: Anoop Mathew
 *
 * Created on December 1, 2023, 7:39 AM
 */


#include <xc.h>
#include<stdio.h>
#include"lcd.h"
#define _XTAL_FREQ 4000000
// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA3/MCLR pin function select (RA3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

void ADC_Init() {
    ANSEL = 0b00000001;   // Set AN0 pin as analog input
    ADCON0 = 0b00000001;  // Select ADC channel AN0 and enable ADC
    ADCON1 = 0b00000000;  // VREF+ is VDD, VREF- is VSS, left-justified result
}

unsigned int ADC_Read() {
             // Start ADC conversion
    ADIF=0;
    ADON=1;
    __delay_ms(1);
    GO_nDONE = 1;
    while (ADIF == 0);     // Wait for conversion to complete
    GO = 0;
    __delay_ms(1);
    return ((ADRESH << 8) | ADRESL); // Return 10-bit ADC result
}

void main(void)
{
    TRISC=0;
    PORTC=0;
    ANSEL=0;
    LCD_Init();
    unsigned int adc_value;
    char adc_string[5];   // Buffer to hold ADC value as a string
    ADC_Init();
    while(1)
    {
        LCD_Command(0x01);
        LCD_Set_Cursor(1,1);
        LCD_Write_String("hello");
        
        adc_value = ADC_Read(); // Read ADC value from AN0
        
        // Convert ADC value to string
        sprintf(adc_string, "%u", adc_value);
        
        // Display ADC value on LCD
        LCD_Set_Cursor(1, 1); // Set cursor to first line, first position
        LCD_Write_String("ADC Value: ");
        LCD_Set_Cursor(2, 1); // Set cursor to second line, first position
        LCD_Write_String(adc_string);
        
        __delay_ms(500);
        
    }
}

