require 'libChessUp'

InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();
LibChessUp::initialisation();
InterfaceAsservissement.goTo(LibChessUp::Point.new(200,200),90);
InterfaceAsservissement.debugGraphique();
