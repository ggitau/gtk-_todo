# Makefile
# Author George Gitau
# --------------------------------------------------------
#gcc `pkg-config --cflags gtk+-3.0` -o main src/main.c src/mainwindow.h src/mainwindow.c `pkg-config --libs gtk+-3.0` `pkg-config --libs gmodule-2.0` 
CC=gcc
CFLAGS=-g -Wall `pkg-config --cflags gtk+-3.0`
LIBS=`pkg-config --libs gtk+-3.0` `pkg-config --libs gmodule-2.0`
# Add more object files below
OBJECTS=main.o mainwindow.o taskwindow.o todoengine.o windowbuilder.o

EXECUTABLE=bin/main

main: $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LIBS)
main.o: src/main.c src/mainwindow.h
	$(CC) $(CFLAGS) -c  src/main.c $(LIBS)
mainwindow.o: src/mainwindow.c src/mainwindow.h src/windowbuilder.h src/taskwindow.h src/definitions.h
	$(CC) $(CFLAGS) -c  src/mainwindow.c $(LIBS)
taskwindow.o: src/taskwindow.c src/taskwindow.h src/windowbuilder.h src/definitions.h
	$(CC) $(CFLAGS) -c  src/taskwindow.c $(LIBS)
todoengine.o: src/todoengine.c src/todoengine.h  src/taskwindow.h src/definitions.h
	$(CC) $(CFLAGS) -c  src/todoengine.c $(LIBS)
windowbuilder.o: src/windowbuilder.c src/windowbuilder.h
	$(CC) $(CFLAGS) -c  src/windowbuilder.c $(LIBS)
# Add more targets here
clean: 
	rm $(EXECUTABLE) *.o


