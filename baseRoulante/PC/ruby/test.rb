require 'libChessUp'

InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance(50);
LibChessUp::initialisation();
InterfaceAsservissement.goTo(LibChessUp::Point.new(200,1900),80);
#InterfaceAsservissement.debugConsignes();
InterfaceAsservissement.debugGraphique();
