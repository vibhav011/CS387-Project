lex scan.l
yacc -dv parser.y

g++ -c receiver/query.cpp
# g++ -c receiver/commit.cpp
# g++ -c lex.yy.c
# g++ -c y.tab.c
g++ -c ast.cc
# g++ daemon.cc y.tab.o query.o ast.o -o daemon -pthread