require "./Consigne"
require "./Vecteur"

class Point
 
 	# Un point est défini par son abscisse et son ordonnée
	attr_accessor :x, :y
 
 	# Initialisation avec un x et y donné
	def initialize x = 0, y = 0
		@x = x
		@y = y
	end
 
 	# Somme des abscisses et des ordonnées de 2 points
	def + q
		Point.new((@x + q.x), (@y + q.y))
	end
	
 	# Produit d'un point par une constante
	def * k
		Point.new((@x * k), (@y * k))
	end
	
	def - q
	        Point.new((@x - q.x), (@y - q.y))
	end
	
 	# Division d'un point par une constante
	def / k
		self.*(1.0/k)
	end
	
	# Test d'égalité de 2 points
	def == p
		if p==nil
			return nil
		end
		(p.x == @x && p.y == @y)
	end
	
	# Affiche les attributs d'un point
	def prettyPrint
	        puts "x = " + x.to_s + ", y = " + y.to_s
	end
	
	def to_i
	        @x = @x.to_i
	        @y = @y.to_i
                self
	end
	
	#calcule la distance à un autre point (évite de créer un vecteur juste pour ça...)
	def distance point2
		return Math.sqrt((point2.x - @x)**2 + (point2.y - @y)**2)
	end
	
	def symetrie
	       Point.new(@x, -1 * @y)
	end
	
	
 
end

class ListePoints < Array
	
	def initialize
		super
	end
	
	def prettyPrint
		each do |point|
			point.prettyPrint
		end
	end
	
	def * k
		each do |point|
			point*k
		end
	end
	
	def / k
		each do |point|
			point / k
		end
	end
	
	def lissageBezier n
		bezier=ListePoints.new
		for i in (0..n)
			bezier.push(PointBezier(self,i.to_f/n))
		end
		return bezier
	end
	
	def convertirEnConsignes
		@@listeConsignes = ListeConsignes.new
		@@consigneAPush = Consigne.new
		@@vecteurConsignes = Vecteur.new
		for i in (0..self.length-2)
			@@vecteurConsignes.x = self[i+1].x - self[i].x
			@@vecteurConsignes.y = self[i+1].y - self[i].y
			@@consigneAPush.rayon+=@@vecteurConsignes.norme
			@@consigneAPush.angle=@@vecteurConsignes.angle
			@@listeConsignes.push(Consigne.new(@@consigneAPush.rayon,@@consigneAPush.angle))
		end
		return @@listeConsignes
	end
	
	def elementCommun pointRecherche
		each do |point|
		    if(point.x==pointRecherche.x and point.y==pointRecherche.y)
				return point
		    end
		end
		return nil
	end
	
private

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
