CFLAGS := --std=c11
CC := gcc

all : cal

cal : regex_table.o parse.o tokenize.o eval.o main.o
	$(CC) $(CFLAGS) regex_table.o parse.o tokenize.o eval.o main.o -o cal

regex_table.o : regex_table.c syntax.h regex_table.h
	$(CC) $(CFLAGS) -c regex_table.c

parse.o : parse.c parse.h regex_table.o
	$(CC) $(CFLAGS) -c parse.c

tokenize.o : tokenize.c tokenize.h parse.o
	$(CC) $(CFLAGS) -c tokenize.c

eval.o : eval.c eval.h tokenize.o
	$(CC) $(CFLAGS) -c eval.c

main.o : main.c parse.o tokenize.o eval.o regex_table.o
	$(CC) $(CFLAGS) -c main.c
