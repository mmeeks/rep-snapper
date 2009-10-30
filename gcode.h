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
	Command(){};
	GCodes Code;
	Vector3fT where;
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


	/*--------------ArcBall-------------------*/

	GLUquadricObj *quadratic;											// Used For Our Quadric

	Matrix4fT   Transform;
	Matrix3fT   LastRot;
	Matrix3fT   ThisRot;
	ArcBallT    *ArcBall;								                // NEW: ArcBall Instance
	Point2fT    MousePt;												// NEW: Current Mouse Point
	/*--------------View-------------------*/

	Vector3fT Center;
	Vector3fT Min;
	Vector3fT Max;
	float zoom;
	/*--------------GCode-------------------*/
	std::vector<Command> commands;
};