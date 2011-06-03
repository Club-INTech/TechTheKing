require '../libChessUp'
include LibChessUp

LibChessUp.ouvrir_adaptateur_i2c();

#Création des interfaces
InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;
InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();
InterfaceCapteurs = LibChessUp::InterfaceCapteurs.Instance();

InterfaceAsservissement.Open();

#initialisation des obstacles
LibChessUp::initialisation();

#Initialisation des bras.
puts "Initialisation de la hauteur des bras";
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);

sleep(0.5);

puts "Repli des bras";
InterfaceActionneurs.angleBrasGauche(LibChessUp::REPLIE);
InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);

puts "Abaissement de l'aimant";
InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);
InterfaceActionneurs.positionAimantGauche(LibChessUp::BAS);

puts "Recalage";
require 'recalage.rb'


#############################################################
################# DEBUT DU MATCH ############################
#############################################################

puts "Attente du jumper";
InterfaceCapteurs.gestionJumper();

InterfaceAsservissement.avancer(300);

#Démarrage des capteurs
InterfaceCapteurs.ouvrirThread();

puts "Démarrage";

puts "Go To 1500 1050";
InterfaceAsservissement.goTo(Point.new(1500,1050),80);

puts "Prise de pion";
sleep(0.5);

InterfaceActionneurs.angleBrasDroit(LibChessUp::CENTRE);

sleep(0.5);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::MILIEU);

sleep(0.5);


if(getCouleurRobot()==BLEU)
        InterfaceAsservissement.tourner(-Math::PI/2);
end
if(getCouleurRobot()==ROUGE)
        InterfaceAsservissement.tourner(Math::PI/2);
end

InterfaceAsservissement.avancer(700);

if(getCouleurRobot()==BLEU)
        InterfaceAsservissement.tourner(-3*Math::PI/4);
end
if(getCouleurRobot()==ROUGE)
        InterfaceAsservissement.tourner(3*Math::PI/4);
end

puts "Lacher pion";
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);

sleep(0.5);

InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT);
InterfaceActionneurs.positionAimantGauche(LibChessUp::HAUT);

sleep(0.5);

InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);

#############################################################
################# DEPLACEMENT RANDOM ########################
#############################################################

puts "Déplacement random jusqu'à trouver un pion"

InterfaceAsservissement.reculer(300);
InterfaceAsservissement.tourner(0);

ajoutPion();

tabPosRouge = [Point.new(2500,1100),Point.new(2500,1100),Point.new(2500,1100)]
tabPosBleu = [Point.new(2500,1100),Point.new(2500,1100),Point.new(2500,1100)]
i = 0

# Mouvement jusqu'à trouver un pion
while (InterfaceCapteurs.EtatCentre())
	InterfaceAsservissement.goTo(tabPos[i],80);
	i += 1
end

# Pion détecté au centre
if (InterfaceCapteurs.EtatCentre() == false)

	# Déplacement sur la case bonus
	if(getCouleurRobot()==BLEU)
        InterfaceAsservissement.goTo(Point.new(,),80);
	end
	if(getCouleurRobot()==ROUGE)
		InterfaceAsservissement.goTo(Point.new(,),80);
	end
	
	# Dépose le pion
	
end


puts "Arrêt du robot"
#InterfaceAsservissement.stopAll();
InterfaceActionneurs.arret();
