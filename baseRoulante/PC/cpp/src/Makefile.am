DEFS = @DEFS@
lib_LTLIBRARIES = libChessUp.la
libChessUp_la_SOURCES = hiddata.c i2cLib.c AStar.cpp Consigne.cpp Debug.cpp Interfaces.cpp InterfaceRuby_wrap.cxx Obstacles.cpp Point.cpp Socket.cpp Thread.cpp Constantes.cpp
 
InterfaceRuby_wrap.cxx: InterfaceRuby.i
	swig -c++ -ruby InterfaceRuby.i
