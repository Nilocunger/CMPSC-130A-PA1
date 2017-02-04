CXX = clang++
CXXFLAGS = --std=c++11

SRCDIR = ../src
LIBDIR = ../lib
BUILDDIR = ../build
BINDIR = ./bin

.PHONY: clean

$(BUILDDIR)/%.o : %.cpp
	$(CXX) $(CXXFLAGS)-c -I $(LIBDIR) $< -o $(BUILDDIR)/$@

clean::
	rm -rf $(BUILDDIR)/* $(BINDIR)/*
