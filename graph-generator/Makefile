CXX = clang++
CXXFLAGS = --std=c++11 -g -Wall -Wextra

.PHONY: clean 

generate: command.o
	$(CXX) $(CXXFLAGS) $< -o $@

command.o : command.cpp generator.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

clean:: 
	rm generate command.o
