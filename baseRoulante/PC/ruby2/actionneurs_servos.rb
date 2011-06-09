require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

#Modèles et tests pour les servomoteurs des aimants

InterfaceActionneurs.positionAimantGauche(LibChessUp::BAS);
InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);

sleep(1);

InterfaceActionneurs.positionAimantGauche(LibChessUp::HAUT);
InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT);
