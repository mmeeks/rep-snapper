/* -------------------------------------------------------- *
*
* ProcessController.cpp
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */
#include "StdAfx.h"
#include "ProcessController.h"


void ProcessController::ConvertToGCode(string &GcodeTxt, const string &GcodeStart, const string &GcodeLayer, const string &GcodeEnd)
{

	// Make Layers
	UINT LayerNr = 0;

	float zSize = (stl.Max.z-stl.Min.z);
	float z=stl.Min.z+0.01;
	float zStep = stl.LayerThickness;

	gcode.commands.clear();

	MakeRaft();

	while(z<stl.Max.z)
	{
		{
		CuttingPlane plane;
		stl.CalcCuttingPlane(z, plane);	// output is alot of un-connected line segments with individual vertices

		float hackedZ = z;
		while(plane.LinkSegments(hackedZ, stl.ShrinkValue, stl.Optimization, stl.DisplayCuttingPlane) == false)	// If segment linking fails, re-calc a new layer close to this one, and use that.
			{										// This happens when there's triangles missing in the input STL
			hackedZ+= 0.1f;
			plane.polygons.clear();
			stl.CalcCuttingPlane(hackedZ, plane);	// output is alot of un-connected line segments with individual vertices
			}

//		plane.Draw(z);

		// inFill
		vector<Vector2f> infill;

		CuttingPlane infillCuttingPlane = plane;
		infillCuttingPlane.polygons = infillCuttingPlane.offsetPolygons;
		infillCuttingPlane.vertices = infillCuttingPlane.offsetVertices;
		infillCuttingPlane.offsetPolygons.clear();
		infillCuttingPlane.offsetVertices.clear();
		if(stl.ShellOnly == false)
			{
			infillCuttingPlane.Shrink(stl.ShrinkValue, stl.Optimization, stl.DisplayCuttingPlane);
			infillCuttingPlane.CalcInFill(infill, LayerNr, z, stl.InfillDistance, stl.InfillRotation, stl.InfillRotationPrLayer, stl.DisplayDebuginFill);
			}
		// Make the GCode from the plane and the infill
		plane.MakeGcode(infill, gcode, z, gcode.MinPrintSpeedXY, gcode.MaxPrintSpeedXY, gcode.MinPrintSpeedZ, gcode.MaxPrintSpeedZ, gcode.accelerationSteps, gcode.distanceBetweenSpeedSteps, gcode.extrusionFactor, stl.EnableAcceleration);
		LayerNr++;
		}
	z+=zStep;
	}

	GcodeTxt.clear();
	gcode.MakeText(GcodeTxt, GcodeStart, GcodeLayer, GcodeEnd);
}

void ProcessController::MakeRaft()
{
/*
float RaftSize;
int RaftBaseLayerCount;
float RaftMaterialPrDistanceRatio;
float RaftRotation;
float RaftBaseDistance;
float RaftBaseThickness;
float RaftBaseTemperature;
int RaftInterfaceLayerCount;
float RaftInterfaceMaterialPrDistanceRatio;
float RaftRotationPrLayer;
float RaftInterfaceDistance;
float RaftInterfaceThickness;
float RaftInterfaceTemperature;
*/

	// Do the base(s)

	UINT LayerNr = 0;

	float step = RaftBaseDistance;

	Vector2f Min = Vector2f(stl.Min.x*RaftSize, stl.Min.y*RaftSize);
	Vector2f Max = Vector2f(stl.Max.x*RaftSize, stl.Max.y*RaftSize);

	float Length = sqrtf(2)*(   ((Max.x)>(Max.y)? (Max.x):(Max.y))  -  ((Min.x)<(Min.y)? (Min.x):(Min.y))  )/2.0f;	// bbox of object

	float rot = RaftRotation/180.0f*M_PI;
	rot += (float)LayerNr*RaftInterfaceMaterialPrDistanceRatio/180.0f*M_PI;
	Vector2f InfillDirX(cosf(rot), sinf(rot));
	Vector2f InfillDirY(-InfillDirX.y, InfillDirX.x);
	Vector2f Center = (Max+Min)/2.0f;

	Vector3f LastPosition;
	float z = 0.0f;
	bool reverseLines = false;
	for(float x = -Length ; x < Length ; x+=step)
		{
		Vector2f P1 = (InfillDirX * Length)+(InfillDirY*x)+ Center;
		Vector2f P2 = (InfillDirX * -Length)+(InfillDirY*x) + Center;

		if(reverseLines)
		{
		Vector2f tmp = P1;
		P1 = P2;
		P2 = tmp;
		}

		Command command;

		command.Code = COORDINATEDMOTION;
		command.where = Vector3f(P1.x, P1.y, z);
		float len = (P2 - P1).length();
		command.e = 0;		// move or extrude?
		command.f = 500.0f;
		gcode.commands.push_back(command);
		LastPosition = command.where;

		command.Code = COORDINATEDMOTION;
		command.where = Vector3f(P2.x, P2.y, z);
		len = (P2 - P1).length();
		command.e = len*RaftMaterialPrDistanceRatio;		// move or extrude?
		command.f = 500.0f;
		gcode.commands.push_back(command);
		LastPosition = command.where;


		// Crop lines to bbox*size
		{
			glBegin(GL_LINES);
			glColor3f(1.0f,1.0f,0);
			glVertex3f(P1.x, P1.y, 0);
			glVertex3f(P2.x, P2.y, 0);
			glEnd();
		}

		reverseLines = !reverseLines;
		}
}


void ProcessController::Draw()
{
	printer.Draw();
	stl.draw();
	previewCuttingPlane.Draw(0.5f, stl.DrawVertexNumbers, stl.DrawLineNumbers);
	gcode.draw();
}
void WriteGCode(string &GcodeTxt, const string &GcodeStart, const string &GcodeLayer, const string &GcodeEnd, string filename)
{

}
