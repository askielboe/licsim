#include <iostream>

#include "LightCurve.h"

using namespace std;

int main (int argc, char *argv[])
{
    // if (argc != 6) {
    //     cout << "Usage: licsim [length] [dt] [mean] [SF] [tau]\n";
    //     exit(EXIT_FAILURE);
    // }

    // Read parameters from command line
    // int length = atof(argv[1]);
    // double dt = atof(argv[2]);
    // double mean = atof(argv[3]);
    // double sf = atof(argv[4]);
    // double tau = atof(argv[5]);

    // Get a light curve object
    LightCurve lightcurve(100);

    // Get random walk parameters from MacLeod fits
    double tau, sf;
    lightcurve.getMacLeodParameters(tau, sf, 5100.0, -23.0, 1e9, 0.0);

    // Generate continuum light curve
    lightcurve.genCont(1.0, 0.0, 0.14, 2.0);

    // Convolve with gamma distribution transfer function
    lightcurve.genLine(2.0, 2.0);

    // Print output to prompt
    for (int i = 0; i < lightcurve.len; i++) {
        cout << lightcurve.t[i] << ' ' << lightcurve.cont[i] << ' ' << lightcurve.line[i] << '\n';
    }
}
