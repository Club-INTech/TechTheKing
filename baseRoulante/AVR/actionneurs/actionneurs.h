#ifndef ACTIONNEURS_H
#define ACTIONNEURS_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 *  Valeurs de comparaison pour le fast PWM
 *    entre 0 et 255
 */ 
#define SERVO2      OCR0A
#define SERVO1      OCR0B
#define MOTEUR2     OCR2A
#define MOTEUR1     OCR2B

/*
 *  Pins des codeurs
 *  TODO : Mettre les bons ports
 */
#define CODEUR11    PORTB1
#define CODEUR12    PORTB1
#define CODEUR21    PORTB1
#define CODEUR22    PORTB1

/*
 *  Position des ascenseurs
 */
volatile int ascenseur1;
volatile int ascenseur2;

/*
 *  Initialisations
 *    pins en sortie
 *    fast pwm
 *    timers
 */
void init (void);

/*
 *  Lecture analogique de la sortie sense des PHs
 *    int entre 0 et 1024
 *    sert au contrôle du courant
 */
int adc_sense1 (void); // PH 1
int adc_sense2 (void); // PH 2

/*
 *  Fonctions pour les AX12
 *    ID : ID du servo
 *    angleCW : angle max antitrigo
 *    angleCCW : angle max trigo
 *    angle : consigne en angle
 *    vitesse : vitesse de rotation
 */
void AX12Init (uint8_t ID, uint16_t angleCW, uint16_t angleCCW, uint16_t vitesse);
void AX12GoTo (uint8_t ID, uint16_t angle);

#endif