# Makefile for building the C++ port of ddar and graph with pybind11
# Requires: a C++17 compiler, pybind11 installed (e.g. via pip), and Python 3

CXX = g++
CXXFLAGS = -O3 -std=c++17 -Wall -shared -fPIC $(shell python3 -m pybind11 --includes)
LDFLAGS = -shared
PYTHON = python3

# Directories
SRCDIR = cpp
BINDIR = build
MODULE = graph_cpp

# List of source files (for our minimal port, we only build ddar and graph)
SOURCES = $(SRCDIR)/ddar.cpp $(SRCDIR)/graph.cpp $(SRCDIR)/geometry.cpp $(SRCDIR)/node.cpp
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BINDIR)/%.o,$(SOURCES))

all: $(BINDIR)/$(MODULE).so

$(BINDIR)/$(MODULE).so: $(OBJECTS)
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: all
	PYTHONPATH=build $(PYTHON) python/ddar_test.py

clean:
	rm -rf $(BINDIR) 
