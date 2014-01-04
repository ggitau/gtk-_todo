#ifndef __MAINWINDOW_H
#define __MAINWINDOW_H
#include<gtk/gtk.h>
#include<assert.h>
#include<string.h>
#include <json-glib/json-glib.h>

#include "definitions.h"
#include "taskwindow.h"
#include "windowbuilder.h"
#include "todoengine.h"

GObject* todo_mainwindow_init();
void init_treeview(GObject** treeview);
void todo_mainwindow_show(GObject** mainwindow);

void todo_on_add_btn_clicked();
void todo_on_edit_btn_clicked();
void todo_on_delete_btn_clicked();
void todo_on_close_clicked();

#endif
