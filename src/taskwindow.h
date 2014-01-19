#ifndef __TASK_WINDOW_H_
#define __TASK_WINDOW_H_
#include<gtk/gtk.h>
#include "definitions.h"
GObject* todo_taskwindow_init();
void todo_taskwindow_display(GObject** window);
void todo_taskwindow_set_task (TodoTask* task);

#endif
