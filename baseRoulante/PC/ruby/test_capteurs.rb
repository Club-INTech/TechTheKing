require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceCapteurs = LibChessUp::InterfaceCapteurs.new;

#Modèles et tests pour les capteurs

print InterfaceCapteurs.DistanceUltrason();
