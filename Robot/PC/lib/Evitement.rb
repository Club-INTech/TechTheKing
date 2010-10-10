#fusionne le code commente avec le code ci-dessous


=begin
# Gestion de l'arduino des capteurs
# Author::  Guillaume DEMAISON (mailto:guillaume.demaison@minet.net)
# Copyright:: Copyright (c) 2010 INTech - TechTheFruit
# License::   GPL

require "SerieSimple"

# Cette classe hérite des fonctions basiques de gestion d'une liaison série.

class InterfaceEvitement < Serie

  attr_reader :ultrasons, :optiques

  def initialize peripherique
    super(peripherique, 57600)

    @ultrasons = [0] * 4
    @optiques = [0] * 5
  end

  #acquisition des valeurs des capteurs ultrasons et traitement/lissage des valeurs
  def traitementUltrasons
    tableauE = commande("u").split("\t")
    return @ultrasons if tableauE.size != 3

    tableauE.map! { |s| 
      s.split("-").map! { |e| e.to_i } 
    }

    tableauE.each { |s| 
      return @ultrasons if s.size != 4
    }

    diviseur = [3] * 4

    (0..3).each { |j|
      tableauE[1][j] = tableauE[1][j].to_i
      if (tableauE[0][j] - tableauE[1][j]).abs <= 12
        diviseur[j] = 2
      end
    }

    for j in (0..3)
      if diviseur[j] == 2
        @ultrasons[j] = (tableauE[0][j] + tableauE[1][j]) / 2
      else
        @ultrasons[j] = (tableauE[0][j] + tableauE[1][j] + tableauE[2][j]) / 3
      end
    end

    @ultrasons
  end

  def traitementOptiques
    tableauE = [[0] * 5] * 3
    identificateur = [1] * 5
    tableau = [0] * 5

    acq=commande('o')
    tableauE[0] = acq.split("\t")[0].split('-')
    tableauE[0].map! { |donnee|
      donnee.to_i
    }

    tableauE[1]= acq.split("\t")[1].split('-')
    (0..4).each { |j|
      tableauE[1][j] = tableauE[1][j].to_i
      if tableauE[0][j] == tableauE[1][j]
        identificateur[j] = 0
      end
    }

    if identificateur.count(1) != 0 
      tableauE[2] = acq.split("\t")[2].split('-')
      tableauE[2].map! { |donnee|
        donnee.to_i
      }
    end

    #nettoyage par moyenne sur 3 selon les besoins
    for j in (0..4)
      if identificateur[j] == 0
        tableau[j] = tableauE[0][j]
      else
        tableau[j] = (tableauE[0][j] + tableauE[1][j] + tableauE[2][j]) / 3
      end
    end

    tableau.each_with_index { |valeur, index|
      if valeur <= 0.5
        @optiques[index] = 0
      else
        @optiques[index] = 1
      end
    }
  end

end


=end
require "Log"

require "InterfaceEvitement"

class Evitement

	def initialize peripherique
		@log = Logger.instance

		if peripherique == nil
			raise "Pas de carte pour Evitement" 
		end

		@log.debug "Evitement sur " + peripherique if peripherique != nil

		@interface = InterfaceEvitement.new peripherique
	end

	def demarrer
	end

	def arreter
	end

	def ultrasons
		@interface.traitementUltrasons
	end

	def optiques
		@interface.traitementOptiques
	end

	def controleEtatHaut
		renvoie = ""
		pb = 0 #identificateur de problème
		if @interface.ultrasons[0]<=80
			renvoie = renvoie + "problème capteur HAvG, proximitée\n"
			pb = 1
		end
		if @interface.ultrasons[1]<=80
			renvoie = renvoie + "problème capteur HAvM, proximitée\n"
			pb = 1
		end
		if @interface.ultrasons[2]<=80
			renvoie = renvoie + "problème capteur HAvD, proximitée\n"
			pb = 1
		end
		if @interface.ultrasons[3]<=80
			renvoie = renvoie + "problème capteur HAr, proximitée\n"
			pb = 1
		end
		if pb == 0
			renvoie = renvoie + "à priori, il n'y a pas de problème avec les capteurs ultrason haut.\n"
		end
		renvoie
	end

	def controleEtatUS
		renvoie = ""
		renvoie = renvoie + controleEtatHaut
		renvoie
	end

	def presenceOranges
		@interface.traitementOptiques
		capteurs = @interface.optiques
		retour = ""
		if capteurs[1]==0 && capteurs[3]==0
			retour = retour + "Rien à gauche\n"
		else
			if capteurs[1]==1 && capteurs[3]==0
				retour = retour + "Une orange à gauche\n"
			else
				retour = retour + "Deux oranges à gauche\n"
			end
		end
		if capteurs[2]==0 && capteurs[4]==0
			retour = retour + "Rien à droite\n"
		else
			if capteurs[2]==1 && capteurs[4]==0
				retour = retour + "Une orange à droite\n"
			else
				retour = retour + "Deux oranges à droite\n"
			end
		end
		retour
	end

	def placeSurRailGauche
		@interface.traitementOptiques
		capteurs = @interface.optiques
		retour = 2
		if capteurs[1]==0 && capteurs[3]==0
			retour = retour#hahahahaha
		else
			if capteurs[1]==1 && capteurs[3]==0
				retour = retour - 1
			else
				retour = retour - 2
			end
		end
		retour
	end

	def placeSurRailDroit
		@interface.traitementOptiques
		capteurs = @interface.optiques
		retour = 2
		if capteurs[2]==0 && capteurs[4]==0
			retour = retour 
		else
			if capteurs[2]==1 && capteurs[4]==0
				retour = retour - 1
			else
				retour = retour - 2
			end
		end
		retour
	end

	def nombrePlaceSurRails
		retour = placeSurRailGauche + placeSurRailDroit
		retour
	end

	def nombreOranges
		@interface.traitementOptiques
		capteurs = @interface.optiques
		retour=capteurs[0]+capteurs[1]+capteurs[2]+capteurs[3]+capteurs[4]
		retour
	end

end
