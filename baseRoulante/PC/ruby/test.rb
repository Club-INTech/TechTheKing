require 'libChessUp'

InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();
LibChessUp::initialisation();
InterfaceAsservissement.goTo(LibChessUp::Point.new(2600,150),100);
InterfaceAsservissement.debugConsignes();
#InterfaceAsservissement.debugGraphique();
