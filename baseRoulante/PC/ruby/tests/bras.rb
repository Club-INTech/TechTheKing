require '../libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;



puts "Test des bras : initialisation"
InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT);
InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT); 
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::SOCLE);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::SOCLE);



puts "Test bras droit :"

puts "Aimant position basse"
sleep(1);
InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);

puts "Aimant position haute"
sleep(1);
InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT);

puts "Glissière hauteur de capture"
sleep(1);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);

puts "Glissière hauteur de transport"
sleep(1);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::MILIEU);

puts "Glissière hauteur maximale"
sleep(1);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::TOUR);

puts "Servomoteur AX12 angle replié"
sleep(1);
InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);

puts "Servomoteur AX12 angle centré"
sleep(1);
InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);

puts "Servomoteur AX12 angle de balayage"
sleep(1);
InterfaceActionneurs.angleBrasDroit(LibChessUp::BALAYAGE);

puts "Remise à zéro bras droit"
sleep(1);
InterfaceActionneurs.angleBrasDroit(LibChessUp::DROIT);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::SOCLE);
InterfaceActionneurs.positionAimantDroit(LibChessUp::HAUT);



puts "Test bras gauche :"

puts "Aimant position basse"
sleep(1);
InterfaceActionneurs.positionAimantGauche(LibChessUp::BAS);

puts "Aimant position haute"
sleep(1);
InterfaceActionneurs.positionAimantGauche(LibChessUp::HAUT);

puts "Glissière hauteur de capture"
sleep(1);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::CAPTURE);

puts "Glissière hauteur de transport"
sleep(1);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::MILIEU);

puts "Glissière hauteur maximale"
sleep(1);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);

puts "Servomoteur AX12 angle replié"
sleep(1);
InterfaceActionneurs.angleBrasGauche(LibChessUp::REPLIE);

puts "Servomoteur AX12 angle centré"
sleep(1);
InterfaceActionneurs.angleBrasGauche(LibChessUp::REPLIE);

puts "Servomoteur AX12 angle de balayage"
sleep(1);
InterfaceActionneurs.angleBrasGauche(LibChessUp::BALAYAGE);

puts "Remise à zéro bras gauche"
sleep(1);
InterfaceActionneurs.angleBrasGauche(LibChessUp::DROIT);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::SOCLE);
InterfaceActionneurs.positionAimantGauche(LibChessUp::HAUT);



puts "Tests bras terminés"
