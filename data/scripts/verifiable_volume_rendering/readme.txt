Requires following python packages:
matplotlib
python-dateutil
pyparsing
numpy
pillow
six

Packages can be found here:
http://www.lfd.uci.edu/~gohlke/pythonlibs/

First run verifiablevolumerendering.py within inviwo with an appropriate workspace. Then run convergence.py with generated images as input.

For example:
python convergence.py -p version(0.9)_windowRes(512,512)_
dvrType(raycast)_dataset(grid-01.dat)_samplesPerRay(100.000000).png version(0.9)_windowRes(512,512)_dvrType(raycast)_dataset(grid-02.dat)_samplesPerRay(100.000000).png version(0.9)_windowRes(512,512)_dvrType(raycast)_dataset(grid-03.dat)_samplesPerRay(100.000000).png version(0.9)_windowRes(512,512)_dvrType(raycast)_dataset(grid-04.dat)_samplesPerRay(100.000000).png version(0.9)_windowRes(512,512)_dvrType(raycast)_dataset(grid-05.dat)_samplesPerRay(100.000000).png version(0.9)_windowRes(512,512)_dvrType(raycast)_dataset(grid-06.dat)_samplesPerRay(100
.000000).png version(0.9)_windowRes(512,512)_dvrType(raycast)_dataset(grid-07.dat)_samplesPerRay(100.000000).png version(0.9)_windowRes(512,512)_dvrType(raycast)_dataset(grid-08.dat)_samplesPerRay(100.000000).png version(0.9)_windowRes(512,512)_dvrType(raycast)_dataset(grid-09.dat)_samplesPerRay(100.000000).png -o datasetSize