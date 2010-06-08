#
# Combined Makefile for Linux and OS/X
#

CC=gcc
CXX=g++
CFLAGS=-c -O2 -g
UNAME := $(shell uname)
LIB_DIR=../Libraries

# Linux
ifeq ($(UNAME),Linux)
    GTK_LIBS=`pkg-config --libs gtk+-2.0 gthread-2.0`
    GTK_CFLAGS=`pkg-config --cflags gtk+-2.0 gthread-2.0` -DHAVE_GTK
    INC=$(GTK_CFLAGS) -I/usr/include -I$(LIB_DIR) -I$(LIB_DIR)/vmmlib/include -I$(LIB_DIR)/ann_1.1.1/include -I/usr/include/boost -I/usr/include/lua5.1
    LDFLAGS=$(GTK_LIBS) -L/usr/lib -lGLU -lfltk -lfltk_gl -lfltk_forms -lglut -lboost_thread-mt -lboost_system-mt 
endif

# Mac
ifeq ($(UNAME),Darwin)
# assumes you installed MacPorts http://www.macports.org and run:
# sudo port install boost fltk lua
    OPT_DIR=/opt/local
	INC=-I$(OPT_DIR)/include -I$(LIB_DIR)/vmmlib/include -I$(LIB_DIR)/ann_1.1.1/include -I$(LIB_DIR)
	LDFLAGS=-L$(OPT_DIR)/lib -lpthread -lfltk -lfltk_forms -lfltk_gl -L$(LIB_DIR)/xml -lcustomxml
	LDFLAGS+= -lboost_thread-mt -l boost_system-mt
    LDFLAGS+= -framework Carbon -framework OpenGL -framework GLUT -framework AGL
endif

SOURCES=AsyncSerial.cpp RepSnapper.cpp stl.cpp gpc.c RepRapSerial.cpp \
	ProcessController.cpp Printer.cpp ModelViewController.cpp \
	glutils.cpp GCode.cpp ArcBall.cpp stdafx.cpp UI.cxx \
	RFO.cpp Flu_DND.cpp flu_pixmaps.cpp FluSimpleString.cpp \
	Flu_Tree_Browser.cpp ivcon.cpp File.cpp \
	$(LIB_DIR)/xml/XML.CPP 

HEADERS=ArcBall.h AsyncSerial.h Convert.h Flu_DND.h Flu_Enumerations.h \
	flu_export.h flu_pixmaps.h FluSimpleString.h Flu_Tree_Browser.h \
	gcode.h glutils.h gpc.h ivcon.h miniball.h ModelViewController.h \
	Printer.h ProcessController.h RepRapSerial.h RFO.h search.h \
	Serial.h stdafx.h stl.h triangle.h UI.h platform.h

OBJECTS=$(subst .c,.o,$(subst .cxx,.o,$(subst .cpp,.o,$(subst .CPP,.o,$(SOURCES)))))

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
	rm -f *.o

# make update-deps will re-write the dependenciues below
update-depends:
	makedepend -Y $(SOURCES) $(HEADERS)

# not needed
#	<Kulitorum> fillet.cpp
#	<Kulitorum> pathfinder.* (will someday keep the extruder inside the object for move commands, to avoid ooze)
#	<Kulitorum> reprapusb.h
#	<Kulitorum> search.*
#	<Kulitorum> simulator.*

# DO NOT DELETE THIS LINE -- make depend depends on it, deletes after here and updates the deps.

AsyncSerial.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h
AsyncSerial.o: UI.h Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
AsyncSerial.o: FluSimpleString.h stl.h ProcessController.h Printer.h RFO.h
AsyncSerial.o: glutils.h RepRapSerial.h AsyncSerial.h File.h ivcon.h
RepSnapper.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h
RepSnapper.o: UI.h Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
RepSnapper.o: FluSimpleString.h stl.h ProcessController.h Printer.h RFO.h
RepSnapper.o: glutils.h RepRapSerial.h AsyncSerial.h File.h ivcon.h
stl.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h UI.h
stl.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h FluSimpleString.h
stl.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
stl.o: AsyncSerial.h File.h ivcon.h gpc.h
gpc.o: gpc.h
RepRapSerial.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h
RepRapSerial.o: UI.h Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
RepRapSerial.o: FluSimpleString.h stl.h ProcessController.h Printer.h RFO.h
RepRapSerial.o: glutils.h RepRapSerial.h AsyncSerial.h File.h ivcon.h
RepRapSerial.o: Convert.h
ProcessController.o: stdafx.h platform.h ArcBall.h gcode.h
ProcessController.o: ModelViewController.h UI.h Flu_Tree_Browser.h
ProcessController.o: Flu_Enumerations.h flu_export.h FluSimpleString.h stl.h
ProcessController.o: ProcessController.h Printer.h RFO.h glutils.h
ProcessController.o: RepRapSerial.h AsyncSerial.h File.h ivcon.h
Printer.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h UI.h
Printer.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
Printer.o: FluSimpleString.h stl.h ProcessController.h Printer.h RFO.h
Printer.o: glutils.h RepRapSerial.h AsyncSerial.h File.h ivcon.h
ModelViewController.o: stdafx.h platform.h ArcBall.h gcode.h
ModelViewController.o: ModelViewController.h UI.h Flu_Tree_Browser.h
ModelViewController.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
ModelViewController.o: stl.h ProcessController.h Printer.h RFO.h glutils.h
ModelViewController.o: RepRapSerial.h AsyncSerial.h File.h ivcon.h
glutils.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h UI.h
glutils.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
glutils.o: FluSimpleString.h stl.h ProcessController.h Printer.h RFO.h
glutils.o: glutils.h RepRapSerial.h AsyncSerial.h File.h ivcon.h
GCode.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h UI.h
GCode.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h FluSimpleString.h
GCode.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
GCode.o: AsyncSerial.h File.h ivcon.h
ArcBall.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h UI.h
ArcBall.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
ArcBall.o: FluSimpleString.h stl.h ProcessController.h Printer.h RFO.h
ArcBall.o: glutils.h RepRapSerial.h AsyncSerial.h File.h ivcon.h
stdafx.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h UI.h
stdafx.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
stdafx.o: FluSimpleString.h stl.h ProcessController.h Printer.h RFO.h
stdafx.o: glutils.h RepRapSerial.h AsyncSerial.h File.h ivcon.h
UI.o: UI.h stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h stl.h
UI.o: ProcessController.h Printer.h RFO.h Flu_Tree_Browser.h
UI.o: Flu_Enumerations.h flu_export.h FluSimpleString.h glutils.h
UI.o: RepRapSerial.h AsyncSerial.h File.h ivcon.h
RFO.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h UI.h
RFO.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h FluSimpleString.h
RFO.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
RFO.o: AsyncSerial.h File.h ivcon.h flu_pixmaps.h
Flu_DND.o: Flu_DND.h Flu_Enumerations.h flu_export.h
flu_pixmaps.o: flu_pixmaps.h flu_export.h
FluSimpleString.o: FluSimpleString.h Flu_Enumerations.h flu_export.h
Flu_Tree_Browser.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
Flu_Tree_Browser.o: FluSimpleString.h flu_pixmaps.h
ivcon.o: ivcon.h
File.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h UI.h
File.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h FluSimpleString.h
File.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
File.o: AsyncSerial.h File.h ivcon.h
../Libraries/xml/XML.o: ../Libraries/xml/XML.H
Flu_DND.o: Flu_Enumerations.h flu_export.h
Flu_Enumerations.o: flu_export.h
flu_pixmaps.o: flu_export.h
FluSimpleString.o: Flu_Enumerations.h flu_export.h
Flu_Tree_Browser.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
gcode.o: platform.h
ModelViewController.o: UI.h stdafx.h platform.h ArcBall.h gcode.h
ModelViewController.o: ModelViewController.h stl.h ProcessController.h
ModelViewController.o: Printer.h RFO.h Flu_Tree_Browser.h Flu_Enumerations.h
ModelViewController.o: flu_export.h FluSimpleString.h glutils.h
ModelViewController.o: RepRapSerial.h AsyncSerial.h File.h ivcon.h
Printer.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h UI.h
Printer.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
Printer.o: FluSimpleString.h stl.h ProcessController.h Printer.h RFO.h
Printer.o: glutils.h RepRapSerial.h AsyncSerial.h File.h ivcon.h
ProcessController.o: stdafx.h platform.h ArcBall.h gcode.h
ProcessController.o: ModelViewController.h UI.h Flu_Tree_Browser.h
ProcessController.o: Flu_Enumerations.h flu_export.h FluSimpleString.h stl.h
ProcessController.o: ProcessController.h Printer.h RFO.h glutils.h
ProcessController.o: RepRapSerial.h AsyncSerial.h File.h ivcon.h
RepRapSerial.o: UI.h stdafx.h platform.h ArcBall.h gcode.h
RepRapSerial.o: ModelViewController.h stl.h ProcessController.h Printer.h
RepRapSerial.o: RFO.h Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
RepRapSerial.o: FluSimpleString.h glutils.h RepRapSerial.h AsyncSerial.h
RepRapSerial.o: File.h ivcon.h
RFO.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h FluSimpleString.h
RFO.o: stl.h platform.h
Serial.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h UI.h
Serial.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
Serial.o: FluSimpleString.h stl.h ProcessController.h Printer.h RFO.h
Serial.o: glutils.h RepRapSerial.h AsyncSerial.h File.h ivcon.h
stdafx.o: platform.h ArcBall.h gcode.h ModelViewController.h UI.h stdafx.h
stdafx.o: Printer.h ProcessController.h stl.h RFO.h Flu_Tree_Browser.h
stdafx.o: Flu_Enumerations.h flu_export.h FluSimpleString.h File.h
stdafx.o: AsyncSerial.h ivcon.h glutils.h RepRapSerial.h
stl.o: platform.h
UI.o: stdafx.h platform.h ArcBall.h gcode.h ModelViewController.h UI.h
UI.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h FluSimpleString.h
UI.o: stl.h ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
UI.o: AsyncSerial.h File.h ivcon.h
