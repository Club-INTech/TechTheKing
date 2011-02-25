# encoding: utf-8

# Ce fichier définit la classe représentant une carte quelconque

require "Dijkstra"
require "Log"

# C'est l'aire de jeu, avec les fonctions pour la modifier et trouver son chemin dedans
# C'est une classe virtuelle, toujours appeler CarteTechTheFruit qui sa classe fille
class Carte
	
public 
	
	# Crée les structures permettant d'acceuillir les différents objets d'une classe fille
	# On s'appui sur un objet Disjkstra pour donner les chemins
        def initialize
        end

	# Rajoute un objet sur la carte
	def ajouterObjet
	end
	
	# Enleve un objet de la carte
	def enleverObjet
	end
	
	# donne le chemin à suivre pour aller d'un point A à un point B
	def chemin
	end
	
	# Bloque toute entree dans la Zone ou "position" se trouve pour une durée "duree"
	def bloquerZone(position,duree)
	end

	# Renvoie true si la zone liée a la position est bloquée
	def estBloque? position
	end
	
	# Renvoie la liste des Objets appartenant à "listeObjets" qui sont "decalage" proche de la position "position"
	def listeObjetProche
	end

	# renvoie la zone dans laquelle position ce trouve
	def quelleZone(position)
	end

protected	
	
	# Rajoute le noeud numero "numero" qui a pour centre "position"
	# et une fonction "procedure" qui renvoie true uniquement si on lui passe un point à l'intérieur de cette zone
	def ajouterNoeud
	end
	
	# Ajoute une arrete entre deux noeuds déjà crées
        def ajouterArrete

        end

	# enlève une arrete entre deux noeuds
	def enleverArrete
        end


private
	
end
