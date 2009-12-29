/* -------------------------------------------------------- *
*
* stl.cpp
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */


#include "stdafx.h"

#include "stl.h"
#include "gcode.h"
#include "UI.h"
#include "math.h"

#include <gl/glu.h>
#include <glut.h>

#include <iostream>
#include <fstream>
#include <algorithm>

#include <ANN/ANN.h>
/*
extern "C" {
	#include "triangle.h"
}
*/
#define MIN(A,B) ((A)<(B)? (A):(B))
#define MAX(A,B) ((A)>(B)? (A):(B))

using namespace std;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void renderBitmapString(Vector3f pos, void* font, string text)
{
	char asd[100];
	char *a = &asd[0];
	sprintf(asd,text.c_str());
	glRasterPos3f(pos.x, pos.y, pos.z);
	for (int c=0;c<text.size();c++)
		glutBitmapCharacter(font, (int)*a++);
}

// STL constructor
STL::STL()
{
	Min.x = Min.y = Min.z = 0.0f;
	Max.x = Max.y = Max.z = 200.0f;

	CalcBoundingBoxAndZoom();
}

bool STL::Read(string filename, const Vector3f &PrintingMargin)
{
	triangles.clear();
	Min.x = Min.y = Min.z = 0.0f;
	Max.x = Max.y = Max.z = 200.0f;

	unsigned int count;
	ifstream infile;
	try{
		infile.open(filename.c_str(),  ios::in | ios::binary);
		if(!infile.good())
			return false;
			
		// Ascii ot binary?
		long header;
		infile.read(reinterpret_cast < char * > (&header), sizeof(long));	// Header
		//Check if the header is "soli"
		if(header == 0x696c6f73)
		{
		infile.close();
//		ReadAsciiFile();
		return false;	// no can read ascii
		}
		
		Min.x = Min.y = Min.z = 99999999.0f;
		Max.x = Max.y = Max.z = -99999999.0f;

		infile.seekg(80, ios_base::beg);
		infile.read(reinterpret_cast < char * > (&count), sizeof(unsigned int));	// N_Triangles
		triangles.reserve(count);

		for(UINT i = 0; i < count; i++)
		{
			float a,b,c;
			infile.read(reinterpret_cast < char * > (&a), sizeof(float));
			infile.read(reinterpret_cast < char * > (&b), sizeof(float));
			infile.read(reinterpret_cast < char * > (&c), sizeof(float));
			Vector3f N(a,b,c);
			infile.read(reinterpret_cast < char * > (&a), sizeof(float));
			infile.read(reinterpret_cast < char * > (&b), sizeof(float));
			infile.read(reinterpret_cast < char * > (&c), sizeof(float));
			Vector3f Ax(a,b,c);
			infile.read(reinterpret_cast < char * > (&a), sizeof(float));
			infile.read(reinterpret_cast < char * > (&b), sizeof(float));
			infile.read(reinterpret_cast < char * > (&c), sizeof(float));
			Vector3f Bx(a,b,c);
			infile.read(reinterpret_cast < char * > (&a), sizeof(float));
			infile.read(reinterpret_cast < char * > (&b), sizeof(float));
			infile.read(reinterpret_cast < char * > (&c), sizeof(float));
			Vector3f Cx(a,b,c);
			unsigned short xxx;
			infile.read(reinterpret_cast < char * > (&xxx), sizeof(unsigned short));

			Triangle T(N,Ax,Bx,Cx);

			triangles.push_back(T);

			Min.x = MIN(Ax.x, Min.x);			Min.y = MIN(Ax.y, Min.y);			Min.z = MIN(Ax.z, Min.z);			Max.x = MAX(Ax.x, Max.x);			Max.y = MAX(Ax.y, Max.y);			Max.z = MAX(Ax.z, Max.z);			Min.x = MIN(Bx.x, Min.x);			Min.y = MIN(Bx.y, Min.y);
			Min.z = MIN(Bx.z, Min.z);			Max.x = MAX(Bx.x, Max.x);			Max.y = MAX(Bx.y, Max.y);			Max.z = MAX(Bx.z, Max.z);			Min.x = MIN(Cx.x, Min.x);			Min.y = MIN(Cx.y, Min.y);			Min.z = MIN(Cx.z, Min.z);			Max.x = MAX(Cx.x, Max.x);
			Max.y = MAX(Cx.y, Max.y);			Max.z = MAX(Cx.z, Max.z);
		}

	}
	catch (ifstream::failure e)
	{
		cout << "Exception opening/reading file";
		string error = e.what();
	}
	
	OptimizeRotation();
	CalcBoundingBoxAndZoom();
	MoveIntoPrintingArea(PrintingMargin);
	return true;
}

void STL::CalcBoundingBoxAndZoom()
{
	Center = (Max + Min )/2;
}


void STL::draw(const ProcessController &PC)
{
	// polygons
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	float no_mat[] = {0.0f, 0.0f, 0.0f, 1.0f};
	float mat_ambient[] = {0.7f, 0.7f, 0.7f, 1.0f};
	float mat_ambient_color[] = {0.8f, 0.8f, 0.2f, 1.0f};
	float mat_diffuse[] = {0.1f, 0.5f, 0.8f, 1.0f};
	float mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	float no_shininess = 0.0f;
	float low_shininess = 5.0f;
	float high_shininess = 100.0f;
	float mat_emission[] = {0.3f, 0.2f, 0.2f, 0.0f};

	/* draw sphere in first row, first column
	* diffuse reflection only; no ambient or specular
	*/
	glMaterialfv(GL_FRONT, GL_AMBIENT, no_mat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, no_mat);
	glMaterialf(GL_FRONT, GL_SHININESS, high_shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, no_mat);

	glEnable (GL_POLYGON_OFFSET_FILL);
	glPolygonOffset (0.5f, 0.5f);

	if(PC.DisplayPolygons)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glDepthMask(GL_TRUE);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  //define blending factors
		glBegin(GL_TRIANGLES);
		for(UINT i=0;i<triangles.size();i++)
		{
/*			switch(triangles[i].axis)
				{
				case NEGX:	glColor4f(1,0,0,PC.PolygonOpasity); break;
				case POSX:	glColor4f(0.5f,0,0,PC.PolygonOpasity); break;
				case NEGY:	glColor4f(0,1,0,PC.PolygonOpasity); break;
				case POSY:	glColor4f(0,0.5f,0,PC.PolygonOpasity); break;
				case NEGZ:	glColor4f(0,0,1,PC.PolygonOpasity); break;
				case POSZ:	glColor4f(0,0,0.3f,PC.PolygonOpasity); break;
				default: glColor4f(0.2f,0.2f,0.2f,PC.PolygonOpasity); break;
				}*/
			glNormal3fv((GLfloat*)&triangles[i].N);
			glVertex3fv((GLfloat*)&triangles[i].A);
			glVertex3fv((GLfloat*)&triangles[i].B);
			glVertex3fv((GLfloat*)&triangles[i].C);
		}
		glEnd();
		glDisable(GL_BLEND);
	}

	glDisable(GL_LIGHTING);

	// WireFrame
	if(PC.DisplayWireframe)
	{
		glColor4f(0,1,0,1);
		for(UINT i=0;i<triangles.size();i++)
		{
			glBegin(GL_LINE_LOOP);
			glVertex3f(triangles[i].A.x, triangles[i].A.y, triangles[i].A.z);
			glVertex3f(triangles[i].B.x, triangles[i].B.y, triangles[i].B.z);
			glVertex3f(triangles[i].C.x, triangles[i].C.y, triangles[i].C.z);
			glEnd();
		}
	}
	// normals
	if(PC.DisplayNormals)
	{
		glColor4f(0,0,1,1);
		glBegin(GL_LINES);
		for(UINT i=0;i<triangles.size();i++)
		{
			Vector3f center = (triangles[i].A+triangles[i].B+triangles[i].C)/3.0f;
			glVertex3fv((GLfloat*)&center);
			Vector3f N = center + (triangles[i].N*10);
			glVertex3fv((GLfloat*)&N);
		}
		glEnd();
	}

	// Endpoints
	if(PC.DisplayEndpoints)
	{
		glColor4f(1,0,0,1);
		glPointSize(2);
		glBegin(GL_POINTS);
		for(UINT i=0;i<triangles.size();i++)
		{
			glVertex3f(triangles[i].A.x, triangles[i].A.y, triangles[i].A.z);
			glVertex3f(triangles[i].B.x, triangles[i].B.y, triangles[i].B.z);
			glVertex3f(triangles[i].C.x, triangles[i].C.y, triangles[i].C.z);
		}
		glEnd();
	}
	glDisable(GL_DEPTH_TEST);

	// Make Layers
	UINT LayerNr = 0;

	float zSize = (Max.z-Min.z);
	float z=PC.CuttingPlaneValue*zSize+Min.z;
	float zStep = zSize;

	if(PC.DisplayAllLayers)
		{
		z=Min.z;
		zStep = PC.LayerThickness;
		}
	while(z<Max.z)
	{
		{
		CuttingPlane plane;
		CalcCuttingPlane(z, plane);	// output is a lot of unconnected line segments with individual vertices

		float hackedZ = z;
		while(plane.LinkSegments(hackedZ, PC.ExtrudedMaterialWidth*0.5f, PC.Optimization, PC.DisplayCuttingPlane) == false)	// If segment linking fails, re-calc a new layer close to this one, and use that.
			{										// This happens when there's triangles missing in the input STL
			hackedZ+= 0.1f;
			plane.polygons.clear();
			CalcCuttingPlane(hackedZ, plane);	// output is alot of un-connected line segments with individual vertices
			}

		plane.Draw(z, PC.DrawVertexNumbers, PC.DrawLineNumbers);

		// inFill
		vector<Vector2f> infill;

		if(PC.DisplayinFill)
			{
			CuttingPlane infillCuttingPlane;
			infillCuttingPlane = plane;
			infillCuttingPlane.polygons = infillCuttingPlane.offsetPolygons;
			infillCuttingPlane.vertices = infillCuttingPlane.offsetVertices;
			infillCuttingPlane.offsetPolygons.clear();
			infillCuttingPlane.offsetVertices.clear();
			if(PC.ShellOnly == false)
				{
				infillCuttingPlane.Shrink(PC.ExtrudedMaterialWidth*0.5f, PC.Optimization, PC.DisplayCuttingPlane);
				infillCuttingPlane.CalcInFill(infill, LayerNr, z, PC.InfillDistance, PC.InfillRotation, PC.InfillRotationPrLayer, PC.DisplayDebuginFill);
				}
			glColor4f(1,1,0,1);
			glPointSize(5);
			glBegin(GL_LINES);
			for(UINT i=0;i<infill.size();i+=2)
			{
				if(infill.size() > i+1)
				{
				glVertex3f(infill[i  ].x, infill[i  ].y, z);
				glVertex3f(infill[i+1].x, infill[i+1].y, z);
				}
			}
			glEnd();
			}
		
		// Make the GCode from the plane and the infill
		/*
		GCode *code = gui->code;
		code->commands.clear();
		if(code != 0)
			{
			plane.MakeGcode(infill, code, z);

			// Draw GCode

			//--------------- Drawing -----------------

			glLineWidth(4);
			glBegin(GL_LINES);
			Vector3f thisPos(0,0,0);

			float	Distance = 0.0f;
			Vector3f pos(0,0,0);
			UINT start = (UINT)(gui->GCodeDrawStartSlider->value()*(float)(code->commands.size()));
			UINT end = (UINT)(gui->GCodeDrawEndSlider->value()*(float)(code->commands.size()));
			for(UINT i=start;i<code->commands.size() && i < end ;i++)
			{
				switch(code->commands[i].Code)
				{
				case COORDINATEDMOTION:
					if(code->commands[i].f == 0 && code->commands[i].e == 0)
						glColor3f(0.55f,0.55f,0.55f);
					else
						glColor3f(1,1,0);
					Distance += (code->commands[i].where-thisPos).length();
					glVertex3fv((GLfloat*)&pos);
					glVertex3fv((GLfloat*)&code->commands[i].where);
					break;
				case RAPIDMOTION:
					glColor3f(0.75f,0.0f,0.0f);
					Distance += (code->commands[i].where-thisPos).length();
					glVertex3fv((GLfloat*)&pos);
					glVertex3fv((GLfloat*)&code->commands[i].where);
					break;
				}
				pos = code->commands[i].where;
			}
			glEnd();
			glLineWidth(1);
			// Draw GCode end
			}			
		glPointSize(1);
		*/
		LayerNr++;
		}
	z+=zStep;
	}

	// Draw bbox

	glColor3f(1,0,0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(Min.x, Min.y, Min.z);
	glVertex3f(Min.x, Max.y, Min.z);
	glVertex3f(Max.x, Max.y, Min.z);
	glVertex3f(Max.x, Min.y, Min.z);
	glEnd();
	glBegin(GL_LINE_LOOP);
	glVertex3f(Min.x, Min.y, Max.z);
	glVertex3f(Min.x, Max.y, Max.z);
	glVertex3f(Max.x, Max.y, Max.z);
	glVertex3f(Max.x, Min.y, Max.z);
	glEnd();
	glBegin(GL_LINES);
	glVertex3f(Min.x, Min.y, Min.z);
	glVertex3f(Min.x, Min.y, Max.z);
	glVertex3f(Min.x, Max.y, Min.z);
	glVertex3f(Min.x, Max.y, Max.z);
	glVertex3f(Max.x, Max.y, Min.z);
	glVertex3f(Max.x, Max.y, Max.z);
	glVertex3f(Max.x, Min.y, Min.z);
	glVertex3f(Max.x, Min.y, Max.z);
	glEnd();

}

UINT findClosestUnused(std::vector<Vector3f> lines, Vector3f point, std::vector<bool> &used)
{
	UINT closest = -1;
	float closestDist = 9999999999999;
	
	UINT count = lines.size();
	
	for(UINT i=0;i<count;i++)
	{
		if(used[i] == false)
			{
			float dist = (lines[i]-point).length();
			if(dist < closestDist)
				{
				closestDist = dist;
				closest = i;
				}
			}
	}
	
	return closest;
}

UINT findOtherEnd(UINT p)
{
	UINT a = p%2;
	if(a == 0)
		return p+1;
	return p-1;
}

CuttingPlane::CuttingPlane()
{
}

void MakeAcceleratedGCodeLine(Vector3f start, Vector3f end, UINT accelerationSteps, float distanceBetweenSpeedSteps, float extrusionFactor, GCode &code, float z, float minSpeedXY, float maxSpeedXY, float minSpeedZ, float maxSpeedZ, bool UseIncrementalEcode, float &E, bool UseFirmwareAcceleration)
{
	if(UseFirmwareAcceleration)
	{
		if(end != start) //If we are going to somewhere else
		{
			float len;
			Vector3f LastPosition = start;
			Command command;
			float accumulatedE = 0;

			Vector3f direction = end-start;
			len = direction.length();	// total distance
			direction.normalize();

			// Set start feedrage
			command.Code = COORDINATEDMOTION;
			command.where = start;
			command.e = 0.0f;		// move or extrude?
			command.f = minSpeedXY;
			code.commands.push_back(command);

			if(len < distanceBetweenSpeedSteps*2)
			{
				// First point of acceleration is the middle of the line segment
				command.Code = COORDINATEDMOTION;
				command.where = (start+end)*0.5f;
				float extrudedMaterial = (LastPosition - command.where).length()*extrusionFactor;
				if(UseIncrementalEcode)
					E+=extrudedMaterial;
				else
					E = extrudedMaterial;
				command.e = E;		// move or extrude?
				float lengthFactor = (len/2.0f)/distanceBetweenSpeedSteps;
				command.f = (maxSpeedXY-minSpeedXY)*lengthFactor+minSpeedXY;
				code.commands.push_back(command);
				LastPosition = command.where;

				// And then decelerate
				command.Code = COORDINATEDMOTION;
				command.where = end;
				if(UseIncrementalEcode)
					E+=extrudedMaterial;
				else
					E = extrudedMaterial;
				command.e = E;		// move or extrude?
				command.f = minSpeedXY;
				code.commands.push_back(command);
			}
			else
			{
				// Move to max speed
				command.Code = COORDINATEDMOTION;
				command.where = start+(direction*distanceBetweenSpeedSteps);
				float extrudedMaterial = (LastPosition - command.where).length()*extrusionFactor;
				if(UseIncrementalEcode)
					E+=extrudedMaterial;
				else
					E = extrudedMaterial;
				command.e = E;		// move or extrude?
				command.f = maxSpeedXY;
				code.commands.push_back(command);
				LastPosition = command.where;

				// Sustian speed till deacceleration starts
				command.Code = COORDINATEDMOTION;
				command.where = end-(direction*distanceBetweenSpeedSteps);
				extrudedMaterial = (LastPosition - command.where).length()*extrusionFactor;
				if(UseIncrementalEcode)
					E+=extrudedMaterial;
				else
					E = extrudedMaterial;
				command.e = E;		// move or extrude?
				command.f = maxSpeedXY;
				code.commands.push_back(command);
				LastPosition = command.where;

				// deacceleration untill end
				command.Code = COORDINATEDMOTION;
				command.where = end;
				extrudedMaterial = (LastPosition - command.where).length()*extrusionFactor;
				if(UseIncrementalEcode)
					E+=extrudedMaterial;
				else
					E = extrudedMaterial;
				command.e = E;		// move or extrude?
				command.f = minSpeedXY;
				code.commands.push_back(command);
			}
		}
	}
	else
	{

		float len;
		Vector3f LastPosition = start;
		Command command;
		float accumulatedE = 0;


		// Make a accelerated line from LastPosition to lines[thisPoint]
		if(end != start) //If we are going to somewhere else
		{
			float speed = minSpeedXY;
			float deltaSpeed = (maxSpeedXY-minSpeedXY)/accelerationSteps;	// 1000-4000 = 3000/5 = 600

			Vector3f direction = end-start;
			direction.normalize();

			Vector3f pos = start;
			UINT currrentAccelerationLevel = accelerationSteps;
			for(UINT i=0;i<accelerationSteps;i++)
			{
				pos = start+(direction*distanceBetweenSpeedSteps*(float)(i+1));	// Go somewhere else, if i==0 we goto same position as where we are
				float speed = deltaSpeed*(float)i+minSpeedXY;

				float distRemain = (end - pos).length();
				float distTraveled = (pos-start).length();

				// store thisPoint
				command.Code = COORDINATEDMOTION;
				command.where = pos;
				float extrudedMaterial = (LastPosition - command.where).length()*extrusionFactor;
				/*			if(extrudedMaterial < 1.0f)
				{
				accumulatedE += extrudedMaterial;		// Gather untill we have a real amount
				extrudedMaterial = 0.0f;				// and don't extrude anything this move (ooze takes care of it)
				if(accumulatedE > 1.0f)					// Unless we are ready to extrude again
				{
				extrudedMaterial = (float)(int)accumulatedE;	// Extrude the int value of the collected material
				accumulatedE -= extrudedMaterial;				// and remove it from the buffer
				}
				}*/
				if(UseIncrementalEcode)
					E+=extrudedMaterial;
				else
					E = extrudedMaterial;
				command.e = E;		// move or extrude?
				command.f = speed;
				code.commands.push_back(command);
				LastPosition = pos;
				if(distRemain < distTraveled)	// Start deacceleration?
				{
					currrentAccelerationLevel = i;
					break;						// break loop, start deacceleration
				}
			}
			// Deacceleration
			for(int i=currrentAccelerationLevel;i>=0;i--)	// Don't use UINT, it needs to go below zero
			{
				pos = end-(direction*distanceBetweenSpeedSteps*(float)i);
				float speed = deltaSpeed*(float)i+minSpeedXY;

				// store thisPoint
				command.Code = COORDINATEDMOTION;
				command.where = pos;
				float len = (LastPosition - command.where).length();
				float extrudedMaterial = len*extrusionFactor;
				if(UseIncrementalEcode)
					E+=extrudedMaterial;
				else
					E = extrudedMaterial;
				command.e = E;		// move or extrude?
				command.f = speed;
				code.commands.push_back(command);
				LastPosition = pos;
			}
		}// If we are going to somewhere else*/
	}// If using firmware acceleration

}

void CuttingPlane::MakeGcode(const std::vector<Vector2f> &infill, GCode &code, float z, float MinPrintSpeedXY, float MaxPrintSpeedXY, float MinPrintSpeedZ, float MaxPrintSpeedZ, UINT accelerationSteps, float distanceBetweenSpeedSteps, float extrusionFactor, bool UseAcceleration, bool UseIncrementalEcode, bool UseFirmwareAcceleration)
{
	// Make an array with all lines, then link'em

	float E = 0.0f;

	Vector3f LastPosition= Vector3f(0,0,z);

	Command command;

	// Select Extruder (Reset XY pos?)
	command.Code = RESET_XY_AXIES;
	command.where = Vector3f(0,0,LastPosition.z);
	command.e = E;					// move
	command.f = MaxPrintSpeedXY;					// Use Max XY speed
	code.commands.push_back(command);


	// Move Z axis
	command.Code = COORDINATEDMOTION;
	command.where = Vector3f(0,0,z);
	command.e = E;					// move
	command.f = MinPrintSpeedZ;		// Use Max Z speed
	code.commands.push_back(command);

	/*
	M107 ;cooler off
	G4 P20 ;delay
	G1 Z0.0 ;z move
	T0; select new extruder
	*/

	std::vector<Vector3f> lines;

	for(UINT i=0;i<infill.size();i++)
		lines.push_back(Vector3f(infill[i].x, infill[i].y, z));

	// Copy polygons
	if(offsetPolygons.size() != 0)
		{
		for(UINT p=0;p<offsetPolygons.size();p++)
			{
			for(UINT i=0;i<offsetPolygons[p].points.size();i++)
				{
				Vector2f P3 = offsetVertices[offsetPolygons[p].points[i]];
				Vector2f P4 = offsetVertices[offsetPolygons[p].points[(i+1)%offsetPolygons[p].points.size()]];
				lines.push_back(Vector3f(P3.x, P3.y, z));
				lines.push_back(Vector3f(P4.x, P4.y, z));
				}
			}
		}
	// Find closest point to last point

	std::vector<bool> used;
	used.resize(lines.size());
	for(UINT i=0;i<used.size();i++)
		used[i] = false;


	UINT thisPoint = findClosestUnused(lines, LastPosition, used);
	if(thisPoint == -1)	// No lines = no gcode
		return;
	used[thisPoint] = true;

	while(thisPoint != -1)
		{
		float len;
		// Make a accelerated line from LastPosition to lines[thisPoint]
		if(LastPosition != lines[thisPoint]) //If we are going to somewhere else
			{
			if(UseAcceleration)
				MakeAcceleratedGCodeLine(LastPosition, lines[thisPoint], accelerationSteps,distanceBetweenSpeedSteps, 0.0f, code, z, MinPrintSpeedXY, MaxPrintSpeedXY, MinPrintSpeedZ, MaxPrintSpeedZ, UseIncrementalEcode, E, UseFirmwareAcceleration);
			else
				{
				command.Code = COORDINATEDMOTION;
				command.where = lines[thisPoint];
				//len = (LastPosition - command.where).length();
				command.e = E;//len*extrusionFactor;		// move or extrude?
				command.f = MinPrintSpeedXY;
				code.commands.push_back(command);
				}
			}// If we are going to somewhere else
			
		LastPosition = lines[thisPoint];
		used[thisPoint] = true;
		// Find other end of line
		thisPoint = findOtherEnd(thisPoint);
		used[thisPoint] = true;
		// store thisPoint

		if(UseAcceleration)
			MakeAcceleratedGCodeLine(LastPosition, lines[thisPoint], accelerationSteps, distanceBetweenSpeedSteps, extrusionFactor, code, z, MinPrintSpeedXY, MaxPrintSpeedXY, MinPrintSpeedZ, MaxPrintSpeedZ, UseIncrementalEcode, E, UseFirmwareAcceleration);
		else
			{
			command.Code = COORDINATEDMOTION;
			command.where = lines[thisPoint];
			len = (LastPosition - command.where).length();
			if(UseIncrementalEcode)
				E+=len*extrusionFactor;
			else
				E = len*extrusionFactor;
			command.e = E;		// move or extrude?
			command.f = MinPrintSpeedXY;
			code.commands.push_back(command);
			}
		LastPosition = lines[thisPoint];
		thisPoint = findClosestUnused(lines, LastPosition, used);
		if(thisPoint != -1)
			used[thisPoint] = true;
		}
}


void STL::CalcCuttingPlane(float where, CuttingPlane &plane)
{
	// intersect lines with plane
	
	plane.lines.clear();
	plane.vertices.clear();
	plane.polygons.clear();
	
	plane.Min.x = Min.x;
	plane.Min.y = Min.y;
	plane.Max.x = Max.x;
	plane.Max.y = Max.y;

	UINT pointNr = 0;
	bool foundOne = false;
	for(UINT i=0;i<triangles.size();i++)
	{
		foundOne=false;
		Segment line(-1,-1);
		Vector3f P1 = triangles[i].A;
		Vector3f P2 = triangles[i].B;
		if(where <= P1.z != where <= P2.z)
		{
			float t = (where-P1.z)/(float)(P2.z-P1.z);
			Vector3f p = P1+((Vector3f)(P2-P1)*t);
			line.start = pointNr++;
			plane.vertices.push_back(Vector2f(p.x,p.y));;
			foundOne = true;
		}
		P1 = triangles[i].B;
		P2 = triangles[i].C;
		if(where <= P1.z != where <= P2.z)
		{
			float t = (where-P1.z)/(float)(P2.z-P1.z);
			Vector3f p = P1+((Vector3f)(P2-P1)*t);
			if(foundOne)
				line.end = pointNr++;
			else
				line.start = pointNr++;
			plane.vertices.push_back(Vector2f(p.x,p.y));;
			if(foundOne)
			{
				plane.lines.push_back(line);
//				continue;// next triangle
			}
			foundOne=true;
		}
		P1 = triangles[i].C;
		P2 = triangles[i].A;
		if(where <= P1.z != where <= P2.z)
		{
			float t = (where-P1.z)/(P2.z-P1.z);
			Vector3f p = P1+((Vector3f)(P2-P1)*t);
			line.end = pointNr++;
			plane.vertices.push_back(Vector2f(p.x,p.y));;
			plane.lines.push_back(line);
		}
	// Check segment normal against triangle normal. Flip segment, as needed.
	if(line.start != -1 && line.end != -1)	// if we found a intersecting triangle
		{
		Vector2f triangleNormal = Vector2f(triangles[i].N.x, triangles[i].N.y);
		Vector2f p = plane.vertices[line.start];
		Vector2f segmentNormal = (plane.vertices[line.end] - p);
		segmentNormal = Vector2f(-segmentNormal.y, segmentNormal.x);
		triangleNormal.normalise();
		segmentNormal.normalise();
/*
	if(1)
	{
		glColor3f(1,0,0);
		glBegin(GL_LINES);
		Vector2f Center = (p + plane.vertices[line.end]) /2;
		glVertex3f( Center.x, Center.y, where);
		glVertex3f( Center.x+triangleNormal.x*2,  Center.y+triangleNormal.y*2,  where);
		glColor3f(0,0,1);
		glVertex3f( Center.x, Center.y, where);
		glVertex3f( Center.x+segmentNormal.x,  Center.y+segmentNormal.y,  where);
		glEnd();
	}	*/
		if( (triangleNormal-segmentNormal).lengthSquared() > 0.2f)	// if normals does not align, flip the segment
			{
			int tmp = line.start;
			line.start = line.end;
			line.end = tmp;
			plane.lines.back() = line;

/*	if(1)
	{
		Vector2f triangleNormal = Vector2f(triangles[i].N.x, triangles[i].N.y);
		Vector2f p = plane.vertices[line.start];
		Vector2f segmentNormal = (plane.vertices[line.end] - p);
		segmentNormal = Vector2f(-segmentNormal.y, segmentNormal.x);
		triangleNormal.normalise();
		segmentNormal.normalise();

		Vector2f Center = (p + plane.vertices[line.end]) /2;
		glBegin(GL_LINES);
		glColor3f(0,1,0);
		glVertex3f( Center.x, Center.y, where);
		glVertex3f( Center.x+segmentNormal.x,  Center.y+segmentNormal.y,  where);
		glEnd();
	}	*/

			}
		}
	}

}

vector<InFillHit> HitsBuffer;


void CuttingPlane::CalcInFill(vector<Vector2f> &infill, UINT LayerNr, float z, float InfillDistance, float InfillRotation, float InfillRotationPrLayer, bool DisplayDebuginFill)
{
	int c=0;

	float step = InfillDistance;

	bool examine = false;

	float Length = sqrtf(2)*(   ((Max.x)>(Max.y)? (Max.x):(Max.y))  -  ((Min.x)<(Min.y)? (Min.x):(Min.y))  )/2.0f;	// bbox of lines to intersect the poly with

	float rot = InfillRotation/180.0f*M_PI;
	rot += (float)LayerNr*InfillRotationPrLayer/180.0f*M_PI;
	Vector2f InfillDirX(cosf(rot), sinf(rot));
	Vector2f InfillDirY(-InfillDirX.y, InfillDirX.x);
	Vector2f Center = (Max+Min)/2.0f;

	for(float x = -Length ; x < Length ; x+=step)
		{
		bool examineThis = false;

		HitsBuffer.clear();

		Vector2f P1 = (InfillDirX * Length)+(InfillDirY*x)+ Center;
		Vector2f P2 = (InfillDirX * -Length)+(InfillDirY*x) + Center;

		if(DisplayDebuginFill)
			{
			glBegin(GL_LINES);
			glColor3f(0,0.2f,0);
			glVertex3f(P1.x, P1.y, z);
			glVertex3f(P2.x, P2.y, z);
			glEnd();
			}

/*		if(DisplayDebuginFill)
			if(!examine && ((Examine-0.5f)*2 * Length <= x))
				{
				examineThis = examine = true;
				glColor3f(1,1,1);				// Draw the line
				glVertex3f(P1.x, P1.y, z);
				glVertex3f(P2.x, P2.y, z);
				}
*/
			if(offsetPolygons.size() != 0)
				{
				for(UINT p=0;p<offsetPolygons.size();p++)
					{
					for(UINT i=0;i<offsetPolygons[p].points.size();i++)
						{
						Vector2f P3 = offsetVertices[offsetPolygons[p].points[i]];
						Vector2f P4 = offsetVertices[offsetPolygons[p].points[(i+1)%offsetPolygons[p].points.size()]];

						Vector3f point;
						InFillHit hit;
						if(IntersectXY(P1,P2,P3,P4,hit))
							{
							if(examineThis)
								int a=0;
							HitsBuffer.push_back(hit);
							}
						}
					}
				}
			else
				{
				// Fallback, collide with lines rather then polygons
				for(UINT i=0;i<lines.size();i++)
					{
					Vector2f P3 = vertices[lines[i].start];
					Vector2f P4 = vertices[lines[i].end];

					Vector3f point;
					InFillHit hit;
					if(IntersectXY(P1,P2,P3,P4,hit))
						{
						if(examineThis)
							int a=0;
						HitsBuffer.push_back(hit);
						}
					}
				}
			// Sort hits
			// Sort the vector using predicate and std::sort
			std::sort(HitsBuffer.begin(), HitsBuffer.end(), InFillHitCompareFunc);

			if(examineThis)
				{
				glPointSize(4);
				glBegin(GL_POINTS);
				for(UINT i=0;i<HitsBuffer.size();i++)
					glVertex3f(HitsBuffer[0].p.x, HitsBuffer[0].p.y, z);
				glEnd();
				glPointSize(1);
				}

			// Verify hits intregrety
			// Check if hit extists in table
restart_check:
			for(UINT i=0;i<HitsBuffer.size();i++)
				{
				if(examineThis)
					int a=0;
				bool found = false;

				for(UINT j=i+1;j<HitsBuffer.size();j++)
					if( abs(HitsBuffer[i].d - HitsBuffer[j].d) < 0.0001)
						{
						found = true;
						// Delete both points, and continue
						HitsBuffer.erase(HitsBuffer.begin()+j);
						if(i != 0 && i != HitsBuffer.size()-1)	//If we are "Going IN" to solid material, and there's more points, keep one of the points
							HitsBuffer.erase(HitsBuffer.begin()+i);
						goto restart_check;
						}
					if(found)
						continue;
				}		


			// Sort hits by distance and transfer to InFill Buffer
			if(HitsBuffer.size() != 0 && HitsBuffer.size() % 2)
				continue;	// There's a uneven number of hits, skip this infill line (U'll live)
			c=0;	// Color counter
			while(HitsBuffer.size())
				{
				infill.push_back(HitsBuffer[0].p);
				if(examineThis)
					{
					switch(c)
						{
						case 0: glColor3f(1,0,0); break;
						case 1: glColor3f(0,1,0); break;
						case 2: glColor3f(0,0,1); break;
						case 3: glColor3f(1,1,0); break;
						case 4: glColor3f(0,1,1); break;
						case 5: glColor3f(1,0,1); break;
						case 6: glColor3f(1,1,1); break;
						case 7: glColor3f(1,0,0); break;
						case 8: glColor3f(0,1,0); break;
						case 9: glColor3f(0,0,1); break;
						case 10: glColor3f(1,1,0); break;
						case 11: glColor3f(0,1,1); break;
						case 12: glColor3f(1,0,1); break;
						case 13: glColor3f(1,1,1); break;
						}
					c++;
					glPointSize(10);
					glBegin(GL_POINTS);
					glVertex3f(HitsBuffer[0].p.x, HitsBuffer[0].p.y, z);
					glEnd();
					glPointSize(1);
					}
				HitsBuffer.erase(HitsBuffer.begin());
				}
		}
}


#define SMALL_NUM  0.00000001 // anything that avoids division overflow
// dot product (3D) which allows vector operations in arguments
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y)
#define perp(u,v)  ((u).x * (v).y - (u).y * (v).x)  // perp product (2D)

//===================================================================

// inSegment(): determine if a point is inside a segment
//    Input:  a point P, and a collinear segment S
//    Return: 1 = P is inside S
//            0 = P is not inside S
int inSegment( const Vector2f &P, const Vector2f &p1, const Vector2f &p2)
{
	if (p1.x != p2.x) {    // S is not vertical
		if (p1.x <= P.x && P.x <= p2.x)
			return 1;
		if (p1.x >= P.x && P.x >= p2.x)
			return 1;
	}
	else {    // S is vertical, so test y coordinate
		if (p1.y <= P.y && P.y <= p2.y)
			return 1;
		if (p1.y >= P.y && P.y >= p2.y)
			return 1;
	}
	return 0;
}
// intersect2D_2Segments(): the intersection of 2 finite 2D segments
//    Input:  two finite segments S1 and S2
//    Output: *I0 = intersect point (when it exists)
//            *I1 = endpoint of intersect segment [I0,I1] (when it exists)
//    Return: 0=disjoint (no intersect)
//            1=intersect in unique point I0
//            2=overlap in segment from I0 to I1
int intersect2D_Segments( const Vector2f &p1, const Vector2f &p2, const Vector2f &p3, const Vector2f &p4, Vector2f &I0, Vector2f &I1, float &t0, float &t1 )
{
	Vector2f    u = p2 - p1;
	Vector2f    v = p4 - p3;
	Vector2f    w = p1 - p3;
	float    D = perp(u,v);

	// test if they are parallel (includes either being a point)
	if (fabs(D) < SMALL_NUM) {          // S1 and S2 are parallel
		if (perp(u,w) != 0 || perp(v,w) != 0) {
			return 0;                   // they are NOT collinear
		}
		// they are collinear or degenerate
		// check if they are degenerate points
		float du = dot(u,u);
		float dv = dot(v,v);
		if (du==0 && dv==0) {           // both segments are points
			if (p1 != p3)         // they are distinct points
				return 0;
			I0 = p1;                // they are the same point
			return 1;
		}
		if (du==0) {                    // S1 is a single point
			if (inSegment(p1, p3, p4) == 0)  // but is not in S2
				return 0;
			I0 = p1;
			return 1;
		}
		if (dv==0) {                    // S2 a single point
			if (inSegment(p3, p1,p2) == 0)  // but is not in S1
				return 0;
			I0 = p3;
			return 1;
		}
		// they are collinear segments - get overlap (or not)
		//float t0, t1;                   // endpoints of S1 in eqn for S2
		Vector2f w2 = p2 - p3;
		if (v.x != 0) {
			t0 = w.x / v.x;
			t1 = w2.x / v.x;
		}
		else {
			t0 = w.y / v.y;
			t1 = w2.y / v.y;
		}
		if (t0 > t1) {                  // must have t0 smaller than t1
			float t=t0; t0=t1; t1=t;    // swap if not
		}
		if (t0 > 1 || t1 < 0) {
			return 0;     // NO overlap
		}
		t0 = t0<0? 0 : t0;              // clip to min 0
		t1 = t1>1? 1 : t1;              // clip to max 1
		if (t0 == t1) {                 // intersect is a point
			I0 = p3 + v*t0;
			return 1;
		}

		// they overlap in a valid subsegment
		I0 = p3 + v*t0;
		I1 = p3 + v*t1;
		return 2;
	}

	// the segments are skew and may intersect in a point
	// get the intersect parameter for S1
	float     sI = perp(v,w) / D;
	if (sI < 0 || sI > 1)               // no intersect with S1
		return 0;

	// get the intersect parameter for S2
	float     tI = perp(u,w) / D;
	if (tI < 0 || tI > 1)               // no intersect with S2
		return 0;

	I0 = p1 + u * sI;               // compute S1 intersect point
	return 1;
}


// calculates intersection and checks for parallel lines.  
// also checks that the intersection point is actually on  
// the line segment p1-p2
bool IntersectXY(const Vector2f &p1, const Vector2f &p2, const Vector2f &p3, const Vector2f &p4, InFillHit &hit)
{

	if(abs(p1.x-p3.x) < 0.01 && abs(p1.y - p3.y) < 0.01)
	{
		hit.p = p1;
		hit.d = sqrtf( (p1.x-hit.p.x) * (p1.x-hit.p.x) + (p1.y-hit.p.y) * (p1.y-hit.p.y));
		return true;
	}
	if(abs(p2.x-p3.x) < 0.01 && abs(p2.y - p3.y) < 0.01)
	{
		hit.p = p2;
		hit.d = sqrtf( (p1.x-hit.p.x) * (p1.x-hit.p.x) + (p1.y-hit.p.y) * (p1.y-hit.p.y));
		return true;
	}
	if(abs(p1.x-p4.x) < 0.01 && abs(p1.y - p4.y) < 0.01)
	{
		hit.p = p1;
		hit.d = sqrtf( (p1.x-hit.p.x) * (p1.x-hit.p.x) + (p1.y-hit.p.y) * (p1.y-hit.p.y));
		return true;
	}
	if(abs(p2.x-p4.x) < 0.01 && abs(p2.y - p4.y) < 0.01)
	{
		hit.p = p2;
		hit.d = sqrtf( (p1.x-hit.p.x) * (p1.x-hit.p.x) + (p1.y-hit.p.y) * (p1.y-hit.p.y));
		return true;
	}

	InFillHit hit2;
	float t0,t1;
	if(intersect2D_Segments(p1,p2,p3,p4,hit.p, hit2.p, t0,t1) == 1)
	{
	hit.d = sqrtf( (p1.x-hit.p.x) * (p1.x-hit.p.x) + (p1.y-hit.p.y) * (p1.y-hit.p.y));
	return true;
	}
	return false;
/*

  float xD1,yD1,xD2,yD2,xD3,yD3;  
  float dot,deg,len1,len2;  
  float segmentLen1,segmentLen2;  
  float ua,ub,div;  
  
  // calculate differences  
  xD1=p2.x-p1.x;  
  xD2=p4.x-p3.x;  
  yD1=p2.y-p1.y;  
  yD2=p4.y-p3.y;  
  xD3=p1.x-p3.x;  
  yD3=p1.y-p3.y;    
  
  // calculate the lengths of the two lines  
  len1=sqrt(xD1*xD1+yD1*yD1);  
  len2=sqrt(xD2*xD2+yD2*yD2);  
  
  // calculate angle between the two lines.  
  dot=(xD1*xD2+yD1*yD2); // dot product  
  deg=dot/(len1*len2);  
  
  // if abs(angle)==1 then the lines are parallell,  
  // so no intersection is possible  
  if(abs(deg)==1)
	  return false;
  
  // find intersection Pt between two lines  
  hit.p=Vector2f (0,0);
  div=yD2*xD1-xD2*yD1;  
  ua=(xD2*yD3-yD2*xD3)/div;  
  ub=(xD1*yD3-yD1*xD3)/div;  
  hit.p.x=p1.x+ua*xD1;  
  hit.p.y=p1.y+ua*yD1;  
  
  // calculate the combined length of the two segments  
  // between Pt-p1 and Pt-p2  
  xD1=hit.p.x-p1.x;  
  xD2=hit.p.x-p2.x;  
  yD1=hit.p.y-p1.y;  
  yD2=hit.p.y-p2.y;  
  segmentLen1=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);  
  
  // calculate the combined length of the two segments  
  // between Pt-p3 and Pt-p4  
  xD1=hit.p.x-p3.x;  
  xD2=hit.p.x-p4.x;  
  yD1=hit.p.y-p3.y;  
  yD2=hit.p.y-p4.y;  
  segmentLen2=sqrt(xD1*xD1+yD1*yD1)+sqrt(xD2*xD2+yD2*yD2);  
  
  // if the lengths of both sets of segments are the same as  
  // the lenghts of the two lines the point is actually  
  // on the line segment.  
  
  // if the point isn't on the line, return null  
  if(abs(len1-segmentLen1)>0.00 || abs(len2-segmentLen2)>0.00)  
    return false;

  hit.d = segmentLen1-segmentLen2;
  return true;*/
}
/*
int PntOnLine(Vector2f p1, Vector2f p2, Vector2f t)
{
/*
 * given a line through P:(px,py) Q:(qx,qy) and T:(tx,ty)
 * return 0 if T is not on the line through      <--P--Q-->
 *        1 if T is on the open ray ending at P: <--P
 *        2 if T is on the closed interior along:   P--Q
 *        3 if T is on the open ray beginning at Q:    Q-->
 *
 * Example: consider the line P = (3,2), Q = (17,7). A plot
 * of the test points T(x,y) (with 0 mapped onto '.') yields:
 *
 *     8| . . . . . . . . . . . . . . . . . 3 3
 *  Y  7| . . . . . . . . . . . . . . 2 2 Q 3 3    Q = 3
 *     6| . . . . . . . . . . . 2 2 2 2 2 . . .
 *  a  5| . . . . . . . . 2 2 2 2 2 2 . . . . .
 *  x  4| . . . . . 2 2 2 2 2 2 . . . . . . . .
 *  i  3| . . . 2 2 2 2 2 . . . . . . . . . . .
 *  s  2| 1 1 P 2 2 . . . . . . . . . . . . . .    P = 1
 *     1| 1 1 . . . . . . . . . . . . . . . . .
 *      +--------------------------------------
 *        1 2 3 4 5 X-axis 10        15      19
 *
 * Point-Line distance is normalized with the Infinity Norm
 * avoiding square-root code and tightening the test vs the
 * Manhattan Norm. All math is done on the field of integers.
 * The latter replaces the initial ">= MAX(...)" test with
 * "> (ABS(qx-px) + ABS(qy-py))" loosening both inequality
 * and norm, yielding a broader target line for selection.
 * The tightest test is employed here for best discrimination
 * in merging collinear (to grid coordinates) vertex chains
 * into a larger, spanning vectors within the Lemming editor.
 

    if ( abs((p2.y-p1.y)*(t.x-p1.x)-(t.y-p1.y)*(p2.x-p1.x)) >= (MAX(abs(p2.x-p1.x), abs(p2.y-p1.y)))) return(0);
    if (((p2.x<=p1.x)&&(p1.x<=t.x)) || ((p2.y<=p1.y)&&(p1.y<=t.y))) return(1);
    if (((t.x<=p1.x)&&(p1.x<=p2.x)) || ((t.y<=p1.y)&&(p1.y<=p2.y))) return(1);
    if (((p1.x<=p2.x)&&(p2.x<=t.x)) || ((p1.y<=p2.y)&&(p2.y<=t.y))) return(3);
    if (((t.x<=p2.x)&&(p2.x<=p1.x)) || ((t.y<=p2.y)&&(p2.y<=p1.y))) return(3);
    return(2);
}
*/
float dist ( float x1, float y1, float x2, float y2)
{
return sqrt( ((x1 - x2) * (x1 - x2)) + ((y1 - y2) * (y1 - y2)) ) ;
}

int PntOnLine(Vector2f p1, Vector2f p2, Vector2f t, float &where)
{

	float A = t.x - p1.x;
	float B = t.y - p1.y;
	float C = p2.x - p1.x;
	float D = p2.y - p1.y;

	where = abs(A * D - C * B) / sqrt(C * C + D * D);

	if(where > 0.01)
		return 0;

	float dot = A * C + B * D;
	float len_sq = C * C + D * D;
	where = dot / len_sq;

/*	float xx,yy;
	xx = p1.x + where * C;
	yy = p1.y + where * D;

	glPointSize(8);
	glBegin(GL_POINTS);
	glColor3f(1,0,1);
	glVertex2f(xx, yy);
	glEnd();
*/
	if(where <= 0.0f)	// before p1
		{
//		where = param;
		return 1;
/*		xx = p1.x;
		yy = p1.y;
		where = dist(t.x, t.y, xx, yy);//your distance function
		return 1;*/
		}
	else if(where >= 1.0f) // after p2
		{
//		where = param;
		return 3;
/*		xx = p2.x;
		yy = p2.y;
		where = dist(t.x, t.y, xx, yy);//your distance function
		return 3;*/
		}
	else				// between p1 and p2
		{
//		where = param;
		return 2;			// fast exit, don't need where for this case
/*		xx = p1.x + param * C;
		yy = p1.y + param * D;
		where = dist(t.x, t.y, xx, yy);//your distance function
		return 2;*/
		}
}

class OverlapLine{
public:
	OverlapLine(Vector2f start, Vector2f end){s=start;e=end;};
	bool overlaps(Vector2f p1, Vector2f p2)
	{
	int res[2];
	float t1,t2;
	
	if(p1 == s || p2==s)
		return 1;
	if(p1 == e || p2==e)
		return 3;
	
	res[0] = PntOnLine(s,e,p1, t1);	// Is p1 on my line?
	if(res[0] == 0)
		return false;
	res[1] = PntOnLine(s,e,p2, t2);	// Is p2 on my line?
	if(res[1] == 0)
		return false;

	glPointSize(2);
	glBegin(GL_POINTS);
	glColor3f(1,0,0);
	glVertex2f(s.x, s.y);
	glColor3f(0,1,0);
	glVertex2f(e.x, e.y);
	glEnd();


	if(res[0] != res[1])	// expanding both ends
		{
		Vector2f i1 = s+(e-s)*t1;
		Vector2f i2 = s+(e-s)*t2;

		if(t1 < 0 && t1 < t2)	// Move p1
			s = p1;
		else if(t2 < 0)	// Move p1
			s = p2;
		if(t1 > 1 && t1 > t2)
			e = p1;
		else if(t2 > 1)
//			e = p2;

			glPointSize(5);
			glBegin(GL_POINTS);
			glColor3f(1,0,0);
			glVertex2f(s.x, s.y);
			glColor3f(0,1,0);
			glVertex2f(e.x, e.y);
			glEnd();
		
			return true;
		}

			glPointSize(1);
			glBegin(GL_POINTS);
			glColor3f(0.5,0.5,0.5);
			glVertex2f(s.x, s.y);
			glColor3f(0.5,0.5,0.5);
			glVertex2f(e.x, e.y);
			glEnd();
	return false;
	}
	Vector2f s,e;
};


bool CuttingPlane::LinkSegments(float z, float ShrinkValue, float Optimization, bool DisplayCuttingPlane)
{
	if(vertices.size() == 0)
		return true;

	// Find doublepoints (they all should be)
	vector<int> myDouble;
	myDouble.resize(vertices.size());
	for(int i=0;i<myDouble.size();i++)
		myDouble[i] = -1;

	int nearestPoint = 0;
	for(int i=0;i<vertices.size();i++)
		{
		float nearestDist = 9999999;
		for(int j=0;j<vertices.size();j++)
			{
			if(i==j)
				continue;
			float dist = (vertices[i]-vertices[j]).length();
			if(dist < nearestDist)
				{
				nearestDist = dist;
				nearestPoint = j;
				}
			}
		// nearestPoint = the nearestPoint that's not me
		myDouble[i] = nearestPoint;
		}

	vector<int> single_vertices;
	for(int i=0;i<vertices.size();i++)
		{
		// Check doubleness
		if(myDouble[myDouble[i]] != i)
			single_vertices.push_back(i);
		}

	if(single_vertices.size() != 0)	// Some doubleVertices don't match. Try to link unconnected vertices
		{
		glColor3f(0.0f, 1.0f, 0.0f);
		glEnable(GL_POINT_SMOOTH);
		glPointSize(15);
		glBegin(GL_POINTS);
		for(int i=0;i<single_vertices.size();i++)
			glVertex3f(vertices[single_vertices[i]].x, vertices[single_vertices[i]].y, z);
		glEnd();
/*		if(single_vertices.size() == 2)	// Only 2 unconnected, link'em
			{
			Segment s(single_vertices[0], single_vertices[1]);
			lines.push_back(s);
			}
		else return;*/
		}
		

	// Reassign double vertices to their copy
	for(int i=0;i<vertices.size();i++)
		{
		if(myDouble[i] > 0 && myDouble[myDouble[i]] > 0)
			myDouble[myDouble[i]] = 0-i;
		}

	// Adjust lines
	for(int i=0;i<lines.size();i++)
		{
		if(myDouble[lines[i].end] <= 0)
			lines[i].end = 0-myDouble[lines[i].end];
		if(myDouble[lines[i].start] <= 0)
			lines[i].start = 0-myDouble[lines[i].start];
		}

	// Make new vertex array
	std::vector<Vector2f> newVertices;
	std::vector<int> oldVertexNumbers;
	std::vector<int> oldVertexNumbersToNew;

	oldVertexNumbersToNew.resize(vertices.size());

	for(int i=0;i<vertices.size();i++)
		{
		if(myDouble[i] >= 0)
			{
			newVertices.push_back(vertices[i]);
			oldVertexNumbersToNew[i] = oldVertexNumbers.size();
			oldVertexNumbers.push_back(i);
			}
		}

	// adjust lines to use new vertices
	for(int i=0;i<lines.size();i++)
		{
		lines[i].start = oldVertexNumbersToNew[lines[i].start];
		lines[i].end = oldVertexNumbersToNew[lines[i].end];
		}

	vertices = newVertices;

	// Build polygons
	vector<bool> used;
	used.resize(lines.size());
	for(int i=0;i>used.size();i++)
		used[i]= false;

	bool error = false;
	int startLine = 0;//Examine*(float)(lines.size()-1);
	used[startLine]=true;
	while(startLine != -1)
		{
		int startPoint = lines[startLine].start;
		int endPoint = lines[startLine].end;

		Poly poly;
		poly.points.push_back(endPoint);
		int count = lines.size()+100;
		while(endPoint != startPoint && count != 0)	// While not closed
			{
			// Find a lines that starts with my endPoint point
			for(int i=0;i<lines.size();i++)
				{
				if(i==startLine)
					continue;				// avoid infinite loop
				if(lines[i].start == endPoint)	// store point
					{
					startLine = i;
					endPoint = lines[startLine].end;
					break;				// done
					}
				else if(lines[i].end == endPoint)	// This should never happen
					{
					startLine = i;
					endPoint = lines[startLine].start;
					break;				// done
					}
				if(endPoint==startPoint)
					break;				// done
				}
			used[startLine]=true;
			poly.points.push_back(endPoint);
			count--;
			}

		// Check if loop is complete
		if(count != 0)
			polygons.push_back(poly);		// This is good
		else
			{
			return false;
			assert(-1);
			error = true;
			// Should return here or try and fix problem
			//Solution: Call myself recursive, with a differetn Z
			}

		startLine = -1;
		// find next unused line
		for(int l=0;l < lines.size(); l++)
			{
			if(used[l] == false)
				{
				startLine = l;
				used[startLine]=true;
				break;	// process this line loop
				}
			}
		}	// while startLine != -1

	// Cleanup polygons
	CleanupPolygons(Optimization);
	Shrink(ShrinkValue, z, DisplayCuttingPlane);
	// Draw resulting poly
	glColor3f(1,1,0);
	for(int p=0; p<polygons.size();p++)
		{
		glBegin(GL_LINE_LOOP);
		for(int v=0; v<polygons[p].points.size();v++)
			glVertex3f(vertices[polygons[p].points[v]].x, vertices[polygons[p].points[v]].y, z);
		glEnd();
		glColor3f(1,0,1);
		glEnable(GL_POINT_SMOOTH);
		glPointSize(10);
		glBegin(GL_POINTS);
		for(int v=0; v<polygons[p].points.size();v++)
			glVertex3f(vertices[polygons[p].points[v]].x, vertices[polygons[p].points[v]].y, z);
		glEnd();
		}
	return true;
}

/*

	// Orientate polygons, based on a triangulation
  struct triangulateio in;
  struct triangulateio mid;
  struct triangulateio out;
  struct triangulateio vorout;
/* 
  Define the input points. 
*
		in.pointlist = 0;
		in.pointmarkerlist = 0;
		in.numberofpointattributes = 0;
		in.pointattributelist = 0;
		//
		in.numberofholes = 0;
		in.numberofregions = 0;
		in.regionlist = 0;
		
		for(unsigned int p=0; p<polygons.size();p++)
			{
			UINT count = polygons[p].points.size();
			in.numberofpoints = count;
			in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
			in.numberofsegments = count;
			in.numberofpointattributes = 0;
			in.segmentlist = (int *) malloc(in.numberofpoints * 2 * sizeof(int));
			UINT v=0;
			for(unsigned int i=0; i<count;i++)
				{
				in.segmentlist[v] = i;
				in.pointlist[v++] = vertices[polygons[p].points[i]].x;
				in.segmentlist[v] = i;
				in.pointlist[v++] = vertices[polygons[p].points[i]].y;
				}
/*
  in.numberofpointattributes = 1;
  in.pointlist = (REAL *) malloc(in.numberofpoints * 2 * sizeof(REAL));
  in.pointlist[0] = 0.0;
  in.pointlist[1] = 0.0;
  in.pointlist[2] = 1.0;
  in.pointlist[3] = 0.0;
	in.pointlist[4] = 1.0;
  in.pointlist[5] = 10.0;
  in.pointlist[6] = 0.0;
  in.pointlist[7] = 10.0;
  in.pointattributelist = (REAL *) malloc(in.numberofpoints *
                                          in.numberofpointattributes *
                                          sizeof(REAL));
  in.pointattributelist[0] = 0.0;
  in.pointattributelist[1] = 1.0;
  in.pointattributelist[2] = 11.0;
  in.pointattributelist[3] = 10.0;
  in.pointmarkerlist = (int *) malloc(in.numberofpoints * sizeof(int));
  in.pointmarkerlist[0] = 0;
  in.pointmarkerlist[1] = 2;
  in.pointmarkerlist[2] = 0;
  in.pointmarkerlist[3] = 0;
  in.numberofsegments = 0;
  in.numberofholes = 0;
  in.numberofregions = 1;
  in.regionlist = (REAL *) malloc(in.numberofregions * 4 * sizeof(REAL));
  in.regionlist[0] = 0.5;
  in.regionlist[1] = 5.0;
*/
/* 
  Regional attribute (for whole mesh). 
*
	in.regionlist = 0;
	in.segmentmarkerlist = 0;
//  in.regionlist[2] = 7.0;
/* 
  Area constraint that will not be used. 
*
//  in.regionlist[3] = 0.1;          

  printf("Input point set:\n\n");

//  report ( &in, 1, 0, 0, 0, 0, 0 );
/* 
  Make necessary initializations so that Triangle can return a 
  triangulation in `mid' and a Voronoi diagram in `vorout'.  
*
  mid.pointlist = (REAL *) NULL;            
  mid.pointattributelist = (REAL *) NULL;
  mid.pointmarkerlist = (int *) NULL; 
  mid.trianglelist = (int *) NULL;
  mid.triangleattributelist = (REAL *) NULL;
  mid.neighborlist = (int *) NULL;
  mid.segmentlist = (int *) NULL;
  mid.segmentmarkerlist = (int *) NULL;
  mid.edgelist = (int *) NULL;
  mid.edgemarkerlist = (int *) NULL;

  vorout.pointlist = (REAL *) NULL;
  vorout.pointattributelist = (REAL *) NULL;
  vorout.edgelist = (int *) NULL;
  vorout.normlist = (REAL *) NULL;
/* 
  Triangulate the points.  Switches are chosen to 
    read and write a PSLG (p), 
    preserve the convex hull (c), 
    number everything from zero (z), 
    assign a regional attribute to each element (A),
    produce an edge list (e), 
    produce a Voronoi diagram (v),
    produce a triangle neighbor list (n).
*
//  triangulate ( "pczAevn", &in, &mid, &vorout );
  triangulate ( "pczAevn", &in, &mid, &vorout );//pAzYYe
  
  glBegin(GL_LINES);
  for(int i=0;i<mid.numberofedges;i++)
	  {
	  if(mid.edgemarkerlist[i] == 0)	// Shared edge
		  glColor3f(1,0,0);
	  else if(mid.edgemarkerlist[i] == 1)
		  glColor3f(1.0f,0.5f,0.0f);
		else
		  glColor3f(1.0f,1.5f,1.0f);
	  Vector3f p1 = Vector3f(mid.pointlist[mid.edgelist[i*2]*2], mid.pointlist[mid.edgelist[i*2]*2+1], z-2);
	  Vector3f p2 = Vector3f(mid.pointlist[mid.edgelist[i*2+1]*2], mid.pointlist[mid.edgelist[i*2+1]*2+1], z-2);
	  glVertex3f(p1.x, p1.y, p1.z);
	  glVertex3f(p2.x, p2.y, p2.z);
	  }
	glEnd();  

  for(int i=0;i<mid.numberoftriangles;i++)
	  {
	  glBegin(GL_LINE_LOOP);
	  if(mid.triangleattributelist[i] == 0)	// Shared edge
		  glColor3f(0,1,0);
	  else
		  glColor3f(0.0f,0.5f,1.0f);
	  Vector3f p1 = Vector3f(mid.pointlist[mid.trianglelist[i*3]*2], mid.pointlist[mid.trianglelist[i*3]*2+1], z-4);
	  Vector3f p2 = Vector3f(mid.pointlist[mid.trianglelist[i*3+1]*2], mid.pointlist[mid.trianglelist[i*3+1]*2+1], z-4);
	  Vector3f p3 = Vector3f(mid.pointlist[mid.trianglelist[i*3+2]*2], mid.pointlist[mid.trianglelist[i*3+2]*2+1], z-4);
	  glVertex3f(p1.x, p1.y, p1.z);
	  glVertex3f(p2.x, p2.y, p2.z);
	  glVertex3f(p3.x, p3.y, p3.z);
		glEnd();  
	  }

  
  }
  
/*
  printf("Initial triangulation:\n\n");

//  report ( &mid, 1, 1, 1, 1, 1, 0 );

  printf("Initial Voronoi diagram:\n\n");

//  report ( &vorout, 0, 0, 0, 0, 1, 1 );
 
//  Attach area constraints to the triangles in preparation for
//  refining the triangulation.

  mid.trianglearealist = (REAL *) malloc(mid.numberoftriangles * sizeof(REAL) );
  mid.trianglearealist[0] = 3.0;
  mid.trianglearealist[1] = 1.0;

//  Make necessary initializations so that Triangle can return a
//  triangulation in `out'.                                    

  out.pointlist = (REAL *) NULL;
  out.pointattributelist = (REAL *) NULL;
  out.trianglelist = (int *) NULL;          
  out.triangleattributelist = (REAL *) NULL;
 
//  Refine the triangulation according to the attached 
//  triangle area constraints.                       

  triangulate ( "prazBP", &mid, &out, (struct triangulateio *) NULL );
*/
/* 
  Free all allocated arrays, including those allocated by Triangle. 
*/
/*
  free(in.pointlist);
  free(in.pointattributelist);
  free(in.pointmarkerlist);
  free(in.regionlist);

  free(mid.pointlist);
  free(mid.pointattributelist);
  free(mid.pointmarkerlist);
  free(mid.trianglelist);
  free(mid.triangleattributelist);
  free(mid.trianglearealist);
  free(mid.neighborlist);
  free(mid.segmentlist);
  free(mid.segmentmarkerlist);
  free(mid.edgelist);
  free(mid.edgemarkerlist);

  free(vorout.pointlist);
  free(vorout.pointattributelist);
  free(vorout.edgelist);
  free(vorout.normlist);

  free(out.pointlist);
  free(out.pointattributelist);
  free(out.trianglelist);
  free(out.triangleattributelist);

*/

void CuttingPlane::Shrink(float distance, float z, bool DisplayCuttingPlane)
{
	glColor4f(1,1,1,1);
	for(int p=0; p<polygons.size();p++)
		{
		Poly offsetPoly;
		if(DisplayCuttingPlane)
			glBegin(GL_LINE_LOOP);
		UINT count = polygons[p].points.size();
		for(int i=0; i<count;i++)
			{
			Vector2f Na = Vector2f(vertices[polygons[p].points[(i-1+count)%count]].x, vertices[polygons[p].points[(i-1+count)%count]].y);
			Vector2f Nb = Vector2f(vertices[polygons[p].points[i]].x, vertices[polygons[p].points[i]].y);
			Vector2f Nc = Vector2f(vertices[polygons[p].points[(i+1)%count]].x, vertices[polygons[p].points[(i+1)%count]].y);
			
			Vector2f V1 = (Nb-Na);
			Vector2f V2 = (Nc-Nb);
			
			Vector2f N1 = Vector2f(-V1.y, V1.x);
			Vector2f N2 = Vector2f(-V2.y, V2.x);
			
			N1.normalise();
			N2.normalise();

			Vector2f Normal = N1+N2;
			Normal.normalise();
			
			int vertexNr = polygons[p].points[i];
			
			Vector2f p = vertices[vertexNr] - (Normal * distance);
			offsetPoly.points.push_back(offsetVertices.size());
			offsetVertices.push_back(p);
			if(DisplayCuttingPlane)
				glVertex3f(p.x,p.y,z);
			}
		if(DisplayCuttingPlane)
			glEnd();
		offsetPolygons.push_back(offsetPoly);
		}
}


void CuttingPlane::Draw(float z, bool DrawVertexNumbers, bool DrawLineNumbers)
{
//	if(DisplayCuttingPlane)
		{
		glColor4f(1,0,0,1);
		glBegin(GL_LINES);
		for(UINT i=0;i<lines.size();i++)
			{
/*				if(DisplayDebug && (int)(Examine*((float)lines.size()-1)) == i)
				{
				glEnd();
				glColor4f(1,1,0,1);
				glLineWidth(3);
				glBegin(GL_LINES);
				glVertex3f(vertices[lines[i].start].x, vertices[lines[i].start].y, z);
				glVertex3f(vertices[lines[i].end].x, vertices[lines[i].end].y, z);
				glEnd();
				glColor4f(0.5f,0.5f,0.5f,1);
				glLineWidth(1);
				glBegin(GL_LINES);
				glColor4f(1,0,0,1);
				}*/

			glVertex3f(vertices[lines[i].start].x, vertices[lines[i].start].y, z);
			glVertex3f(vertices[lines[i].end].x, vertices[lines[i].end].y, z);
			}
		glEnd();


	// Endpoints
		glColor4f(1,1,0,1);
		glPointSize(2);
		glBegin(GL_POINTS);
		for(UINT i=0;i<lines.size();i++)
		{
				glVertex3f(vertices[lines[i].start].x, vertices[lines[i].start].y, z);
				glVertex3f(vertices[lines[i].end].x, vertices[lines[i].end].y, z);
		}
		glEnd();
	}


	// Vertex numbers
	if(DrawVertexNumbers)
		for(int v=0;v<vertices.size();v++)
		{
			ostringstream oss;
			oss << v;
			renderBitmapString(Vector3f (vertices[v].x, vertices[v].y, z) , GLUT_BITMAP_8_BY_13 , oss.str());
		}
	if(DrawLineNumbers)
		for(int l=0;l<lines.size();l++)
		{
			ostringstream oss;
			oss << l;
			Vector2f Center = (vertices[lines[l].start]+vertices[lines[l].end]) *0.5f;
			renderBitmapString(Vector3f (Center.x, Center.y, z) , GLUT_BITMAP_8_BY_13 , oss.str());
		}
}

void STL::OptimizeRotation()
{
	// Find the axis that has the largest surface
	// Rotate to point this face towards -Z

	// if dist center <|> 0.1 && Normal points towards, add area

	Vector3f AXIS_VECTORS[3];
	AXIS_VECTORS[0] = Vector3f(1,0,0);
	AXIS_VECTORS[1] = Vector3f(0,1,0);
	AXIS_VECTORS[2] = Vector3f(0,0,1);

	float area[6];
	for(UINT i=0;i<6;i++)
		area[i] = 0.0f;

	for(UINT i=0;i<triangles.size();i++)
		{
		triangles[i].axis = NOT_ALIGNED;				
		for(UINT triangleAxis=0;triangleAxis<3;triangleAxis++)
			{
			if (  triangles[i].N.cross(AXIS_VECTORS[triangleAxis]).length() < 0.1)
				{
				int positive=0;
				if(triangles[i].N[triangleAxis] > 0)// positive
					positive=1;
				AXIS axisNr = (AXIS)(triangleAxis*2+positive);
				triangles[i].axis = axisNr;
				if( ! (abs(Min[triangleAxis]-triangles[i].A[triangleAxis]) < 1.1 || abs(Max[triangleAxis]-triangles[i].A[triangleAxis]) < 1.1) )	// not close to boundingbox edges?
					{
					triangles[i].axis = NOT_ALIGNED;	// Not close to bounding box
					break;
					}
				area[axisNr] += triangles[i].area();
				break;
				}
			}
		}


	AXIS down = NOT_ALIGNED;
	float LargestArea = 0;
	for(UINT i=0;i<6;i++)
	{
	if(area[i] > LargestArea)
		{
		LargestArea = area[i];
		down = (AXIS)i;
		}
	}

	switch(down)
	{
	case NEGX: RotateObject(Vector3f(0,-1,0), M_PI/2.0f); break;
	case POSX: RotateObject(Vector3f(0,1,0), M_PI/2.0f); break;
	case NEGY: RotateObject(Vector3f(1,0,0), M_PI/2.0f); break;
	case POSY: RotateObject(Vector3f(-1,0,0), M_PI/2.0f); break;
	case POSZ: RotateObject(Vector3f(1,0,0), M_PI); break;
	}

}

void STL::RotateObject(Vector3f axis, float angle)
{
	Vector3f min,max;

	min.x = min.y = min.z = 99999999.0f;
	max.x = max.y = max.z = -99999999.0f;

	for(UINT i=0; i<triangles.size() ; i++)
	{
	triangles[i].N = triangles[i].N.rotate(angle, axis.x, axis.y, axis.z);
	triangles[i].A = triangles[i].A.rotate(angle, axis.x, axis.y, axis.z);
	triangles[i].B = triangles[i].B.rotate(angle, axis.x, axis.y, axis.z);
	triangles[i].C = triangles[i].C.rotate(angle, axis.x, axis.y, axis.z);
	min.x = MIN(min.x, triangles[i].A.x);
	min.y = MIN(min.y, triangles[i].A.y);
	min.z = MIN(min.z, triangles[i].A.z);
	max.x = MAX(max.x, triangles[i].A.x);
	max.y = MAX(max.y, triangles[i].A.y);
	max.z = MAX(max.z, triangles[i].A.z);
	min.x = MIN(min.x, triangles[i].B.x);
	min.y = MIN(min.y, triangles[i].B.y);
	min.z = MIN(min.z, triangles[i].B.z);
	max.x = MAX(max.x, triangles[i].B.x);
	max.y = MAX(max.y, triangles[i].B.y);
	max.z = MAX(max.z, triangles[i].B.z);
	min.x = MIN(min.x, triangles[i].C.x);
	min.y = MIN(min.y, triangles[i].C.y);
	min.z = MIN(min.z, triangles[i].C.z);
	max.x = MAX(max.x, triangles[i].C.x);
	max.y = MAX(max.y, triangles[i].C.y);
	max.z = MAX(max.z, triangles[i].C.z);
	}
	Min = min;
	Max = max;
}

float Triangle::area()
{
	return ( ((C-A).cross(B-A)).length() );
}

void CuttingPlane::CleanupPolygons(float Optimization)
{
	float allowedError = Optimization;
	for(int p=0;p<polygons.size();p++)
	{
	for(int v=0;v<polygons[p].points.size();)
		{
			Vector2f p1 =vertices[polygons[p].points[(v-1+polygons[p].points.size())%polygons[p].points.size()]];
			Vector2f p2 =vertices[polygons[p].points[v]];
			Vector2f p3 =vertices[polygons[p].points[(v+1)%polygons[p].points.size()]];

			Vector2f v1 = (p2-p1);
			Vector2f v2 = (p3-p2);

			v1.normalize();
			v2.normalize();

			if((v1-v2).lengthSquared() < allowedError)
				{
				polygons[p].points.erase(polygons[p].points.begin()+v);
				}
			else
				v++;
		}
	}
}

void STL::MoveIntoPrintingArea(const Vector3f &PrintingMargin)
{

	for(UINT i=0; i<triangles.size() ; i++)
	{
		triangles[i].A = triangles[i].A - Min + PrintingMargin;
		triangles[i].B = triangles[i].B - Min + PrintingMargin;
		triangles[i].C = triangles[i].C - Min + PrintingMargin;
	}

	Max = Max - Min + PrintingMargin;
	Min = PrintingMargin;
	CalcBoundingBoxAndZoom();
}

void STL::CenterAroundXY(const Vector3f &Point)
{
	Vector3f MyCenter((Max+Min)*0.5f);

	Vector3f displacement = Point - MyCenter;
	displacement.z = 0.0f;

	for(UINT i=0; i<triangles.size() ; i++)
	{
		triangles[i].A = triangles[i].A + displacement;
		triangles[i].B = triangles[i].B + displacement;
		triangles[i].C = triangles[i].C + displacement;
	}

	Max += displacement;
	Min += displacement;
	CalcBoundingBoxAndZoom();
}