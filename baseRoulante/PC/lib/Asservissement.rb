# encoding: utf-8

# Ce fichier définit la classe d'asservissement permettant de se déplacer

require "Log"
require "Serie"

# Cette classe les déplacements (haut niveau) en commande pour l'asservissement (bas niveau)
class Asservissement

public

	# Initialise l'asservissement à partir d'un périphérique série, valeurs 
	# par défaut pour la précision
	def initialize peripherique, positionParDefaut = Position.new(0, 0, 0)
	end
	
	# retourne la position du robot
	def position
	end

	# retourne l'abcisse du robot
	def x
	end
	
	# retourne l'ordonnée du robot
	def y
	end
	
	# retourne l'angle du robot
	def angle
	end

	# remet la position a zero (utile lors d'un recalage)
	def remiseAZero nouvellePosition = Position.new(0, 0, 0)
	end

	# Va a la position demandé avec l'angle voulu
	def goto position
	end

	# à commenter
	def tourner angle
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
	def stop
	end

	# dans quel sens avance le robot on doit pouvoir gérer le sens précisement
	# 1 avance
	# -1 recule
	# 0 prend le plus court
	def sens
	end
	
	# change le sens d'avancement du robot
	def changerSens
	end

	# change les vitesse de l'asservissement
	def changerVitesse(translation,rotation)
	end

	# change les accélérations
	def changerAcceleration(valeur)
	end

	# change les PWM
	def changerPWM(valeur)
	end

	# change les Kp
	def changerKp(valeur)
	end

	# change les Kd
	def changerKd(valeur)
	end
	
private

end
