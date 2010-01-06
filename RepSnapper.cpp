/* -------------------------------------------------------- *
*
* RepSnapper.cpp
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.

	todo:
	Add variables to start, layer and end gcode
	Avoid voids when moving - pathfinding
	Make scoring

*
* ------------------------------------------------------------------------- */
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
	gui->MVC->serial.setGUI(gui);
	gui->MVC->ProcessControl.gui = gui;
	gui->show(1,&W);
	return Fl::run();
}

