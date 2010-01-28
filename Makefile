
CC=gcc
CFLAGS=-c -ggdb
INC=-I/usr/include -I/usr/include/CGAL -I../Libraries/vmmlib/include -I../Libraries -I../Libraries/ann_1.1.1/include -I/usr/include/boost -I/usr/include/lua5.1
LDFLAGS=-L/usr/lib -L../Libraries/xml -lfltk -lfltk_gl -lfltk_forms -lglut -lcustomxml -lboost_thread-mt -lboost_system-mt
SOURCES=AsyncSerial.cpp RepSnapper.cpp stl.cpp RepRapSerial.cpp ProcessController.cpp Printer.cpp ModelViewController.cpp glutils.cpp GCode.cpp ArcBall.cpp stdafx.cpp UI.cxx
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=repsnapper

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) ${INC} $(OBJECTS) $(LDFLAGS) -o $@ 

.cpp.o:
	$(CC) ${INC} $(CFLAGS) $< -o $@

clean:
	rm -f *.o
	

# not needed
#	<Kulitorum> fillet.cpp
#	<Kulitorum> pathfinder.* (will someday keep the extruder inside the object for move commands, to avoid ooze)
#	<Kulitorum> reprapusb.h
#	<Kulitorum> search.*
#	<Kulitorum> simulator.*
