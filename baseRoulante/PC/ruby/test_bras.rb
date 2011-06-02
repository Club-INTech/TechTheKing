require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

sleep(1);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::SOCLE);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::SOCLE);

sleep(1);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);

sleep(1);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::MILIEU);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::MILIEU);

sleep(1);

InterfaceActionneurs.hauteurBrasDroit(LibChessUp::TOUR);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::TOUR);

sleep(1);

InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);
InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE); 

sleep(1);

InterfaceActionneurs.angleBrasDroit(LibChessUp::CENTRE);
InterfaceActionneurs.angleBrasDroit(LibChessUp::CENTRE); 

sleep(1);

InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT);
InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT); 

sleep(1);

InterfaceActionneurs.angleBrasDroit(LibChessUp::BALAYAGE);
InterfaceActionneurs.angleBrasDroit(LibChessUp::BALAYAGE); 

sleep(1);

InterfaceActionneurs.angleBrasDroit(LibChessUp::EXTERIEUR);
InterfaceActionneurs.angleBrasDroit(LibChessUp::EXTERIEUR); 

sleep(1);

InterfaceActionneurs.angleBrasDroit(LibChessUp::EXTERIEUR);
InterfaceActionneurs.angleBrasDroit(LibChessUp::EXTERIEUR); 
