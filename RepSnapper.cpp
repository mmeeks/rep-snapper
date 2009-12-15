#include "stdafx.h"

#include <windows.h>													// Header File For The Windows Library
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library

#include "ModelViewController.h"

#include "gcode.h"
#include "UI.h"

GUI *gui;

int _tmain(int argc, _TCHAR* argv[])
{
	gui = new GUI();

	Fl::visual(FL_DOUBLE|FL_INDEX);

	char WindowTitle[100] = "GCodeView";
	char* W = &WindowTitle[0];
	gui->MVC->gui = gui;
	gui->show(1,&W);
	//	gui->code->Read("c:/ViewMe.gcode");

	return Fl::run();
}

