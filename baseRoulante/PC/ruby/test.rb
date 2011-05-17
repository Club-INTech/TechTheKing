require 'libChessUp'

InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance(50);
LibChessUp::initialisation();
#InterfaceAsservissement.avancer(1050);
InterfaceAsservissement.goTo(LibChessUp::Point.new(2000,700),80);
sleep(2);
InterfaceAsservissement.goTo(LibChessUp::Point.new(1500,500),80);
InterfaceAsservissement.debugConsignes();
InterfaceAsservissement.debugGraphique();
