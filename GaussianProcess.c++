#include "GaussianProcess.h"

void GaussianProcess(double* t, double* x, int length, double dt, double mean, double sf, double tau) {

    // Based on equation 6 in MacLeod et al. 2010
    double exponential = exp(-dt/tau);
    double norm = mean * (1.0 - exponential);
    double var = 0.5 * pow(sf, 2.0) * (1.0 - exp(-2.0*dt/tau));

    // Initialize random number generator (C++11)
    std::default_random_engine generator;
    generator.seed(time(NULL));

    // Calculate the first values
    t[0] = 0.0;
    std::normal_distribution<double> distribution(mean, var);
    x[0] = distribution(generator);

    // Calculate subsequent values
    for (int i = 1; i < length; i++) {
        double E = exponential * x[i-1] + norm;
        std::normal_distribution<double> distribution(E, var);
        x[i] = distribution(generator);
        t[i] = t[i-1] + dt;
    }
}