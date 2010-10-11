# Ce fichier définit la classe d'asservissement permettant de se déplacer

require "Float"
require "Fixnum"
require "Log"
require "Point"
require "Vecteur"
require "SerieThread"
require "Position"


# Cette classe les déplacements (haut niveau) en commande pour l'asservissement (bas niveau)
class Asservissement

	# Précision du goTo : cercle à partir duquel on n'envoit plus de nouvelle consigne
	attr_accessor :precisionSimple,:sens

	# sens actuel du robot
	attr_accessor :sens

	# Précision du goTo : cercle à partir duquel on déblocage le déroulement de la stratégie
	attr_accessor :precisionDouble

	# Initialise l'asservissement à partir d'un périphérique série, valeurs 
	# par défaut pour la précision
	def initialize peripherique, positionParDefaut = Position.new(0, 0, 0)
		@log = Logger.instance

		if peripherique == nil
			raise "Pas de carte pour Asservissement" 
		end

		@log.debug "Asservissement sur " + peripherique

		@precisionSimple = 50
		@precisionDouble = 20

		@drapeauArret = false

		@interface = InterfaceAsservissement.new peripherique, 57600, positionParDefaut
		@interface.demarrer

		@interface.changerVitesse([3000, 3000])
		@interface.changerPWM([2000, 2046])
		@log.debug "Asservissement prêt"
		@sens = 1
	end
	
	# à commenter
	def position
	end

	# à commenter
	def demarrer
	end

	# à commenter
	def arreter
	end

	# à commenter
	def reset
	end

	# à commenter
	def remiseAZero nouvellePosition = Position.new(0, 0, 0)
	end

	# à commenter
	def goto(where)
	end

	# à commenter
	def avancer distance
	end

	# à commenter
	def tourner angle
	end

	# à commenter
	def tourneDe angleDonne
	end

	# à commenter
	def desactiveAsservissement
	end

	# à commenter
	def desactiveAsservissementTranslation
	end

	# à commenter
	def desactiveAsservissementRotation
	end

	# à commenter
	def codeuses
	end

	# à commenter
	def blocageTranslation
	end

	# à commenter
	def blocageRotation
	end

	# à commenter
	def blocage
	end

	# à commenter
	def recalageZone(zone)
	end

	# à commenter
	def recalageMatch
	end

	# à commenter
	def arret
	end

	# à commenter
	def stop
	end

	# à commenter
	def stopUrgence
	end

	# à commenter
	def sens
	end

	# à commenter
	def changerVitesse(rotation, translation)
	end

	# à commenter
	def alignement angle
	end

end
