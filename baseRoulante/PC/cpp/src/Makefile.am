DEFS = @DEFS@
lib_LTLIBRARIES = libChessUp.la
libChessUp_la_SOURCES = hiddata.c i2cLib.c AStar.cpp Consigne.cpp Debug.cpp Interfaces.cpp InterfaceRuby_wrap.cxx Obstacles.cpp Point.cpp Server.cpp Thread.cpp
noinst_HEADERS = AStar.h Consigne.h Contrats.h Debug.h Interfaces.h Obstacles.h Point.h ClientSocket.h Thread.h
 
InterfaceRuby_wrap.cxx: InterfaceRuby.i
	swig -c++ -ruby InterfaceRuby.i
