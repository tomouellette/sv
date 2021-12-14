#!/bin/bash

# Miscellaneous script for building pybind11 python wrappers

# Haploid Wright Fisher simulations
g++ -O3 -Wall -shared -std=c++17 -undefined dynamic_lookup -fPIC $(python3 -m pybind11 --includes) sv_evolution.cpp -o sv_evolution$(python3-config --extension-suffix) -I ./pybind11/include