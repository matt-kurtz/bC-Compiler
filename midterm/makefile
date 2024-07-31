BIN  = bC  # name of thing to be built goes hereunknown class
CC   = g++

CPPFLAGS = -O0 -g -I./ -std=c++11
SRCS = parser.y parser.l
OBJS = lex.yy.o parser.tab.o
LIBS = -lm 

$(BIN): $(OBJS)
	$(CC) $(CCFLAGS) $(OBJS) $(LIBS) -o $(BIN)

parser.tab.h parser.tab.c: parser.y
	bison -v -t -d --debug parser.y

lex.yy.c: parser.l parser.tab.h
	flex parser.l

all:
	make

clean:
	rm -f $(OBJS) c- lex.yy.c parser.tab.h parser.tab.c c-.tar parser.output core
	rm -f bC

test: $(BIN)
	./test.sh

tar:
	tar -cvf c-.tar $(SRCS) makefile 
