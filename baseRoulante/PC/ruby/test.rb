require 'libChessUp'

InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();
InterfaceAsservissement.goTo(LibChessUp::Point.new(100,100),90);
