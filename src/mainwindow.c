#include "definitions.h"
#include "mainwindow.h"
#include "windowbuilder.h"
enum
  {
    TASK_ID= 0,
    TASK_DESCRIPTION,
    NUM_COLS
  };
static GObject* task_window;

static void on_task_added(GObject* obj,gpointer data){
    assert(data!=NULL);
    char* task=(char*)data;
    assert(task!=NULL);
    g_print("size %d\n",sizeof(data));
    g_print("We are here %s\n",task);
}
GObject* todo_mainwindow_init(){
    GObject* mainwindow=NULL;
    GObject* tasks_treeview;

    GtkBuilder* builder=gtk_builder_new();
    mainwindow=wb_build_from_file(&builder,"../ui/mainwindow.ui","main_window");
    assert(mainwindow!=NULL);
    gtk_builder_connect_signals(builder,NULL);
    tasks_treeview=gtk_builder_get_object(builder,"tasks_trv");
    init_treeview(&tasks_treeview);
    task_window=todo_taskwindow_init();
    assert(task_window!=NULL);
    TodoTask* task=g_malloc(sizeof(TodoTask));
    task->id="taskID";
    task->description="Walk dog";
    char* x="this is a test";
    char* str=g_malloc(strlen(x)+1);
    strcpy(str,x);
   g_print("The size %d\n",sizeof(x));
    g_signal_connect(task_window,"task-added",G_CALLBACK(on_task_added),str);
    
    return mainwindow;
}

void init_treeview(GObject** treeview){
    assert(*treeview!=NULL);
    GtkTreeIter iter;
    GtkTreeViewColumn* task_description_col;
    GtkListStore* list_store;
    /*GtkTreeViewColumn* task_id_col;*/

    GtkCellRenderer* renderer=gtk_cell_renderer_text_new();

    // Create columns for the tree view
    /*task_id_col=gtk_tree_view_column_new_with_attributes("ID",renderer,"text",TASK_ID,NULL);*/
    task_description_col=gtk_tree_view_column_new_with_attributes("TASK",renderer,"text",TASK_DESCRIPTION,NULL);

    // Append the columns to the tree view
    /*gtk_tree_view_append_column(GTK_TREE_VIEW(*treeview),task_id_col);*/
    gtk_tree_view_append_column(GTK_TREE_VIEW(*treeview),task_description_col);

    // Create a list store (tree model) for the tree view
    list_store=gtk_list_store_new(NUM_COLS,G_TYPE_STRING,G_TYPE_STRING);
    assert(list_store!=NULL);

    // Add the list store(tree model) to the tree view.
    gtk_tree_view_set_model(GTK_TREE_VIEW(*treeview),GTK_TREE_MODEL(list_store));

    //TODO remove this later...
    // Append new row to the list store
    gtk_list_store_append(list_store,&iter);
    // Add data to the newly appended row.
    gtk_list_store_set(list_store,&iter,TASK_ID,"1",TASK_DESCRIPTION,"Walk the dog",-1);

}

void todo_mainwindow_show(GObject** mainwindow){
    assert(*mainwindow!=NULL);
    gtk_widget_show_all(GTK_WIDGET(*mainwindow));
}

void todo_on_add_btn_clicked(){
    //TODO not yet implemented
    // if task window hasn't been created, create it.

    todo_taskwindow_display(&task_window);
    
}

void todo_on_edit_btn_clicked(){
    //TODO not yet implemented
    g_print("btn_edit_clicked");
}

void todo_on_delete_btn_clicked(){
    //TODO not yet implemented
    g_print("btn_delete_clicked");
}
