#include <iostream>
#include <cmath>

#include "gaussian_process.h"
#include "transfer_convolutions.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc != 6) {
        cout << "Usage: licsim [length] [dt] [mean] [SF] [tau]\n";
        exit(EXIT_FAILURE);
    }

    // Read parameters from command line
    int length = atof(argv[1]);
    double dt = atof(argv[2]);
    double mean = atof(argv[3]);
    double sf = atof(argv[4]);
    double tau = atof(argv[5]);

    // Generate continuum light curve using a gaussian process
    // Allocate arrays for output
    double *t = new double[length];
    double *c = new double[length];
    double *l = new double[length];

    // Generate continuum light curve
    // Example to get parameters from MacLeod
    // macleod_parameters(sf, tau, 3000.0, -23.0, 12.0, 2.0);

    gaussian_process(length, t, c, dt, mean, sf, tau);

    // Convolve continuum with transfer function to get the emission line light curve
    double k = 2.0;
    double theta = 2.0;

    transfer_convolution_gamma(length, t, c, l, k, theta);

    // Print output to prompt
    for (int i = 0; i < length; i++) {
        cout << t[i] << ' ' << c[i] << ' ' << l[i] << '\n';
    }
}
