g++ -c ./receiver/query.cpp    
echo "query.o"
g++ -c -std=c++11 ./dblayer/tbl.c    
echo "tbl.o"
g++ -c utils.cpp 
echo "utils.o"
g++ -c ast.cc        
echo "ast.o"                            
gcc pflayer/pf.o ./pflayer/buf.o ./dblayer/codec.o ./pflayer/hash.o ./dblayer/tbl.o ast.o query.o utils.o no_parse.cpp
echo "done?"