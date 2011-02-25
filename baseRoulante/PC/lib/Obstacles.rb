class Obstacle < Point
	attr_accessor :x, :y
	
	def initialize x=0, y=0
		@x=x
		@y=y
	end
	
	def contientPoint point
	end
	
	def afficher draw
	end
	
end

class RondObstacle < Obstacle
	attr_accessor :rayon
	
	def initialize x=0, y=0,rayon=100
		super(x,y)
		@rayon=rayon
	end
	
	def contientPoint point
		if self.distance(point)<@rayon+200
			return true
		end
		return false
	end
	
	def afficher draw
		draw.fill('Yellow')
		draw.ellipse(@x,@y, @rayon,@rayon,0,360)
	end
end

class RectangleObstacle < Obstacle
	#rectangle non penché, sinon ça devient assez galère et cette année y'en aura pas d'autre de toute façon...
	attr_accessor :demiCoteX, :demiCoteY
	
	def initialize x=0, y=0,demiCoteX=0,demiCoteY=0
		super(x,y)
		@demiCoteX=demiCoteX
		@demiCoteY=demiCoteY
	end
	
	def contientPoint point
		if(((point.x)-@x).abs<@demiCoteX+200 and ((point.y)-@y).abs<@demiCoteY+200)
			return true
		end
		return false
	end
	
	def afficher draw
		draw.fill('Dark Orange')
		draw.rectangle(@x - @demiCoteX , @y - @demiCoteY , @x + @demiCoteX , @y + @demiCoteY)
	end
	
end


class ListeObstacles < ListePoints
	#il y a quelques obstacles qui seront de toute façon là.
	def initialize
		#la planche en dessous de la case de départ rouge
		push(RectangleObstacle.new(200,411,200,11))
		#la planche en dessous de la case de départ bleue
		push(RectangleObstacle.new(2800,411,200,11))
		#la planche sous la zone protégée gauche
		push(RectangleObstacle.new(800,2040,350,60))
			#les deux petites planches
			push(RectangleObstacle.new(461,1915,11,65))
			push(RectangleObstacle.new(1139,1915,11,65))
		#la planche sous la zone protégée droite
		push(RectangleObstacle.new(2200,2040,350,60))
			#les deux petites planches
			push(RectangleObstacle.new(1861,1915,11,65))
			push(RectangleObstacle.new(2539,1915,11,65))
	end
	
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
