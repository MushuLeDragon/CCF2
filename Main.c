#include <xc.h>
#include <stdio.h>
#include "config_bits.h"



int tempdebase = 30;
int temp = tempdebase;


void changeTemperature();
void afficheTemperature();

void main(void) {

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

    //Allume la LED du port RC3
    //PORTCbits.RC3 = 1;

    while (1) {
        //Mettre les foctions a appeler
        //tempcapteurs recus
        //régulateur de température
        changeTemperature();
        afficheTemperature(temp);
    }


    return;
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