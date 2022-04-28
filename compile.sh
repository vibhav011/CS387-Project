# shopt -s expand_aliases
# source ~/.zprofile

rm -rf data.db *.tbl a.out

cd ./dblayer 
make
cd ..
cd ./pflayer
make
cd ..
flex --header-file=lex.yy.h scanner.l
bison -dv parser.y

g++ -c receiver/query.cpp
# g++ -c receiver/commit.cpp
g++ -c lex.yy.c
g++ -c parser.tab.c
g++ -c utils.cpp
g++ -c ast.cc

g++ -o daemon daemon.cc parser.tab.o lex.yy.o ast.o query.o dblayer/tbl.o dblayer/codec.o pflayer/pf.o pflayer/buf.o pflayer/hash.o utils.o -pthread
g++ -o dbprocess db-process.cc
# g++ daemon.cc y.tab.o query.o ast.o -o daemon -pthread