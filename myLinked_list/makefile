CXX=g++ -g -std=c++11 -Wall -pedantic -Werror=vla
all: tests

tests: tests.o tests.cpp linkedlist.cpp linkedlist.h
	$(CXX) -o tests tests.o linkedlist.cpp
tests.o: tests.cpp
	$(CXX) -c -o tests.o tests.cpp
