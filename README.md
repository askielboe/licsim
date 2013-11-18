# Light Curve Simulator (licsim)

Simulates continuum and emission line light curves from [active galactic nuclei](https://en.wikipedia.org/wiki/Active_galactic_nucleus).

## Description

The code provides a C++ class that uses a Gaussian random process to simulate the continuum emission from an AGN. The class as methods for convolving the generated continuum light curve with a transfer function to simulate a corresponding emission line light curve. It also has a method for adding noise to the light curves.

## Basic usage

### Using the class

Using the class is as simple as doing

    #include "LightCurve.h"
    int main {
        LightCurve lightcurve(1000);
    }

### Compiling and running the included example

An example program and Makefile is included as licsim_example.c++. To run the example program edit Makefile to your compiler of choise and run

    make

Then run the example by invoking

    ./licsim_example

The output from the standard example consists of three columns of doubles:

    time | continuum flux | emission line flux

For convenience you can pipe the output by doing

    ./licsim_example > lightcurves.txt

## Requirements

* C++11 compatible compiler

## Issues

* Only a gamma distribution transfer function is currently supported. More transfer functions will be added later.
* Light curves are *not* truncated after convolution. This means that fluxes in the emission line light curve at early times are wrong, because information required to do the convolution is missing in the continuum light curve. In this case the continuum is only convolved with parts of the transfer function. If you want to use the code in its current state you should truncate the emission line light curves at early epochs.
* The random generator is seeded from system clock at runtime. If several processes are spawned in quick succession this may cause trouble.

## More information

For more information on the modeling of AGN variability see e.g. [MacLeod et al. (2010)](http://adsabs.harvard.edu/abs/2010ApJ...721.1014M).

*Note: Everything you see here is work-in-progress. This documentation is incomplete.*
