#pragma once
#include <vector>
#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glaux.h>											// Header File For The GLaux Library

#include "math.h"                                               // Needed for sqrtf
#include "ArcBall.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

#include "ArcBall.h"												// NEW: ArcBall Header
#include "Cstl.h"

using namespace std;

extern STL stl;


/*
Extruder0_ExtrudeRatio(0..)=1.0

If you are using the 4D firmware in the Arduino, this decides how much extrudate actually to lay down for a given movement. Thus if you set this to 0.7 and the head moves 100mm, then 70mm of extrudate will be deposited in that move. You can use this to control how fat or thin the extrudate trail is, though the machine should be set up so that this factor is 1.0.

Extruder0_ExtrusionBroadWidth(mm)=2.0

The gap between the infill zig-zag pattern used to fill the interior of an object when coarse infill is being used. Set this negative to suppress coarse infill.

Extruder0_ExtrusionDelayForLayer(ms)=1000

For the first use of the extruder in a layer the time delay between turning on the extruder motor and starting to move the extruder to lay down material. See also ValveDelayForLayer (below).

Extruder0_ExtrusionDelayForPolygon(ms)=200

For the second and all subsequent use of the extruder in a layer the time delay between turning on the extruder motor and starting to move the extruder to lay down material. See also ValveDelayForPolygon (below).

Extruder0_ExtrusionFoundationWidth(mm)=2

The gap between the infill zig-zag pattern used to fill the interior of the foundations (if any).

Extruder0_ExtrusionHeight(mm)=0.4

The depth of each layer. This value should be the same for all extruders in the machine at any one time.

Extruder0_ExtrusionInfillWidth(mm)=0.5

The gap between the infill zig-zag pattern used to fill the interior of an object when fine infill is being used.

Extruder0_ExtrusionLastFoundationWidth(mm)=1

The gap between the infill zig-zag pattern used to fill the interior of the last layer of the foundations (if any).

Extruder0_ExtrusionOverRun(mm)=-1

The distance before the end of a sequence of infill or outline depositions to turn off the extruder motor. See also ValveOverRun (below). Set this negative to keep running to the end.

Extruder0_ExtrusionSize(mm)=0.7

The width of the filament laid down by the extruder.

Extruder0_ExtrusionSpeed(mm/minute)=3000

On the latest extruders (see here) this sets the rate at which the extrudate exits the nozzle in millimeters per minute when the extruder is running in thin air (i.e. not building, or running the delay at the start of a layer or polygon - see above).

Legacy. On earlier extruders it sets the PWM signal to send the extruder motor, as a fraction of 255. See also Extruder0_t0 (below).

In either case, if there is no motor in use, set this negative.

Extruder0_ExtrusionTemp(C)=190

The temperature to run the extruder at.

Extruder0_FastXYFeedrate(mm/minute)=3000.0

The fastest that this extruder can plot. This is the speed that the system will accelerate writing up to when accelerations are enabled. The system takes the minimum of this vale and the equivalent value for the machine overall when extruding.


Extruder0_Reverse(ms)=0

The time to reverse the extruder motor when it is turned off, drawing the extrudate back into it and reducing dribble. 

Extruder0_UpperFineLayers(0...)=2

Give this many layers at the top of the object fine infill. 


Movement Codes

G0 Rapid Motion Implemented - supports X, Y, and Z axes.
G1 Coordinated Motion Implemented - supports X, Y, and Z axes.
G2 Arc – Clockwise Not used by Skienforge
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
M121 Igain “
M122 Dgain “
M123 Imax “
M124 Imin “


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
M105 Custom code for temperature reading – not used by RapMan
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


enum GCodes{GOTO, DRAWTO, MILLIMETERSASUNITS, RAPIDMOTION, COORDINATEDMOTION, ARCCLOCKWISE, ARCCOUNTERCLOCKWISE, DWELL, INCHESASUNITS, GOHOME, GOHOMEVIAINTERMEDIATEPOINT, ABSOLUTEPOSITIONING, INCREMENTALPOSITIONING, SETCURRENTASHOME};

class Command
{
public:
	Command(){f=e=0.0f;};
	GCodes Code;
	Vector3f where;
	float f,e;	// Feedrates f=speed, e=extrusion to preform while moving
};

class GCode : public Fl_Gl_Window
{
public:
    GCode(int x,int y,int w,int h,const char *l=0);

	void Read(string filename);
	void draw();
	void CenterView();
	int  handle(int);
	void resize (int x,int y, int width, int height);									// Reshape The Window When It's Moved Or Resized

	void ReadStl(string filename) {stl.Read(filename);};
	void RotateObject(float x, float y, float z, float a) {stl.RotateObject(Vector3f(x,y,z),a);}
	void CalcBoundingBoxAndZoom(GCode* code) { stl.CalcBoundingBoxAndZoom(code);}
	void OptimizeRotation() { stl.OptimizeRotation();}

	/*--------------ArcBall-------------------*/

	GLUquadricObj *quadratic;											// Used For Our Quadric

	Matrix4fT   Transform;
	Matrix3fT   LastRot;
	Matrix3fT   ThisRot;
	ArcBallT    *ArcBall;								                // NEW: ArcBall Instance
	Vector2fT    MousePt;												// NEW: Current Mouse Point
	/*--------------View-------------------*/

	Vector3f Center;
	Vector3f Min;
	Vector3f Max;
	float zoom;
	/*--------------GCode-------------------*/
	std::vector<Command> commands;
};