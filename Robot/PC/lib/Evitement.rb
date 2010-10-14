# encoding: utf-8

# Ce fichier définit la classe pour l'évitement du robot

require "Log"

# Cette classe permet de gérer tout l'évitement du robot
class Evitement

	# initialise la connection avec la carte liée a l'évitement
	def initialize peripherique
		@log = Logger.instance

		if peripherique == nil
			raise "Pas de carte pour Evitement" 
		end
		@log.debug "Evitement sur " + peripherique if peripherique != nil
	end
	
	# à commenter
	def fonction1
		# à coder
	end

	# à commenter
	def fonction2
		# à coder
	end
end
