
OBJS=main.o Point.o Consigne.o Obstacles.o AStar.o Debug.o Interfaces.o
CC=g++
MAGIC_CONFIG=`Magick++-config --cxxflags --cppflags --ldflags --libs`
CFLAGS_LINK=-O3 -lm -lserial $(MAGIC_CONFIG) -o 
CFLAGS_COMP=-O3 -Wall $(MAGIC_CONFIG) -c

compil: $(OBJS)
	@echo -n "Création de l'executable : compilation classique ..."
	@$(CC) $(OBJS) $(CFLAGS_LINK) debug
	@echo " fait."

main.o : main.cpp Point.h AStar.h Debug.h Contrats.h
	@echo -n "Compilation de main.cpp ..."
	@$(CC) $(CFLAGS_COMP) main.cpp
	@echo  " fait."
	
Point.o : Point.cpp Point.h Consigne.h Contrats.h
	@echo -n "Compilation de Point.cpp ..."
	@$(CC) $(CFLAGS_COMP) Point.cpp
	@echo " fait."
	
Consigne.o : Consigne.cpp Consigne.h Contrats.h
	@echo -n "Compilation de Consigne.cpp ..."
	@$(CC) $(CFLAGS_COMP) Consigne.cpp
	@echo " fait."
	
Obstacles.o : Obstacles.cpp Obstacles.h Point.h Contrats.h
	@echo -n "Compilation de Obstacles.cpp ..."
	@$(CC) $(CFLAGS_COMP) Obstacles.cpp
	@echo " fait."
	
AStar.o : AStar.cpp AStar.h Point.h Obstacles.h Contrats.h
	@echo -n "Compilation de AStar.cpp ..."
	@$(CC) $(CFLAGS_COMP) AStar.cpp
	@echo " fait."
	
Interfaces.o : Interfaces.cpp Interfaces.h Point.h AStar.h
	@echo -n "Compilation de Interfaces.cpp ..."
	@$(CC) $(CFLAGS_COMP) Interfaces.cpp
	@echo " fait."
	
Debug.o : Debug.cpp Debug.h Point.h Obstacles.h Consigne.h Contrats.h
	@echo -n "Compilation de Debug.cpp ..."
	@$(CC) $(CFLAGS_COMP) Debug.cpp
	@echo " fait."
	
clean :
	@echo -n "Suppression des .o et des ~ ..."
	@rm -f $(OBJS) rm *~
	@echo " fait."
	
mrpropre : clean
	@echo "Suppression de l'executable ..."
	@rm -f debug
	@echo -n " fait."
	



