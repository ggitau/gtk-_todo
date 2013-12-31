#include "taskwindow.h"
#include "windowbuilder.h"
#include "definitions.h"

static GObject* text_view=NULL;
static GObject* task_window=NULL;
static GObject* save_btn=NULL;
static GObject* cancel_btn=NULL;

static void clear_text_buffer(){
    GtkTextBuffer* text_buffer= gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_set_text(text_buffer,"",g_utf8_strlen("",-1));
}

static TodoTask* get_task(){
    assert(text_view!=NULL);

    GtkTextBuffer* text_buffer= gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));

    GtkTextIter start_iter;
    GtkTextIter end_iter;

    gtk_text_buffer_get_start_iter(text_buffer,&start_iter);
    gtk_text_buffer_get_end_iter(text_buffer,&end_iter);

    gchar* text=gtk_text_buffer_get_text(text_buffer,&start_iter,&end_iter,TRUE);

    TodoTask* task=g_malloc(sizeof(TodoTask));
    assert(task!=NULL);

    // Get number of microseconds since 1 Jan 1970
    gint64 time_stamp=g_get_real_time();
    gchar* id=g_malloc(21);
    sprintf(id,"TASK_%ld",time_stamp);
    task->id=id;
    task->description=text;

    return task;
}

static gboolean on_task_window_delete(GtkWidget *widget,
        GdkEvent  *event,
        gpointer   data){
    gtk_widget_hide(widget);
    clear_text_buffer();

    return TRUE;
}
static void on_cancel_btn_clicked(){
    gtk_widget_hide(GTK_WIDGET(task_window));
    clear_text_buffer();
}
static void on_save_btn_clicked(){
    TodoTask* task=get_task();
    assert(task!=NULL);
    g_signal_emit_by_name(task_window,"task-added",task);
    gtk_widget_hide(GTK_WIDGET(task_window));
    clear_text_buffer();
}

static void connect_signals(GObject* window){
    assert(window!=NULL);

    g_signal_connect(GTK_WIDGET(window),"delete-event",G_CALLBACK(on_task_window_delete),NULL);
    g_signal_connect(GTK_WIDGET(save_btn),"clicked",G_CALLBACK(on_save_btn_clicked),NULL);
    g_signal_connect(GTK_WIDGET(cancel_btn),"clicked",G_CALLBACK(on_cancel_btn_clicked),NULL);
}



GObject* todo_taskwindow_init(){
    GtkBuilder* builder=gtk_builder_new();

    task_window=wb_build_from_file(&builder,(gchar*)"../ui/taskwindow.ui",(gchar*)"task_window");
    assert(task_window!=NULL);

    text_view=gtk_builder_get_object(builder,"main_txtv");
    assert(text_view!=NULL);

    save_btn=gtk_builder_get_object(builder,"save_btn");
    assert(save_btn!=NULL);

    cancel_btn=gtk_builder_get_object(builder,"cancel_btn");
    assert(cancel_btn!=NULL);

    gtk_builder_connect_signals(builder,NULL);
    connect_signals(task_window);

    g_signal_new("task-added",G_TYPE_OBJECT,G_SIGNAL_RUN_FIRST,0,NULL,NULL,g_cclosure_marshal_VOID__POINTER,G_TYPE_NONE,1,G_TYPE_POINTER);

    return task_window;
}

void todo_taskwindow_display(GObject** window){
    assert(window!=NULL);
    assert(*window!=NULL);
    gtk_widget_show_all(GTK_WIDGET(*window));
}
