#include "taskwindow.h"
#include "windowbuilder.h"
static gboolean on_task_window_delete(GtkWidget *widget,
         GdkEvent  *event,
         gpointer   data){
    gtk_widget_hide(widget);
    return TRUE;
}

static void connect_signals(GObject* window){
    assert(window!=NULL);
    g_signal_connect(GTK_WIDGET(window),"delete-event",G_CALLBACK(on_task_window_delete),NULL);
}

GObject* todo_taskwindow_init(){
    GtkBuilder* builder=gtk_builder_new();
    GObject* task_window=wb_build_from_file(&builder,(gchar*)"../ui/taskwindow.ui",(gchar*)"task_window");
    connect_signals(task_window);

    return task_window;
}

void todo_taskwindow_display(GObject** window){
    assert(window!=NULL);
    assert(*window!=NULL);
    gtk_widget_show_all(GTK_WIDGET(*window));
}
