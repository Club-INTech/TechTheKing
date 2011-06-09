require 'libChessUp'

LibChessUp.ouvrir_adaptateur_i2c();

InterfaceCapteurs = LibChessUp::InterfaceCapteurs.Instance();

puts "Cinq mesures ultrason";

puts InterfaceCapteurs.DistanceUltrason();
puts InterfaceCapteurs.DistanceUltrason();
puts InterfaceCapteurs.DistanceUltrason();
puts InterfaceCapteurs.DistanceUltrason();
puts InterfaceCapteurs.DistanceUltrason();
