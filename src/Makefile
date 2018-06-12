CXX=g++
CXXFLAGS=-Wall -pedantic -std=c++11 -c -I.
LDFLAGS=-Wall -pedantic -std=c++11

OBJ=bit_container.o get_insecure_randomness.o hmac_sha256.o hmac_sha512.o main.o multiblockshamir.o oneblockshamir.o pbkdf2.o rijndael.o sha256.o sha512.o share.o slip39_wrapper.o wordlist.o

main: $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -o $@ $<

bit_container.o: bit_container.cpp
get_insecure_randomness.o: get_insecure_randomness.cpp
hmac_sha256.o: hmac_sha256.cpp
hmac_sha512.o: hmac_sha512.cpp
main.o: main.cpp
multiblockshamir.o: multiblockshamir.cpp
oneblockshamir.o: oneblockshamir.cpp
pbkdf2.o: pbkdf2.cpp
rijndael.o: rijndael.cpp
sha256.o: sha256.cpp
sha512.o: sha512.cpp
share.o: share.cpp
slip39_wrapper.o: slip39_wrapper.cpp
wordlist.o: wordlist.cpp
