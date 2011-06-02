require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

sleep(1);

InterfaceActionneurs.positionAimantGauche(LibChessUp::BAS);
InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);

sleep(1);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::SOCLE);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::SOCLE);

sleep(1);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::CAPTURE);

sleep(1);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::TOUR);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);

sleep(1);

InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT);
InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT); 

sleep(1);

InterfaceActionneurs.angleBrasDroit(LibChessUp::BALAYAGE);
InterfaceActionneurs.angleBrasGauche(LibChessUp::BALAYAGE); 

sleep(1);

InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT);
InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT);

sleep(1);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::SOCLE);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::SOCLE);

sleep(1);


InterfaceActionneurs.positionAimantGauche(LibChessUp::HAUT);
InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT);
