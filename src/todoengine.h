#ifndef __TODO_ENGINE_H_
#define __TODO_ENGINE_H_
#include<gtk/gtk.h>
#include<assert.h>
#include<string.h>

void todo_engine_add_task(TodoTask* task, GData** data_list);
void todo_engine_remove_task(TodoTask* task,GData** data_list);
TodoTask* todo_engine_get_task(gchar* id,GData** data_list);

#endif
