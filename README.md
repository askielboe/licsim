# Light Curve Simulator (licsim)

Simulates continuum and emission line light curves from [active galactic nuclei](https://en.wikipedia.org/wiki/Active_galactic_nucleus).

## Basic usage

### Compiling

Edit Makefile and run

    make

### Running

To run simply type

    ./licsim [length] [dt] [mean] [SF] [tau]

where

* length: Length of the light curves in arbitrary units
* dt: Time resolution in arbitrary units
* mean: Mean flux of the light curve
* SF: Structure function for the Gaussian process simulation
* tau: Characteristic time scale for the Gaussian process simulation

### Output

The output consists of three columns of doubles:

    time | continuum flux | emission line flux

For convenience you can pipe the output by doing

    ./licsim [length] [dt] [mean] [SF] [tau] > lightcurves.txt

## Description

The code uses a Gaussian random process to simulate the continuum emission from an AGN. It then convolves the continuum light curve with a transfer function modeled as a gamma distribution to create the emission line light curve.

## Issues

* Transfer function is hard coded at the moment, so if you want to change it you will have to manually edit the source.
* Light curves are *not* truncated after convolution. This means that fluxes in the emission line light curve at early times are wrong, because information required to do the convolution is missing in the continuum light curve. In this case the continuum is only convolved with parts of the transfer function, which is clearly wrong. If you want to use the code in its current state you should truncate the emission line light curves at early epochs.

## More information

For more information on the modeling of AGN variability see e.g. [MacLeod et al. (2010)](http://adsabs.harvard.edu/abs/2010ApJ...721.1014M).
