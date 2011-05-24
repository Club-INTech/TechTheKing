require 'libChessUp'
include LibChessUp

puts(Constantes.TAILLE_ROBOT);
InterfaceAsservissement = InterfaceAsservissement.Instance(50);
initialisation();
InterfaceAsservissement.avancer(1050);
InterfaceAsservissement.goTo(Point.new(2000,700),80);
sleep(2);
InterfaceAsservissement.goTo(Point.new(1500,500),50);
InterfaceAsservissement.debugConsignes();
InterfaceAsservissement.debugGraphique();
