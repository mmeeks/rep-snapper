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

	gcode.MakeText(GcodeTxt, GcodeStart, GcodeLayer, GcodeEnd);
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
