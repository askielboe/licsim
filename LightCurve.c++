#include "LightCurve.h"

LightCurve::LightCurve (int len_in) {
    // Initialize class
    len = len_in;
    t.resize(len);
    cont.resize(len);
    line.resize(len);
    cont_err.resize(len);
    line_err.resize(len);
}

void LightCurve::genCont (double dt, double mu, double tau, double sf) {
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

void LightCurve::genLine (double lag) {
    // Convolve with delta function transfer function
    double tmax = 50;
    int width = 2;
    int tf_len = tmax;

    double *tf = new double[tf_len];

    for (int i = 0; i < tf_len; i++) {
        if (i >= lag - width && i <= lag + width) {
            tf[i] = 1.0/(2*width);
        }
        else {
            tf[i] = 0.0;
        }
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

void LightCurve::addGaussianNoise (double SNR) {
    // Adds simple Gaussian noise in each light curve individually

    std::default_random_engine generator;
    generator.seed(time(NULL));

    double std_noise;

    for (int i = 0; i < len; i++) {
        std_noise = std::fabs(cont[i]) / SNR;
        std::normal_distribution<double> distribution(cont[i], std_noise);

        cont[i] = distribution(generator);
        cont_err[i] = std_noise;
    }

    for (int i = 0; i < len; i++) {
        std_noise = std::fabs(line[i]) / SNR;
        std::normal_distribution<double> distribution(line[i], std_noise);

        line[i] = distribution(generator);
        line_err[i] = std_noise;
    }
}

// void LightCurve::genObserved (double cadence) {
//     // Remove points in the light curves to match cadence
//     double baseline = t[len-1] - t[0];
//     int len_new = (int)(baseline / cadence);


// }

// void LightCurve::getCCF (double minLag=-50, double maxLag=100, double resLag=1) {

//     // First do interpolation
//     double *cont_interpolated = new double[len];
//     double *line_interpolated = new double[len];
// }

