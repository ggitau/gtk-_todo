#include<stdio.h>
#include<gtk/gtk.h>
#include "mainwindow.h"
int main(int argc,char* argv[]){
    gtk_init(&argc,&argv);
    GObject* mainwindow=todo_mainwindow_init();
    todo_mainwindow_show(&mainwindow);
    gtk_main();
    return 0;
}
