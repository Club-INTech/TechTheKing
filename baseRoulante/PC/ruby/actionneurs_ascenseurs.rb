require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;

#Modèles et tests pour les ascenseurs

InterfaceActionneurs.hauteurDeuxBras(LibChessUp::MILIEU);
InterfaceActionneurs.hauteurDeuxBras(LibChessUp::MILIEU);







