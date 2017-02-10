CXX = clang++
CXXFLAGS = --std=c++11 -g

SRCDIR = ./src
LIBDIR = ./lib
BUILDDIR = ./build
BINDIR = ./bin

.PHONY: clean

simulator : $(BUILDDIR)/simulator.o pqueue.hpp heap.hpp
	$(CXX) $(CXXFLAGS) $< -o $(BINDIR)/$@

$(BUILDDIR)/heap_test.o : heap_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/pqueue_test.o : pqueue_test.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

heap : $(BUILDDIR)/heap_test.o heap.hpp
	$(CXX) $(CXXFLAGS) $< -o $(BINDIR)/$@

pqueue : $(BUILDDIR)/pqueue_test.o pqueue.hpp heap.hpp
	$(CXX) $(CXXFLAGS) $< -o $(BINDIR)/$@

clean::
	rm -rf $(BUILDDIR)/* $(BINDIR)/*
