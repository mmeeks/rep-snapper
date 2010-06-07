/*
 * Simple file & file-chooser abstraction to allow
 * native gtk+ file selection under Unix.
 */
#ifndef FILE_H
#define FILE_H

namespace FileChooser {
  std::string open (const char *directory, const char *filter, int type, const char *title);
};

#endif
