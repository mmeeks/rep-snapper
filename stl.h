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
#include <list>
#include "platform.h"

#include "math.h"                                               // Needed for sqrtf

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <list>

#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>

#include <vmmlib/vmmlib.h>
#include "Polygon2f.h"

#ifdef __GNUC__
#  define _BACKWARD_BACKWARD_WARNING_H 1 // kill annoying warning
#  include <ext/hash_map>
namespace std
{
  using namespace __gnu_cxx;
}
#else
#  include <hash_map>
using namespace stdext;
#endif

/*
Vector3f position, normal;
// fill vertices
glNormal3fv( normal.xyz );
glVertex3fv( position.xyz );
*/

using namespace std;
using namespace vmml;
using namespace PolyLib;

typedef vector<Vector2f> outline;

enum AXIS {NEGX, POSX, NEGY, POSY, NEGZ, POSZ, NOT_ALIGNED};

class Triangle
{
public:
	Triangle(const Vector3f &Norml, const Vector3f &Point1, const Vector3f &Point2, const Vector3f &Point3){ Normal = Norml ; A=Point1;B=Point2;C=Point3;}
	Triangle(){};

	void SetPoints(const Vector3f &P1, const Vector3f &P2, const Vector3f &P3) { A=P1;B=P2;C=P3; }
	void SetNormal(const Vector3f &Norml) { Normal=Norml;}
	float area();

	AXIS axis;			// Used for auto-rotation
	Vector3f A,B,C,Normal;	// p1,p2,p3, Normal
};


struct InFillHit{
	Vector2f p;		// The intersection point
	float d;		// Distance from the infill-line start point, used for sorting hits
	float t;		// intersection point on first line
};

struct Segment{
	Segment(uint s, uint e){start = s; end = e;}
	int start;		// Vertex index of start point
	int end;		// Vertex index of end point
};

class Poly{
public:
	Poly(){};
	void cleanup();				// Removed vertices that are on a straight line
	void calcHole(vector<Vector2f> &offsetVertices);
	vector<uint> points;			// points, indices into ..... a CuttingPlane or a GCode object
	bool hole;
	Vector2f center;
};

struct locator{
	locator(int polygon, int vertex, float where){p=polygon; v=vertex; t=where;}
	int p;
	int v;
	float t;
};


class Segment2f;
class CuttingPlaneOptimizer;
class Point2f;

// A (set of) 2D polygon extracted from a 3D model
class CuttingPlane{
public:
	CuttingPlane();
	~CuttingPlane();
	void ShrinkFast(float distance, float optimization, bool DisplayCuttingPlane, bool useFillets, int ShellCount);		// Contracts polygons
	void ShrinkLogick(float distance, float optimization, bool DisplayCuttingPlane, bool useFillets, int ShellCount);		// Contracts polygons
	void ShrinkNice(float distance, float optimization, bool DisplayCuttingPlane, bool useFillets, int ShellCount);		// Contracts polygons
	void selfIntersectAndDivide();
	uint selfIntersectAndDivideRecursive(float z, uint startPolygon, uint startVertex, vector<outline> &outlines, const Vector2f endVertex, uint &level);

	void MakeContainedPlane(CuttingPlane& res)
	{
		res = *this;
		res.polygons = res.offsetPolygons;
		res.vertices = res.offsetVertices;
		res.advVertices = res.advVertices;
		res.offsetPolygons.clear();
		res.offsetVertices.clear();
	}
	void ClearShrink()
	{
		offsetPolygons.clear();
		offsetVertices.clear();
	}
	void recurseSelfIntersectAndDivide(float z, vector<locator> &EndPointStack, vector<outline> &outlines, vector<locator> &visited);
	void CalcInFill(vector<Vector2f> &infill, uint LayerNr, float InfillDistance, float InfillRotation, float InfillRotationPrLayer, bool DisplayDebuginFill);	// Collide a infill-line with the polygons
	void Draw(bool DrawVertexNumbers, bool DrawLineNumbers, bool DrawOutlineNumbers);
	bool LinkSegments(float z, float shrinkValue, float Optimization, bool DisplayCuttingPlane, bool ShrinkNice, int ShellCount);		// Link Segments to form polygons
	void CleanupPolygons(float Optimization);			// remove redudant points
	void CleanupOffsetPolygons(float Optimization);			// remove redudant points
	void MakeGcode(const std::vector<Vector2f> &infill, GCode &code, float &E, float z, float MinPrintSpeedXY, float MaxPrintSpeedXY, float MinPrintSpeedZ, float MaxPrintSpeedZ, float DistanceToReachFullSpeed, float extrusionFactor, bool UseIncrementalEcode, bool Use3DGcode, bool EnableAcceleration);	// Convert Cuttingplane to GCode
	bool VertexIsOutsideOriginalPolygon( Vector2f point, float z);

	Vector2f Min, Max;				// Bounding box

	void Clear() 
	{ 	
		lines.clear();
		vertices.clear();
		advVertices.clear();
		polygons.clear();
		points.clear();
		offsetPolygons.clear();
		offsetVertices.clear();
	}
	void SetZ(float value)
	{
		Z = value;
	}
	float getZ() { return Z; }

	int RegisterPoint(Vector2f &p);
	void AddLine(Segment &line);

	vector<Poly>& GetPolygons() { return polygons; }
	vector<Vector2f>& GetVertices() { return vertices; }
	vector<Point2f*>& GetAdvVertices() { return advVertices; }

private:
	uint GetHash(float x, float y);
	uint IndexOfPoint(uint hash, Vector2f &p);
	hash_map<uint, pair<Point2f*, int> > points;

	vector<CuttingPlaneOptimizer*> optimizers;

	vector<Segment> lines;			// Segments - 2 points pr. line-segment

	vector<Poly> polygons;			// Closed loops
	vector<Vector2f> vertices;		// points
	vector<Point2f*> advVertices;	// points
	float Z;

	vector<Poly> offsetPolygons;	// Shrinked closed loops
	vector<Vector2f> offsetVertices;// points for shrinked closed loops
};


#define sqr(x) ((x)*(x))

class Point2f
{
public:
	static uint GetHash(float x, float y) { return ((uint)(x*1000))+((uint)(y*1000))*1000000; }
	Point2f(Vector2f p, int idx)
	{
		Point = p;
		Index = idx;
	}
	list<Point2f*> ConnectedPoints;
	list<Segment2f*> Lines;
	Vector2f Point;
	int Index;
	bool FindNextPoint(Point2f* origin, Point2f* destination, bool expansion);
	float AngleTo(Point2f* point);
};

class Segment2f
{
public:
	Segment2f(Point2f* p1, Point2f* p2) { Point1 = p1; Point2 = p2; }
	Point2f* Point1;
	Point2f* Point2;
};

class Outline2f
{
public:
	list<Segment2f> Segments;
};


class CuttingPlaneOptimizer
{
public:
	float Z;
	CuttingPlaneOptimizer(float z) { Z = z; }
	CuttingPlaneOptimizer(CuttingPlane* cuttingPlane, float z);
	list<Polygon2f*> positivePolygons;
	void Shrink(float distance, bool useFillets, list<Polygon2f*> &resPolygons);
	void Draw();
	void Dispose();
private:
	void PushPoly(Polygon2f* poly);
};



class STL
{
public:
	STL();

	bool Read(string filename,bool force_binary = false );
	void GetObjectsFromIvcon();
	void clear(){triangles.clear();}
	void displayInfillOld(const ProcessController &PC, CuttingPlane &plane, uint LayerNr, vector<int>& altInfillLayers);
	void draw(const ProcessController &PC, float opasity = 1.0f);
	void CenterAroundXY();
	void CalcCuttingPlane(float where, CuttingPlane &plane, const Matrix4f &T);	// Extract a 2D polygonset from a 3D model
	void OptimizeRotation();									// Auto-Rotate object to have the largest area surface down for printing
	void CalcBoundingBoxAndZoom();
	void RotateObject(Vector3f axis, float angle);				// Rotation for manual rotate and used by OptimizeRotation

	vector<Triangle> triangles;
	Vector3f Min, Max, Center;
};

