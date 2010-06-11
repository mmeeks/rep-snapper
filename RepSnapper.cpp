/* -------------------------------------------------------- *
*
* RepSnapper.cpp
*
* Copyright 2009+ Michael Holm - www.kulitorum.com
*
* This file is part of RepSnapper and is made available under
* the terms of the GNU General Public License, version 2, or at your
* option, any later version, incorporated herein by reference.
*
* ------------------------------------------------------------------------- */



/*************************************************************************

	TODO LIST:

Joaz>	btw. Kulitorum a dialog " are you sure to restart" would be nice :)

Connect to printer crashes if settings are not first saved, even if com port is right, per Nightwork_zZz 

[08:42]	<Tonok>	RepSnapper M105 doesn't expect "ok"
[08:42]	<Kulitorum>	Hmmm, that's true
[08:42]	<Tonok>	firmware responds anyway, and RepSnapper sends the next two lines instead of one
[08:43]	<Kulitorum>	I guess that might cause the error?
[08:43]	<Tonok>	yes
[08:43]	<Kulitorum>	I should make resnapper expect a "ok" on M105's then
[08:43]	<Tonok>	but we can turn off M105 now :)


*************************************************************************/

#include "stdafx.h"

#include "ModelViewController.h"

#include "gcode.h"
#include "UI.h"

GUI *gui;


#include <boost/thread.hpp>

using namespace std;

int main(int argc, char **argv)
{
	//initialize threading support in FLTK
	Fl::lock();
	gui = new GUI();
	MVC = gui->MVC;

	Fl::visual(FL_DOUBLE|FL_INDEX);

	char WindowTitle[100] = "GCodeView";
	char* W = &WindowTitle[0];
	MVC->gui = gui;
	MVC->serial.setGUI(gui);
	MVC->ProcessControl.gui = gui;
	

	/* All command-line arguments are file-names */
	if (argc > 1) {
	  for (int i = 1; i < argc; i++)
	    MVC->ReadStl(argv[i]);
	}
  
	
	gui->show(1,&W);

	return Fl::run();
}

