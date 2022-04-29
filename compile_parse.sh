# shopt -s expand_aliases
# source ~/.zprofile

# rm -rf data.db *.tbl a.out

cd ./dblayer 
make
cd ..
cd ./pflayer
make
cd ..
flex --header-file=lex.yy.h scanner.l
bison -dv parser.y

g++ -c receiver/query.cpp
g++ -c -std=c++17 receiver/commit.cpp
g++ -c receiver/helper.cpp
g++ -c lex.yy.c
g++ -c parser.tab.c
g++ -c utils.cpp
g++ -c ast.cc

g++ -o daemon daemon.cc parser.tab.o lex.yy.o ast.o query.o dblayer/tbl.o dblayer/codec.o pflayer/pf.o pflayer/buf.o pflayer/hash.o utils.o -pthread helper.o commit.o
g++ -o dbprocess db-process.cc