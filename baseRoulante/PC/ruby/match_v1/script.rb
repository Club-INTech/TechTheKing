require '../libChessUp'
include LibChessUp

LibChessUp.ouvrir_adaptateur_i2c();

#Création des interfaces
InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;
InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();
InterfaceCapteurs = LibChessUp::InterfaceCapteurs.Instance();

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
InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT);
InterfaceActionneurs.positionAimantGauche(LibChessUp::HAUT);

puts "Recalage";
require 'recalage.rb'


#############################################################
################# DEBUT DU MATCH ############################
#############################################################

puts "Attente du jumper";
InterfaceCapteurs.gestionJumper();

#Démarrage des capteurs
InterfaceCapteurs.ouvrirThread();

puts "Démarrage";

puts "Go To 1500 1050";
InterfaceAsservissement.goTo(Point.new(1500,1050),80);

puts "Prise de pion";
sleep(0.5);

InterfaceActionneurs.angleBrasDroit(LibChessUp::CENTRE);

sleep(1);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::MILIEU);

sleep(1);


if (getCouleurRobot() == ROUGE)
	InterfaceAsservissement.goTo(Point.new(1500,350),80);
	InterfaceAsservissement.tourner(Math::PI/2);
else
	InterfaceAsservissement.goTo(Point.new(1500,350),80);
end

puts "Lacher pion";
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);

sleep(1.5);

InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);
InterfaceActionneurs.positionAimantGauche(LibChessUp::BAS);

sleep(1);

InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);

#############################################################
################# DEPLACEMENT RANDOM ########################
#############################################################

puts "Déplacement random jusqu'à trouver un pion"

InterfaceAsservissement.reculer(300);

ajoutPion();

if (getCouleurRobot() == ROUGE)
	InterfaceAsservissement.goTo(Point.new(2500,1100),80);
	InterfaceAsservissement.goTo(Point.new(500,1100),80);
	InterfaceAsservissement.goTo(Point.new(1500,1800),80);
else
	InterfaceAsservissement.goTo(Point.new(500,1100),80);
	InterfaceAsservissement.goTo(Point.new(2500,1100),80);
	InterfaceAsservissement.goTo(Point.new(1500,1800),80);
end

puts "Arrêt du robot"
#InterfaceAsservissement.stopAll();
InterfaceActionneurs.arret();
