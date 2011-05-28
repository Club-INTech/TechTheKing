detection: main.o traitement.o loi_normale.o segmentation.o fonctions.o detectionEllipse.o
	g++ -Wall main.o traitement.o loi_normale.o segmentation.o fonctions.o detectionEllipse.o -lcv -lhighgui -o detection -O2

main.o: main.cpp traitement.h segmentation.h detectionEllipse.h param.h
	g++ -Wall -c main.cpp -O2
	
traitement.o: traitement.cpp param.h
	g++ -Wall -c traitement.cpp -O2 
	
loi_normale.o: loi_normale.cpp param.h
	g++ -Wall -c loi_normale.cpp -O2 
	
segmentation.o: segmentation.cpp loi_normale.h traitement.h
	g++ -Wall -c segmentation.cpp -O2 
	
fonctions.o: fonctions.cpp param.h
	g++ -Wall -c fonctions.cpp -O2 
	
detectionEllipse.o: detectionEllipse.cpp param.h
	g++ -Wall -c detectionEllipse.cpp -O2 
	
clean:
	rm *.o
