require "./SerieThread"

class Consigne
	attr_accessor :rayon, :angle
	
	def initialize rayon=0,angle=0
		@rayon=rayon
		@angle=angle
	end
	
	def transfertSerie interfaceDeTransfert
		puts("g" + @angle.to_s.rjust(8, "0"))
		puts("q" + @rayon.to_s.rjust(8, "0"))
		interfaceDeTransfert.ecrire ("g" + @angle.to_s.rjust(8, "0")) #ajout de la distance à la consigne actuelle
		interfaceDeTransfert.ecrire("q" + @rayon.to_s.rjust(8, "0")) #ajout de l'angle puis ajout d'une case dans le tableaux de consigne.
	end
	
	def convertirEnTicks
		#le /2 est un subterfuge pour 360°
		@rayon=(@rayon*1.02633).to_i
		@angle=(@angle*1559.71).to_i
	end
	
	def prettyPrint
		puts "A = " + @angle.to_i.to_s.rjust(8, "0") + ", R = " + @rayon.to_i.to_s.rjust(8, "0")
	end
	
end

class ListeConsignes < Array
	
	def transfertSerie interfaceDeTransfert
		interfaceDeTransfert.ecrire("e") 
		each do |consigne|
			consigne.transfertSerie interfaceDeTransfert
		end
		interfaceDeTransfert.ecrire("e") 
	end
	
	def convertirEnTicks
		each do |consigne|
			consigne.convertirEnTicks
		end
	end
	
	def prettyPrint
		each do |consigne|
			consigne.prettyPrint
		end
	end
	
end
