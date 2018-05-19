CC=g++
INCLUDE=include
CFLAG= -g -I$(INCLUDE)
OBJS=test_gen_main.o parse.o scan.o exp.o util.o exe.o TypeCheck.o
tinyc:$(OBJS)
	$(CC) $(CFLAG) $(OBJS) -o tinyc
test_gen_main.o:test_gen_main.cpp
	$(CC) -c $(CFLAG) test_gen_main.cpp
parse.o:parse.cpp
	$(CC) -c $(CFLAG) parse.cpp
scan.o:scan.cpp
	$(CC) -c $(CFLAG) scan.cpp
exp.o:exp.cpp
	$(CC) -c $(CFLAG) exp.cpp
util.o:util.cpp
	$(CC) -c $(CFLAG) util.cpp
exe.o:exe.cpp
	$(CC) -c $(CFLAG) exe.cpp
#	$(CC) -c $(CFLAG) gen.cpp -o gen.o
TypeCheck.o:TypeCheck.cpp
	$(CC) -c $(CFLAG) TypeCheck.cpp
clean:
	-rm -f $(OBJS)
