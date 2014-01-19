/* 
 * Copyright (c) 2013 g.gitau
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal 
 * in the Software without restriction, including without limitation the rights 
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions: 
 * The above copyright notice and this permission notice shall be included in 
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN 
 * THE SOFTWARE.
 *
 */
#include "mainwindow.h"
enum
{
    TASK_ID= 0,
    TASK_DESCRIPTION,
    NUM_COLS
};

static GObject* task_window     = NULL;
static GObject* mainwindow      = NULL;
static GtkListStore* list_store = NULL;
static GData* data_list         = NULL;
static GObject* tasks_treeview  = NULL;
static gchar* selected_task_id  = NULL;


static void
destroy_objects()
{
    g_object_unref (task_window);
    g_object_unref (mainwindow);
    g_object_unref (list_store);
}

static void 
add_task_to_ui(TodoTask* task)
{
        GtkTreeIter iter;
        gtk_list_store_append (list_store, &iter);
        gtk_list_store_set (list_store, &iter, TASK_ID,task->id,
                TASK_DESCRIPTION, task->description,-1);
}

static void 
store_task (GQuark key_id, gpointer data, gpointer user_data)
{
    assert (user_data != NULL);
    assert (data != NULL);

    JsonBuilder* json_builder=(JsonBuilder*)user_data;

    TodoTask* task = (TodoTask*) data;

    json_builder_begin_object (json_builder);

    json_builder_set_member_name (json_builder, "id");
    json_builder_add_string_value (json_builder, task->id);

    json_builder_set_member_name (json_builder, "description");
    json_builder_add_string_value (json_builder, task->description);

    json_builder_end_object (json_builder);
}

static void 
destroy_task(gpointer data)
{
    assert (data != NULL);

    TodoTask* task = (TodoTask*)data;

    g_free (task->id);
    g_free (task->description);
    g_free (task);
}

static void
add_task_to_datalist(TodoTask* task)
{
    assert (task != NULL);

    GQuark quark = g_quark_from_string (task->id);
    g_datalist_id_set_data_full (&data_list, quark, task, destroy_task);
}

static void 
load_tasks()
{
    JsonParser* parser=json_parser_new();

    GError* err = NULL;

    if (!json_parser_load_from_file (parser, "tasks.json", &err))
    {
        g_print ("Error loading tasks:%s\n", err->message);
        g_object_unref (parser);

        return;
    }

    JsonNode* root = json_parser_get_root (parser);
    JsonReader* reader = json_reader_new (root);

    /* Get the number of elements in the tasks json array */
    gint element_num = json_reader_count_elements(reader);

    int i;
    for (i = 0; i < element_num; i++ )
    {
        /* Read the ith element from the json array of tasks */
        json_reader_read_element (reader, i);

        /* From the ith element (task) read the id attribute */
        json_reader_read_member (reader, "id");
        const gchar* id = json_reader_get_string_value (reader);
        json_reader_end_member (reader);

        /* From the ith element (task) read the description attribute */
        json_reader_read_member (reader, "description");
        const gchar* desc = json_reader_get_string_value (reader);
        json_reader_end_member (reader);

        /* Set aside memory for a TodoTask struct */ 
        TodoTask* task=g_malloc (sizeof (TodoTask));

        /* Set aside memory for the id string and copy the string
         * from json into the newly created one.
         */
        glong task_id_size = g_utf8_strlen (id,-1) + 1;
        gchar* task_id = g_malloc (task_id_size);
        g_utf8_strncpy (task_id, id, task_id_size);

        task->id = task_id;

        /*  Allocate memory for the task description string and copy
         *  the description obtained json into the newly created description
         *  string.
         */
        glong task_desc_size = g_utf8_strlen (desc,-1) +  1;
        gchar* task_desc = g_malloc (task_desc_size);
        g_utf8_strncpy (task_desc, desc, task_desc_size);

        task->description = task_desc;

        /* Add the parsed task to the ui and datalist data structure */
        add_task_to_ui(task);
        add_task_to_datalist(task);
        
        json_reader_end_element (reader);
    }

    g_object_unref (reader);
    g_object_unref (parser);
}

/*
 *static void print_all_tasks (GQuark key_id,
 *        gpointer data,
 *        gpointer user_data){
 *    TodoTask* task=(TodoTask*)data;
 *    g_print("--> %s\n",task->id);
 *}
 */
static void 
store_tasks()
{
    JsonBuilder* json_builder = json_builder_new ();
    assert (data_list != NULL);

    json_builder_begin_array (json_builder);
    g_datalist_foreach (&data_list, store_task, json_builder); 
    json_builder_end_array (json_builder);

    JsonGenerator* gen = json_generator_new();
    JsonNode* root = json_builder_get_root(json_builder);
    json_generator_set_root (gen, root);

    GError* err = NULL;

    if (!json_generator_to_file(gen, "tasks.json", &err))
    {
        g_print ("Error:%s\n", err->message);
    }

    g_datalist_clear (&data_list);

    json_node_free (root);
    g_object_unref (gen);
    g_object_unref (json_builder);
    destroy_objects ();
}


static void 
on_task_added(GObject* obj, gpointer data){
    assert (data != NULL);
    TodoTask* task = (TodoTask*) data;
    assert (task != NULL);

    if (selected_task_id == NULL || task->id != selected_task_id)
    {
        add_task_to_ui (task);

        GQuark quark = g_quark_from_string (task->id);
        g_datalist_id_set_data_full (&data_list, quark, task, destroy_task);
    }
    else
    {
        g_print ("Task already exists!\n");
    }
}

static gboolean
on_main_window_delete(GtkWidget *widget, GdkEvent *event, gpointer data){
    store_tasks ();

    return FALSE;
}

static void
connect_signals()
{
    assert (mainwindow != NULL);

    g_signal_connect (GTK_WIDGET (mainwindow), "delete-event",
            G_CALLBACK (on_main_window_delete), NULL);

    g_signal_connect (task_window, "todotask-added",
            G_CALLBACK (on_task_added), NULL);
}

GObject* 
todo_mainwindow_init()
{

    GtkBuilder* builder = gtk_builder_new ();

    mainwindow=wb_build_from_file (&builder, "../ui/mainwindow.ui", "main_window");
    assert (mainwindow != NULL);

    gtk_builder_connect_signals (builder, NULL);

    tasks_treeview = gtk_builder_get_object (builder, "tasks_trv");
    init_treeview (&tasks_treeview);

    task_window = todo_taskwindow_init ();
    assert (task_window != NULL);

    g_datalist_init (&data_list);
    connect_signals ();
    load_tasks ();

    return mainwindow;
}

void
init_treeview(GObject** treeview)
{
    assert (*treeview != NULL);

    GtkTreeViewColumn* task_description_col;
    GtkCellRenderer* renderer = gtk_cell_renderer_text_new ();

    task_description_col = gtk_tree_view_column_new_with_attributes ("TASK", 
            renderer, "text", TASK_DESCRIPTION, NULL);

    gtk_tree_view_append_column (GTK_TREE_VIEW (*treeview),
            task_description_col);

    list_store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);
    assert (list_store != NULL);

    gtk_tree_view_set_model (GTK_TREE_VIEW (*treeview), GTK_TREE_MODEL (list_store));
}

void 
todo_mainwindow_show(GObject** mainwindow)
{
    assert (mainwindow != NULL);
    assert (*mainwindow!=NULL);

    gtk_widget_show_all (GTK_WIDGET(*mainwindow));
}

void 
todo_on_add_btn_clicked()
{
    todo_taskwindow_display (&task_window);
}

void 
todo_on_edit_btn_clicked()
{
    GtkTreeIter iter;
    GtkTreeSelection* selection=gtk_tree_view_get_selection (GTK_TREE_VIEW (tasks_treeview));
    GtkTreeModel* model = GTK_TREE_MODEL(list_store);

    GList* selected = gtk_tree_selection_get_selected_rows (selection,&model);

    GList* first = g_list_first (selected);

    if (first != NULL)
    {
        GtkTreePath* path = (GtkTreePath*)first->data;
        gchar* path_str = gtk_tree_path_to_string (path);

        if (path != NULL)
        {
            gchar* id = NULL;
            gtk_tree_model_get_iter (GTK_TREE_MODEL (list_store), &iter, path);
            gtk_tree_model_get (model, &iter, TASK_ID, &id, -1);

            GQuark quark = g_quark_from_string (id);
            TodoTask* task = (TodoTask*) g_datalist_id_get_data (&data_list, quark);

            selected_task_id = id;
            todo_taskwindow_set_task (task);
            todo_taskwindow_display (&task_window);

        }
    }

    //TODO not yet implemented
    g_print ("btn_edit_clicked");
}

void 
todo_on_delete_btn_clicked()
{
    //TODO not yet implemented
    g_print ("btn_delete_clicked");
}

void 
todo_on_close_clicked()
{
    store_tasks ();
    gtk_widget_destroy (GTK_WIDGET(mainwindow));
}
