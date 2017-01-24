#include "Firth17.hpp"

#include <math.h>

#include <iostream>
#include <limits>


Firth17::Firth17() : Model() {
    noParams_ = 8;
    paramNames_ = {"A",
                   "B",
                   "t1",
                   "Trise",
                   "Tfall",
                   "t0",
                   "t2",
                   "Textra"};

    priorRange_ = {{1e-5, 1000.0}, // A
                   {1e-5, 100.0},  // B
                   {0.0, 100.0},   // t1
                   {0.2, 100.0},   // Trise
                   {0.0, 100.0},   // Tfall
                   {0.0, 100.0},   // t0
                   {0.0, 100.0},   // t2
                   {0.2, 100.0}};  // Textra

    paramGuess_ = {1.0,   // A
                   0.1,   // B
                   10.0,  // t1
                   5.0,   // Trise
                   20.0,  // Tfall
                   1.0,   // t0
                   5.0,   // t2
                   10.0}; // Textra

    priorType_ = {"log",  // A
                  "log",  // B
                  "flat", // t1
                  "flat", // Trise
                  "flat", // Tfall
                  "flat", // t0
                  "flat", // t2
                  "log"}; // Textra
}


double Firth17::function(double t) {
    double flux = params_[0] * (1.0 + params_[1] * pow(t - params_[2], 2.0));
    flux *= exp(-(t-params_[5])/params_[4]);
    flux /= (1.0 + exp(-(t - params_[5]) / params_[3]));
    flux /= (1.0 + exp(-(t - params_[6]) / params_[7]));

    return flux;
}


std::vector<double> Firth17::function(std::vector<double>& t) {
    std::vector<double> res(t.size(), 0);
    for (size_t i = 0; i < t.size(); ++i) {
        res[i] = function(t[i]);
    }

    return res;
}


std::vector<double> Firth17::residual() {
    std::vector<double> res(x_.size(), 0);

    double flux;
    for (size_t i = 0; i < x_.size(); ++i) {
        flux = function(x_[i]);
        if (flux < 0) {
            res[i] = std::numeric_limits<double>::max();
        } else {
            res[i] = (y_[i] - flux) / sigma_[i];
        }
    }

    return res;
}
