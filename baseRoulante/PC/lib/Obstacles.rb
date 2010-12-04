class Obstacle < Point
	attr_accessor :x, :y, :rayon
#les obstacles ne seront que des cylindres. Ouf, pas de polymorphisme :p
#un obstacle est un point avec une "zone morte autour de lui", d'où l'héritage (vrai que pour les cercles.)

	def initialize x=0, y=0, rayon=0
		@x=x
		@y=y
		@rayon=rayon
	end
	
	def contientPoint point
		if self.distance(point)<@rayon*1.5
			return true
		end
		return false
	end
	
end


class ListeObstacles < ListePoints

	def contientPoint point
		each do |obstacle|
			#il suffit que le point soit dans un seul obstacle (et il le faut aussi d'ailleurs.)
			if(obstacle.contientPoint(point))
				return true
			end
		end
		return false
	end
	
end
