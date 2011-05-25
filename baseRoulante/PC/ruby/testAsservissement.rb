require 'libChessUp'
include LibChessUp

<<<<<<< HEAD
InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance(50);
LibChessUp::initialisation();
#InterfaceAsservissement.avancer(1050);
#InterfaceAsservissement.goTo(LibChessUp::Point.new(2000,700),80);
#sleep(2);
InterfaceAsservissement.goTo(LibChessUp::Point.new(2700,1500),80);
InterfaceAsservissement.debugConsignes();
#InterfaceAsservissement.debugGraphique();
=======
puts(Constantes.TAILLE_ROBOT);
InterfaceAsservissement = InterfaceAsservissement.Instance(50);
initialisation();
InterfaceAsservissement.avancer(1050);
InterfaceAsservissement.goTo(Point.new(2000,700),80);
sleep(2);
InterfaceAsservissement.goTo(Point.new(1500,500),50);
InterfaceAsservissement.debugConsignes();
InterfaceAsservissement.debugGraphique();
>>>>>>> 275a4490d94d7916e82667454a6c2c293af1b5b0
