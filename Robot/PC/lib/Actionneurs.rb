# fusionner la partie commente avec l'autre

=begin
# Ce fichier contient la classe d'interfaçage des actionneurs.
# Author::    Guillaume Rose  (mailto:guillaume.rose@gmail.com)
# Copyright:: Copyright (c) 2010 INTech - TechTheFruit
# License::   GPL

require "SerieSimple"

# Cette classe hérite des fonctions basiques de gestion d'une liaison série.

class InterfaceActionneurs < Serie

  # Initialise avec un périphérique à une certaine vitesse
  def initialize(peripherique = "/dev/ttyUSB0", vitesse = 57600)
    super(peripherique, vitesse)
  end

  # Allume la led du jumper
  def allumerLed
    commande "b"
  end

  # Retourne l'état de l'interrupteur jack déclenchant le lancement du 
  # robot
  def etatJumper
    commande("a").to_i
  end

  # Envoi la commande pour lever la fourche
  def leveFourche
    commande "e"
  end

  # Envoi la commande pour baisser la fourche
  def baisseFourche
    commande "f"
  end

  # Envoi la commande pour rentrer complètement la fourche
  def rangeFourche
    commande "d"
  end

  def stopUrgence
    commande "c"
  end

  def rouleauDirect
    commande "g"
  end

  def rouleauIndirect
    commande "h"
  end

  def stopRouleau
    commande "i"
  end

  def selecteurGauche
    commande "j"
  end

  def selecteurDroite
    commande "k"
  end

  def selecteurMilieu
    commande "p"
  end

  def stopSelecteur
    commande "o"
  end

end

=end

require "Log"

require "InterfaceActionneurs"

# Cette classe contient les fonctions permettant de gérer les actionneurs

class Actionneurs
	
	# Initialise l'asservissement à partir d'un périphérique série
	def initialize peripherique
		@log = Logger.instance
		
		if peripherique == nil
			raise "Pas de carte pour Actionneurs" 
		end
		
		@log.debug "Actionneurs sur " + peripherique
		
		@interface = InterfaceActionneurs.new peripherique
	end
	
	# Démarre le service
	def demarrer
	end
	
	# Arrête le service
	def arreter
		stopRouleau
		stopSelecteur
	        @interface.stopUrgence
	end
	
	# Reset le service
	def reset
	
	end
	
	# Allume la led du jumper
	def allumerLed
	        @interface.allumerLed
	end
	
	# Retourne l'état du jumper, prise jack
	def etatJumper
		@interface.etatJumper
	end
		
	# Vide les oranges en baissant la fourche
	def baisseFourche
		@interface.baisseFourche
	end
	
	# Attrape les oranges en relevant la fourche
	def leveFourche
		@interface.leveFourche
	end

	# Attrape les oranges en relevant la fourche
	def rangeFourche
		@interface.rangeFourche
	end
	
	def stopUrgence
                @interface.stopUrgence
	end
	
        def rouleauDirect
                @interface.rouleauDirect
        end
        
        def rouleauIndirect
		puts "Rouleau indirect"
                @interface.rouleauIndirect
        end
        
        def stopRouleau
                @interface.stopRouleau
        end
        
        def selecteurGauche
                @interface.selecteurGauche
        end
        
        def selecteurDroite
                @interface.selecteurDroite
        end

	def selecteurMilieu
		@interface.selecteurMilieu
	end

	def stopSelecteur
		@interface.stopSelecteur
	end

end
