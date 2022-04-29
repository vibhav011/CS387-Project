CC=g++
CFLAGS = -std=c++11
OBJS=lex.yy.o parser.tab.o ast.o commit.o helper.o loader.o query.o utils.o ../dblayer/dblayer.a
SRC=ast.cc commit.cc helper.cc loader.cc parser.tab.c lex.yy.c query.cc utils.cc

all: daemon dbprocess

dbprocess: db-process.cc
	$(CC) $(CFLAGS) -o dbprocess db-process.cc

daemon: $(OBJS)
	$(CC) -o daemon daemon.cc $(OBJS) -pthread

../dblayer/dblayer.a:
	cd ../dblayer; make

lex.yy.c lex.yy.h:
	flex --header-file=lex.yy.h scanner.l

parser.tab.c parser.tab.h:
	bison -dv parser.y

lex.yy.o: lex.yy.c parser.tab.h
	$(CC) -c $(CFLAGS) lex.yy.c

parser.tab.o: parser.tab.c
	$(CC) -c $(CFLAGS) parser.tab.c

ast.o: ast.cc
	$(CC) -c $(CFLAGS) ast.cc

commit.o: commit.cc
	$(CC) -c -std=c++17 commit.cc

helper.o: helper.cc
	$(CC) -c $(CFLAGS) helper.cc

loader.o: loader.cc
	$(CC) -c -std=c++17 loader.cc

query.o: query.cc
	$(CC) -c $(CFLAGS) query.cc

utils.o: utils.cc
	$(CC) -c $(CFLAGS) utils.cc

clean:
	rm -rf *.o *.a a.out* *~ dbprocess daemon lex.yy.* parser.tab.* *.output
