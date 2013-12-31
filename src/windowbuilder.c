#include "windowbuilder.h"

GObject* wb_build_from_file(GtkBuilder** builder,const gchar* file_path,const gchar* window_name){
    assert(builder!=NULL);
    assert(*builder!=NULL);
    assert(file_path!=NULL);
    assert(window_name!=NULL);

    GObject* window=NULL;
    gtk_builder_add_from_file(*builder,file_path,NULL);
    window=gtk_builder_get_object(*builder,window_name);

    if(window==NULL){
       g_print("Could not retrieve window witn name %s from path %s\n",window_name,file_path);
    }

    return window;
}
