
#include "Manager.h"

#define CONVERSION_RADIAN_TIC 1356.00012 // (1/CONVERSION_TIC_RADIAN)
#define CONVERSION_TIC_RADIAN 0.000737463064 // 2Pi/8520
#define CONVERSION_RADIAN_ANGLE 65536.1858 // 102944/(Pi/2)
#define CONVERSION_TIC_ANGLE 48.3305164 // CONVERSION_TIC_RADIAN*CONVERSION_RADIAN_ANGLE
#define CONVERSION_TIC_MM 1.04195690364
#define CONVERSION_TIC_DISTANCE 0.00000158990006049 // CONVERSION_TIC_MM/65536
#define CONVERSION_COURBURE_TIC_MM  0.00215590466574 // CONVERSION_TIC_MM/(CONVERSION_TIC_RADIAN*65536)
#define ABS(x) ( (x) > 0 ? (x) : -(x))

volatile double x;
volatile double y;

Couleur CouleurRobot = BLEU;

void
Manager::assPolaire(){
    int32_t infos[2];
    int32_t distance,angle;
    int16_t delta_angle, delta_distance;
    double r;
    get_all(infos);
    distance = infos[0];
    angle = infos[1];
    
    delta_angle = angle - angleBkp;
    delta_distance = distance - distanceBkp;
    
    if(delta_angle==0)
    {
        x -= ( delta_distance * CONVERSION_TIC_DISTANCE * fp_cos( CONVERSION_TIC_ANGLE * angleBkp ) );
        y += ( delta_distance * CONVERSION_TIC_DISTANCE * fp_sin( CONVERSION_TIC_ANGLE * angleBkp ) );
    }
    else
    {
        r = CONVERSION_COURBURE_TIC_MM * (double)delta_distance/(double)delta_angle;
        x-= r * (-fp_sin(CONVERSION_TIC_ANGLE * angle) + fp_sin(CONVERSION_TIC_ANGLE * angleBkp));
        y+= r * (fp_cos(CONVERSION_TIC_ANGLE * angle) - fp_cos(CONVERSION_TIC_ANGLE * angleBkp));
        //printlnLong(r*(-fp_sin(CONVERSION_TIC_ANGLE * angle) + fp_sin(CONVERSION_TIC_ANGLE * angleBkp)));
    }

    // R�actualisation des vitesses du robot
    assRotation.setVitesse((angle-angleBkp));
    assTranslation.setVitesse((distance-distanceBkp));
    
    
    angleBkp = angle;
    distanceBkp = distance;

    /*
    * On changera de consigne si :
    *   -on est suffisament proche de la consigne en distance
    *   -on est suffisament aligné avec la consigne
    * Ceci ne s'applique pas à la dernière consigne
    */

    if(consigneActuelle>1 && consigneActuelle==tableauConsignes.nbConsignes)
    {
        tableauConsignes.listeConsignes[0].angle = tableauConsignes.listeConsignes[tableauConsignes.nbConsignes-1].angle;
        tableauConsignes.listeConsignes[0].distance = tableauConsignes.listeConsignes[tableauConsignes.nbConsignes-1].distance;
        tableauConsignes.nbConsignes = 1;
        consigneActuelle = 1;
        printChar('f');
    }
    
    
    
    /*
    if(consigneActuelle ==tableauConsignes.nbConsignes){
        assRotation.setActivationKd(1);
        assTranslation.setActivationKd(1);
    }
    else
    {
        assRotation.setActivationKd(0);
        assTranslation.setActivationKd(0);
    }
    */
    
    if( ABS(tableauConsignes.listeConsignes[consigneActuelle-1].distance - distance) < 0.5 * (tableauConsignes.listeConsignes[consigneActuelle+1].distance - tableauConsignes.listeConsignes[consigneActuelle-1].distance)) {
            if( consigneActuelle < tableauConsignes.nbConsignes){
                    consigneActuelle++;
            }
    }
    
    /*
    *Calcul des PWM
    */
    int16_t pwmRotation = (activationAssAngle?assRotation.calculePwm(((tableauConsignes.listeConsignes)[consigneActuelle-1]).angle,angle):0);
    int16_t pwmTranslation = (activationAssDistance?assTranslation.calculePwm(((tableauConsignes.listeConsignes)[consigneActuelle-1]).distance,distance):0);


    //Blocage
    if( (ABS(pwmTranslation)>0 && ABS(tableauConsignes.listeConsignes[consigneActuelle-1].distance - distance)>0)
        || ( ABS(pwmRotation)>0 && ABS(tableauConsignes.listeConsignes[consigneActuelle-1].angle - angle)>0 ) ){
        tableauConsignes.listeConsignes[0].angle = angle;
        tableauConsignes.listeConsignes[0].distance = distance;
        tableauConsignes.nbConsignes = 1;
        consigneActuelle = 1;
        printChar('f');
    }
    /*
    if(pwmTranslation!=0 && (distance==distanceBkp)){
        resetListeConsignes();
        tableauConsignes.listeConsignes[consigneActuelle-1].distance-=200;
    }
    */
    
    int16_t pwmG = pwmTranslation - pwmRotation;
    int16_t pwmD = pwmTranslation + pwmRotation;
    
    
    /*
    * Envoi des PWM
    */  
    if (pwmG > PWM_MAX)
        pwmG = PWM_MAX;
    else if (pwmG < -PWM_MAX)
        pwmG = -PWM_MAX;

    if (pwmD > PWM_MAX)
        pwmD = PWM_MAX;
    else if (pwmD < -PWM_MAX)
        pwmD = -PWM_MAX;
    
    if (pwmG > 0) {
        // Direction gauche = 0
        // PWM gauche = pwmG
        PORTD |= PINDIR1;
        MOTEUR1 = pwmG;
    }
    else {
        // Direction gauche = 1
        // PWM gauche = -pwmG
        PORTD &= ~PINDIR1;
        MOTEUR1 = -pwmG;
    }
    
    
    if (pwmD > 0) {
        // Direction droite = 0
        // PWM droite = pwmD
        PORTB |= PINDIR2;
        MOTEUR2 = pwmD;
    }
    else {
        // Direction droite = 1
        // PWM droite = -pwmD
        PORTB &= ~PINDIR2;
        MOTEUR2 = -pwmD;
    }
}

/*
* Initialisation des pins
*/
Manager::Manager(){
}

void Manager::resetListeConsignes(){
        tableauConsignes.listeConsignes[0].angle = tableauConsignes.listeConsignes[tableauConsignes.nbConsignes-1].angle;
        tableauConsignes.listeConsignes[0].distance = tableauConsignes.listeConsignes[tableauConsignes.nbConsignes-1].distance;
        tableauConsignes.nbConsignes = 1;
        consigneActuelle = 1;
        printlnChar('f');
}


void Manager::stop(){
    tableauConsignes.listeConsignes[0].angle = angleBkp;
    tableauConsignes.listeConsignes[0].distance = distanceBkp;
    tableauConsignes.nbConsignes = 1;
    consigneActuelle = 1;
}
void Manager::init()
{
    if(CouleurRobot==BLEU){
        x=2760;
        y=1940;
    }
    else{
        x=240;
        y=1940;
    }
    
    distanceBkp=0;
    angleBkp=0;
    
    assRotation.setActivationKd(1);
    assTranslation.setActivationKd(1);

    activationAssDistance = true;
    activationAssAngle = true;

    // Initialisation PWM pour le PH sur timer0 (moteur 2)
    // Initialisation pin 12
    DDRD |= ( 1 << PORTD6 );
    TCCR0A &= ~( 1 << COM0A0);
    TCCR0A |=  ( 1 << COM0A1 );
    // Fast PWM
    TCCR0A |= ( 1 << WGM00 );
    TCCR0A |= ( 1 << WGM01 );
    TCCR0B &= ~( 1 << WGM02 );
    // Pas de prescaler
    TCCR0B |= ( 1 << CS00 );

    // Initialisation PWM pour le PH sur timer2 (moteur 1)
    // Initialisation pin 6
    DDRD |= ( 1 << PORTD3 );
    TCCR2A &= ~( 1 << COM2B0 );
    TCCR2A |= ( 1 << COM2B1 );
    // Fast PWM
    TCCR2A |= ( 1 << WGM20 );
    TCCR2A |= ( 1 << WGM21 );
    TCCR2B &= ~( 1 << WGM22 );
    // Pas de prescaler
    TCCR2B |= ( 1 << CS20 );

    // Pins de direction des PH
    DDRD |= ( 1 << PORTD4 );
    DDRB |= ( 1 << PORTB0 );

    // Initialisation ADC
    // ADCSRA |= (1 << ADEN);

    // Timer de l'asservissement (16bit, 20 MHz)
    TIMSK1 |= (1 << TOIE1);
    //prescaler 8
    TCCR1B |= (1 << CS11);
   //prescaler 64
   //TCCR1B |= (1 << CS11) | (1 << CS10 );
    
    // initialisation de la liste de point
    tableauConsignes.nbConsignes=1;
    consigneActuelle=1;

    // initialisation des constantes
    assRotation.changeKp(5);
    assRotation.changePWM(PWM_MAX);
    assRotation.changeKd(300);
    assRotation.changeKi(0);
    assRotation.changeVmax(0);
    assRotation.changeKpVitesse(0);

    assTranslation.changeKp(3);
    assTranslation.changePWM(PWM_MAX);
    assTranslation.changeKd(200);
    assTranslation.changeKi(0);
    assTranslation.changeVmax(0);
    assTranslation.changeKpVitesse(0);

    distanceTotale=0;
    angleTotal=0;
    angleBkp=0;
    distanceBkp=0;

}

void
Manager::changeIemeConsigne(int32_t angleDonne, int32_t distanceDonnee,int16_t i)
{
    (tableauConsignes.listeConsignes[i-1]).distance=distanceDonnee;
    (tableauConsignes.listeConsignes[i-1]).angle=angleDonne;
}


/*
* Fonctions utiles au transfert de la liste de points via la liaison série.
*/
void
Manager::setNbConsignes(int16_t nbConsignesDonne)
{
    tableauConsignes.nbConsignes=nbConsignesDonne;
}

void 
Manager::changeIemeConsigneDistance(int32_t distanceDonnee, int16_t i){
    (tableauConsignes.listeConsignes[i-1]).distance=distanceDonnee;
}

void
Manager::changeIemeConsigneAngle(int32_t angleDonne, int16_t i)
{
    (tableauConsignes.listeConsignes[i-1]).angle=angleDonne;
}

void
Manager::reset()
{
    tableauConsignes.nbConsignes=1;
    consigneActuelle=1;
    send_reset();
}
/*
* A voir, si on peut envoyer via un int32_t à la fois la distance et l'angle.
* Diviserait par environ deux le temp de chargement de la liste de points en série.
*/
void 
Manager::pushConsigneDistance(int32_t distanceDonnee) // on transfert d'abord la distance (pas d'incr�mentation de nbConsignes)
{
    tableauConsignes.nbConsignes++;
    changeIemeConsigneDistance(distanceDonnee, (tableauConsignes.nbConsignes));
}

void 
Manager::pushConsigneAngle(int32_t angleDonne)
{
    changeIemeConsigneAngle(angleDonne, (tableauConsignes.nbConsignes) );
}

/*
* Change les asservissements d'état
*/
void 
Manager::switchAssDistance()
{
    activationAssDistance = !activationAssDistance;
}

void 
Manager::switchAssAngle(){
    activationAssAngle = !activationAssAngle;
}

ISR(TIMER1_OVF_vect, ISR_NOBLOCK){
    manager.assPolaire();
}

Manager manager;
