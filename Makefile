#
# Combined Makefile for Linux and OS/X
#

CC=gcc
CXX=g++
LIB_DIR=../Libraries
UNAME := $(shell uname)
ifeq ($(TARGET),)
	TARGET=RELEASE
endif
ifeq ($(UNAME),Darwin)
	WARNING_FLAGS = -Wall
else
	WARNING_FLAGS = -Wall -Wno-pragmas
endif

ifeq ($(TARGET),RELEASE)
	CFLAGS = -c -O2 $(WARNING_FLAGS)
	EXECUTABLE=repsnapper
else
	CFLAGS = -c -g -O0 $(WARNING_FLAGS)
	EXECUTABLE=repsnapper_debug
endif

# Linux
ifeq ($(UNAME),Linux)
    GTK_LIBS=`pkg-config --libs gtk+-2.0 gthread-2.0`
    GTK_CFLAGS=`pkg-config --cflags gtk+-2.0 gthread-2.0` -DHAVE_GTK
    INC=$(GTK_CFLAGS) -I/usr/include -I$(LIB_DIR) -I$(LIB_DIR)/vmmlib/include -I$(LIB_DIR)/ann_1.1.1/include -I/usr/include/boost -I/usr/include/lua5.1
	INC+=-I$(LIB_DIR)/polylib
    LDFLAGS=$(GTK_LIBS) -L/usr/lib -lGLU -lfltk -lfltk_gl -lfltk_forms -lglut -lboost_thread-mt -lboost_system-mt 
	LDFLAGS+=-L$(LIB_DIR)/polylib -lpolylib
endif

# Mac
ifeq ($(UNAME),Darwin)
    # assumes you have installed MacPorts from http://www.macports.org and run:
    # sudo port install boost fltk lua
	# assumes you have built boost as in the Readme.MacOsx.txt

    BOOST_HOME=../Libraries/boost-darwin
    BOOST_INC=-I$(BOOST_HOME)/include/boost-1_43
	ifeq ($(TARGET),RELEASE)
		BOOST_LIB=$(BOOST_HOME)/lib/libboost_thread-xgcc40-mt.a $(BOOST_HOME)/lib/libboost_system-xgcc40-mt.a
	else
		BOOST_LIB=$(BOOST_HOME)/lib/libboost_thread-xgcc40-mt-d.a $(BOOST_HOME)/lib/libboost_system-xgcc40-mt-d.a
	endif
    MACPORTS_DIR=/opt/local
	INC=$(BOOST_INC) -I$(MACPORTS_DIR)/include -I$(LIB_DIR)/vmmlib/include -I$(LIB_DIR)/ann_1.1.1/include -I$(LIB_DIR)
	INC+=-I$(LIB_DIR)/polylib
	LDFLAGS=$(BOOST_LIB) -L$(MACPORTS_DIR)/lib -lpthread -lfltk -lfltk_forms -lfltk_gl -L$(LIB_DIR)/xml
	LDFLAGS+=-L$(LIB_DIR)/polylib -lpolylib
    LDFLAGS+=-framework Carbon -framework OpenGL -framework GLUT -framework AGL
endif

GENERATED=UI.cxx UI.h

SOURCES=AsyncSerial.cpp RepSnapper.cpp stl.cpp RepRapSerial.cpp \
	ProcessController.cpp Printer.cpp ModelViewController.cpp \
	glutils.cpp GCode.cpp ArcBall.cpp stdafx.cpp UI.cxx \
	RFO.cpp Flu_DND.cpp flu_pixmaps.cpp FluSimpleString.cpp \
	Flu_Tree_Browser.cpp ivcon.cpp File.cpp platform.cpp \
	$(LIB_DIR)/xml/xml.cpp 

HEADERS=ArcBall.h AsyncSerial.h Convert.h Flu_DND.h Flu_Enumerations.h \
	flu_export.h flu_pixmaps.h FluSimpleString.h Flu_Tree_Browser.h \
	gcode.h glutils.h ivcon.h miniball.h ModelViewController.h \
	Printer.h ProcessController.h RepRapSerial.h RFO.h search.h \
	Serial.h stdafx.h stl.h triangle.h UI.h platform.h

OBJECTS=$(subst .c,.o,$(subst .cxx,.o,$(subst .cpp,.o,$(SOURCES))))


all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): xml_lib poly_lib $(OBJECTS)
	$(CXX) ${INC} $(OBJECTS) $(LDFLAGS) -o $@

%.cxx %.h:%.fl
	rm -f $@ # fluid doesn't remove on failure.
	fluid -c $<
%.o:%.cxx
	$(CXX) ${INC} $(CFLAGS) $< -o $@
%.o:%.cpp
	$(CXX) ${INC} $(CFLAGS) $< -o $@
%.o:%.c
	$(CC) ${INC} $(CFLAGS) $< -o $@

xml_lib:
	make -C ../Libraries/xml/ all

poly_lib:
	make -C ../Libraries/polylib/ all
	
clean:
	rm -f $(OBJECTS) $(EXECUTABLE) $(GENERATED)
	make -i -C ../Libraries/xml/ clean
	make -i -C ../Libraries/polylib/ clean

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
stl.o: glutils.h RepRapSerial.h AsyncSerial.h
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
