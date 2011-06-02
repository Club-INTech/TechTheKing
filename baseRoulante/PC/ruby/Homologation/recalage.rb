require '../libChessUp'

include LibChessUp

COULEUR_ROBOT=BLEU;

InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();

InterfaceAsservissement.pwmMaxTranslation(70);
InterfaceAsservissement.pwmMaxRotation(0);
InterfaceAsservissement.reculer(500);
InterfaceAsservissement.pwmMaxRotation(200);

if(COULEUR_ROBOT=BLEU)
	InterfaceAsservissement.setXRobot(3000-DEMI_LARGEUR_ROBOT);
end
if(COULEUR_ROBOT=ROUGE)
	InterfaceAsservissement.setXRobot(DEMI_LARGEUR_ROBOT);
end

InterfaceAsservissement.avancer(500);

if(COULEUR_ROBOT=BLEU)
	InterfaceAsservissement.tourner(-Math::PI/2);
end
if(COULEUR_ROBOT=ROUGE)
	InterfaceAsservissement.tourner(Math::PI/2);
end
InterfaceAsservissement.pwmMaxRotation(0);
InterfaceAsservissement.reculer(500);

InterfaceAsservissement.setYRobot(2100-DEMI_LARGEUR_ROBOT);

InterfaceAsservissement.pwmMaxRotation(200);
InterfaceAsservissement.avancer(120);
InterfaceAsservissement.tourner(0);
InterfaceAsservissement.reculer(300);

