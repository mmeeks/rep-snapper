#pragma once

#include "StdAfx.h"
#include "Printer.h"

class ProcessController
{
public:
	ProcessController(){
};
	
	void Draw();
	
	// STL Functions
	void ReadStl(string filename, const Vector3f &PrintingMargin) {stl.Read(filename, PrintingMargin);};
	void OptimizeRotation() { stl.OptimizeRotation();}
	void RotateObject(float x, float y, float z, float a) {stl.RotateObject(Vector3f(x,y,z),a);}

	void ConvertToGCode(string &GcodeTxt, const string &GcodeStart, const string &GcodeLayer, const string &GcodeEnd);

	// GCode Functions
	void ReadGCode(string filename) {gcode.Read(filename);};
	void WriteGCode(string &GcodeTxt, const string &GcodeStart, const string &GcodeLayer, const string &GcodeEnd, string filename);

	// Process functions
	

	// Start, layer, end GCode
	string GCodeStartText;
	string GCodeLayerText;
	string GCodeEndText;

	/*--------------Models-------------------*/
	Printer printer;
	STL stl;
	CuttingPlane previewCuttingPlane;
	GCode gcode;
	string GcodeTxt;
};