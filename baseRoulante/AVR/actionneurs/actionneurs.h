#ifndef ACTIONNEURS_H
#define ACTIONNEURS_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "adc.h"

/*
 *  Registres de comparaison pour le fast PWM
 *    entre 0 et 255
 */
#define SERVO_CONS2      OCR0A
#define SERVO_CONS1      OCR0B
#define ASC_MOTEUR2      OCR2A
#define ASC_MOTEUR1      OCR2B

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
#define ASC_PWM_MAX1           150
#define ASC_PWM_MAX2           150
#define ASC_KP1                1
#define ASC_KP2                2
#define ASC_ASSERV_INDEP       0
#define ASC_ASSERV_SYNCHRO     1
#define ASC_ASSERV_STOP       -1
#define ASC_CONS_MAX           8000

#define AX_ID1                 1
#define AX_ID2                 2
#define AX_ANGLE_MEDIAN        511
#define AX_ANGLE_EXT1          200
#define AX_ANGLE_EXT2          800
#define AX_SPEED               511

#define SERVO_PWM_UP1           26
#define SERVO_PWM_DOWN1         13
#define SERVO_PWM_UP2           13
#define SERVO_PWM_DOWN2         26

/**
 * Définition du courant de seuil maximum
 */
#define CUR_LIM 512
#define CUR_MAX 700

/**
 * Définition du temps max pour la limite de courant
 */
#define TEMPS_MAX 100

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
void actio_init (void);

/*
 *  Lecture analogique de la sortie sense des PHs
 *    int entre 0 et 1024
 *    sert au contrôle du courant
 */
int adc_sense1 (void); // PH 1
int adc_sense2 (void); // PH 2


#endif
