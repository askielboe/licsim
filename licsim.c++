#include <iostream>

#include "GaussianProcess.h"

using namespace std;

int main (int argc, char *argv[])
{
    if (argc != 6) {
        cout << "Usage: test [length] [dt] [mean] [SF] [tau]\n";
        exit(EXIT_FAILURE);
    }

    int length = atof(argv[1]);
    double dt = atof(argv[2]);
    double mean = atof(argv[3]);
    double sf = atof(argv[4]);
    double tau = atof(argv[5]);

    // Allocate arrays for output
    double *t = new double[length];
    double *x = new double[length];

    GaussianProcess(t, x, length, dt, mean, sf, tau);

    for (int i; i < length; i++) {
        cout << t[i] << ' ' << x[i] << '\n';
    }
}
