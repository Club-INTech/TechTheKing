#require '../libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

sleep(2);

InterfaceActionneurs.angleBrasDroit(LibChessUp::CENTRE);
InterfaceActionneurs.angleBrasDroit(LibChessUp::CENTRE);

sleep(2);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);
