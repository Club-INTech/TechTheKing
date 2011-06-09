require '../libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);

InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);

sleep(2);

InterfaceActionneurs.angleBrasGauche(LibChessUp::REPLIE);
InterfaceActionneurs.angleBrasGauche(LibChessUp::REPLIE);

InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);
InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);

InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);
InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);