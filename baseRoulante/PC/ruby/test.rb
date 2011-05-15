require 'libChessUp'

InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();
LibChessUp::initialisation();
InterfaceAsservissement.goTo(LibChessUp::Point.new(2000,1000),80);
InterfaceAsservissement.debugConsignes();
InterfaceAsservissement.debugGraphique();
