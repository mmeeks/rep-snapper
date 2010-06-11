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

struct CommandLineOptions
{
public:
	bool use_gui;
	string stl_input_path;
	string gcode_output_path;
	string settings_path;
	std::vector<std::string> files;
private:
	void init ()
	{
		// specify defaults here or in the block below
		use_gui = true;
	}
	void usage ()
	{
		fprintf (stderr, "Usage: repsnapper [OPTION]... [FILE]...\n"
			 "Start reprap control software and load [FILES]\n"
			 "Options:\n"
			 "  -t, --no-gui           act as a head-less renderer\n"
			 "  -i, --input [file]     read file gcode to [file]\n"
			 "  -o, --output [file]    output gcode to [file]\n"
			 "  -s, --settings [file]  read render settings [file]\n"
			 "  -h, --help             show this help\n"
			 "\n"
			 "Report bugs to #repsnapper, irc.freenode.net\n\n");
		exit (1);
	}
public:
	// FIXME: should really use boost::program_options
	CommandLineOptions(int argc, char **argv)
	{
		init ();
		for (int i = 1; i < argc; i++) {
			const char *arg = argv[i];
			bool param = i < argc - 1;

		        /**/ if (param && (!strcmp (arg, "-i") ||
					   !strcmp (arg, "--input")))
				stl_input_path = argv[++i];
			else if (param && (!strcmp (arg, "-o") ||
					   !strcmp (arg, "--output")))
				gcode_output_path = argv[++i];
			else if (param && (!strcmp (arg, "-s") ||
					   !strcmp (arg, "--settings")))
				settings_path = argv[++i];
			else if (!strcmp (arg, "--no-gui"))
				use_gui = false;
			else if (!strcmp (arg, "--help") || !strcmp (arg, "-h") ||
				 !strcmp (arg, "/?"))
				usage();
			else
				files.push_back (std::string (argv[i]));
		}
	}
	// add more options above
};


int main(int argc, char **argv)
{
	//initialize threading support in FLTK
	Fl::lock();

	CommandLineOptions opts (argc, argv);

	gui = new GUI();
	MVC = gui->MVC;

	Fl::visual(FL_DOUBLE|FL_INDEX);

	MVC->gui = gui;
	MVC->serial->setGUI(gui);
	MVC->ProcessControl.gui = gui;
	
	if (!opts.use_gui) {
		if (opts.stl_input_path.size() > 0) {
			MVC->ReadStl(opts.stl_input_path);

			if (opts.settings_path.size() > 0) {
				MVC->ProcessControl.LoadXML(opts.settings_path);
				MVC->CopySettingsToGUI();
			}

			MVC->ConvertToGCode();

			if (opts.gcode_output_path.size() > 0) {
				Fl_Text_Buffer *buffer = gui->GCodeResult->buffer();
				return buffer->savefile(opts.gcode_output_path.c_str());
			}
		}
		return 0;
	}
  
	for (int i = 0; i < opts.files.size(); i++)
		MVC->ReadStl (opts.files[i].c_str());

	char WindowTitle[100] = "GCodeView";
	char* W = &WindowTitle[0];
	gui->show (1, &W);

	return Fl::run ();
}

