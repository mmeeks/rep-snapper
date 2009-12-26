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

	float destinationZ=0.0f;

	Vector3f RaftPrintingMargin;
	MakeRaft(destinationZ, RaftPrintingMargin);
	stl.MoveIntoPrintingArea(RaftPrintingMargin);


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
		plane.MakeGcode(infill, gcode, destinationZ, gcode.MinPrintSpeedXY, gcode.MaxPrintSpeedXY, gcode.MinPrintSpeedZ, gcode.MaxPrintSpeedZ, gcode.accelerationSteps, gcode.distanceBetweenSpeedSteps, gcode.extrusionFactor, stl.EnableAcceleration);
		LayerNr++;
		destinationZ += stl.LayerThickness;
		}
	z+=zStep;
	}

	GcodeTxt.clear();
	gcode.MakeText(GcodeTxt, GcodeStart, GcodeLayer, GcodeEnd);
}


void ProcessController::MakeRaft(float &z, Vector3f &PrintMargin)
{
	vector<InFillHit> HitsBuffer;

	UINT LayerNr = 0;

	float step;

	Vector2f StlCenter( (stl.Min.x+stl.Max.x)/2.0f, (stl.Min.y+stl.Max.y)/2.0f);
	Vector2f stlSize( (stl.Max.x-stl.Min.x)/2.0f, (stl.Max.y-stl.Min.y)/2.0f);

	Vector2f Min = StlCenter-stlSize*RaftSize;
	Vector2f Max = StlCenter+stlSize*RaftSize;

	Max = Max - Min + Vector2f(printer.PrintMargin.x, printer.PrintMargin.y);
	Min = Vector2f(printer.PrintMargin.x, printer.PrintMargin.y);
	Vector2f Center = (Max+Min)*0.5f;

	Vector2f newMargin = Center-Vector2f(stl.Center.x, stl.Center.y);
	PrintMargin = Vector3f(newMargin.x, newMargin.y, 0)+printer.PrintMargin;

	float Length = sqrtf(2)*(   ((Max.x)>(Max.y)? (Max.x):(Max.y))  -  ((Min.x)<(Min.y)? (Min.x):(Min.y))  )/2.0f;	// bbox of object

	float rot = RaftRotation/180.0f*M_PI;

	while(LayerNr < RaftBaseLayerCount+RaftInterfaceLayerCount)
	{
		rot = (RaftRotation+(float)LayerNr*RaftRotationPrLayer)/180.0f*M_PI;
		Vector2f InfillDirX(cosf(rot), sinf(rot));
		Vector2f InfillDirY(-InfillDirX.y, InfillDirX.x);

		Vector3f LastPosition;
		bool reverseLines = false;

		if(LayerNr < RaftBaseLayerCount)
			step = RaftBaseDistance;
		else
			step = RaftInterfaceDistance;

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

			// Crop lines to bbox*size
			Vector3f point;
			InFillHit hit;
			HitsBuffer.clear();
			Vector2f P3(Min.x, Min.y);
			Vector2f P4(Min.x, Max.y);
			if(IntersectXY(P1,P2,P3,P4,hit))	//Intersect edges of bbox
				HitsBuffer.push_back(hit);
			P3 = Vector2f(Max.x,Max.y);
			if(IntersectXY(P1,P2,P3,P4,hit))
				HitsBuffer.push_back(hit);
			P4 = Vector2f(Max.x,Min.y);
			if(IntersectXY(P1,P2,P3,P4,hit))
				HitsBuffer.push_back(hit);
			P3 = Vector2f(Min.x,Min.y);
			if(IntersectXY(P1,P2,P3,P4,hit))
				HitsBuffer.push_back(hit);
			if(HitsBuffer.size() == 0)	// it can only be 2 or zero
				continue;


			std::sort(HitsBuffer.begin(), HitsBuffer.end(), InFillHitCompareFunc);

			P1 = HitsBuffer[0].p;
			P2 = HitsBuffer[1].p;

			float materialRatio;
			if(LayerNr < RaftBaseLayerCount)
				materialRatio = RaftMaterialPrDistanceRatio;		// move or extrude?
			else
				materialRatio = RaftInterfaceMaterialPrDistanceRatio;		// move or extrude?

			MakeAcceleratedGCodeLine(Vector3f(P1.x,P1.y,z), Vector3f(P2.x,P2.y,z), gcode.accelerationSteps, gcode.distanceBetweenSpeedSteps, materialRatio, gcode, z, gcode.MinPrintSpeedXY, gcode.MaxPrintSpeedXY, gcode.MinPrintSpeedZ, gcode.MaxPrintSpeedZ);

			reverseLines = !reverseLines;
		}
		if(LayerNr < RaftBaseLayerCount)
			z+=RaftBaseThickness*stl.LayerThickness;
		else
			z+=RaftInterfaceThickness*stl.LayerThickness;
		LayerNr++;
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
