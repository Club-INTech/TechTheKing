require '../libChessUp'
include LibChessUp

LibChessUp.ouvrir_adaptateur_i2c();


#Création des interfaces
InterfaceActionneurs = LibChessUp::InterfaceActionneurs.new;
InterfaceAsservissement = LibChessUp::InterfaceAsservissement.Instance();
InterfaceCapteurs = LibChessUp::InterfaceCapteurs.Instance();

#initialisation des obstacles
LibChessUp::initialisation();

InterfaceCapteurs.gestionJumper();

#Démarrage des capteurs
InterfaceCapteurs.ouvrirThread();

#Initialisation des bras.
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);
InterfaceActionneurs.hauteurBrasDroit(LibChessUp::CAPTURE);

InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);
InterfaceActionneurs.hauteurBrasGauche(LibChessUp::TOUR);

sleep(2);

InterfaceActionneurs.angleBrasGauche(LibChessUp::REPLIE);
InterfaceActionneurs.angleBrasGauche(LibChessUp::REPLIE);

InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);
InterfaceActionneurs.angleBrasDroit(LibChessUp::REPLIE);

InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);
InterfaceActionneurs.positionAimantDroit(LibChessUp::BAS);

puts "Recalage"
InterfaceAsservissement.recalage();

#puts "Go To"
InterfaceAsservissement.goTo(Point.new(1500,1050),80);
#InterfaceAsservissement.debugGraphique();

