#Makefile for project 3

# Make environment
CC=gcc
CFLAGS= -c -g -Wall $(INCLUDES)

# Files
OBJECT_FILES=	shellex.o csapp.o
TARGET = sh257
# Productions

all: sh257

sh257 : $(OBJECT_FILES)
	$(CC)  $(OBJECT_FILES) -g -o $@ -pthread

shellex.o : shellex.c csapp.h
	$(CC) $(CFLAGS) $< -o $@
csapp.o : csapp.c csapp.h
	$(CC) $(CFLAGS) $< -o $@

clean : 
	rm -v sh257 $(OBJECT_FILES)

