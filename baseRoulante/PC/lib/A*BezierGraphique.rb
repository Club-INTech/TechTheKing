require 'RMagick'
require "./Point"
require "./Obstacles"
require "./AStar"

canvas = Magick::Image.new(3000, 2100)
gc = Magick::Draw.new


listeObstacles = ListeObstacles.new
pion1=RondObstacle.new(1300,850)
pion2=RondObstacle.new(1000,500)
pion3=RondObstacle.new(2000,950)
pion4=RondObstacle.new(1200,300)
listeObstacles.push(pion1)
listeObstacles.push(pion2)
listeObstacles.push(pion3)
listeObstacles.push(pion4)

points = AStar.new(80,Point.new(2350,550),Noeud.new(600,200),listeObstacles).lissageBezier(100)

#affiche le terrain
# gc.fill('Red')
# gc.rectangle(0,0,400,400)
# caseBleue=true
# for j in [0,350,700,1050,1400,1750]
# 	for i in [450,800,1150,1500,1850,2200]
# 		if caseBleue==true
# 			gc.fill('Blue')
# 			caseBleue=false
# 		else
# 			gc.fill('Red')
# 			caseBleue=true
# 		end
# 	gc.rectangle(i,j,i+350,j+350)
# 	end
# 	if caseBleue==true
# 		gc.fill('Blue')
# 		caseBleue=false
# 	else
# 		gc.fill('Red')
# 		caseBleue=true
# 	end
# end
# gc.fill('Blue')
# gc.rectangle(2600,0,3000,400)
# #les lignes noires
# gc.fill('black')
# gc.rectangle(400,0,450,2100)
# gc.rectangle(2550,0,2600,2100)
# #la partie verte
# gc.fill('Green')
# gc.rectangle(0,400,400,2100)
# gc.rectangle(2600,400,3000,2100)
# #les cases speciales.
# gc.fill('black')
# gc.ellipse(975,525, 50, 50, 0, 360)
# gc.ellipse(2025,525, 50, 50, 0, 360)
# gc.ellipse(975,1225, 50, 50, 0, 360)
# gc.ellipse(2025,1225, 50, 50, 0, 360)
# gc.ellipse(1325,1925, 50, 50, 0, 360)
# gc.ellipse(1675,1925, 50, 50, 0, 360)
# 
# 
# 
# #affiche les obstacles
# 
# #affiche les rectangles(planches)
# listeObstacles.each do |obstacle|
# 	obstacle.afficher gc
# end
# 
# 
# ##affiche la courbe,
# gc.stroke('Dark Magenta')
# gc.stroke_width(10)
# for i in (0..99)
# 	gc.line(points[i].x, points[i].y,points[i+1].x, points[i+1].y)
# end
# 
# gc.draw(canvas)
# canvas.adaptive_resize(1000, 700).display