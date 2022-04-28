# shopt -s expand_aliases
# source ~/.zprofile

rm -rf data.db *.tbl a.out

cd ./dblayer 
make
cd ..
cd ./pflayer
make
cd ..
lex scanner.l
yacc -dv parser.y

g++ -c receiver/query.cpp
# g++ -c receiver/commit.cpp
# g++ -c lex.yy.c
# g++ -c y.tab.c
g++ -c utils.cpp
g++ -c ast.cc
g++ no_parse.cpp ast.o query.o dblayer/tbl.o dblayer/codec.o pflayer/pf.o pflayer/buf.o pflayer/hash.o utils.o
# g++ daemon.cc y.tab.o query.o ast.o -o daemon -pthread