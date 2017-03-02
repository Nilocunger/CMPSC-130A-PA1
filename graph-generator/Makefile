CXX = clang++
CXXFLAGS = --std=c++11 -g -Wall -Wextra

.PHONY: clean 

graph : test.o generator.hpp 
	$(CXX) $(CXXFLAGS) $< -o $@

test.o : test.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

clean:: 
	rm graph test.o
