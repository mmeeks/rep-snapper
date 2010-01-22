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
#include "stdafx.h"

//#include <windows.h>													// Header File For The Windows Library
#include <GL/gl.h>														// Header File For The OpenGL32 Library
#include <GL/glu.h>														// Header File For The GLu32 Library

#include "ModelViewController.h"

#include "gcode.h"
#include "UI.h"

GUI *gui;


#include <boost/thread.hpp>

using namespace std;


void received(const char *data, unsigned int len)
{
	vector<char> v(data,data+len);
	for(unsigned int i=0;i<v.size();i++)
	{
		if(v[i]=='\n')
		{
			cout<<endl;
		} else {
			if(v[i]<32 || v[i]>=0x7f) cout.put(' ');//Remove non-ascii char
			else cout.put(v[i]);
		}
	}
	cout.flush();//Flush screen buffer
}

int main(int argc, char* argv[])
{
	gui = new GUI();
/*
	try {
		BufferedAsyncSerial serial("COM5", 19200);
//		serial.setCallback(received);
	/*	for(;;)
		{
			if(serial.errorStatus() || serial.isOpen()==false)
			{
				cerr<<"Error: serial port unexpectedly closed"<<endl;
				break;
			}
			char c;
			cin.get(c); //blocking wait for standard input
			if(c==3) //if Ctrl-C
			{
				cin.get(c);
				switch(c)
				{
				case 3:
					serial.write(&c,1);//Ctrl-C + Ctrl-C, send Ctrl-C
					break;
				case 'x': //fall-through
				case 'X':
					goto quit;//Ctrl-C + x, quit
				default:
					serial.write(&c,1);//Ctrl-C + any other char, ignore
				}
			} else serial.write(&c,1);
		}
quit:
		serial.close();*
	} catch (std::exception& e) {
		cerr<<"Exception: "<<e.what()<<endl;
	}
*/
	Fl::visual(FL_DOUBLE|FL_INDEX);

	char WindowTitle[100] = "GCodeView";
	char* W = &WindowTitle[0];
	gui->MVC->gui = gui;
	gui->MVC->serial.setGUI(gui);
	gui->MVC->ProcessControl.gui = gui;
	gui->show(1,&W);
	return Fl::run();
}

