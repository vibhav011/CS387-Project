g++ -c receiver/query.cpp
# g++ -c receiver/commit.cpp
g++ -c y.tab.c
g++ -c ast.cc
g++ daemon.cc y.tab.o query.o ast.o -o daemon -pthread