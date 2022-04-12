echo "Testcase 1" > out.txt
./dumpdb i EQUAL 44271041 >> out.txt
echo "" >> out.txt

echo "Testcase 2" >> out.txt
./dumpdb i LESS_THAN 100000 >> out.txt
echo "" >> out.txt

echo "Testcase 3" >> out.txt
./dumpdb i LESS_THAN_EQUAL 430835 >> out.txt
echo "" >> out.txt

echo "Testcase 4" >> out.txt
./dumpdb i GREATER_THAN 100000 >> out.txt
echo "" >> out.txt

echo "Testcase 5" >> out.txt
./dumpdb i GREATER_THAN_EQUAL 430835 >> out.txt
echo "" >> out.txt