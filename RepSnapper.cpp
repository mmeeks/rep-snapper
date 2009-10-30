#include "stdafx.h"

#include <windows.h>													// Header File For The Windows Library
#include <gl/gl.h>														// Header File For The OpenGL32 Library
#include <gl/glu.h>														// Header File For The GLu32 Library

#include "gcode.h"
#include "UI.h"

CubeViewUI *cvui;

int _tmain(int argc, _TCHAR* argv[])
{
	cvui=new CubeViewUI;

	Fl::visual(FL_DOUBLE|FL_INDEX);

	char WindowTitle[100] = "GCodeView";
	char* W = &WindowTitle[0];
	cvui->show(1,&W);
	//	cvui->code->Read("c:/ViewMe.gcode");

	return Fl::run();
}

