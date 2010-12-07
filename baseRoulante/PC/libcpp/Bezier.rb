#nécéssite la classe "Point" permettant la définition et les opérations élémentaires sur des points du plan
require "Point"
require "Consigne"

class Bezier < ListePoints
	
	def initialize(listePointsControle,n)
		#on remplit, point par point, la liste des points de contrôles.
		for i in (0..n)
			self.push(PointBezier(listePointsControle,i.to_f/n))
		end
	end

	#afficher les coordonnées des points de la courbe
	def prettyPrint
		self.each do |point|
			point.prettyPrint
		end
	end
	

	private

	#Algorithme de De Casteljau
	def PointBezier(listePointsControle,t)
		#On initialiste la liste des barycentres
		@@listeBarycentres=listePointsControle.clone
		@@longueur=listePointsControle.length-1
		#On recalcule les barycentres. (Algorithme de De Casteljau)
		for i in (0..@@longueur-1)
			for j in (0 ..@@longueur-1-i)
				@@listeBarycentres[j]=@@listeBarycentres[j] * (1-t) + @@listeBarycentres[j+1] * (t)
			end
		end
		#Le dernier barycentre calculé est le point de la courbe		
		return @@listeBarycentres[0]
	end
	
	
end



#un petit test avec un carré, les 10 points donnés en résultat semblent en effet appartenir à la bonne courbe de Bézier
point1=Point.new(0,0)
point2=Point.new(0,20000)
point3=Point.new(20000,20000)
point4=Point.new(20000,0)
sp = SerialPort.new("/dev/ttyUSB0",57600)
while 1
sp.write("?\r\n")
end

