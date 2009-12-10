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