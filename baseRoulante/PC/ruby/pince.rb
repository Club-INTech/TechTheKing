require '../libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;


puts "Pincer pion : initialisation"
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::SOCLE);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::SOCLE);
InterfaceActionneurs.angleBrasDroit(LibChessUp::BALAYAGE);
InterfaceActionneurs.angleBrasGauche(LibChessUp::BALAYAGE); 

sleep(1);

# ATTENTION, peut être que les AX12 forcent trop, à vérifier
puts "Pincer pion : serrage"
InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT);
InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT);

sleep(1);

puts "Pince pion : levage"
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::MILIEU);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::MILIEU);

#ensuite, à placer au dessus d'un autre pion (lolilol) et désserrer en donnant un angle balayage
#on peut aussi faire une pile avec un aimant, il suffit de lever plus haut le pion capturé