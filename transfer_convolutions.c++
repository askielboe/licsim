#include "transfer_convolutions.h"

void transfer_convolution_gamma(int length, double* t, double* c, double* l, double k, double theta) {

    // Calculate transfer function with same resolution as the light curve
    double tmax = 25;
    double dt = t[1] - t[0];
    int tf_length = tmax / dt;

    double *tf = new double[tf_length];

    for (int i = 0; i < tf_length; i++) {
        tf[i] = pow(i*dt, k-1.0) * exp(-i*dt/theta) / pow(theta,k) / std::tgamma(k);
    }

    // Convolve with the continuum light curve to get the emission line light curve
    for (int i = 0; i < length; i++) {
        l[i] = 0.0;
        // Loop over transfer function to sum up the flux for the emission line at time i*dt
        if (i < tf_length) {
            for (int j = 0; j <= i; j++) {
                l[i] += tf[j] * c[i-j]; // Transfer equation
            }
        }
        else {
            for (int j = 0; j < tf_length; j++) {
                l[i] += tf[j] * c[i-j]; // Transfer equation
            }
        }
    }
}
