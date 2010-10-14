# encoding: utf-8

# Ce fichier définit les classes d'utilitées générales
# Il y a notament les classes représentant un point, un vecteur, une position et diverses opérations mathématiques 

# definit un Point (2D)
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
	
	# Soustraction par un point
	def - q
	        Point.new((@x - q.x), (@y - q.y))
	end
	
 	# Division d'un point par une constante
	def / k
		self.*(1.0/k)
	end
	
	# Test d'égalité de 2 points
	def == p
		(p.x == @x && p.y == @y)
	end
	
	# Affiche les attributs d'un point
	def prettyprint
	        puts "x = " + x.to_s + ", y = " + y.to_s
	end
	
	# on arrondie a l'unite les coordonnées
	def to_i
	        @x = @x.to_i
	        @y = @y.to_i
                self
	end
	
	# on fait le symétrique du point par rapport au milieu de la table
	def symetrie
	       Point.new(@x, -1 * @y)
	end
end

# définit ce qu'est un Vecteur (2D)
class Vecteur

	# Un vecteur est défini par son dx et dy
	attr_accessor :x, :y
	
	# Initialisation avec 2 points, départ et arrivée.
	def initialize p = Point.new, q = Point.new
		@x = q.x - p.x
		@y = q.y - p.y
	end
	
	# Calcule la norme du vecteur
	def norme
		Math.sqrt(@x**2 + @y**2)
	end

	# Calcule l'angle du vecteur par rapport à l'axe (Ox)
	def angle
		Math.atan2 @y, @x
	end
	
	# renvoie produit scalaire de deux vecteurs
	def produitScalaire v
	       (@x * v.x) + (@y * v.y)
	end
	
	# renvoie le vecteur normalisé
	def normalise
	       n = norme
		v = Vecteur.new()
		v.x = (@x / n)
		v.y = (@y/n)
		v
	end
	
	# a documenter
	def *(k)
		v = Vecteur.new()
		v.x = @x*k
		v.y =  @y*k
		v
	end
	
	# renvoie le vecteur orthogonal
	def ortho
	        v = Vecteur.new()
		v.y = @x
		v.x = @y*-1
		v
	end

	# définit l'addition de deux vecteurs
	def +(vect)
		v = Vecteur.new()
		v.x = @x + vect.x
		v.y = @y + vect.y 
	end

	# définit la soustraction de deux vecteurs
	def -(vect)
		v = Vecteur.new()
		v.x =  vect.x - @x
		v.y =  vect.y - @y
		v 
	end
end


# Représente la position du robot, un point + un angle
class Position < Point

        # Nouvel attribut à une position, l'orientation du robot
        attr_accessor :angle

        # Initialisation à partir de x, y et de l'angle par rapport à (Ox).
        # Par défaut, la position est l'origine.
        def initialize x = 0, y = 0, a = 0
                super(x, y)
                @angle = a
        end

        # Affiche les attributs d'une position
        def prettyprint
                puts "x = " + @x.to_s + ", y = " + @y.to_s + ", angle = " + @angle.to_s
        end

	# définit l'opérateur d'égalité sur les Positions
        def == p
                (x == p.x) && (y == p.y) && ((angle - p.angle) % (2 * Math::PI) == 0)
        end

	# regarde si la position est compatible avec la table
        def existe?
                (x >= 0) && (x <= 3000) && (y >= 0) && (y <= 2100)
        end
        
        # regarde si la position est assez loin du bord
        def assezLoinDuBord?
                (x >= 250) && (x <= 2750) && (y >= 250) && (y <= 1850)
        end
        
        # definit la symétrie par rapport à la table
        def symetrie
                Position.new(@x, -@y, -@angle)
        end
end

# Définit des opérations supplémentaire sur les flotants
class Float
	
	# définit le modulo 2*PI
        def modulo2 arg = (2 * Math::PI)
                angle = self % arg
                
                if 2 * angle > arg 
                        angle -= arg
                end   
                angle
        end
end

# Définit des opérations supplémentaires sur les nombres entiers
class Fixnum
	
	# définit le modulo 2*P
        def modulo2 arg = (2 * Math::PI)
                angle = self.to_f % arg
                
                if 2 * angle > arg 
                        angle -= arg
                end
                
                angle
        end
end
