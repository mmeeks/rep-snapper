/* -------------------------------------------------------- *
*
* ProcessController.h
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */
#pragma once

#include "StdAfx.h"
#include "Printer.h"

class ProcessController
{
public:
	ProcessController(){
		RaftSize = 1.33f;
		RaftBaseLayerCount = 1;
		RaftMaterialPrDistanceRatio = 1.8f;
		RaftRotation = 90.0f;
		RaftBaseDistance = 2.0f;
		RaftBaseThickness = 1.0f;
		RaftBaseTemperature = 1.10f;
		RaftInterfaceLayerCount = 1;
		RaftInterfaceMaterialPrDistanceRatio = 1.0f;
		RaftRotationPrLayer = 90.0f;
		RaftInterfaceDistance = 2.0f;
		RaftInterfaceThickness = 1.0f;
		RaftInterfaceTemperature = 1.0f;
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

	void MakeRaft();


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


	// Raft
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
};