require 'libChessUp'

InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();
#LibChessUp::initialisation();
InterfaceAsservissement.goTo(LibChessUp::Point.new(1800,300),80);
#InterfaceAsservissement.debugConsignes();
InterfaceAsservissement.debugGraphique();
