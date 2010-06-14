#
# Combined Makefile for Linux and OS/X
#

CC=gcc
CXX=g++
CFLAGS=-c -O0 -g -Wall
UNAME := $(shell uname)
LIB_DIR=Libraries

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
	LDFLAGS=-L$(OPT_DIR)/lib -lpthread -lfltk -lfltk_forms -lfltk_gl -L$(LIB_DIR)/xml
	LDFLAGS+= -lboost_thread-mt -l boost_system-mt
    LDFLAGS+= -framework Carbon -framework OpenGL -framework GLUT -framework AGL
endif

GENERATED=UI.cxx UI.h

SOURCES=AsyncSerial.cpp RepSnapper.cpp stl.cpp gpc.c RepRapSerial.cpp \
	ProcessController.cpp Printer.cpp ModelViewController.cpp \
	glutils.cpp GCode.cpp ArcBall.cpp stdafx.cpp UI.cxx \
	RFO.cpp Flu_DND.cpp flu_pixmaps.cpp FluSimpleString.cpp \
	Flu_Tree_Browser.cpp ivcon.cpp File.cpp platform.cpp \
	$(LIB_DIR)/xml/xml.cpp 

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

%.cxx %.h:%.fl
	fluid -c $<
%.o:%.cxx
	$(CXX) ${INC} $(CFLAGS) $< -o $@
%.o:%.cpp
	$(CXX) ${INC} $(CFLAGS) $< -o $@
%.o:%.CPP # sexy XML.CPP naming ...
	$(CXX) ${INC} $(CFLAGS) $< -o $@
%.o:%.c
	$(CC) ${INC} $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(GENERATED)

VER=0.1.0
PKG_NAME=rep-snapper
PKG_TARBALL=$(PKG_NAME)-$(VER).tar.bz2

dist:
	COMMIT_HASH=`git show-ref -s -h | head -n 1` ; \
	git archive --prefix=$(PKG_NAME)-$(VER)/ --format=tar $$COMMIT_HASH \
		| bzip2 -f > $(PKG_TARBALL)

# make update-deps will re-write the dependenciues below
update-depends:
	makedepend -Y $(SOURCES)

# not needed
#	<Kulitorum> fillet.cpp
#	<Kulitorum> pathfinder.* (will someday keep the extruder inside the object for move commands, to avoid ooze)
#	<Kulitorum> reprapusb.h
#	<Kulitorum> search.*
#	<Kulitorum> simulator.*

# DO NOT DELETE THIS LINE -- make depend depends on it, deletes after here and updates the deps.

AsyncSerial.o: stdafx.h config.h platform.h ArcBall.h ivcon.h AsyncSerial.h
RepSnapper.o: stdafx.h config.h platform.h ArcBall.h ivcon.h
RepSnapper.o: ModelViewController.h UI.h File.h Flu_Tree_Browser.h
RepSnapper.o: Flu_Enumerations.h flu_export.h FluSimpleString.h gcode.h stl.h
RepSnapper.o: ProcessController.h Printer.h RFO.h glutils.h RepRapSerial.h
RepSnapper.o: AsyncSerial.h
stl.o: stdafx.h config.h platform.h ArcBall.h ivcon.h stl.h gcode.h UI.h
stl.o: File.h ModelViewController.h ProcessController.h Printer.h RFO.h
stl.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h FluSimpleString.h
stl.o: glutils.h RepRapSerial.h AsyncSerial.h gpc.h
gpc.o: gpc.h
RepRapSerial.o: stdafx.h config.h platform.h ArcBall.h ivcon.h RepRapSerial.h
RepRapSerial.o: UI.h File.h ModelViewController.h gcode.h stl.h
RepRapSerial.o: ProcessController.h Printer.h RFO.h Flu_Tree_Browser.h
RepRapSerial.o: Flu_Enumerations.h flu_export.h FluSimpleString.h glutils.h
RepRapSerial.o: AsyncSerial.h Convert.h
ProcessController.o: stdafx.h config.h platform.h ArcBall.h ivcon.h
ProcessController.o: ModelViewController.h UI.h File.h Flu_Tree_Browser.h
ProcessController.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
ProcessController.o: gcode.h stl.h ProcessController.h Printer.h RFO.h
ProcessController.o: glutils.h RepRapSerial.h AsyncSerial.h
Printer.o: stdafx.h config.h platform.h ArcBall.h ivcon.h ProcessController.h
Printer.o: Printer.h gcode.h RFO.h Flu_Tree_Browser.h Flu_Enumerations.h
Printer.o: flu_export.h FluSimpleString.h stl.h
ModelViewController.o: stdafx.h config.h platform.h ArcBall.h ivcon.h
ModelViewController.o: ModelViewController.h UI.h File.h Flu_Tree_Browser.h
ModelViewController.o: Flu_Enumerations.h flu_export.h FluSimpleString.h
ModelViewController.o: gcode.h stl.h ProcessController.h Printer.h RFO.h
ModelViewController.o: glutils.h RepRapSerial.h AsyncSerial.h
glutils.o: stdafx.h config.h platform.h ArcBall.h ivcon.h glutils.h
GCode.o: stdafx.h config.h platform.h ArcBall.h ivcon.h gcode.h UI.h File.h
GCode.o: ModelViewController.h stl.h ProcessController.h Printer.h RFO.h
GCode.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h FluSimpleString.h
GCode.o: glutils.h RepRapSerial.h AsyncSerial.h
ArcBall.o: stdafx.h config.h platform.h ArcBall.h ivcon.h
stdafx.o: stdafx.h config.h platform.h ArcBall.h ivcon.h stl.h
UI.o: UI.h config.h stdafx.h platform.h ArcBall.h ivcon.h File.h
UI.o: ModelViewController.h gcode.h stl.h ProcessController.h Printer.h RFO.h
UI.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h FluSimpleString.h
UI.o: glutils.h RepRapSerial.h AsyncSerial.h
RFO.o: stdafx.h config.h platform.h ArcBall.h ivcon.h UI.h File.h
RFO.o: ModelViewController.h gcode.h stl.h ProcessController.h Printer.h
RFO.o: RFO.h Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
RFO.o: FluSimpleString.h glutils.h RepRapSerial.h AsyncSerial.h flu_pixmaps.h
Flu_DND.o: Flu_DND.h Flu_Enumerations.h flu_export.h
flu_pixmaps.o: flu_pixmaps.h flu_export.h
FluSimpleString.o: FluSimpleString.h Flu_Enumerations.h flu_export.h
Flu_Tree_Browser.o: Flu_Tree_Browser.h Flu_Enumerations.h flu_export.h
Flu_Tree_Browser.o: FluSimpleString.h flu_pixmaps.h
ivcon.o: ivcon.h
File.o: stdafx.h config.h platform.h ArcBall.h ivcon.h File.h
File.o: ModelViewController.h UI.h Flu_Tree_Browser.h Flu_Enumerations.h
File.o: flu_export.h FluSimpleString.h gcode.h stl.h ProcessController.h
File.o: Printer.h RFO.h glutils.h RepRapSerial.h AsyncSerial.h
../Libraries/xml/xml.o: ../Libraries/xml/xml.h
