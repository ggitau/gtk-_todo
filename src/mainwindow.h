#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H
#include<gtk/gtk.h>
#include<assert.h>
#include "taskwindow.h"
GObject* todo_mainwindow_init();
void init_treeview(GObject** treeview);
void todo_mainwindow_show(GObject** mainwindow);

void todo_on_add_btn_clicked();
void todo_on_edit_btn_clicked();
void todo_on_delete_btn_clicked();

#endif
