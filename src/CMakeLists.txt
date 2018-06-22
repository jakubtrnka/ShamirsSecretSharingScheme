include_directories( . )

add_library(shamirsharing
	hmac_sha256.cpp
	hmac_sha256.h
	#hmac_sha512.cpp
	#hmac_sha512.h
	sha256.cpp
	sha256.h
	#sha512.cpp
	#sha512.h
	pbkdf2.cpp
	pbkdf2.h
	rijndael.cpp
	rijndael.h
	bit_container.cpp
	bit_container.h
	oneblockshamir.cpp
	oneblockshamir.h
	multiblockshamir.cpp
	multiblockshamir.h
	share.cpp
	share.h
	get_insecure_randomness.cpp
	get_insecure_randomness.h
	slip39_wrapper.cpp
	slip39_wrapper.h
	wordlist.cpp
	wordlist.h)

add_executable(Shamir-cli main.cpp)
target_link_libraries(Shamir-cli shamirsharing)

add_subdirectory(qt)
