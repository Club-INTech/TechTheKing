# encoding: utf-8

# Ce fichier définit les classes qui représentent des objets sur le plateau de jeu
# On peut afficher ces objets avec la sdl sur une carte virtuelle

# Classe Objet générique
class Objet

	# la position du centre de l'objet
	attr_accessor :position
	
	# crée un objet rond avec le rayon et la position passés en paramètre
	def initialize(position,rayon)
		@position= position
		@rayon=rayon
	end
end

class Pion < Objet

	# on peut rajouter des trucs dans le constructeur
	def initialize(position,rayon)
		super(position,rayon)
	end
	
	# Affiche un cercle mis à la bonne taille grace à l'utilisation de "diviseur" (voir classe chemin)
	def afficher
	end
end

# Objet Reine
class Reine < Objet

	# on peut rajouter des trucs dans le constructeur
	def initialize(position,rayon)
		super(position,rayon)
	end
	
	# Affiche un cercle mis à la bonne taille grace à l'utilisation de "diviseur" (voir classe chemin)
	def afficher
	end
end

class Roi < Objet
	
	# on peut rajouter des trucs dans le constructeur
	def initialize(position,rayon)
		super(position,rayon)
	end
	
	# Affiche un cercle mis à la bonne taille grace à l'utilisation de "diviseur" (voir classe chemin)
	def afficher
	end
end

