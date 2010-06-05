
CC=gcc
CXX=g++
CFLAGS=-c -ggdb
LIB_DIR=Libraries
INC=-I/usr/include -I$(LIB_DIR)/vmmlib/include -I$(LIB_DIR) -I$(LIB_DIR)/ann_1.1.1/include -I/usr/include/boost -I/usr/include/lua5.1
LDFLAGS=-L/usr/lib -L$(LIB_DIR)/xml -lGLU -lfltk -lfltk_gl -lfltk_forms -lglut -lcustomxml -lboost_thread-mt -lboost_system-mt
SOURCES=AsyncSerial.cpp RepSnapper.cpp stl.cpp gpc.c RepRapSerial.cpp ProcessController.cpp Printer.cpp ModelViewController.cpp glutils.cpp GCode.cpp ArcBall.cpp stdafx.cpp UI.cxx RFO.cpp Flu_DND.cpp flu_pixmaps.cpp FluSimpleString.cpp Flu_Tree_Browser.cpp ivcon.cpp
HEADERS=ArcBall.h AsyncSerial.h Convert.h Flu_DND.h Flu_Enumerations.h flu_export.h flu_pixmaps.h FluSimpleString.h Flu_Tree_Browser.h gcode.h glutils.h gpc.h ivcon.h miniball.h ModelViewController.h Printer.h ProcessController.h RepRapSerial.h RFO.h search.h Serial.h stdafx.h stl.h triangle.h UI.h
OBJECTS=gpc.o $(SOURCES:.cpp=.o)
EXECUTABLE=repsnapper

Libraries:
	@echo "Missing a sym-link to the libraries we need, read the README ..."

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) ${INC} $(OBJECTS) $(LDFLAGS) -o $@ 

.cpp.o: Libraries
	$(CXX) ${INC} $(CFLAGS) $< -o $@
.c.o: Libraries
	$(CC) ${INC} $(CFLAGS) $< -o $@

clean:
	rm -f *.o

# not needed
#	<Kulitorum> fillet.cpp
#	<Kulitorum> pathfinder.* (will someday keep the extruder inside the object for move commands, to avoid ooze)
#	<Kulitorum> reprapusb.h
#	<Kulitorum> search.*
#	<Kulitorum> simulator.*
