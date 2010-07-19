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
			else if (!strcmp (arg, "-t") || !strcmp (arg, "--no-gui"))
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
	ModelViewController *MVC = gui->MVC;

	Fl::visual(FL_DOUBLE|FL_INDEX);
	
	MVC->Init(gui);
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
  
	for (uint i = 0; i < opts.files.size(); i++)
		MVC->ReadStl (opts.files[i].c_str());

	char WindowTitle[100] = "GCodeView";
	char* W = &WindowTitle[0];
	gui->show (1, &W);

	return Fl::run ();
}

#ifdef WIN32
#ifndef UNITTEST

CHAR wide_to_narrow(WCHAR w)
{
    // simple typecast
    // works because UNICODE incorporates ASCII into itself
    return CHAR(w);
}
class CmdLineArgs : public std::vector<char*>
{
public:
    CmdLineArgs (LPWSTR args)
    {

        // Save local copy of the command line string, because
        // ParseCmdLine() modifies this string while parsing it.
        m_cmdline = new char [wcslen (args) + 1];
        if (m_cmdline)
        {
			std::transform(args, args+wcslen (args) + 1, m_cmdline, wide_to_narrow);
            ParseCmdLine(); 
        }
    }
    ~CmdLineArgs()
    {
        delete m_cmdline;
    }

private:
    PSZ m_cmdline; // the command line string

    ////////////////////////////////////////////////////////////////////////////////
    // Parse m_cmdline into individual tokens, which are delimited by spaces. If a
    // token begins with a quote, then that token is terminated by the next quote
    // followed immediately by a space or terminator.  This allows tokens to contain
    // spaces.
    // This input string:     This "is" a ""test"" "of the parsing" alg"o"rithm.
    // Produces these tokens: This, is, a, "test", of the parsing, alg"o"rithm
    ////////////////////////////////////////////////////////////////////////////////
    void ParseCmdLine ()
    {
        enum { TERM  = '\0',
               QUOTE = '\"' };

        bool bInQuotes = false;
        PSZ pargs = m_cmdline;

        while (*pargs)
        {
            while (isspace (*pargs))        // skip leading whitespace
                pargs++;

            bInQuotes = (*pargs == QUOTE);  // see if this token is quoted

            if (bInQuotes)                  // skip leading quote
                pargs++; 

            push_back (pargs);              // store position of current token

            // Find next token.
            // NOTE: Args are normally terminated by whitespace, unless the
            // arg is quoted.  That's why we handle the two cases separately,
            // even though they are very similar.
            if (bInQuotes)
            {
                // find next quote followed by a space or terminator
                while (*pargs && 
                      !(*pargs == QUOTE && (isspace (pargs[1]) || pargs[1] == TERM)))
                    pargs++;
                if (*pargs)
                {
                    *pargs = TERM;  // terminate token
                    if (pargs[1])   // if quoted token not followed by a terminator
                        pargs += 2; // advance to next token
                }
            }
            else
            {
                // skip to next non-whitespace character
                while (*pargs && !isspace (*pargs)) 
                    pargs++;
                if (*pargs && isspace (*pargs)) // end of token
                {
                   *pargs = TERM;    // terminate token
                    pargs++;         // advance to next token or terminator
                }
            }
        } // while (*pargs)
    } // ParseCmdLine()
}; // class CmdLineArgs


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	CmdLineArgs cmdArgs(lpCmdLine);
	char** arg = (char**)malloc(cmdArgs.size()*sizeof(char*));
	for(int i=0; i<cmdArgs.size(); i++)
	{
		arg[i] = cmdArgs[i];
	}	 

	return main(cmdArgs.size(), arg);
}
#endif // UNITTEST
#endif // WIN32
