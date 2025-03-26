# AlphaGeometry C++ Port

This repository contains a minimal C++ port of the DDAR (Deductive Deduction Architecture for Geometry) system. The project uses pybind11 to expose C++ functionality to Python, allowing for integration tests and further development using Python.

## Overview

The project implements a minimal version of a geometry deduction system. It includes:

- C++ Components:
  
  - DDAR: A minimal deduction engine for geometric proofs.

  - Graph: A graph-based representation of geometric problems.

  - Geometry & Node: Basic geometric operations and node representations.

- Python Interface:

  - Minimal bindings are provided for testing and prototyping.

  - A dummy problem parser (in python/problem.py) simulates input parsing.

- Testing:

  - Integration tests (in python/ddar_test.py) validate that the basic pipeline is operational.
  
## Directory Structure

- cpp/
Contains all C++ source files:

  - ddar.cpp / ddar.hpp

  - graph.cpp / graph.hpp

  - geometry.cpp / geometry.hpp

  - node.cpp / node.hpp

  - (Additional supporting headers/sources)

- python/
Contains Python files:

  - problem.py – Minimal parser and dummy definitions.

  - ddar_test.py – Integration tests for the DDAR system.

- Makefile
Provides build and test commands.

## Requirements

- A C++17 compliant compiler.

- pybind11 installed (e.g., via pip install pybind11).

- Python 3.

## Building

To compile the C++ module and create the shared library, run:

```bash
make
```

This command compiles the source files and generates the shared library (graph_cpp.so) in the `build`/ directory.

## Running Tests

To run the integration tests, execute:

```bash
make test
```

This command sets the Python module search path to the `build/` directory and runs `python/ddar_test.py.`

## Notes

- **Dummy Implementation**:
  The current implementation uses dummy factories and minimal parsing to simulate geometry problem solving. For example, the dummy problem factory always creates a problem with a known point ("A") and uses the goal names dummy_goal_success or dummy_goal_fail to simulate success or failure in geometric checking.

- **Future Improvements**:
  Full parsing of problem texts and proper implementation of geometric predicates in the graph checking functions are planned.
