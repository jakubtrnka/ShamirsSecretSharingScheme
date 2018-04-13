CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++14 -I.
LD=g++
LDFLAGS=-Wall -pedantic

onetimeshamir_test: onetimeshamir_test.o onetimeshamir.o get_insecure_randomness.o
	$(LD) $(LDFLAGS) -o $@ $^

rijndael_test: rijndael.o rijndael_test.o
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

rijndael.o: rijndael.cpp rijndael.h
rijndael_test.o: rijndael_test.cpp rijndael.h
onetimeshamir.o: onetimeshamir.cpp onetimeshamir.h
onetimeshamir_test.o: onetimeshamir_test.cpp onetimeshamir.h
get_insecure_randomness.o: get_insecure_randomness.cpp get_insecure_randomness.h

check: rijndael_test
	./rijndael_test

clean:
	rm -f *.o rijndael_test onetimeshamir_test
