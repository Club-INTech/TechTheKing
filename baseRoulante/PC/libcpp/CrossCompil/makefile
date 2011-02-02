
OBJS=main.o Point.o Consigne.o Obstacles.o AStar.o Debug.o Interfaces.o
CC=arm-linux-gnueabi-g++ -static
CFLAGS_LINK=-O -L /usr/lib/gcc/arm-linux-gnueabi/4.5.1/lib/ -lm -lserial -o 
CFLAGS_COMP=-O -Wall -c

compil: $(OBJS)
	@echo "Création de l'executable : Cross-Compilation"
	@$(CC) $(OBJS) $(CFLAGS_LINK) debug

main.o : main.cpp Point.h AStar.h Debug.h Contrats.h
	@echo "Compilation de main.cpp"
	@$(CC) $(CFLAGS_COMP) main.cpp

Point.o : Point.cpp Point.h Consigne.h Contrats.h
	@echo "Compilation de Point.cpp"
	@$(CC) $(CFLAGS_COMP) Point.cpp

Consigne.o : Consigne.cpp Consigne.h Contrats.h
	@echo "Compilation de Consigne.cpp"
	@$(CC) $(CFLAGS_COMP) Consigne.cpp

Obstacles.o : Obstacles.cpp Obstacles.h Point.h Contrats.h
	@echo "Compilation de Obstacles.cpp"
	@$(CC) $(CFLAGS_COMP) Obstacles.cpp

AStar.o : AStar.cpp AStar.h Point.h Obstacles.h Contrats.h
	@echo "Compilation de AStar.cpp"
	@$(CC) $(CFLAGS_COMP) AStar.cpp
	
Interfaces.o : Interfaces.cpp Interfaces.h Point.h AStar.h
	@echo "Compilation de Interfaces.cpp"
	@$(CC) $(CFLAGS_COMP) Interfaces.cpp
	
Debug.o : Debug.cpp Debug.h Point.h Obstacles.h Consigne.h Contrats.h
	@echo "Compilation de Debug.cpp"
	@$(CC) $(CFLAGS_COMP) Debug.cpp

clean :
	@echo "Suppression des .o et des ~"
	@rm -f $(OBJS) rm *~

mrpropre : clean
	@echo "Suppression de l'executable."
	@rm -f debug
	



