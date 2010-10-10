# Ce fichier définit la classe pour l'évitement du robot
# Cette classe est utilisée pour mettre à jour la position du robot adverse

require "Log"

class Evitement

	def initialize peripherique
		@log = Logger.instance

		if peripherique == nil
			raise "Pas de carte pour Evitement" 
		end
		@log.debug "Evitement sur " + peripherique if peripherique != nil
	end

	def fonction1
		# à coder
	end
	
	def fonction2
		# à coder
	end
end
