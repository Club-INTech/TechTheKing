require "SerieThread"

class Consigne
	attr_accessor :rayon, :angle
	
	def initialize rayon=0,angle=0
		@rayon=rayon
		@angle=angle
	end
	
	def transfertSerie interfaceDeTransfert
		if(@angle>=0)
			puts("g1" + @angle.to_s.rjust(7, "0"))
			interfaceDeTransfert.ecrire("g1" + @angle.to_s.rjust(7, "0")) #ajout de la distance à la consigne actuelle
		else
			puts("g" + (-1*@angle).to_s.rjust(8, "0"))
			interfaceDeTransfert.ecrire("g" + (-1*@angle).to_s.rjust(8, "0"))
		end
		if(@rayon>=0)
			puts("q1" + @rayon.to_s.rjust(7, "0"))
			interfaceDeTransfert.ecrire("q1" + @rayon.to_s.rjust(7, "0"))
		else
			puts("q" + (-1*@rayon).to_s.rjust(8, "0"))
			interfaceDeTransfert.ecrire("q" + (-1*@rayon).to_s.rjust(8, "0")) #ajout de l'angle puis ajout d'une case dans le tableaux de consigne.
		end
	end
	
	def convertirEnTicks
		#le /2 est un subterfuge pour 360°
		@rayon=(@rayon*1.01315).to_i
		@angle=(@angle*1523.9085).to_i
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
