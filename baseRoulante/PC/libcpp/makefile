OBJS=main.o Point.o Consigne.o Obstacles.o AStar.o
CC=g++
CFLAGS_LINK=-lm -lserial -o
CFLAGS_COMP=-O -c

debug: $(OBJS)
	g++ $(OBJS) $(CFLAGS_LINK) debug

main.o : main.cpp Point.h
	g++ $(CFLAGS_COMP) main.cpp

Point.o : Point.cpp Point.h Consigne.h
	g++ $(CFLAGS_COMP) Point.cpp

Consigne.o : Consigne.cpp Consigne.h
	g++ $(CFLAGS_COMP) Consigne.cpp

Obstacles.o : Obstacles.cpp Obstacles.h Point.h
	g++ $(CFLAGS_COMP) Obstacles.cpp

AStar.o : AStar.cpp AStar.h Point.h Obstacles.h
	g++ $(CFLAGS_COMP) AStar.cpp

clean :
	rm -f $(OBJS)

mrpropre :
	rm -f $(OBJS) debug



