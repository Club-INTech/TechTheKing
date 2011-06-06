require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

#Modèles et tests pour les servomoteurs AX12

InterfaceActionneurs.angleBrasGauche(LibChessUp::CENTRE);
InterfaceActionneurs.angleBrasGauche(LibChessUp::CENTRE);