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


// Construct a model and a view, and link them together.


class ModelViewController : public Fl_Gl_Window
{
public:
	ModelViewController(int x,int y,int w,int h,const char *l);

	// My own view functions
	void draw();
	void CenterView();
	int  handle(int);
	
	// Callback functions
	void resize (int x,int y, int width, int height);		// Reshape The Window When It's Moved Or Resized
	void ReadStl(string filename) {stl.Read(filename);CalcBoundingBoxAndZoom();};
	void ReadGCode(string filename) {gcode.Read(filename);};
	void RotateObject(float x, float y, float z, float a) {stl.RotateObject(Vector3f(x,y,z),a);}
	void CalcBoundingBoxAndZoom();
	void OptimizeRotation() { stl.OptimizeRotation();}

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
	/*--------------Models-------------------*/

	STL stl;
	GCode gcode;
};