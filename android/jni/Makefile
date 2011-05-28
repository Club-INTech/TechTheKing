OBJS=traitement.o detectionEllipse.o main.o

all:test

test:$(OBJS)
	gcc traitement.o detectionEllipse.o main.o -lcv -lhighgui -o test
	
main.o: main.cpp
	gcc -c main.cpp
	
detectionEllipse.o: detectionEllipse.cpp
	gcc -c detectionEllipse.cpp
	
traitement.o: traitement.cpp
	gcc -c traitement.cpp

clean:
	rm $(OBJS) test
