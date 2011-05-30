require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceCapteurs = LibChessUp::InterfaceCapteurs.new;

#InterfaceCapteurs.ouvrirThread();
#Modèles et tests pour les capteurs

#while(1)

#end;
puts InterfaceCapteurs.DistanceUltrason();

puts InterfaceCapteurs.EtatBras(LibChessUp::BGAUCHE);

puts InterfaceCapteurs.EtatBras(LibChessUp::BDROITE);

puts InterfaceCapteurs.EtatJumper();

puts InterfaceCapteurs.LecteurCB();
