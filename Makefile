
CC=gcc
CFLAGS=-c 
INC=-I/usr/include -I/usr/include/CGAL -I../Libraries/vmmlib/include -I../Libraries -I../Libraries/ann_1.1.1/include
LDFLAGS=-L/usr/lib -L../Libraries/xml -lfl -lfltk -lfltk_gl -lfltk_forms -lglut -lcustomxml
SOURCES=RepSnapper.cpp stl.cpp RepRapSerial.cpp ProcessController.cpp Printer.cpp ModelViewController.cpp glutils.cpp GCode.cpp ArcBall.cpp stdafx.cpp UI.cxx
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=repsnapper

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CC) ${INC} $(OBJECTS) $(LDFLAGS) -o $@ 

.cpp.o:
	$(CC) ${INC} $(CFLAGS) $< -o $@


# not needed
#	<Kulitorum> fillet.cpp
#	<Kulitorum> pathfinder.* (will someday keep the extruder inside the object for move commands, to avoid ooze)
#	<Kulitorum> reprapusb.h
#	<Kulitorum> search.*
#	<Kulitorum> simulator.*