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
/usr/local/opt/bison@3.8/bin/bison -dv parser.y

g++ -c receiver/query.cpp
# g++ -c receiver/commit.cpp
# g++ -c lex.yy.c
# g++ -c y.tab.c
g++ -c utils.cpp
g++ -c ast.cc
g++ no_parse-vibhav.cpp ast.o query.o dblayer/tbl.o dblayer/codec.o pflayer/pf.o pflayer/buf.o pflayer/hash.o utils.o
# g++ daemon.cc y.tab.o query.o ast.o -o daemon -pthread