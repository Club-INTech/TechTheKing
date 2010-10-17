# encoding: utf-8

# Ce fichier définit la classe représentant la table de jeu
# Toutes les informations sur la position des éléments du jeu est enregistré ici

require "Carte"
require "Objets"

require "Log"

# C'est la carte de techthefruit, avec au départ les épis, les tomates et tout autre objets positionné comme à la coupe
class CarteTechTheFruit < Carte

public

	# Initialise la carte en ajoutant à l'intérieur les tomates, les épis ...
	# Crée également le graphe correspondant à la CarteTechTheFruit pour pouvoir y faire des calculs de pathfinding
        def initialize
        	# initialise objets, noeuds etc... fait donc juste appel aux fonctions private
	end

private

	# Ajoute tous les objets sur la carte
	def initObjet
	end

	# Défini les noeuds de la carte, leur numéro, position ainsi que leur étendus spaciale
	def initNoeuds
	end

	# Défini les noeuds ponctuels de la carte, leur numéro, position. Ils représentent des raccourcis dans la carte
	def initNoeudsEtArretesFaibles
	end

	# Défini les zones de départ
	def initZonesDepart
	end

end
