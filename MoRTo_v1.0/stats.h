#pragma once
#include <cmath>
#include <algorithm>

struct Stats {
    long long lost = 0;
    long long win  = 0;
    long long asso = 0;
    long long push = 0;
    long long total = 0;

    int punt_asso = 0;        // IMPORTANT: tienilo coerente ovunque
    long long shoes = 0;      // nGame (numero shoe)

    double p_win()  const { return total ? double(win)  / total : 0.0; }
    double p_lost() const { return total ? double(lost) / total : 0.0; }
    double p_asso() const { return total ? double(asso) / total : 0.0; }
    double p_push() const { return total ? double(push) / total : 0.0; }

    double profit() const {
        return double(win) - double(lost) + double(punt_asso) * double(asso);
    }

    double ev() const {
        return total ? profit() / double(total) : 0.0;
    }

    double ev_pct() const { return ev() * 100.0; }              // EV in %
    double win_per_100_hands() const { return ev() * 100.0; }    // same numero (unità per 100 mani)

    double win_per_shoe() const {
        return shoes ? profit() / double(shoes) : 0.0;
    }

    double var() const {
        if (!total) return 0.0;
        const double mu = ev();
        return p_win()  * std::pow( 1.0 - mu, 2) +
               p_lost() * std::pow(-1.0 - mu, 2) +
               p_asso() * std::pow(double(punt_asso) - mu, 2) +
               p_push() * std::pow( 0.0 - mu, 2);
    }


    double err_var() const {
        if (total <= 0) return 0.0;

        const double N = (double)total;
        const double m = ev();
        const double A = (double)punt_asso;

        const double pW = p_win();
        const double pL = p_lost();
        const double pA = p_asso();
        const double pP = p_push();

        const double dW = 1.0 - 2.0*m;
        const double dL = 1.0 + 2.0*m;
        const double dA = A*A - 2.0*m*A;
        const double dP = 0.0;

        const double S1 = pW*dW*dW + pL*dL*dL + pA*dA*dA + pP*dP*dP;
        const double S2 = pW*dW    + pL*dL    + pA*dA    + pP*dP;

        double var_g = (S1 - S2*S2) / N;
        if (var_g < 0.0) var_g = 0.0; // clamp per roundoff

        return std::sqrt(var_g);
    }
};