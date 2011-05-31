require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

#Modèles et tests pour les servomoteurs AX12


InterfaceActionneurs.hauteurBrasGauche(LibChessUp::MILIEU);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::MILIEU);

sleep(1);

InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT);
InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT);

sleep(2);


InterfaceActionneurs.angleBrasGauche(LibChessUp::CENTRE);
InterfaceActionneurs.angleBrasGauche(LibChessUp::CENTRE);

sleep(1);

InterfaceActionneurs.hauteurBrasGauche(LibChessUp::CAPTURE);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::CAPTURE);

sleep(1);

InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);