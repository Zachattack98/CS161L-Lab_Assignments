all: cache.cpp 
	g++ -g -Wall -o a.out cache.cpp
clean: 
	rm a.out