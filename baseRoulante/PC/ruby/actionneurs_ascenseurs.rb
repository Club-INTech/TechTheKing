require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

#Modèles et tests pour les ascenseurs

InterfaceActionneurs.hauteurBrasGauche(0);
InterfaceActionneurs.hauteurBrasDroit(100);

sleep(2);

InterfaceActionneurs.hauteurBrasGauche(100);
InterfaceActionneurs.hauteurBrasDroit(0);

sleep(2);

InterfaceActionneurs.hauteurDeuxBras(50);

sleep(2);

InterfaceActionneurs.recalage();