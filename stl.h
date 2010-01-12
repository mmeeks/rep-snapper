/* -------------------------------------------------------- *
*
* stl.h
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
#include <windows.h>											// Header File For Windows
#include <gl\gl.h>												// Header File For The OpenGL32 Library
#include <gl\glu.h>												// Header File For The GLu32 Library
//#include <gl\glaux.h>											// Header File For The GLaux Library

#include "math.h"                                               // Needed for sqrtf

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

#include <vmmlib/vmmlib.h>

class GUI;
class GCode;
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

	AXIS axis;			// Used for auto-rotation
	Vector3f A,B,C,N;	// p1,p2,p3, Normal
};


struct InFillHit{
	Vector2f p;		// The intersection point
	float d;		// Distance from the infill-line start point, used for sorting hits
};

struct Segment{
	Segment(UINT s, UINT e){start = s; end = e;}
	int start;		// Vertex index of start point
	int end;		// Vertex index of end point
};

class Poly{
public:
	Poly(){};
	void cleanup();				// Removed vertices that are on a straight line

	vector<UINT> points;			// points, indices into ..... a CuttingPlane or a GCode object
};

// A (set of) 2D polygon extracted from a 3D model
class  CuttingPlane{
public:
	CuttingPlane();
	void Shrink(float distance, float z, bool DisplayCuttingPlane, bool useFillets);		// Contracts polygons
	void selfIntersectAndDivide(float z);
	void CalcInFill(vector<Vector2f> &infill, UINT LayerNr, float z, float InfillDistance, float InfillRotation, float InfillRotationPrLayer, bool DisplayDebuginFill);	// Collide a infill-line with the polygons
	void Draw(float z, bool DrawVertexNumbers, bool DrawLineNumbers);
	bool LinkSegments(float z, float shrinkValue, float Optimization, bool DisplayCuttingPlane);		// Link Segments to form polygons
	void CleanupPolygons(float Optimization);			// remove redudant points
	void MakeGcode(const std::vector<Vector2f> &infill, GCode &code, float &E, float z, float MinPrintSpeedXY, float MaxPrintSpeedXY, float MinPrintSpeedZ, float MaxPrintSpeedZ, UINT accelerationSteps, float distanceBetweenSpeedSteps, float extrusionFactor, bool UseAcceleration, bool UseIncrementalEcode, bool UseFirmwareAcceleration);	// Convert Cuttingplane to GCode

	Vector2f Min, Max;				// Bounding box
	vector<Segment> lines;			// Segments - 2 points pr. line-segment

	vector<Poly> polygons;			// Closed loops
	vector<Vector2f> vertices;		// points

	vector<Poly> offsetPolygons;	// Shrinked closed loops
	vector<Vector2f> offsetVertices;// points for shrinked closed loops

};

class STL
{
public:
	STL();

	bool Read(string filename, const Vector3f &PrintingMargin);
	void draw(const ProcessController &PC);
	void MoveIntoPrintingArea(const Vector3f &PrintingMargin);
	void CenterAroundXY(const Vector3f &Point);
	void CalcCuttingPlane(float where, CuttingPlane &plane);	// Extract a 2D polygonset from a 3D model
	void OptimizeRotation();									// Auto-Rotate object to have the largest area surface down for printing
	void CalcBoundingBoxAndZoom();
	void RotateObject(Vector3f axis, float angle);				// Rotation for manual rotate and used by OptimizeRotation

	vector<Triangle> triangles;
	Vector3f Min, Max, Center;
};

