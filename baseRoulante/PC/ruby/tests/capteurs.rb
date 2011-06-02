require '../libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceCapteurs = LibChessUp::InterfaceCapteurs.Instance();

puts "Debut des tests capteurs"


print "Premiere valeur ultrason : "
puts InterfaceCapteurs.DistanceUltrason();

print "Deuxieme valeur ultrason : "
puts InterfaceCapteurs.DistanceUltrason();

print "Troisieme valeur ultrason : "
puts InterfaceCapteurs.DistanceUltrason();

print "Quatrieme valeur ultrason : "
puts InterfaceCapteurs.DistanceUltrason();

print "Etat capteur bras gauche : "
puts InterfaceCapteurs.EtatBras(LibChessUp::BGAUCHE);

print "Etat capteur bras droit : "
puts InterfaceCapteurs.EtatBras(LibChessUp::BDROITE);

print "Etat du capteur central : "
puts IntefaceCapteurs.EtatCentre();

print "Etat du jumper : "
puts InterfaceCapteurs.EtatJumper();

print "Derniere valeur lue par le lecteur de codes barres : "
puts InterfaceCapteurs.LecteurCB();