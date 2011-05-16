require 'libChessUp'

InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();
LibChessUp::initialisation();
InterfaceAsservissement.goTo(LibChessUp::Point.new(625,1000),80);
#InterfaceAsservissement.debugConsignes();
InterfaceAsservissement.debugGraphique();
