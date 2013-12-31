#include "todoengine.h"
#include "taskwindow.h"

void todo_engine_add_task(TodoTask* task,GData** data_list){
    todo_taskwindow_init();
    assert(task!=NULL);
    assert(task->id!=NULL);
    assert(data_list!=NULL);
    assert(*data_list!=NULL);

    GQuark quark=g_quark_from_string(task->id);
    g_datalist_id_set_data(data_list,quark,(gpointer)task);
}
void todo_engine_remove_task(TodoTask* task,GData** data_list){
    assert(task!=NULL);
    assert(task->id!=NULL);
    assert(data_list!=NULL);
    assert(*data_list!=NULL);

    GQuark quark=g_quark_from_string(task->id);
    g_datalist_id_remove_no_notify(data_list,quark);
}

TodoTask* todo_engine_get_task(gchar* id,GData** data_list){
    assert(id!=NULL);

    GQuark quark=g_quark_from_string(id);
    TodoTask* task= g_datalist_id_get_data(data_list,quark);

    return task;
}
