CXX = clang++
CXXFLAGS = --std=c++11 -g

SRCDIR = ./src
LIBDIR = ./lib
BUILDDIR = ./build
BINDIR = ./bin

.PHONY: clean

$(BUILDDIR)/heap_test.o : heap_test.cpp heap.hpp
	$(CXX) $(CXXFLAGS) -I heap.hpp -c $< -o $@

$(BUILDDIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c -I $< -o $@

heap: $(BUILDDIR)/heap_test.o
	$(CXX) $(CXXFLAGS) $< -o $(BINDIR)/$@

clean::
	rm -rf $(BUILDDIR)/* $(BINDIR)/*
