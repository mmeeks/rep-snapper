/* -------------------------------------------------------- *
*
* GCode.h
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */
#pragma once
#include <vector>
//#include <windows.h>											// Header File For Windows
#include <GL/gl.h>												// Header File For The OpenGL32 Library
#include <GL/glu.h>												// Header File For The GLu32 Library
//#include <gl\glaux.h>											// Header File For The GLaux Library

#include "math.h"                                               // Needed for sqrtf

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <vmmlib/vmmlib.h>

using namespace std;
using namespace vmml;

class ProcessController;
/*

Movement Codes

G0 Rapid Motion Implemented - supports X, Y, and Z axes.
G1 Coordinated Motion Implemented - supports X, Y, and Z axes.
G2 Arc � Clockwise Not used by Skienforge
G3 Arc - Counter Clockwise Not used by Skienforge
G4 Dwell Implemented.
G20 Inches as units Implemented.
G21 Millimetres as units Implemented.
G28 Go Home Implemented. (X = -135mm, Y = 100mm, Z = 0mm)
G90 Absolute Positioning Implemented. V1.0.5
G92 Set current as home Implemented V1.0.5

G1

Coordinated Motion

G1 X-53.67 Y52.16 Z0.54 E54.394 F213.333

Go to X=-53.67, Y=52.16, Z=0.54 using SPEED=213.333 mm per minute (3.5mm/sec)
E is distance and is still not implemented in RapMan firmware
Note that if E values (E-code) exists for the G1 the RapMan firmware up to 1.0.5 will not parse it properly and will cause all different problems during print. You can disable E value generation in INSET tab by setting the "Extrusion Distance Format Choice" to "Do Not Add Extrusion Distance".
Special functions (M Codes)

M101 Turn extruder on Forward.
M102 Turn extruder on Reverse. Still to add
M103 Turn extruder off.
M104 S145.0 Set target temperature to 145.0 C.
M105 Custom code for temperature reading. Not used
M106 Turn fan on.
M107 Turn fan off.
M108 S400 Set Extruder speed to S value/10 = 40rpm.

Future functions (M Codes)

M codes that may be introduced for the RepRap
M120 Pgain PWM control values
M121 Igain �
M122 Dgain �
M123 Imax �
M124 Imin �


Special BFB-only M Codes

M220 Turn off AUX V1.0.5
M221 Turn on AUX V1.0.5
M222 Set speed of fast XY moves V1.0.5
M223 Set speed of fast Z moves V1.0.5
M224 Enable extruder motor during fast move
M225 Disable extruder motor during fast move
M226 Pause the printer until ESC key is pressed
M227 S1000 P800 on extruder stop (M103) reverse the extruder stepper for S turns and on extruder start (M101) prepare (push) filament P steps (available from firmware 1.0.8)

Typical File header produced by Skeinforge

G21 millimeter system selection
G90 absolute distance mode
G28 Return to home position
M222 S1024 Manually added to file M222 S500 Set speed of fast XY moves. Default value 500 -V1.0.5
M223 S768 Manually added to file M223 S500 Set speed of fast Z moves. Default value 500 -V1.0.5
M103 Turn extruder OFF.
M105 Custom code for temperature reading � not used by RapMan
M104 S247.0 Set temperature to 145.0 DegC.
G1 X0.0 Y0.0 Z0.0 F480.0 linear interpolation
M101 Turn extruder ON.
Main code follows

*/


inline int ToInt(const std::string& s)
{
	std::istringstream i(s);
	int x;
	if (!(i >> x))
		return -1;
	return x;
}

inline float ToFloat(const std::string& s)
{
	std::istringstream i(s);
	float x;
	if (!(i >> x))
		return -1;
	return x;
}

inline string FromInt(const int i)
{
	std::stringstream s;
	s << i;
	return s.str();
}

inline string FromFloat(const float i)
{
	std::stringstream s;
	s << i;
	return s.str();
}


enum GCodes{GOTO, DRAWTO, MILLIMETERSASUNITS, RAPIDMOTION, COORDINATEDMOTION, COORDINATEDMOTION3D,  EXTRUDERON, EXTRUDEROFF, ARCCLOCKWISE, ARCCOUNTERCLOCKWISE, DWELL, INCHESASUNITS, GOHOME, GOHOMEVIAINTERMEDIATEPOINT, ABSOLUTEPOSITIONING, INCREMENTALPOSITIONING, SETCURRENTASHOME, SELECTEXTRUDER, ZMOVE, SETSPEED};

class Command
{
public:
	Command(){f=e=0.0f;};
	GCodes Code;
	Vector3f where;
	float f,e;	// Feedrates f=speed, e=extrusion to preform while moving (Pythagoras)
	string comment;
};

class GCode
{
public:
    GCode();

	void Read(string filename);
	void draw(const ProcessController &PC);
	void MakeText(string &GcodeTxt, const string &GcodeStart, const string &GcodeLayer, const string &GcodeEnd, bool UseIncrementalEcode, bool Use3DGcode);


	/*--------------GCode-------------------*/
	std::vector<Command> commands;
	Vector3f Min,Max,Center;			// Boundingbox
};