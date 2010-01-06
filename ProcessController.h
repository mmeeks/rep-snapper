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
#include "xml/xml.h"
#include <vmmlib/vmmlib.h>
#include "UI.h"
#include "stl.h"
#include "gcode.h"

class GCode;

using namespace std;

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
		RaftInterfaceLayerCount = 2;
		RaftInterfaceMaterialPrDistanceRatio = 1.0f;
		RaftRotationPrLayer = 90.0f;
		RaftInterfaceDistance = 2.0f;
		RaftInterfaceThickness = 1.0f;
		RaftInterfaceTemperature = 1.0f;
		m_Filename = "./repsnapper";

		// Printer
		m_fVolume = Vector3f(200,200,140);
		PrintMargin = Vector3f(10,10,0);
		ExtrudedMaterialWidth = 0.7f;	// 0.7

		//GCode
		GCodeDrawStart = 0.0f;;
		GCodeDrawEnd = 1.0f;

		MinPrintSpeedXY = 1000.0f;
		MaxPrintSpeedXY = 4000.0f;
		MinPrintSpeedZ = 50.0f;
		MaxPrintSpeedZ = 150.0f;

		accelerationSteps = 5;
		distanceBetweenSpeedSteps= 0.5f;
		extrusionFactor = 1.0f;
		UseIncrementalEcode = false;
		UseFirmwareAcceleration = true;

		LayerThickness = 0.4f;
		CuttingPlaneValue = 0.5f;
//		PolygonOpasity = 0.5f;

		DisplayEndpoints = false;
		DisplayNormals = false;
		DisplayBBox = false;
		DisplayWireframe = false;
		DisplayWireframeShaded = true;
		DisplayPolygons = false;
		DisplayAllLayers = false;
		DisplayinFill = false;

		InfillDistance = 2.0f;
		InfillRotation = 45.0f;
		InfillRotationPrLayer = 90.0f;
		Optimization = 0.02f;
		Examine = 0.5f;

		DisplayDebuginFill = false;
		DisplayDebug = false;
		DisplayCuttingPlane = true;
		DrawVertexNumbers=false;
		DrawLineNumbers=false;

		ShellOnly = false;
		ShellCount = 1;

		EnableAcceleration = true;
		DisplayDebuginFill = true;
		DisplayCuttingPlane = true;

		gui = 0;

};

	ProcessController::~ProcessController();

	void Draw();
	
	// STL Functions
	void ReadStl(string filename, const Vector3f &PrintingMargin) {stl.Read(filename, PrintingMargin);};
	void OptimizeRotation() { stl.OptimizeRotation();}
	void RotateObject(float x, float y, float z, float a) {stl.RotateObject(Vector3f(x,y,z),a);}

	void ConvertToGCode(string &GcodeTxt, const string &GcodeStart, const string &GcodeLayer, const string &GcodeEnd);

	// GCode Functions
	void ReadGCode(string filename) {gcode.Read(filename);};
	void WriteGCode(string &GcodeTxt, const string &GcodeStart, const string &GcodeLayer, const string &GcodeEnd, string filename);

	Vector3f MakeRaft(float &z);	// Returns center of raft
	//Printer
	void SetVolume(float x, float y, float z) { m_fVolume = Vector3f(x,y,z);}

	// Load and save settings
	void LoadXML();
	void SaveXML();
	void SaveXML(string filename);
	void LoadXML(XMLElement *e);
	void SaveXML(XMLElement *e);

	// Process functions
	string m_Filename;

	// Start, layer, end GCode
	string GCodeStartText;
	string GCodeLayerText;
	string GCodeEndText;

	/*--------------Models-------------------*/
	Printer printer;					// Printer settings and functions
	STL stl;							// A STL file
	CuttingPlane previewCuttingPlane;	//The cuttingplane that's drawn as a live preview
	GCode gcode;						// Gcode as binary data
	string GcodeTxt;					// Final GCode as text

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

	// GCode
	float GCodeDrawStart;
	float GCodeDrawEnd;
	float MinPrintSpeedXY;
	float MaxPrintSpeedXY;
	float MinPrintSpeedZ;
	float MaxPrintSpeedZ;

	UINT accelerationSteps;
	float distanceBetweenSpeedSteps;
	float extrusionFactor;
	bool UseFirmwareAcceleration;

	// Printer
	Vector3f	m_fVolume;				// Max print volume
	Vector3f	PrintMargin;
	float		ExtrudedMaterialWidth;	// Width of extruded material
	bool		UseIncrementalEcode;

	// STL 
	float LayerThickness;
	float CuttingPlaneValue;
//	float PolygonOpasity;

	// CuttingPlane
	float InfillDistance;
	float InfillRotation;
	float InfillRotationPrLayer;
	float Optimization;
	float Examine;

	bool ShellOnly;
	UINT ShellCount;

	bool EnableAcceleration;

	// GUI... ?
	bool DisplayEndpoints;
	bool DisplayNormals;
	bool DisplayBBox;
	bool DisplayWireframe;
	bool DisplayWireframeShaded;
	bool DisplayPolygons;
	bool DisplayAllLayers;
	bool DisplayinFill;
	bool DisplayDebuginFill;
	bool DisplayDebug;
	bool DisplayCuttingPlane;
	bool DrawVertexNumbers;
	bool DrawLineNumbers;

	// Rendering
	float PolygonVal;
	float PolygonSat;
	float PolygonHue;
	float WireframeVal;
	float WireframeSat;
	float WireframeHue;
	float NormalsSat;
	float NormalsVal;
	float NormalsHue;
	float EndpointsSat;
	float EndpointsVal;
	float EndpointsHue;
	float GCodeExtrudeHue;
	float GCodeExtrudeSat;
	float GCodeExtrudeVal;
	float GCodeMoveHue;
	float GCodeMoveSat;
	float GCodeMoveVal;
	float Highlight;
	float NormalsLength;
	float EndPointSize;

	bool LuminanceShowsSpeed;
	bool DisplayGCode;

	bool ApronEnable;
	bool ApronPreview;
	bool RaftEnable;
	float ApronSize;
	float ApronHeight;
	float ApronCoverageX;
	float ApronCoverageY;
	float ApronDistanceToObject;
	float ApronInfillDistance;

	// Maybe a pointer to the gui
	GUI *gui;
};