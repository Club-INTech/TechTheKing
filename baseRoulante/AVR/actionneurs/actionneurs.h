#ifndef ACTIONNEURS_H
#define ACTIONNEURS_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 *  Registres de comparaison pour le fast PWM
 *    entre 0 et 255
 */
#define SERVO2      OCR0A
#define SERVO1      OCR0B
#define MOTEUR2     OCR2A
#define MOTEUR1     OCR2B

/*
 * Pins de direction des PH
 */
#define DIR2    (1 << PORTD4)
#define DIR1    (1 << PORTD2)

/*
 *  Constantes
 *    PWM max pour les maxons
 *    facteur proportionnel
 *    ID des AX12
 *    PWM pour les servos
 *    Type d'asservissement
 */
#define PWM_MAX1    150
#define PWM_MAX2    200
#define KP1         1
#define KP2         2
#define ID_AX1      2
#define ID_AX2      1
#define PWM_UP      18
#define PWM_DOWN    26
#define ASSERV_INDEP    0
#define ASSERV_SYNCHRO  1
#define ASSERV_STOP     -1

/*
 *  Pins des codeurs
 */
#define CODEUR11    (1 << PORTC2)
#define CODEUR12    (1 << PORTC3)
#define CODEUR21    (1 << PORTB1)
#define CODEUR22    (1 << PORTB2)

/*
 *  Variables des maxons
 *    consignes
 *    position
 *    synchronisation
 */
extern int16_t consigne1;
extern int16_t consigne2;
extern int16_t consigneb;
extern volatile int16_t ascenseur1;
extern volatile int16_t ascenseur2;
extern int8_t etat_asservissement;

/*
 *  Fonctions d'asservissement
 */
void asservissement (void);
void asservissement_synchro (void);
void recalage (void);

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
