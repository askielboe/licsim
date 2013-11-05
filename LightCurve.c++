#include "LightCurve.h"

LightCurve::LightCurve (int len_in) {
    // Initialize class
    len = len_in;
}

void LightCurve::genCont (double dt, double mu, double sf, double tau) {
    // Based on equation 6 in MacLeod et al. 2010
    double exponential = exp(-dt/tau);
    double norm = mu * (1.0 - exponential);
    double var = 0.5 * pow(sf, 2.0) * (1.0 - exp(-2.0*dt/tau));

    // Initialize random number generator (C++11)
    std::default_random_engine generator;
    generator.seed(time(NULL));

    // Calculate the first values
    t[0] = 0.0;
    std::normal_distribution<double> distribution(mu, var);
    cont[0] = distribution(generator);

    // Calculate subsequent values
    for (int i = 1; i < len; i++) {
        double E = exponential * cont[i-1] + norm;
        std::normal_distribution<double> distribution(E, var);
        cont[i] = distribution(generator);
        t[i] = t[i-1] + dt;
    }
}

void LightCurve::genLine (double k, double theta) {
    // Calculate transfer function with same resolution as the light curve
    double tmax = 25;
    double dt = t[1] - t[0];
    int tf_len = tmax / dt;

    double *tf = new double[tf_len];

    for (int i = 0; i < tf_len; i++) {
        tf[i] = pow(i*dt, k-1.0) * exp(-i*dt/theta) / pow(theta,k) / std::tgamma(k);
    }

    // Convolve with the continuum light curve to get the emission line light curve
    for (int i = 0; i < len; i++) {
        line[i] = 0.0;
        // Loop over transfer function to sum up the flux for the emission line at time i*dt
        if (i < tf_len) {
            for (int j = 0; j <= i; j++) {
                line[i] += tf[j] * cont[i-j]; // Transfer equation
            }
        }
        else {
            for (int j = 0; j < tf_len; j++) {
                line[i] += tf[j] * cont[i-j]; // Transfer equation
            }
        }
    }
}

void LightCurve::getMacLeodParameters (double& tau, double& sf, double lambdarf, double magabsolute, double mass, double z) {
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