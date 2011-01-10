OBJS=main.o Point.o Consigne.o Obstacles.o AStar.o Debug.o 
CC=g++ `Magick++-config --cxxflags --cppflags --ldflags --libs`
CFLAGS_LINK=-O3 -lm -lserial `Magick++-config --cxxflags --cppflags --ldflags --libs` -o 
CFLAGS_COMP=-O3 -Wall `Magick++-config --cxxflags --cppflags --ldflags --libs` -c

all:	debug clean

debug: $(OBJS)
	@echo "Création de l'executable"
	@g++ $(OBJS) $(CFLAGS_LINK) debug

main.o : main.cpp Point.h AStar.h Debug.h Contrats.h
	@echo "Compilation de main.cpp"
	@g++ $(CFLAGS_COMP) main.cpp

Point.o : Point.cpp Point.h Consigne.h Contrats.h
	@echo "Compilation de Point.cpp"
	@g++ $(CFLAGS_COMP) Point.cpp

Consigne.o : Consigne.cpp Consigne.h Contrats.h
	@echo "Compilation de Consigne.cpp"
	@g++ $(CFLAGS_COMP) Consigne.cpp

Obstacles.o : Obstacles.cpp Obstacles.h Point.h Contrats.h
	@echo "Compilation de Obstacles.cpp"
	@g++ $(CFLAGS_COMP) Obstacles.cpp

AStar.o : AStar.cpp AStar.h Point.h Obstacles.h Contrats.h
	@echo "Compilation de AStar.cpp"
	@g++ $(CFLAGS_COMP) AStar.cpp
	
Debug.o : Debug.cpp Debug.h Point.h Obstacles.h Consigne.h Contrats.h
	@echo "Compilation de Debug.cpp"
	@g++ $(CFLAGS_COMP) Debug.cpp

clean :
	@echo "Suppression des .o et des ~"
	@rm -f $(OBJS) rm *~

mrpropre : clean
	@echo "Suppression de l'executable."
	@rm -f debug
	



