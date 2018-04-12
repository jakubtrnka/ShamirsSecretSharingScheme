CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++14 -I.
LD=g++
LDFLAGS=-Wall -pedantic



check: rijndael_test
	./rijndael_test

rijndael_test: rijndael.o rijndael_test.o
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

rijndael.o: rijndael.cpp rijndael.h
rijndael_test.o: rijndael_test.cpp rijndael.h



clean:
	rm -f *.o rijndael_test

