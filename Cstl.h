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

enum AXIS {NEGX, POSX, NEGY, POSY, NEGZ, POSZ, NOT_ALIGNED};

class Triangle
{
public:
	Triangle(const Vector3f &Normal, const Vector3f &Point1, const Vector3f &Point2, const Vector3f &Point3){ N = Normal ; A=Point1;B=Point2;C=Point3;}
	Triangle(){};

	void SetPoints(const Vector3f &P1, const Vector3f &P2, const Vector3f &P3) { A=P1;B=P2;C=P3; }
	void SetNormal(const Vector3f &Normal) { N = Normal;}
	float area();

	AXIS axis;
	Vector3f A,B,C,N;
};


struct InFillHit{
	Vector2f p;
	float d;
};

struct Segment{
	Segment(UINT s, UINT e){start = s; end = e;}
	int start;
	int end;
};

class Poly{
public:
	Poly(){};
	void cleanup();

	vector<int> points;			// points
};

class  CuttingPlane{
public:
	CuttingPlane(){}
	void Shrink(float distance, float z);
	void CalcInFill(vector<Vector2f> &infill, UINT LayerNr, float z=0);
	bool IntersectXY(const Vector2f &p1, const Vector2f &p2, const Vector2f &p3, const Vector2f &p4, InFillHit &hit);
	void Draw(float z);
	bool LinkSegments(float z);
	void CleanupPolygons();

	Vector2f Min, Max;				// Bounding box
	vector<Vector2f> vertices;		// points
	vector<Segment> lines;			// points

	vector<Poly> polygons;		// Closed loops
	vector<Poly> offsetPolygons;		// Closed loops
	vector<Vector2f> offsetVertices;		// points
};

class GCode;

class STL
{
public:
	STL();

	void Read(string filename);
	void draw();
	void CalcCuttingPlane(float where, CuttingPlane &plane);
	void OptimizeRotation();
	void CalcBoundingBoxAndZoom(GCode *code);
	void RotateObject(Vector3f axis, float angle);

	vector<Triangle> triangles;
	Vector3f Min, Max;
};

