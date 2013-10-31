#include <iostream>
#include <random>
#include <cmath>

using namespace std;

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

void transfer_convolution(int length, double* t, double* c, double* l, double k, double theta) {

    // Calculate transfer function with same resolution as the light curve
    double tmax = 25;
    double dt = t[1] - t[0];
    int tf_length = tmax / dt;

    double *tf = new double[tf_length];

    for (int i = 0; i < tf_length; i++) {
        tf[i] = pow(i*dt, k-1.0) * exp(-i*dt/theta) / pow(theta,k) / std::tgamma(k);
        l[i] = tf[i];
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

int main (int argc, char *argv[])
{
    if (argc != 6) {
        cout << "Usage: test [length] [dt] [mean] [SF] [tau]\n";
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
    gaussian_process(length, t, c, dt, mean, sf, tau);

    // Convolve continuum with transfer function to get the emission line light curve
    double k = 2.0;
    double theta = 2.0;

    transfer_convolution(length, t, c, l, k, theta);

    // Print output to prompt
    for (int i = 0; i < length; i++) {
        cout << t[i] << ' ' << c[i] << ' ' << l[i] << '\n';
    }
}