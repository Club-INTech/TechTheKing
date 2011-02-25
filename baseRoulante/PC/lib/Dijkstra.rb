# encoding: utf-8

# ce fichier définit la classe Dijstra, cela permet de trouve le plus court chemin

require "Log"

# C'est l'algorithme de pathfinding en lui même. On peut modifier le graphe qu'il utilise et lui demander ce
# pour quoi il est fait c'est à dire le plus court chemin d'un noeud A à un noeud B
class Dijkstra

public

	# Crée le graphe qui est composée de noeuds et d'arretes
        def initialize
        end

	# Rajoute le noeud numero "numero" qui a pour centre "position"
	# et une fonction "procedure" qui renvoie true uniquement si on lui passe un point à l'intérieur de cette zone
        def ajoutNoeud

        end

	# Ajoute une arrete entre deux noeuds déjà crées et calcul automatiquement la longueur de l'arrete
        def ajoutArrete
        end

	# Assigne la distance "distance" entre les zones "premier" et "second" 
	def modifArrete
	end

	# Supprime une arrete
        def suppArrete(premier,second)
        end
	
	# La fonction appelée par la carte qui renvoie une liste des points pour aller de la zone "n_depart" à la zone "n_arrive"
	def chemin
	end

	# Trouve dans quelle Zone ce trouve le point "position"
	def quelleZone()

	end

	
	# algorithme de dijstra qui trouve le plus petit chemin entre deux zones
	# où "n_depart" est le numéro de la zone de départ et "n_arrive" celui de l'arrivée
	def parcours(n_depart,n_arrive)
	end

private


	# Enlève le maximum de points intermédiaires (ceux qui sont alignés).
	def enleverIntermediaires(chemin)
	end
	
	# Renvoie le noeuds que l'on a pas encore parcouru qui est le plus proche en distance du noeud de départ
	def minfalse(liste_distances,liste_parcours)
	end

end
