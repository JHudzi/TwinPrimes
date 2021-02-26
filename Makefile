#Author:	Josh Hudziak
#Date:		15/02/2021
#Use:		Makefile to manage Prime tests.
#See http://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/ for info.

EXEC=
CC = g++
CFLAGS = -std=c++11 -fopenmp
OBJFILES = prime_parallel_1core.o

%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS) $(DEBUGFLAGS)

ALL: $(OBJFILES)
	$(CC) -o prime_parallel_1core $(OBJFILES) $(CFLAGS) $(DEBUGFLAGS)

DEBUG: DEBUGFLAGS = -DDEBUG -Wall
DEBUG: ALL

#generate documents
doc: doxygen Doxyfile

#this is customary and just deletes all .o files
CLEAN:
	rm *.o
