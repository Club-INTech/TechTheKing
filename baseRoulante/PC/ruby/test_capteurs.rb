require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceCapteurs = LibChessUp::InterfaceCapteurs.new;

#Modèles et tests pour les capteurs

puts InterfaceCapteurs.DistanceUltrason();

puts InterfaceCapteurs.EtatBras(LibChessUp::BGAUCHE);

puts InterfaceCapteurs.EtatBras(LibChessUp::BDROITE);

puts InterfaceCapteurs.LecteurCB();