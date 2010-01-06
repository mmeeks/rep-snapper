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

ProcessController::~ProcessController()
{
	SaveXML();
}

void ProcessController::ConvertToGCode(string &GcodeTxt, const string &GcodeStart, const string &GcodeLayer, const string &GcodeEnd)
{
	if(gui)
	{
		gui->ProgressBar->value(0);
		gui->ProgressBar->label("Converting");
		gui->ProgressBar->maximum(stl.Max.z);
	}

	// Make Layers
	UINT LayerNr = 0;

	float z=stl.Min.z+0.0001f;				// Offset it a bit in Z, z=0 gives a empty slice because no triangles crosses this Z value

	gcode.commands.clear();

	float destinationZ=0.0f;

	Vector3f RaftCenter = Vector3f(0,0,0);
	if(RaftEnable)
		{
		RaftCenter = MakeRaft(destinationZ);
		stl.CenterAroundXY(RaftCenter);
		}
	while(z<stl.Max.z+0.0001f)
	{
		if(gui)
		{
			gui->ProgressBar->value(z);
			gui->ProgressBar->redraw();
			Fl::check();
		}
		{
		CuttingPlane plane;
		stl.CalcCuttingPlane(z, plane);	// output is alot of un-connected line segments with individual vertices

		float hackedZ = z;
		while(plane.LinkSegments(hackedZ, ExtrudedMaterialWidth*0.5f, Optimization, DisplayCuttingPlane) == false)	// If segment linking fails, re-calc a new layer close to this one, and use that.
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
		if(ShellOnly == false)
			{
			infillCuttingPlane.Shrink(ExtrudedMaterialWidth*0.5f, Optimization, DisplayCuttingPlane);
			infillCuttingPlane.CalcInFill(infill, LayerNr, destinationZ, InfillDistance, InfillRotation, InfillRotationPrLayer, DisplayDebuginFill);
			}
		// Make the GCode from the plane and the infill
		plane.MakeGcode(infill, gcode, destinationZ, MinPrintSpeedXY, MaxPrintSpeedXY, MinPrintSpeedZ, MaxPrintSpeedZ, accelerationSteps, distanceBetweenSpeedSteps, extrusionFactor, EnableAcceleration, UseIncrementalEcode, UseFirmwareAcceleration);
		LayerNr++;
		destinationZ += LayerThickness;
		}
	z+=LayerThickness;
	}

	GcodeTxt.clear();
	gcode.MakeText(GcodeTxt, GcodeStart, GcodeLayer, GcodeEnd, UseIncrementalEcode);
	gui->ProgressBar->label("Done");
}


Vector3f ProcessController::MakeRaft(float &z)
{
	vector<InFillHit> HitsBuffer;

	UINT LayerNr = 0;

	float step;

	Vector2f StlCenter( (stl.Min.x+stl.Max.x)/2.0f, (stl.Min.y+stl.Max.y)/2.0f);
	Vector2f stlSize( (stl.Max.x-stl.Min.x)/2.0f, (stl.Max.y-stl.Min.y)/2.0f);

	Vector2f Min = -stlSize*RaftSize;
	Vector2f Max = stlSize*RaftSize;

	Max = Max - Min + Vector2f(PrintMargin.x, PrintMargin.y);
	Min = Vector2f(PrintMargin.x, PrintMargin.y);
	Vector2f Center = (Max+Min)*0.5f;

	float Length = sqrtf(2)*(   ((Max.x)>(Max.y)? (Max.x):(Max.y))  -  ((Min.x)<(Min.y)? (Min.x):(Min.y))  )/2.0f;	// bbox of object

	float E = 0.0f;

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
		Vector2f P1, P2;
		for(float x = -Length ; x < Length ; x+=step)
		{
			P1 = (InfillDirX * Length)+(InfillDirY*x)+ Center;
			P2 = (InfillDirX * -Length)+(InfillDirY*x) + Center;

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

			MakeAcceleratedGCodeLine(Vector3f(P1.x,P1.y,z), Vector3f(P2.x,P2.y,z), accelerationSteps, distanceBetweenSpeedSteps, materialRatio, gcode, z, MinPrintSpeedXY, MaxPrintSpeedXY, MinPrintSpeedZ, MaxPrintSpeedZ, UseIncrementalEcode, E, UseFirmwareAcceleration);

			reverseLines = !reverseLines;
		}
		// Set startspeed for Z-move
		Command g;
		g.Code = SETSPEED;
		g.where = Vector3f(P2.x, P2.y, z);
		g.f=MinPrintSpeedZ;
		g.comment = "Move Z";
		g.e = E;
		gcode.commands.push_back(g);
		if(LayerNr < RaftBaseLayerCount)
			z+=RaftBaseThickness*LayerThickness;
		else
			z+=RaftInterfaceThickness*LayerThickness;
		// Move Z
		g.Code = ZMOVE;
		g.where = Vector3f(P2.x, P2.y, z);
		g.f=MinPrintSpeedZ;
		g.comment = "Move Z";
		g.e = E;
		gcode.commands.push_back(g);

		LayerNr++;
	}
	return Vector3f(Center.x, Center.y, 0);
}


void ProcessController::Draw()
{
	printer.Draw(*this);
	stl.draw(*this);
	if(DisplayCuttingPlane)
		previewCuttingPlane.Draw(CuttingPlaneValue, DrawVertexNumbers, DrawLineNumbers);
	if(DisplayGCode)
		gcode.draw(*this);
}
void WriteGCode(string &GcodeTxt, const string &GcodeStart, const string &GcodeLayer, const string &GcodeEnd, string filename)
{

}

void ProcessController::SaveXML(string filename)
{
	filename = filename+".xml";

	XML* xml = new XML(filename.c_str()); 
	XMLElement* e = xml->GetRootElement();
	SaveXML(e);
	if (xml->IntegrityTest())
		xml->Save(); // Saves back to file
	delete xml;
}

void ProcessController::SaveXML(XMLElement *e)
{
	XMLElement *x = e->FindElementZ("ProcessController", true);

	// Raft parameters
	x->FindVariableZ("RaftSize", true, "1.7")->SetValueFloat(RaftSize);
	x->FindVariableZ("RaftBaseLayerCount", true, "1")->SetValueInt(RaftSize);
	x->FindVariableZ("RaftMaterialPrDistanceRatio", true, "1.7")->SetValueFloat(RaftMaterialPrDistanceRatio);
	x->FindVariableZ("RaftRotation", true, "90")->SetValueFloat(RaftRotation);
	x->FindVariableZ("RaftBaseDistance", true, "2.5")->SetValueFloat(RaftBaseDistance);
	x->FindVariableZ("RaftBaseThickness", true, "1.7")->SetValueFloat(RaftBaseThickness);
	x->FindVariableZ("RaftBaseTemperature", true, "190")->SetValueFloat(RaftBaseTemperature);
	x->FindVariableZ("RaftInterfaceLayerCount", true, "1")->SetValueInt(RaftInterfaceLayerCount);
	x->FindVariableZ("RaftInterfaceMaterialPrDistanceRatio", true, "1")->SetValueFloat(RaftInterfaceMaterialPrDistanceRatio);
	x->FindVariableZ("RaftRotationPrLayer", true, "90")->SetValueFloat(RaftRotationPrLayer);
	x->FindVariableZ("RaftInterfaceDistance", true, "2")->SetValueFloat(RaftInterfaceDistance);
	x->FindVariableZ("RaftInterfaceThickness", true, "1")->SetValueFloat(RaftInterfaceThickness);
	x->FindVariableZ("RaftInterfaceTemperature", true, "190")->SetValueFloat(RaftInterfaceTemperature);


	// GCode parameters
	x->FindVariableZ("GCodeStartText", true,"[Empty]")->SetValue(GCodeStartText.c_str());	
	x->FindVariableZ("GCodeLayerText", true,"[Empty]")->SetValue(GCodeLayerText.c_str());	
	x->FindVariableZ("GCodeEndText", true,"[Empty]")->SetValue(GCodeEndText.c_str());	
	x->FindVariableZ("GCodeDrawStart", true, "190")->SetValueFloat(GCodeDrawStart);
	x->FindVariableZ("GCodeDrawEnd", true, "190")->SetValueFloat(GCodeDrawEnd);
	x->FindVariableZ("ShellOnly", true, "0.66")->SetValueInt((int)ShellOnly);
	x->FindVariableZ("ShellCount", true, "0.66")->SetValueInt(ShellCount);

	// Printer parameters
	x->FindVariableZ("MinPrintSpeedXY", true, "400")->SetValueFloat(MinPrintSpeedXY);
	x->FindVariableZ("MaxPrintSpeedXY", true, "1500")->SetValueFloat(MaxPrintSpeedXY);
	x->FindVariableZ("MinPrintSpeedZ", true, "50")->SetValueFloat(MinPrintSpeedZ);
	x->FindVariableZ("MaxPrintSpeedZ", true, "150")->SetValueFloat(MaxPrintSpeedZ);
	x->FindVariableZ("accelerationSteps", true, "3")->SetValueInt(accelerationSteps);
	x->FindVariableZ("distanceBetweenSpeedSteps", true, "5")->SetValueFloat(distanceBetweenSpeedSteps);
	x->FindVariableZ("UseFirmwareAcceleration", true, "5")->SetValueInt(UseFirmwareAcceleration);
	x->FindVariableZ("extrusionFactor", true, "0.66")->SetValueFloat(extrusionFactor);
	x->FindVariableZ("EnableAcceleration", true, "0.66")->SetValueInt((int)EnableAcceleration);
	x->FindVariableZ("UseIncrementalEcode", true, "0.66")->SetValueInt((int)UseIncrementalEcode);

	x->FindVariableZ("m_fVolume.x", true, "200")->SetValueFloat(m_fVolume.x);
	x->FindVariableZ("m_fVolume.y", true, "200")->SetValueFloat(m_fVolume.y);
	x->FindVariableZ("m_fVolume.z", true, "140")->SetValueFloat(m_fVolume.z);
	x->FindVariableZ("PrintMargin.x", true, "10")->SetValueFloat(PrintMargin.x);
	x->FindVariableZ("PrintMargin.y", true, "10")->SetValueFloat(PrintMargin.y);
	PrintMargin.z = 0.0f;
	x->FindVariableZ("extrusionFactor", true, "1")->SetValueFloat(extrusionFactor);
	x->FindVariableZ("ExtrudedMaterialWidth", true, "0.66")->SetValueFloat(ExtrudedMaterialWidth);
	x->FindVariableZ("LayerThickness", true, "0.4")->SetValueFloat(LayerThickness);

	// CuttingPlane parameters
	x->FindVariableZ("InfillDistance", true, "2")->SetValueFloat(InfillDistance);
	x->FindVariableZ("InfillRotation", true, "90")->SetValueFloat(InfillRotation);
	x->FindVariableZ("InfillRotationPrLayer", true, "90")->SetValueFloat(InfillRotationPrLayer);
	x->FindVariableZ("Optimization", true, "0.05")->SetValueFloat(Optimization);
//	x->FindVariableZ("PolygonOpasity", true, "0.66")->SetValueFloat(PolygonOpasity);


	// GUI parameters
	x->FindVariableZ("CuttingPlaneValue", true, "0.66")->SetValueFloat(CuttingPlaneValue);
	x->FindVariableZ("Examine", true, "0.66")->SetValueFloat(Examine);

	x->FindVariableZ("DisplayEndpoints", true, "0")->SetValueInt((int)DisplayEndpoints);
	x->FindVariableZ("DisplayNormals", true, "0")->SetValueInt((int)DisplayNormals);
	x->FindVariableZ("DisplayWireframe", true, "0")->SetValueInt((int)DisplayWireframe);
	x->FindVariableZ("DisplayWireframeShaded", true, "1")->SetValueInt((int)DisplayWireframeShaded);

	x->FindVariableZ("DisplayPolygons", true, "1")->SetValueInt((int)DisplayPolygons);
	x->FindVariableZ("DisplayAllLayers", true, "0")->SetValueInt((int)DisplayAllLayers);
	x->FindVariableZ("DisplayinFill", true, "1")->SetValueInt((int)DisplayinFill);
	x->FindVariableZ("DisplayDebuginFill", true, "0")->SetValueInt((int)DisplayDebuginFill);
	x->FindVariableZ("DisplayDebug", true, "0")->SetValueInt((int)DisplayDebug);
	x->FindVariableZ("DisplayCuttingPlane", true, "0")->SetValueInt((int)DisplayCuttingPlane);
	x->FindVariableZ("DrawVertexNumbers", true, "0")->SetValueInt((int)DrawVertexNumbers);
	x->FindVariableZ("DrawLineNumbers", true, "0")->SetValueInt((int)DrawLineNumbers);
	
	x->FindVariableZ("PolygonVal", true, "0.5")->SetValueFloat(PolygonVal);
	x->FindVariableZ("PolygonSat", true, "1")->SetValueFloat(PolygonSat);
	x->FindVariableZ("PolygonHue", true, "0.2")->SetValueFloat(PolygonHue);
	x->FindVariableZ("WireframeVal", true, "0.5")->SetValueFloat(WireframeVal);
	x->FindVariableZ("WireframeSat", true, "1")->SetValueFloat(WireframeSat);
	x->FindVariableZ("WireframeHue", true, "0.3")->SetValueFloat(WireframeHue);
	x->FindVariableZ("NormalsSat", true, "0.5")->SetValueFloat(NormalsSat);
	x->FindVariableZ("NormalsVal", true, "1")->SetValueFloat(NormalsVal);
	x->FindVariableZ("NormalsHue", true, "0.4")->SetValueFloat(NormalsHue);
	x->FindVariableZ("EndpointsSat", true, "1")->SetValueFloat(EndpointsSat);
	x->FindVariableZ("EndpointsVal", true, "0.5")->SetValueFloat(EndpointsVal);
	x->FindVariableZ("EndpointsHue", true, "0.5")->SetValueFloat(EndpointsHue);
	x->FindVariableZ("GCodeExtrudeHue", true, "0.6")->SetValueFloat(GCodeExtrudeHue);
	x->FindVariableZ("GCodeExtrudeSat", true, "1")->SetValueFloat(GCodeExtrudeSat);
	x->FindVariableZ("GCodeExtrudeVal", true, "0.5")->SetValueFloat(GCodeExtrudeVal);
	x->FindVariableZ("GCodeMoveHue", true, "0.7")->SetValueFloat(GCodeMoveHue);
	x->FindVariableZ("GCodeMoveSat", true, "1")->SetValueFloat(GCodeMoveSat);
	x->FindVariableZ("GCodeMoveVal", true, "0.5")->SetValueFloat(GCodeMoveVal);
	x->FindVariableZ("Highlight", true, "0.4")->SetValueFloat(Highlight);
	x->FindVariableZ("NormalsLength", true, "10")->SetValueFloat(NormalsLength);
	x->FindVariableZ("EndPointSize", true, "8")->SetValueFloat(EndPointSize);

	x->FindVariableZ("DisplayGCode", true, "1")->SetValueFloat(DisplayGCode);
	x->FindVariableZ("LuminanceShowsSpeed", true, "1")->SetValueFloat(LuminanceShowsSpeed);

	x->FindVariableZ("RaftEnable", true, "1")->SetValueFloat(RaftEnable);
	x->FindVariableZ("ApronEnable", true, "1")->SetValueFloat(ApronEnable);
	x->FindVariableZ("ApronPreview", true, "1")->SetValueFloat(ApronPreview);
	x->FindVariableZ("ApronSize", true, "1")->SetValueFloat(ApronSize);
	x->FindVariableZ("ApronHeight", true, "1")->SetValueFloat(ApronHeight);
	x->FindVariableZ("ApronCoverageX", true, "1")->SetValueFloat(ApronCoverageX);
	x->FindVariableZ("ApronCoverageY", true, "1")->SetValueFloat(ApronCoverageY);
	x->FindVariableZ("ApronDistanceToObject", true, "1")->SetValueFloat(ApronDistanceToObject);
	x->FindVariableZ("ApronInfillDistance", true, "1")->SetValueFloat(ApronInfillDistance);

	/*

	XMLElement *x = e->FindElementZ("RED_ProcessingSettings", true);

	char text[200];
	sprintf(text,"%s",ImageProcessingLimits::GammaCurveLabels[(int)GammaCurve]);
	x->FindVariableZ("GammaCurve", true,text)->SetValue(text);	

	sprintf(text,"%s",ImageProcessingLimits::ColorSpaceLabels[(int)ColorSpace]);
	x->FindVariableZ("ColorSpace", true,text)->SetValue(text);	

	x->FindVariableZ("ISO", true, "320")->SetValueInt((int)ISO);	

	x->FindVariableZ("Kelvin", true, "3200")->SetValueFloat(Kelvin);	
	x->FindVariableZ("Tint", true, "0")->SetValueFloat(Tint);	
	x->FindVariableZ("ExposureCompensation", true, "0")->SetValueFloat(ExposureCompensation);	
	x->FindVariableZ("GainRed", true, "1")->SetValueFloat(GainRed);	
	x->FindVariableZ("GainGreen", true, "1")->SetValueFloat(GainGreen);	
	x->FindVariableZ("GainBlue", true, "1")->SetValueFloat(GainBlue);	
	x->FindVariableZ("Saturation", true, "1")->SetValueFloat(Saturation);	
	x->FindVariableZ("Contrast", true, "1")->SetValueFloat(Contrast);	
	x->FindVariableZ("Brightness", true, "1")->SetValueFloat(Brightness);	
	x->FindVariableZ("DRX", true, "0")->SetValueFloat(DRX);	
	x->FindVariableZ("CustomPDLogGamma", true, "0")->SetValueFloat(CustomPDLogGamma);	
	x->FindVariableZ("UserCurve0", true, "0.00")->SetValueFloat(UserCurve[0]);	
	x->FindVariableZ("UserCurve1", true, "0.00")->SetValueFloat(UserCurve[1]);	
	x->FindVariableZ("UserCurve2", true, "0.25")->SetValueFloat(UserCurve[2]);	
	x->FindVariableZ("UserCurve3", true, "0.25")->SetValueFloat(UserCurve[3]);	
	x->FindVariableZ("UserCurve4", true, "0.50")->SetValueFloat(UserCurve[4]);	
	x->FindVariableZ("UserCurve5", true, "0.50")->SetValueFloat(UserCurve[5]);	
	x->FindVariableZ("UserCurve6", true, "0.75")->SetValueFloat(UserCurve[6]);	
	x->FindVariableZ("UserCurve7", true, "0.75")->SetValueFloat(UserCurve[7]);	
	x->FindVariableZ("UserCurve8", true, "1.00")->SetValueFloat(UserCurve[8]);	
	x->FindVariableZ("UserCurve9", true, "1.00")->SetValueFloat(UserCurve[9]);	
*/
}
void ProcessController::LoadXML(XMLElement *e)
{
	XMLElement *x = e->FindElementZ("ProcessController", true);

	XMLVariable* y;

	y=x->FindVariableZ("RaftSize", true, "1.33");
	if(y)	RaftSize = y->GetValueFloat();
	y=x->FindVariableZ("RaftBaseLayerCount", true, "1");
	if(y)	RaftBaseLayerCount = y->GetValueInt();
	y=x->FindVariableZ("RaftMaterialPrDistanceRatio", true, "1.7");
	if(y)	RaftMaterialPrDistanceRatio = y->GetValueFloat();
	y=x->FindVariableZ("RaftRotation", true, "90");
	if(y)	RaftRotation = y->GetValueFloat();
	y=x->FindVariableZ("RaftBaseDistance", true, "2.5");
	if(y)	RaftBaseDistance = y->GetValueFloat();
	y=x->FindVariableZ("RaftBaseThickness", true, "1");
	if(y)	RaftBaseThickness = y->GetValueFloat();
	y=x->FindVariableZ("RaftBaseTemperature", true, "190");
	if(y)	RaftBaseTemperature = y->GetValueFloat();
	y=x->FindVariableZ("RaftInterfaceLayerCount", true, "2");
	if(y)	RaftInterfaceLayerCount = y->GetValueInt();
	y=x->FindVariableZ("RaftInterfaceMaterialPrDistanceRatio", true, "1");
	if(y)	RaftInterfaceMaterialPrDistanceRatio = y->GetValueFloat();
	y=x->FindVariableZ("RaftRotationPrLayer", true, "90");
	if(y)	RaftRotationPrLayer = y->GetValueFloat();
	y=x->FindVariableZ("RaftInterfaceDistance", true, "2");
	if(y)	RaftInterfaceDistance = y->GetValueFloat();
	y=x->FindVariableZ("RaftInterfaceThickness", true, "1");
	if(y)	RaftInterfaceThickness = y->GetValueFloat();
	y=x->FindVariableZ("RaftInterfaceTemperature", true, "190");
	if(y)	RaftInterfaceTemperature = y->GetValueFloat();

	// GCode parameters

	char buffer[10000];
	memset(buffer,0,10000);
	x->FindVariableZ("GCodeStartText", true, "; GCode generated by RepSnapper by Kulitorum\nG21                              ;metric is good!\nG90                              ;absolute positioning\nT0                                 ;select new extruder\nG28                               ;go home\nG92 E0                          ;set extruder home\nM104 S73.0                   ;set temperature\nG1 X20 Y20 F500            ;Move away from 0.0, so we use the same reset (in the layer code) for each layer\n\n")->GetValue(buffer);
	GCodeStartText = string(buffer);
	memset(buffer,0,10000);
	x->FindVariableZ("GCodeLayerText", true, "M106                            ;cooler on \n		G1 F2000;\n	G1 X-250 E0 F2000.0       ;horizontal move\n		G1 X-249.9                  ;horizontal move\n		G1 X-250.0 E0 F50.0        ;horizontal move\n		G92 X0                         ;set x 0\n		G1 F2000;\n	G1 Y-250 E0 F2000.0       ;horizontal move\n		G1 Y-249.9  E0 F200          ;horizontal move\n		G1 Y-250.0 F50.0        ;horizontal move\n		G92 Y0                         ;set y 0\n		G1 F500;\n	G1 X20 E20 F500       ;Shield\n		G1 X0 E20 F500         ;Shield\n		T0                                 ;select new extruder\n		G92 E0                         ;zero the extruded length\n")->GetValue(buffer);
	GCodeLayerText = string(buffer);
	memset(buffer,0,10000);
	x->FindVariableZ("GCodeEndText", true, "M107                            ;cooler off\nG1 X0 Y0 E0 F2000.0       ;feed for start of next move\nM104 S0.0                    ;Heater off\n")->GetValue(buffer);
	GCodeEndText = string(buffer);

	y=x->FindVariableZ("GCodeDrawStart", true, "0");
	if(y)	GCodeDrawStart = y->GetValueFloat();
	y=x->FindVariableZ("GCodeDrawEnd", true, "1");
	if(y)	GCodeDrawEnd = y->GetValueFloat();
	y=x->FindVariableZ("MinPrintSpeedXY", true, "400");
	if(y)	MinPrintSpeedXY = y->GetValueFloat();
	y=x->FindVariableZ("MaxPrintSpeedXY", true, "1500");
	if(y)	MaxPrintSpeedXY = y->GetValueFloat();
	y=x->FindVariableZ("MinPrintSpeedZ", true, "50");
	if(y)	MinPrintSpeedZ = y->GetValueFloat();
	y=x->FindVariableZ("MaxPrintSpeedZ", true, "150");
	if(y)	MaxPrintSpeedZ = y->GetValueFloat();

	y=x->FindVariableZ("accelerationSteps", true, "2");
	if(y)	accelerationSteps = y->GetValueInt();
	y=x->FindVariableZ("distanceBetweenSpeedSteps", true, "3");
	if(y)	distanceBetweenSpeedSteps = y->GetValueFloat();
	y=x->FindVariableZ("UseFirmwareAcceleration", true, "1");
	if(y)	UseFirmwareAcceleration = y->GetValueInt();
	y=x->FindVariableZ("extrusionFactor", true, "1");
	if(y)	extrusionFactor = y->GetValueFloat();

	// Printer parameters
	y=x->FindVariableZ("m_fVolume.x", true, "200");
	if(y)	m_fVolume.x = y->GetValueFloat();
	y=x->FindVariableZ("m_fVolume.y", true, "200");
	if(y)	m_fVolume.y = y->GetValueFloat();
	y=x->FindVariableZ("m_fVolume.z", true, "140");
	if(y)	m_fVolume.z = y->GetValueFloat();
	y=x->FindVariableZ("PrintMargin.x", true, "10");
	if(y)	PrintMargin.x = y->GetValueFloat();
	y=x->FindVariableZ("PrintMargin.y", true, "10");
	if(y)	PrintMargin.y = y->GetValueFloat();
	y=x->FindVariableZ("ExtrudedMaterialWidth", true, "0.7");
	if(y)	ExtrudedMaterialWidth = y->GetValueFloat();


	// STL parameters
	y=x->FindVariableZ("LayerThickness", true, "0.4");
	if(y)	LayerThickness = y->GetValueFloat();
	y=x->FindVariableZ("CuttingPlaneValue", true, "0.5");
	if(y)	CuttingPlaneValue = y->GetValueFloat();

	// CuttingPlane
	y=x->FindVariableZ("InfillDistance", true, "2");
	if(y)	InfillDistance = y->GetValueFloat();
	y=x->FindVariableZ("InfillRotation", true, "90");
	if(y)	InfillRotation = y->GetValueFloat();
	y=x->FindVariableZ("InfillRotationPrLayer", true, "90");
	if(y)	InfillRotationPrLayer = y->GetValueFloat();
	y=x->FindVariableZ("Optimization", true, "0.05");
	if(y)	Optimization = y->GetValueFloat();
	y=x->FindVariableZ("ShellOnly", true, "0");
	if(y)	ShellOnly = y->GetValueFloat();
	y=x->FindVariableZ("ShellCount", true, "1");
	if(y)	ShellCount = y->GetValueFloat();
	y=x->FindVariableZ("EnableAcceleration", true, "1");
	if(y)	EnableAcceleration = (bool)y->GetValueInt();
	y=x->FindVariableZ("UseIncrementalEcode", true, "0");
	if(y)	UseIncrementalEcode= (bool)y->GetValueInt();

	// GUI... ?
	y=x->FindVariableZ("DisplayEndpoints", true, "0");
	if(y)	DisplayEndpoints = (bool)y->GetValueInt();
	y=x->FindVariableZ("DisplayNormals", true, "0");
	if(y)	DisplayNormals = (bool)y->GetValueInt();
	y=x->FindVariableZ("DisplayWireframe", true, "0");
	if(y)	DisplayWireframe = (bool)y->GetValueInt();
	y=x->FindVariableZ("DisplayWireframeShaded", true, "1");
	if(y)	DisplayWireframeShaded = (bool)y->GetValueInt();
	y=x->FindVariableZ("DisplayPolygons", true, "1");
	if(y)	DisplayPolygons = (bool)y->GetValueInt();
	y=x->FindVariableZ("DisplayAllLayers", true, "0");
	if(y)	DisplayAllLayers = (bool)y->GetValueInt();
	y=x->FindVariableZ("DisplayinFill", true, "1");
	if(y)	DisplayinFill = (bool)y->GetValueInt();
	y=x->FindVariableZ("DisplayDebuginFill", true, "0");
	if(y)	DisplayDebuginFill = (bool)y->GetValueInt();
	y=x->FindVariableZ("DisplayDebug", true, "0");
	if(y)	DisplayDebug = (bool)y->GetValueInt();
	y=x->FindVariableZ("DisplayCuttingPlane", true, "0");
	if(y)	DisplayCuttingPlane =(bool)y->GetValueInt();
	y=x->FindVariableZ("DrawVertexNumbers", true, "0");
	if(y)	DrawVertexNumbers = (bool)y->GetValueInt();
	y=x->FindVariableZ("DrawLineNumbers", true, "0");
	if(y)	DrawLineNumbers = (bool)y->GetValueInt();

	y=x->FindVariableZ("PolygonVal", true, "0.5");
	if(y)	PolygonVal = y->GetValueFloat();
	y=x->FindVariableZ("PolygonSat", true, "1");
	if(y)	PolygonSat = y->GetValueFloat();
	y=x->FindVariableZ("PolygonHue", true, "0.54");
	if(y)	PolygonHue = y->GetValueFloat();
	y=x->FindVariableZ("WireframeVal", true, "1");
	if(y)	WireframeVal = y->GetValueFloat();
	y=x->FindVariableZ("WireframeSat", true, "1");
	if(y)	WireframeSat = y->GetValueFloat();
	y=x->FindVariableZ("WireframeHue", true, "0.08");
	if(y)	WireframeHue = y->GetValueFloat();
	y=x->FindVariableZ("NormalsSat", true, "1");
	if(y)	NormalsSat = y->GetValueFloat();
	y=x->FindVariableZ("NormalsVal", true, "1");
	if(y)	NormalsVal = y->GetValueFloat();
	y=x->FindVariableZ("NormalsHue", true, "0.23");
	if(y)	NormalsHue = y->GetValueFloat();
	y=x->FindVariableZ("EndpointsSat", true, "1");
	if(y)	EndpointsSat = y->GetValueFloat();
	y=x->FindVariableZ("EndpointsVal", true, "1");
	if(y)	EndpointsVal = y->GetValueFloat();
	y=x->FindVariableZ("EndpointsHue", true, "0.45");
	if(y)	EndpointsHue = y->GetValueFloat();
	y=x->FindVariableZ("GCodeExtrudeHue", true, "0.18");
	if(y)	GCodeExtrudeHue = y->GetValueFloat();
	y=x->FindVariableZ("GCodeExtrudeSat", true, "1");
	if(y)	GCodeExtrudeSat = y->GetValueFloat();
	y=x->FindVariableZ("GCodeExtrudeVal", true, "1");
	if(y)	GCodeExtrudeVal = y->GetValueFloat();
	y=x->FindVariableZ("GCodeMoveHue", true, "1");
	if(y)	GCodeMoveHue = y->GetValueFloat();
	y=x->FindVariableZ("GCodeMoveSat", true, "0.95");
	if(y)	GCodeMoveSat = y->GetValueFloat();
	y=x->FindVariableZ("GCodeMoveVal", true, "1");
	if(y)	GCodeMoveVal = y->GetValueFloat();
	y=x->FindVariableZ("Highlight", true, "0.7");
	if(y)	Highlight = y->GetValueFloat();
	y=x->FindVariableZ("NormalsLength", true, "10");
	if(y)	NormalsLength = y->GetValueFloat();
	y=x->FindVariableZ("EndPointSize", true, "8");
	if(y)	EndPointSize = y->GetValueFloat();

	y=x->FindVariableZ("DisplayGCode", true, "1");
	if(y)	DisplayGCode = (bool)y->GetValueInt();
	y=x->FindVariableZ("LuminanceShowsSpeed", true, "1");
	if(y)	LuminanceShowsSpeed = (bool)y->GetValueInt();

	y=x->FindVariableZ("RaftEnable", true, "1");
	if(y)	RaftEnable = (bool)y->GetValueInt();
	y=x->FindVariableZ("ApronEnable", true, "0");
	if(y)	ApronEnable = (bool)y->GetValueInt();
	y=x->FindVariableZ("ApronPreview", true, "1");
	if(y)	ApronPreview = (bool)y->GetValueInt();
	y=x->FindVariableZ("ApronSize", true, "1.33");
	if(y)	ApronSize = (bool)y->GetValueFloat();
	y=x->FindVariableZ("ApronHeight", true, "7");
	if(y)	ApronHeight = (bool)y->GetValueFloat();
	y=x->FindVariableZ("ApronCoverageX", true, "60");
	if(y)	ApronCoverageX = (bool)y->GetValueFloat();
	y=x->FindVariableZ("ApronCoverageY", true, "60");
	if(y)	ApronCoverageY = (bool)y->GetValueFloat();
	y=x->FindVariableZ("ApronDistanceToObject", true, "0.5");
	if(y)	ApronDistanceToObject = (bool)y->GetValueFloat();
	y=x->FindVariableZ("ApronInfillDistance", true, "2");
	if(y)	ApronInfillDistance = (bool)y->GetValueFloat();

	/*
	ImageProcessingSettings();
	UserCurve[0] = UserCurve[1] = 0.0f;
	UserCurve[2] = UserCurve[3] = 0.250f;
	UserCurve[4] = UserCurve[5] = 0.50f;
	UserCurve[6] = UserCurve[7] = 0.750f;
	UserCurve[8] = UserCurve[9] = 1.0f;
	XMLElement *x = e->FindElementZ("RED_ProcessingSettings", true);


	Detail = R3DSDK::ImageDetail::ImageDetailHigh;
	OLPFCompensation = R3DSDK::ImageOLPFCompensation::ImageOLPFCompHigh;
	Denoise = R3DSDK::ImageDenoise::ImageDenoiseMaximum;

	XMLVariable* y;

	char buffer[100];
	memset(buffer,0,100);
	x->FindVariableZ("GammaCurve", true, "None")->GetValue(buffer);
	string GammaC(buffer);
	for(UINT i=0;i<ImageProcessingLimits::GammaCurveCount;i++)
	{
		string ThisCurveName(ImageProcessingLimits::GammaCurveLabels[i]);
		if(GammaC == ThisCurveName)
			GammaCurve = ImageProcessingLimits::GammaCurveMap[i];
	}

	memset(buffer,0,100);
	x->FindVariableZ("ColorSpace", true, "None")->GetValue(buffer);
	string ColSp(buffer);
	for(UINT i=0;i<ImageProcessingLimits::ColorSpaceCount;i++)
	{
		string ThisCurveName(ImageProcessingLimits::ColorSpaceLabels[i]);
		if(ColSp == ThisCurveName)
			ColorSpace = ImageProcessingLimits::ColorSpaceMap[i];
	}

	y=x->FindVariableZ("ISO", false, "320");
	if(y)
		ISO				= y->GetValueInt();	
	y=x->FindVariableZ("Kelvin", false, "3200");
	if(y)
		Kelvin				= y->GetValueFloat();	
	y = x->FindVariableZ("Tint", false, "0");
	if(y)
		Tint					= y->GetValueFloat();	
	y = x->FindVariableZ("ExposureCompensation", false, "0");
	if(y)
		ExposureCompensation	=y->GetValueFloat();	
	y = x->FindVariableZ("GainRed", false, "1");
	if(y)
		GainRed				= y->GetValueFloat();	
	y = x->FindVariableZ("GainGreen", false, "1");
	if(y)
		GainGreen			= y->GetValueFloat();	
	y = x->FindVariableZ("GainBlue", false, "1");
	if(y)
		GainBlue			= y->GetValueFloat();	
	y = x->FindVariableZ("Saturation", false, "1");
	if(y)
		Saturation			= y->GetValueFloat();	
	y = x->FindVariableZ("Contrast", false, "1");
	if(y)
		Contrast			= y->GetValueFloat();	
	y = x->FindVariableZ("Brightness", false, "1");
	if(y)
		Brightness			= y->GetValueFloat();	
	y = x->FindVariableZ("DRX", false, "0");
	if(y)
		DRX					= y->GetValueFloat();	

	y = x->FindVariableZ("CustomPDLogGamma", false, "0");
	if(y)
		CustomPDLogGamma			= y->GetValueFloat();	

	y = x->FindVariableZ("UserCurve0", false, "0.00");
	if(y)
		UserCurve[0]  = y->GetValueFloat();	
	y = x->FindVariableZ("UserCurve1", false, "0.00");
	if(y)
		UserCurve[1]  = y->GetValueFloat();	
	y = x->FindVariableZ("UserCurve2", false, "0.25");
	if(y)
		UserCurve[2]  = y->GetValueFloat();	
	y = x->FindVariableZ("UserCurve3", false, "0.25");
	if(y)
		UserCurve[3]  = y->GetValueFloat();	
	y = x->FindVariableZ("UserCurve4", false, "0.50");
	if(y)
		UserCurve[4]  = y->GetValueFloat();	
	y = x->FindVariableZ("UserCurve5", false, "0.50");
	if(y)
		UserCurve[5]  = y->GetValueFloat();	
	y = x->FindVariableZ("UserCurve6", false, "0.75");
	if(y)
		UserCurve[6]  = y->GetValueFloat();	
	y = x->FindVariableZ("UserCurve7", false, "0.75");
	if(y)
		UserCurve[7]  = y->GetValueFloat();	
	y = x->FindVariableZ("UserCurve8", false, "1.00");
	if(y)
		UserCurve[8]  = y->GetValueFloat();	
	y = x->FindVariableZ("UserCurve9", false, "1.00");
	if(y)
		UserCurve[9]  = y->GetValueFloat();	*/
}

void ProcessController::LoadXML()
{
	string filename = m_Filename+".xml";
	XML* xml = new XML(filename.c_str()); 
	XMLElement* e = xml->GetRootElement();
	LoadXML(e);
}
void ProcessController::SaveXML()
{
	string filename = m_Filename+".xml";

	XML* xml = new XML(filename.c_str()); 
	XMLElement* e = xml->GetRootElement();
	SaveXML(e);
	if (xml->IntegrityTest())
		xml->Save(); // Saves back to file
	delete xml;
}
