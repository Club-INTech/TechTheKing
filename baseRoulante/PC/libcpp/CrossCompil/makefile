OBJS=main.o Point.o Consigne.o Obstacles.o AStar.o \
		Debug.o Interfaces.o Server.o Thread.o
CC=arm-none-linux-gnueabi-g++
CFLAGS_LINK=-O3 -lm -lserial -lboost_thread -o 
CFLAGS_COMP=-O -Wall -c

compil: $(OBJS)
	@echo "Création de l'executable : Cross-Compilation"
	@$(CC) $(OBJS) $(CFLAGS_LINK) debug
	
Thread.o : Thread.cpp Thread.h
	@echo -n "Cross-Compilation de Thread.cpp ..."
	@$(CC) $(CFLAGS_COMP) Thread.cpp
	@echo " fait."

Server.o : Server.cpp Server.h Obstacles.h Thread.h
	@echo -n "Cross-Compilation de Server.cpp ..."
	@$(CC) $(CFLAGS_COMP) Server.cpp
	@echo " fait."
	
main.o : main.cpp Point.h AStar.h Debug.h Contrats.h
	@echo "Cross-Compilation de main.cpp"
	@$(CC) $(CFLAGS_COMP) main.cpp

Point.o : Point.cpp Point.h Consigne.h Contrats.h
	@echo "Cross-Compilation de Point.cpp"
	@$(CC) $(CFLAGS_COMP) Point.cpp

Consigne.o : Consigne.cpp Consigne.h Contrats.h
	@echo "Cross-Compilation de Consigne.cpp"
	@$(CC) $(CFLAGS_COMP) Consigne.cpp

Obstacles.o : Obstacles.cpp Obstacles.h Point.h Contrats.h
	@echo "Cross-Compilation de Obstacles.cpp"
	@$(CC) $(CFLAGS_COMP) Obstacles.cpp

AStar.o : AStar.cpp AStar.h Point.h Obstacles.h Contrats.h
	@echo "Cross-Compilation de AStar.cpp"
	@$(CC) $(CFLAGS_COMP) AStar.cpp
	
Interfaces.o : Interfaces.cpp Interfaces.h Point.h AStar.h
	@echo "Cross-Compilation de Interfaces.cpp"
	@$(CC) $(CFLAGS_COMP) Interfaces.cpp
	
Debug.o : Debug.cpp Debug.h Point.h Obstacles.h Consigne.h Contrats.h
	@echo "Cross-Compilation de Debug.cpp"
	@$(CC) $(CFLAGS_COMP) Debug.cpp

clean :
	@echo "Suppression des .o et des ~"
	@rm -f $(OBJS) rm *~

mrpropre : clean
	@echo "Suppression de l'executable."
	@rm -f debug
	



