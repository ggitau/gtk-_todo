#ifndef __WINDOW_BUILDER_H_
#define __WINDOW_BUILDER_H_
#include<gtk/gtk.h>
#include<assert.h>
GObject* wb_build_from_file(GtkBuilder** builder,const gchar* file_path,const gchar* window_name);
#endif
