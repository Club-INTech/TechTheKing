require '../libChessUp'
include LibChessUp
setCouleurRobot(BLEU);

$priseBrasDroit=true
$priseBrasGauche=false
$priseBrasDroitGauche=false
$obstacle = false

LibChessUp.ouvrir_adaptateur_i2c();

#Création des interfaces
InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;
InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();
InterfaceCapteurs = LibChessUp::InterfaceCapteurs.Instance();

InterfaceAsservissement.Open();

#initialisation des $obstacles
LibChessUp::initialisation();

#Initialisation des bras.
puts "Initialisation de la hauteur des bras";
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::SOCLE);
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
def evitement()
	while ( InterfaceCapteurs.DistanceUltrason() <= 7500)
		InterfaceAsservissement.reculer(100);
		sleep(1)
		InterfaceAsservissement.avancer(100);
		$obstacle = false
	end
end

def megagoto()
	InterfaceAsservissement.avancer(175)
	if( !InterfaceCapteurs.EtatCentre() && $priseBrasDroit )
		$priseBrasDroit = false
		InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE)
	        sleep(1)
	        InterfaceActionneurs.hauteurBrasDroit(LibChessUp::MILIEU)
	        sleep(1)
		InterfaceActionneurs.angleBrasDroit(LibChessUp::BALAYAGE)
        	sleep(0.5)
		InterfaceActionneurs.hauteurBrasDroit(LibChessUp::TOUR)
		InterfaceActionneurs.angleBrasGauche(LibChessUp::CENTRE)
		sleep(0.5)
		$priseBrasGauche = true
	end
	if( !InterfaceCapteurs.EtatCentre() && $priseBrasGauche )
		$priseBrasGauche = false
		InterfaceActionneurs.angleBrasGauche(LibChessUp::CENTRE)
		sleep(0.5)
		InterfaceActionneurs.hauteurBrasGauche(LibChessUp::CAPTURE)
		sleep(0.5)
#	        InterfaceActionneurs.hauteurBrasGauche(LibChessUp::MILIEU)
#	        sleep(1)
#	        InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT)
#	        sleep(1)
#		InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR)
		$priseBrasDroitGauche=true
	end
	if( InterfaceCapteurs.DistanceUltrason() <= 7500 )
		$obstacle = true
		puts "EVITEMENT"
	end
end

puts "Attente du jumper";
InterfaceAsservissement.reculer(100);
InterfaceCapteurs.gestionJumper();
#Initialisation des bras.
puts "Repli des bras";
InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT);
InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT);

InterfaceAsservissement.avancer(720);
puts "Initialisation de la hauteur des bras"
InterfaceAsservissement.tourner(-Math::PI/2);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::TOUR);
InterfaceActionneurs.angleBrasDroit(LibChessUp::CENTRE);
i=0
while (i != 8) && !$priseBrasGauche
	while ($obstacle || InterfaceCapteurs.DistanceUltrason() <= 7500)
		InterfaceAsservissement.reculer(100);
		sleep(1)
		InterfaceAsservissement.avancer(100);
		$obstacle = false
	end
	megagoto()
	i+=1
	puts i
	puts $priseBrasGauche
end
$priseBrasGauche=false
puts "suite"
if i != 8
	InterfaceAsservissement.avancer(175*(8-i))
end
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::CAPTURE)
InterfaceAsservissement.tourner(-3*Math::PI/4)
InterfaceActionneurs.positionAimantGauche(LibChessUp::HAUT)
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::MILIEU)
InterfaceAsservissement.avancer(150)
InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT)
InterfaceAsservissement.reculer(150)
InterfaceAsservissement.tourner(0)
InterfaceAsservissement.avancer(700)
InterfaceAsservissement.tourner(-5*Math::PI/8)
InterfaceActionneurs.angleBrasDroit(LibChessUp::CENTRE)
sleep(0.5)
InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT)
sleep(0.5)
InterfaceAsservissement.avancer(200)
InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT)
InterfaceAsservissement.reculer(200)

InterfaceAsservissement.tourner(Math::PI)
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::SOCLE);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);

sleep(0.5);

puts "Repli des bras";
InterfaceActionneurs.angleBrasGauche(LibChessUp::REPLIE);
InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);

puts "Abaissement de l'aimant";
InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);
InterfaceActionneurs.positionAimantGauche(LibChessUp::BAS);
InterfaceAsservissement.avancer(350)
InterfaceAsservissement.tourner(Math::PI/2)
InterfaceAsservissement.goTo(Point.new(1850,1900),80)
InterfaceAsservissement.tourner(0)
InterfaceAsservissement.avancer(175)
if( InterfaceCapteurs.DistanceUltrason() <= 7500 )
	evitement()
end
InterfaceAsservissement.avancer(175)
if( InterfaceCapteurs.DistanceUltrason() <= 7500 )
	evitement()
end
InterfaceAsservissement.avancer(175)
if( InterfaceCapteurs.DistanceUltrason() <= 7500 )
	evitement()
end
InterfaceAsservissement.avancer(175)
if( InterfaceCapteurs.DistanceUltrason() <= 7500 )
	evitement()
end
InterfaceAsservissement.avancer(175)
if( InterfaceCapteurs.DistanceUltrason() <= 7500 )
	evitement()
end
InterfaceAsservissement.avancer(175)
if( InterfaceCapteurs.DistanceUltrason() <= 7500 )
	evitement()
end

InterfaceAsservissement.reculer(175)
if( InterfaceCapteurs.DistanceUltrason() <= 7500 )
	evitement()
end
