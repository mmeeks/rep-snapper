#pragma once
#include <vector>
#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
#include <gl\glaux.h>											// Header File For The GLaux Library

#include "math.h"                                               // Needed for sqrtf

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

#include <vmmlib/vmmlib.h>

/*
Vector3f position, normal;
// fill vertices
glNormal3fv( normal.xyz );
glVertex3fv( position.xyz );
*/

using namespace std;
using namespace vmml;

class Triangle
{
public:
	Triangle(const Vector3f &Normal, const Vector3f &Point1, const Vector3f &Point2, const Vector3f &Point3){ N = Normal ; A=Point1;B=Point2;C=Point3;}
	Triangle(){};

	void SetPoints(const Vector3f &P1, const Vector3f &P2, const Vector3f &P3) { A=P1;B=P2;C=P3; }
	void SetNormal(const Vector3f &Normal) { N = Normal;}

	Vector3f A,B,C,N;
};


struct InFillHit{
	Vector3f p;
	float d;
};

class STL
{
public:
	STL();

	void Read(string filename);
	void draw();
	void CalcCuttingPlane(float where, vector<Vector3f> &points);
	void CalcInFill(float where, vector<Vector3f> &CuttingPlane, vector<Vector3f> &infill, UINT LayerNr);
	bool IntersectXY(const Vector3f &p1, const Vector3f &p2, const Vector3f &p3, const Vector3f &p4, InFillHit &hit);

	vector<Triangle> triangles;
	Vector3f Min, Max;
};

