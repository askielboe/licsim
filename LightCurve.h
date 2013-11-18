#include <vector>       // vector
#include <cmath>
#include <random>

using namespace std;

class LightCurve {

    public:
        // Light curves
        int len;
        // double *t = new double[len];
        // double *cont = new double[len];
        // double *line = new double[len];
        // double *cont_err = new double[len];
        // double *line_err = new double[len];

        vector<double> t;
        vector<double> cont;
        vector<double> line;
        vector<double> cont_err;
        vector<double> line_err;

        // Random walk / gaussian process parameters
        double gaus_dt, gaus_mu, gaus_sf, gaus_tau;

        // Physical parameters (used for MacLeod fits to get random walk parameters)
        double phys_lambdarf, phys_absmag, phys_mass, phys_redshift;

        // Tranfer function parameters
        double gamma_k, gamma_theta; // Gamma function

        LightCurve (int);

        void genCont (double, double, double, double);
        void genLine (double);
        void genLine (double, double);
        void getMacLeodParameters (double&, double&, double, double, double, double);
        void addGaussianNoise(double);
};
