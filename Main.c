#include <xc.h>
#include <stdio.h>
#include "config_bits.h"

#define _XTAL_FREQ 4000000  
#define VREF_plus  5.0f  
#define VREF_moins 0.0f 
#define PLEINE_ECH 1024 

int tempdebase = 30;
int temp = tempdebase;
float tempCapteur;

//Pour récupérer la tension
unsigned long ValeurADC = 0L;
float tension = 0.0f;

void changeTemperature();
void afficheTemperature();
void temperatureAmbiante();
void chauffage();
void ventilateur();

void main() {

    //Boutons de température en entrée
    TRISCbits.TRISC0 = 1;
    TRISCbits.TRISC1 = 1;

    //LEDS en sortie
    TRISCbits.TRISC3 = 0;
    TRISCbits.TRISC4 = 0;
    TRISCbits.TRISC5 = 0;
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 0;

    //Chauffage et ventilateur en sortie
    TRISDbits.TRISD0 = 0;
    TRISDbits.TRISD1 = 0;

    //Capteur de température en entrée
    TRISAbits.TRISA0 = 1;

    //Récupère la tension
    TRISA = 0xFF;
    ADCON1 = 0b10001110;
    ADCON0 = 0b01000001;

    while (1) {
        changeTemperature();
        afficheTemperature(temp);
        tempCapteur = temperatureAmbiante();
        chauffage(temp, tempCapteur);
        ventilateur(temp, tempCapteur);
    }
}

void changeTemperature() {
    if (PORTCbits.RC0 == 0 && temp > 22) {
        temp -= 4;
    }
    if (PORTCbits.RC1 == 0 && temp < 38) {
        temp += 4;
    }
}

void afficheTemperature(int temp) {
    PORTC = 0x00;
    switch (temp) {
        case 22:
            PORTCbits.RC3 = 1;
            break;
        case 26:
            PORTCbits.RC4 = 1;
            break;
        case 30:
            PORTCbits.RC5 = 1;
            break;
        case 34:
            PORTCbits.RC6 = 1;
            break;
        case 38:
            PORTCbits.RC7 = 1;
            break;
    }
}

float recupereTension() {
    ADCON0bits.GO_DONE = 1;
    while (ADCON0bits.GO_DONE == 1);
    ValeurADC = ADRESH << 8;
    ValeurADC += ADRESL;
    tension = (VREF_plus - VREF_moins) * ValeurADC / PLEINE_ECH;
    __delay_ms(500);

    return tension;
}

float temperatureAmbiante() {
    int tension = recupereTension();
    tempCapteur = (tension * 1000 / 125) + 10;

    return tempCapteur;
}

void chauffage(float temp, float tempCapteur) {
    if (tempCapteur < temp) {
        if (tempCapteur <= (temp - 1)) {
            PORTDbits.RD1 = 1;
        } else {
            PORTDbits.RD1 = 0;
        }
    } else if (tempCapteur == temp) {
        PORTDbits.RD1 = 0;
    }
    __delay_ms(5000);
}

void ventilateur(float temp, float tempCapteur) {
    if (tempCapteur > temp) {
        if (tempCapteur >= (temp - 1)) {
            PORTDbits.RD0 = 1;
        } else {
            PORTDbits.RD0 = 0;
        }
    } else if (tempCapteur == temp) {
        PORTDbits.RD0 = 0;
    }
    __delay_ms(5000);
}