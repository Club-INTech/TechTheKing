require 'RMagick'
require "./Point"
require "./Obstacles"
require "./AStar"

img = Magick::ImageList.new
img.new_image(1000, 1000)

purplish = "#ff55ff"
yuck = "#5fff62"
bleah = "#3333ff"

pion = Obstacle.new(150,150,50)
pion2 = Obstacle.new(300,450,50)
pion3 = Obstacle.new(450,320,50)
pion4 = Obstacle.new(120,850,50)
pion5 = Obstacle.new(600,450,50)
pion6 = Obstacle.new(700,570,50)
pion7 = Obstacle.new(800,850,50)
puts pion.contientPoint(Point.new(100,100))
listeObstacles = ListeObstacles.new
listeObstacles.push(pion)
listeObstacles.push(pion2)
listeObstacles.push(pion3)
listeObstacles.push(pion4)
listeObstacles.push(pion4)
listeObstacles.push(pion5)
listeObstacles.push(pion6)
listeObstacles.push(pion7)
points = AStar.new(Point.new(0,0),Noeud.new(900,700),listeObstacles).lissageBezier(100)

#affiche les obstacles
cir = Magick::Draw.new
cir.fill('red')
listeObstacles.each do |pion|

	cir.ellipse(pion.x,pion.y, pion.rayon,pion.rayon,0,360)
	cir.draw(img)
end


##affiche la courbe
line = Magick::Draw.new
for i in (0..99)
  line.line(points[i].x, points[i].y,points[i+1].x, points[i+1].y)  # vert line
  line.draw(img)
end




img = img.quantize(256,Magick::GRAYColorspace)

img.write("drawing.gif")
img.display