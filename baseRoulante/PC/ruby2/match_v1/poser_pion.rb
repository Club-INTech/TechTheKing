require '../libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);

sleep(2);

InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT);
InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT);


sleep(2);

InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);
InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);