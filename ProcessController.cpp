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
		while(plane.LinkSegments(hackedZ) == false)	// If segment linking fails, re-calc a new layer close to this one, and use that.
			{										// This happens when there's triangles missing in the input STL
			hackedZ+= 0.1f;
			plane.polygons.clear();
			stl.CalcCuttingPlane(hackedZ, plane);	// output is alot of un-connected line segments with individual vertices
			}

//		plane.Draw(z);

		// inFill
		vector<Vector2f> infill;

		plane.CalcInFill(infill, LayerNr, z);

		// Make the GCode from the plane and the infill
		plane.MakeGcode(infill, gcode, z);
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
	previewCuttingPlane.Draw(0.5f);
	gcode.draw();
}

