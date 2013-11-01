#include "gaussian_process.h"

void gaussian_process(int length, double* t, double* x, double dt, double mean, double sf, double tau) {

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

void macleod_parameters(double& tau, double& sf, double lambdarf, double magabsolute, double mass, double z) {
    // Calculate random walk parameters from physical parameters
    // Relation from MacLeod et al. 2010 eq. 7
    // Parameters from MacLeod et al. 2010 Table 1

    double A, B, C, D, E;
    double logsf, logtau;

    A = -0.51;
    B = -0.479;
    C = 0.131;
    D = 0.18;
    E = 0.0;

    logsf = A
        + B * log(lambdarf/4000.0)
        + C * (magabsolute + 23.0)
        + D * log(mass/1e9)
        + E * log(1.0 + z);

    sf = exp(logsf);

    A = 2.4;
    B = 0.17;
    C = 0.03;
    D = 0.21;
    E = 0.0;

    logtau = A
        + B * log(lambdarf/4000.0)
        + C * (magabsolute + 23.0)
        + D * log(mass/1e9)
        + E * log(1.0 + z);

    tau = exp(logtau);
}
