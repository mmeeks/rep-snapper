/*
 * Simple file & file-chooser abstraction to allow
 * native gtk+ file selection under Unix.
 */
#include "stdafx.h"
#include "File.h"

#ifdef HAVE_GTK
#include <glib/gthread.h>
#include <gtk/gtk.h>
#endif

#include <boost/filesystem/path.hpp>

namespace {

#ifdef HAVE_GTK
  /*
   * This is quite fun. To avoid graunching problems with the gtk+
   * mainloop, we launch our own thread to run the gtk+ main-loop
   * and synchronize threading primitives.
   */
  static int initialized = FALSE;

  struct Args {
    const char  *directory;
    const char  *filter;
    int          type;
    const char  *title;

    bool	 ret;
    std::string  result;
    Args(const char *a, const char *b, int c, const char *d) :
      directory(a), filter(b), type(c), title (d), ret (false) {}
  };

  extern "C" {
    static void enter_fn (void) { Fl::lock (); }
    static void leave_fn (void) { Fl::unlock (); }
    static gpointer main_loop_fn (gpointer)
    {
      Fl::lock();
      gtk_main ();
      Fl::unlock();
      return NULL;
    }
    static gboolean open_gtk (Args *a)
    {
      gboolean multiple = FALSE;
      GtkFileChooserAction action;
      const char *button_text;

      Fl::lock();

      switch (a->type) {
      case Fl_File_Chooser::CREATE:
	button_text = GTK_STOCK_SAVE;
	action = GTK_FILE_CHOOSER_ACTION_SAVE;
	break;
      case Fl_File_Chooser::DIRECTORY:
	action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
	button_text = GTK_STOCK_OPEN;
	break;
      case Fl_File_Chooser::MULTI:
	multiple = TRUE;
	// fall through
      case Fl_File_Chooser::SINGLE:
      default:
	action = GTK_FILE_CHOOSER_ACTION_OPEN;
	button_text = GTK_STOCK_OPEN;
	break;
      }

      GtkWidget *dialog = gtk_file_chooser_dialog_new
	(a->title, NULL /* FIXME transience */, action,
	 GTK_STOCK_CANCEL,
	 GTK_RESPONSE_CANCEL,
	 button_text,
	 GTK_RESPONSE_ACCEPT,
	 NULL);
      GtkFileChooser *chooser = GTK_FILE_CHOOSER (dialog);
      if (a->directory)
	gtk_file_chooser_set_current_folder (chooser, a->directory);
      gtk_file_chooser_set_select_multiple (chooser, multiple);
      GtkFileFilter *filter = gtk_file_filter_new ();
      gtk_file_filter_add_pattern (filter, a->filter);
      gtk_file_chooser_add_filter (chooser, filter);
      if (gtk_dialog_run (GTK_DIALOG (chooser)) == GTK_RESPONSE_ACCEPT) {
	gchar *fname = gtk_file_chooser_get_filename (chooser);
	a->result = std::string (fname ? fname : "");
	g_free (fname);
      }
      gtk_widget_destroy (dialog);
      a->ret = true;

      Fl::unlock();
      Fl::awake ();

      return FALSE;
    }
  }
    
  std::string openGtk (const char *directory, const char *filter, int type, const char *title)
  {
    if (!initialized)
      {
	initialized = TRUE;
	g_thread_init (NULL);
	gtk_init (NULL, NULL);
	gdk_threads_set_lock_functions (enter_fn, leave_fn);
	g_thread_create (main_loop_fn, NULL, FALSE, NULL);
      }
    Args a(directory, filter, type, title);

    g_idle_add ((GSourceFunc)open_gtk, &a);

    while (!a.ret)
      Fl::wait();

    return a.result;
  }
#endif

  std::string openFltk (const char *directory, const char *filter, int type, const char *title)
  {
    Fl_File_Chooser chooser (directory, filter, type, title);

    chooser.show();
    while (chooser.shown())
      Fl::wait();
    const char *v = chooser.value();
    return std::string (v ? v : "");
  }
}

/*
 * Split out code that, otherwise would be busy cluttering
 * UI.fl in multiple places.
 */
void FileChooser::ioDialog (ModelViewController *mvc, Op o, Type t, bool dropRFO)
{
  int type;
  std::string file;
  const char *filter;
  const char *directory;
  const char *title;

  switch (t) {
  case GCODE:
    filter = "*.gcode";
    title = "Choose GCODE filename";
    directory = mvc->ProcessControl.GCodePath.c_str();
    break;
  case RFO:
    filter = "*.xml";
    title = "Choose RFO filename";
    directory = mvc->ProcessControl.RFOPath.c_str();
    break;
  case STL:
  default:
    filter = "*.stl";
    title = "Choose STL filename";
    directory = mvc->ProcessControl.STLPath.c_str();
    break;
  }

  if (!directory || directory[0] == '\0')
    directory = ".";

  if (o == OPEN) /* FIXME: impl. multi-selection cleanly */
    type = Fl_File_Chooser::SINGLE;
  else
    type = Fl_File_Chooser::CREATE;

#ifdef HAVE_GTK
    file = openGtk (directory, filter, type, title);
#else
    file = openFltk (directory, filter, type, title);
#endif

  if (!file.length())
    return;

  /* FIXME: we need to drop that RFO much more often here I suspect */
  if (dropRFO)
    mvc->ClearRFO();

  boost::filesystem::path path(file);
  std::string directory_path = path.branch_path().native_directory_string();

  switch (t) {
  case RFO:
    if (o == OPEN)
      mvc->ReadRFO (file);
    else
      mvc->ProcessControl.rfo.Save(file, mvc->ProcessControl);
    mvc->ProcessControl.RFOPath = directory_path;
    break;
  case GCODE:
    if (o == OPEN)
      mvc->ReadGCode (file);
    else
      mvc->WriteGCode (file);
    mvc->ProcessControl.GCodePath = directory_path;
    break;
  default:
  case STL:
    if (o == OPEN)
      mvc->ReadStl (file);
    else
      fl_alert ("STL saving not yet implemented");
    mvc->ProcessControl.STLPath = directory_path;
    break;
  }
  mvc->redraw();
}
