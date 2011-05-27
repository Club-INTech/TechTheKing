require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

#Initialisation position bras
InterfaceActionneurs.hauteurBrasGauche(30);
InterfaceActionneurs.positionAimantGauche(LibChessUp::BAS);

sleep(5);

#Une fois arrivé au centre
InterfaceActionneurs.angleBrasGauche(75);
InterfaceActionneurs.hauteurBrasGauche(30);
sleep(2);
InterfaceActionneurs.hauteurBrasGauche(75);
InterfaceActionneurs.angleBrasGauche(30);

sleep(5);

#Une fois à destination finale
InterfaceActionneurs.hauteurBrasGauche(30);
InterfaceActionneurs.positionAimantGauche(LibChessUp::HAUT);
