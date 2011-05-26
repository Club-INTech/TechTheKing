require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

#Initialisation position bras
InterfaceActionneurs.hauteurBrasDroit(75);
InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);

sleep(5);

#Une fois arrivé au centre
InterfaceActionneurs.angleBrasDroit(75);
InterfaceActionneurs.hauteurBrasDroit(30);
sleep(2);
InterfaceActionneurs.hauteurBrasDroit(75);
InterfaceActionneurs.angleBrasDroit(30);

sleep(5);

#Une fois à destination finale
InterfaceActionneurs.hauteurBrasDroit(30);
InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT);