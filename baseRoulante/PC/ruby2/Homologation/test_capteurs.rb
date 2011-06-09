require '../libChessUp'
include LibChessUp

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceCapteurs = InterfaceCapteurs::Instance();
InterfaceAsservissement = InterfaceAsservissement::Instance();
InterfaceCapteurs.ouvrirThread();
InterfaceCapteurs.gestionJumper();
while(1)
end
