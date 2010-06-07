
CC=gcc
CXX=g++
CFLAGS=-c -ggdb
LIB_DIR=Libraries
INC=-I/usr/include -I$(LIB_DIR)/vmmlib/include -I$(LIB_DIR) -I$(LIB_DIR)/ann_1.1.1/include -I/usr/include/boost -I/usr/include/lua5.1
LDFLAGS=-L/usr/lib -L$(LIB_DIR)/xml -lGLU -lfltk -lfltk_gl -lfltk_forms -lglut -lcustomxml -lboost_thread-mt -lboost_system-mt
SOURCES=AsyncSerial.cpp RepSnapper.cpp stl.cpp gpc.c RepRapSerial.cpp ProcessController.cpp Printer.cpp ModelViewController.cpp glutils.cpp GCode.cpp ArcBall.cpp stdafx.cpp UI.cxx RFO.cpp Flu_DND.cpp flu_pixmaps.cpp FluSimpleString.cpp Flu_Tree_Browser.cpp ivcon.cpp
HEADERS=ArcBall.h AsyncSerial.h Convert.h Flu_DND.h Flu_Enumerations.h flu_export.h flu_pixmaps.h FluSimpleString.h Flu_Tree_Browser.h gcode.h glutils.h gpc.h ivcon.h miniball.h ModelViewController.h Printer.h ProcessController.h RepRapSerial.h RFO.h search.h Serial.h stdafx.h stl.h triangle.h UI.h
OBJECTS=$(subst .c,.o,$(subst .cxx,.o,$(subst .cpp,.o,$(SOURCES))))

EXECUTABLE=repsnapper

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) ${INC} $(OBJECTS) $(LDFLAGS) -o $@ 

%.cxx:%.fl
	fluid -c $<
%.o:%.cxx
	$(CXX) ${INC} $(CFLAGS) $< -o $@
%.o:%.cpp
	$(CXX) ${INC} $(CFLAGS) $< -o $@
%.o:%.c
	$(CC) ${INC} $(CFLAGS) $< -o $@

clean:
	make -C ../Libraries/xml clean
	rm -f *.o

# not needed
#	<Kulitorum> fillet.cpp
#	<Kulitorum> pathfinder.* (will someday keep the extruder inside the object for move commands, to avoid ooze)
#	<Kulitorum> reprapusb.h
#	<Kulitorum> search.*
#	<Kulitorum> simulator.*

# DO NOT DELETE THIS LINE -- make depend depends on it, deletes after here and updates the deps.

ArcBall.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
ArcBall.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
AsyncSerial.o: ArcBall.h gcode.h ModelViewController.h
AsyncSerial.o: Printer.h RFO.h glutils.h RepRapSerial.h
fillet.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
fillet.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
Flu_DND.o: flu_export.h
flu_pixmaps.o: flu_pixmaps.h flu_export.h
Flu_Tree_Browser.o: flu_pixmaps.h
GCode.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
GCode.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
glutils.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
glutils.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
Main.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
ModelViewController.o: ProcessController.h Printer.h RFO.h glutils.h
Printer.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
Printer.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
ProcessController.o: Printer.h RFO.h glutils.h RepRapSerial.h
RepRapSerial.o: ArcBall.h gcode.h ModelViewController.h
RepRapSerial.o: Printer.h RFO.h glutils.h RepRapSerial.h
RepRapSerial.o: Convert.h
RepSnapper.o: ArcBall.h gcode.h ModelViewController.h
RepSnapper.o: Printer.h RFO.h glutils.h RepRapSerial.h
Search.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
Search.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
stdafx.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
stdafx.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
Flu_DND.o: flu_export.h
Flu_Enumerations.o: flu_export.h
flu_pixmaps.o: flu_export.h
ModelViewController.o: ModelViewController.h stl.h ProcessController.h
ModelViewController.o: Printer.h RFO.h Flu_Tree_Browser.h
Printer.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
Printer.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
ProcessController.o: Printer.h RFO.h glutils.h RepRapSerial.h
RepRapSerial.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
Serial.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
Serial.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
stdafx.o: ProcessController.h stl.h RFO.h Flu_Tree_Browser.h
stdafx.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
stdafx.o: glutils.h RepRapSerial.h
