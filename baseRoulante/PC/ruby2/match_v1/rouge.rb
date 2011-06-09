require '../libChessUp'
include LibChessUp
setCouleurRobot(ROUGE);

$priseBrasGauche=true
$priseBrasDroit=false
$priseBrasGaucheDroit=false
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
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::SOCLE);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::TOUR);

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
	if( !InterfaceCapteurs.EtatCentre() && $priseBrasGauche )
		$priseBrasGauche = false
		InterfaceActionneurs.hauteurBrasGauche(LibChessUp::CAPTURE)
	        sleep(1)
	        InterfaceActionneurs.hauteurBrasGauche(LibChessUp::MILIEU)
	        sleep(1)
		InterfaceActionneurs.angleBrasGauche(LibChessUp::BALAYAGE)
        	sleep(0.5)
		InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR)
		InterfaceActionneurs.angleBrasDroit(LibChessUp::CENTRE)
		sleep(0.5)
		$priseBrasDroit = true
	end
	if( !InterfaceCapteurs.EtatCentre() && $priseBrasDroit )
		$priseBrasDroit = false
		InterfaceActionneurs.angleBrasDroit(LibChessUp::CENTRE)
		sleep(0.5)
		InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE)
		sleep(0.5)
#	        InterfaceActionneurs.hauteurBrasDroit(LibChessUp::MILIEU)
#	        sleep(1)
#	        InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT)
#	        sleep(1)
#		InterfaceActionneurs.hauteurBrasDroit(LibChessUp::TOUR)
		$priseBrasGaucheDroit=true
	end
	if( InterfaceCapteurs.DistanceUltrason() <= 7500 )
		$obstacle = true
		puts "EVITEMENT"
	end
end

puts "Attente du jumper"
InterfaceAsservissement.reculer(100);
InterfaceCapteurs.gestionJumper()
#Initialisation des bras.
puts "Repli des bras";
InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT);
InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT);

InterfaceAsservissement.avancer(720)
puts "Initialisation de la hauteur des bras"

sleep(0.5)
InterfaceAsservissement.tourner(Math::PI/2)
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::TOUR)
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);
InterfaceActionneurs.angleBrasGauche(LibChessUp::CENTRE);
i=0
while i != 8 && !$priseBrasDroit
	while ($obstacle || InterfaceCapteurs.DistanceUltrason() <= 7500)
		InterfaceAsservissement.reculer(100);
		sleep(1)
		InterfaceAsservissement.avancer(100);
		$obstacle = false
	end
	megagoto()
	i+=1
end
if i != 8
	InterfaceAsservissement.avancer(175*(8-i))
end
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE)
InterfaceAsservissement.tourner(3*Math::PI/4)
InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT)
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::MILIEU)
InterfaceAsservissement.avancer(150)
InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT)
InterfaceAsservissement.reculer(150)
InterfaceAsservissement.tourner(0)
InterfaceAsservissement.avancer(700)
InterfaceAsservissement.tourner(5*Math::PI/8)
InterfaceActionneurs.angleBrasGauche(LibChessUp::CENTRE)
sleep(0.5)
InterfaceActionneurs.positionAimantGauche(LibChessUp::HAUT)
sleep(0.5)
InterfaceAsservissement.avancer(200)
InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT)
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
InterfaceAsservissement.tourner(-Math::PI/2)
InterfaceAsservissement.goTo(Point.new(1150,1900),80)
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

