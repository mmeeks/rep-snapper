/* -------------------------------------------------------- *
*
* ModelViewController.h
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */#pragma once

// View related
#include <FL/Fl_Gl_Window.H>
#include <FL/gl.h>
#include "UI.h"
#include <math.h>
#include "ArcBall.h"

// Model related
#include "gcode.h"
#include "stl.h"
#include "ProcessController.h"

// Construct a model and a view, and link them together.

class ModelViewController : public Fl_Gl_Window
{
public:
	ModelViewController(int x,int y,int w,int h,const char *l);
	~ModelViewController();

	// STL Functions
	void ReadStl(string filename) {if(ProcessControl.stl.Read(filename, ProcessControl.PrintMargin)) CalcBoundingBoxAndZoom();};
	void OptimizeRotation() { ProcessControl.stl.OptimizeRotation();}
	void RotateObject(float x, float y, float z, float a) {ProcessControl.stl.RotateObject(Vector3f(x,y,z),a); ProcessControl.stl.MoveIntoPrintingArea(ProcessControl.PrintMargin);}

	// GCode Functions
	void ReadGCode(string filename) {ProcessControl.gcode.Read(filename);}
	void ConvertToGCode();
	void init();
	void SetUseIncrementalEcode(bool val) {ProcessControl.UseIncrementalEcode = val;}

	// My own view functions
	void draw();
	void DrawGridAndAxis();
	void WriteGCode(string filename) {/*ProcessControl.WriteGCode(filename);*/}
	void CopySettingsToGUI();

	void CenterView();
	int  handle(int);
	
	// Callback functions
	void resize (int x,int y, int width, int height);		// Reshape The Window When It's Moved Or Resized

	void CalcBoundingBoxAndZoom();

	void SetDisplaySTL(bool value){}
	void SetDisplayGCode(bool value){}

	// GCode GUI Values
	void SetGCodeDrawStart(float val){ProcessControl.GCodeDrawStart = val; redraw();}
	void SetGCodeDrawEnd(float val){ProcessControl.GCodeDrawEnd = val; redraw();}
	void SetMinPrintSpeedXY(float val) {ProcessControl.MinPrintSpeedXY = val;}
	void SetMaxPrintSpeedXY(float val) {ProcessControl.MaxPrintSpeedXY = val;}
	void SetMinPrintSpeedZ(float val) {ProcessControl.MinPrintSpeedZ = val;}
	void SetMaxPrintSpeedZ(float val) {ProcessControl.MaxPrintSpeedZ = val;}
	void NumAccelerationSteps(float val) {ProcessControl.accelerationSteps = val;}
	void SetDistancePrAccelerationStep(float val) {ProcessControl.distanceBetweenSpeedSteps = val;}
	void SetExtrusionFactor(float val) {ProcessControl.extrusionFactor = val;}
	
	// STL GUI Values
	void SetLayerThickness(float val){ProcessControl.LayerThickness = val; redraw();}
	void SetCuttingPlaneValue(float val){ProcessControl.CuttingPlaneValue = val; redraw();}

	void SetDisplayEndpoints(bool val){ProcessControl.DisplayEndpoints = val; redraw();}
	void SetDisplayNormals(bool val){ProcessControl.DisplayNormals = val; redraw();}
	void SetDisplayWireframe(bool val){ProcessControl.DisplayWireframe = val; redraw();}
	void SetDisplayPolygons(bool val){ProcessControl.DisplayPolygons = val; redraw();}
	void SetDisplayAllLayers(bool val){ProcessControl.DisplayAllLayers = val; redraw();}
	void SetDisplayinFill(bool val){ProcessControl.DisplayinFill = val; redraw();}
	void SetPolygonOpasity(float val){ProcessControl.PolygonOpasity = val; redraw();}

	// CuttingPlane GUI values
	void SetInfillDistance(float val){ProcessControl.InfillDistance = val; redraw();}
	void SetInfillRotation(float val){ProcessControl.InfillRotation = val; redraw();}
	void SetInfillRotationPrLayer(float val){ProcessControl.InfillRotationPrLayer = val; redraw();}
	void SetOptimization(float val){ProcessControl.Optimization = val; redraw();}
	void SetExamine(float val){ProcessControl.Examine = val; redraw();}
	void SetExtrudedMaterialWidth(float val){ProcessControl.ExtrudedMaterialWidth = val; redraw();}

	void SetDisplayDebuginFill(bool val){ProcessControl.DisplayDebuginFill = val; redraw();}
	void SetDisplayDebug(bool val){ProcessControl.DisplayDebug = val; redraw();}
	void SetDisplayCuttingPlane(bool val){ProcessControl.DisplayCuttingPlane = val; redraw();}
	void SetDrawVertexNumbers(bool val){ProcessControl.DrawVertexNumbers = val; redraw();}
	void SetDrawLineNumbers(bool val){ProcessControl.DrawLineNumbers = val; redraw();}

	void SetShellOnly(bool val) {ProcessControl.ShellOnly = val; redraw();}
	void SetShellCount(UINT val) {ProcessControl.ShellCount = val; redraw();}

	void SetEnableAcceleration(bool val) {ProcessControl.EnableAcceleration = val; redraw();}
	
	// Raft GUI values
	void SetRaftSize(float val){ProcessControl.RaftSize=val;}
	void SetRaftBaseLayerCount(int val){ProcessControl.RaftBaseLayerCount=val;}
	void SetRaftMaterialPrDistanceRatio(float val){ProcessControl.RaftMaterialPrDistanceRatio=val;}
	void SetRaftRotation(float val){ProcessControl.RaftRotation=val;}
	void SetRaftBaseDistance(float val){ProcessControl.RaftBaseDistance=val;}
	void SetRaftBaseThickness(float val){ProcessControl.RaftBaseThickness=val;}
	void SetRaftBaseTemperature(float val){ProcessControl.RaftBaseTemperature=val;}
	void SeRaftInterfaceLayerCount(int val){ProcessControl.RaftInterfaceLayerCount=val;}
	void SetRaftInterfaceMaterialPrDistanceRatio(float val){ProcessControl.RaftInterfaceMaterialPrDistanceRatio=val;}
	void SetRaftRotationPrLayer(float val){ProcessControl.RaftRotationPrLayer=val;}
	void SetRaftInterfaceDistance(float val){ProcessControl.RaftInterfaceDistance=val;}
	void SetRaftInterfaceThickness(float val){ProcessControl.RaftInterfaceThickness=val;}
	void SetRaftInterfaceTemperature(float val){ProcessControl.RaftInterfaceTemperature=val;}

	// Apron
	void SetApronEnable(bool val){};
	void SetApronPreview(bool val){};
	void SetApronSize(float val){};
	void SetApronHeight(float val){};
	void SetApronCoverageX(float val){};
	void SetApronCoverageY(float val){};
	void SetApronApronDistanceToObject(float val){};
	void SetApronInfillDistance(float val){};


	/*--------------ArcBall-------------------*/

	GLUquadricObj *quadratic;											// Used For Our Quadric

	Matrix4fT   Transform;
	Matrix3fT   LastRot;
	Matrix3fT   ThisRot;
	ArcBallT    *ArcBall;								                // NEW: ArcBall Instance
	Vector2fT    MousePt;												// NEW: Current Mouse Point
	/*--------------View-------------------*/

	Vector3f Center;
	Vector3f Min;
	Vector3f Max;
	float zoom;
	
	/*--------------User interface (View)-------------------*/
	GUI *gui;
	ProcessController ProcessControl;
};