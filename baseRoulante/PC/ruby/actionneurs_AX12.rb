require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

#Modèles et tests pour les servomoteurs AX12

InterfaceActionneurs.angleBrasGauche(100);
InterfaceActionneurs.angleBrasDroit(0);

sleep(1);

InterfaceActionneurs.angleBrasGauche(50);
InterfaceActionneurs.angleBrasDroit(50);

sleep(1);

InterfaceActionneurs.angleBrasGauche(0);
InterfaceActionneurs.angleBrasDroit(100)