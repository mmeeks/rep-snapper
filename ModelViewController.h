#pragma once

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

	// STL Functions
	void ReadStl(string filename) {if(ProcessControl.stl.Read(filename, ProcessControl.printer.PrintMargin)) CalcBoundingBoxAndZoom();};
	void OptimizeRotation() { ProcessControl.stl.OptimizeRotation();}
	void RotateObject(float x, float y, float z, float a) {ProcessControl.stl.RotateObject(Vector3f(x,y,z),a);}

	// GCode Functions
	void ReadGCode(string filename) {ProcessControl.gcode.Read(filename);};


	// My own view functions
	void draw();
	void DrawGridAndAxis();

	void CenterView();
	int  handle(int);
	
	// Callback functions
	void resize (int x,int y, int width, int height);		// Reshape The Window When It's Moved Or Resized

	void CalcBoundingBoxAndZoom();

	void SetDisplaySTL(bool value){}
	void SetDisplayGCode(bool value){}

	// GCode GUI Values
	void SetGCodeDrawStart(float val){ProcessControl.gcode.GCodeDrawStart = val; redraw();}
	void SetGCodeDrawEnd(float val){ProcessControl.gcode.GCodeDrawEnd = val; redraw();}

	// STL GUI Values
	void SetLayerThickness(float val){ProcessControl.stl.LayerThickness = val; redraw();}
	void SetCuttingPlaneValue(float val){ProcessControl.stl.CuttingPlaneValue = val; redraw();}

	void SetDisplayEndpoints(bool val){ProcessControl.stl.DisplayEndpoints = val; redraw();}
	void SetDisplayNormals(bool val){ProcessControl.stl.DisplayNormals = val; redraw();}
	void SetDisplayWireframe(bool val){ProcessControl.stl.DisplayWireframe = val; redraw();}
	void SetDisplayPolygons(bool val){ProcessControl.stl.DisplayPolygons = val; redraw();}
	void SetDisplayAllLayers(bool val){ProcessControl.stl.DisplayAllLayers = val; redraw();}
	void SetDisplayinFill(bool val){ProcessControl.stl.DisplayinFill = val; redraw();}
	void SetPolygonOpasity(float val){ProcessControl.stl.PolygonOpasity = val; redraw();}

	// CuttingPlane GUI values
	void SetInfillDistance(float val){}
	void SetInfillRotation(float val){}
	void SetInfillRotationPrLayer(float val){}
	void SetOptimization(float val){}
	void SetExamine(float val){}
	void SetShrinkValue(float val){}

	void SetDisplayDebuginFill(bool val){}
	void SetDisplayDebug(bool val){}
	void SetDisplayCuttingPlane(bool val){}
	void SetDrawVertexNumbers(bool val){}
	void SetDrawLineNumbers(bool val){}

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