require "./Point"

class Noeud < Point
	attr_accessor :cout1, :cout2, :cout3, :parent
	
	def initialize x=0, y=0, cout1=0, cout2=0
		super(x,y)
		#cout 1 : distance au point de départ, cout2 : distance au point d'arrivee, cout 3 : somme des deux.
		@cout1 = cout1
		@cout2 = cout2
		@cout3=@cout1+@cout2
		#le noeud parent.
		@parent=Point.new
	end
	
	def meilleurQue noeud2
		if @cout3 < noeud2.cout3
		return true
		end
		return false
	end
	
	def prettyPrint
		super
		puts " cout1 = " + @cout1.to_s + " cout2 = " + @cout2.to_s + " cout3 = " + @cout3.to_s
	end
	
end


class AStar < ListePoints
public

	def initialize depart=Point.new(0,0), arrivee=Noeud.new(0,500)
		@depart=depart
		@arrivee=arrivee
		@listeFermee=ListePoints.new
		@listeOuverte=ListePoints.new
# 		@listeObstacles=ListeObstacles.new
		rechercheChemin
	end
	
private

	def ajouterCasesAdjacentes point
		for i in (point.x-1..point.x+1)
			if(i<0 or i>3000)
				next
			end
			for j in (point.y-1..point.y+1)
				if(j<0 or j>3000)
					next
				end
				if(j==point.y and i==point.x)
					next
				end
# 				if @listeObstacles.toucheObstacle(Point.new(i,j))=true
# 					next
# 				end
				@tmp = Noeud.new(i,j)
				if (@listeFermee.elementCommun(@tmp)==nil)
					@tmp.cout1 = @depart.distance(@tmp)
					@tmp.cout2 = @arrivee.distance(@tmp)
					@tmp.cout3 = @tmp.cout1 + @tmp.cout2
					if(@tmp.cout2==0)
					@arrivee.parent = point
					else
					@tmp.parent=point
					end
# 					@tmp.prettyPrint
					if(@listeOuverte.elementCommun(@tmp)!=nil)
						(@listeOuverte.elementCommun(@tmp)).cout1 = @tmp.cout1
						(@listeOuverte.elementCommun(@tmp)).cout2 = @tmp.cout2
						(@listeOuverte.elementCommun(@tmp)).cout3 = @tmp.cout3
					else
					    @listeOuverte.push(@tmp)
					end
				end
			end
		end
	end
	
	def trouverMeilleurNoeud
		@@meilleurNoeud=@listeOuverte[0]
		@listeOuverte.each do |noeud|
			if(noeud.meilleurQue(@@meilleurNoeud))
				@@meilleurNoeud=noeud
			end
		end
		return @@meilleurNoeud
	end
	
	def transfererNoeud noeud
		@listeFermee.push(noeud);
		@listeOuverte.delete(noeud);
	end
	
	def remonterChemin
		@@noeudCourant=@arrivee
		while(@@noeudCourant.parent!=@depart)
			self.push(Point.new(@@noeudCourant.x,@@noeudCourant.y))
			@@noeudCourant=@@noeudCourant.parent
		end
	end
	
	def rechercheChemin
		@@courant=@depart.clone
		@listeOuverte.push(@@courant)
		puts @listeOuverte
		transfererNoeud(@@courant)
		puts @listeFermee
		ajouterCasesAdjacentes(@@courant)
		puts @listeOuverte.empty?
		puts @@courant.x != @arrivee.x
		puts @@courant.y != @arrivee.y
		while(!(@@courant.x == @arrivee.x and @@courant.y == @arrivee.y) and !@listeOuverte.empty?)
			@@courant = trouverMeilleurNoeud
			transfererNoeud @@courant
			ajouterCasesAdjacentes @@courant
		end
		if( @@courant.x = @arrivee.x and @@courant.y = @arrivee.y)
			remonterChemin
		else
			puts("Pas de chemin trouvé");
		end
	end
	
end

AStar.new().prettyPrint