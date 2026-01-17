#include "printing.h"
#include <iomanip>
#include <cmath>

void printStats(std::ostream& os,
                const std::string& title,
                const Stats& st,
                const Stats* base,
                const Stats* allHands,
                bool sim) {

double bet_freq = 0.0;
double bet_freq_err = 0.0;

    if (allHands && allHands->total > 0) {
        const double N = (double)allHands->total;
        bet_freq = (double)st.total / N;

        // Errore standard binomiale della proporzione
        bet_freq_err = std::sqrt(bet_freq * (1.0 - bet_freq) / N);

    } else {
        bet_freq = (st.total ? 1.0 : 0.0);
        bet_freq_err = 0.0; // non definibile senza N
    }
                                      

    const double profit = st.profit();

    // per 100 mani BETTATE (cioè dentro st)
    const double win100_bet = st.ev() * 100.0;

    // per 100 mani TOTALI (anche non bettate -> profitto 0)
    const double win100_all =
        (allHands && allHands->total) ? (profit * 100.0 / double(allHands->total))
                                      : win100_bet; // se non hai allHands, coincide

    double err_win = sqrt(st.win)/st.total;
    double err_lost = sqrt(st.lost)/st.total;
    double err_asso = sqrt(st.asso)/st.total;
    double err_push = sqrt(st.push)/st.total;
    double mu  = st.p_win() - st.p_lost() + st.punt_asso * st.p_asso();
    double ex2 = st.p_win() + st.p_lost() + (st.punt_asso * st.punt_asso) * st.p_asso();
    double err_ev = sqrt((ex2 - mu * mu) / (double)st.total);

    double err_win100_all = 0.0;

    if (allHands && allHands->total > 0) {
        const double N = (double)allHands->total;
        const double A = (double)st.punt_asso;

        const double mu  = profit / N; // E[X]
        const double ex2 = ((double)st.win + (double)st.lost + (A*A) * (double)st.asso) / N; // E[X^2]

        double var_mean = (ex2 - mu * mu) / N;   // Var(media)
        if (var_mean < 0.0) var_mean = 0.0;      // clamp per roundoff

        err_win100_all = 100.0 * std::sqrt(var_mean);
    }

    double mean_profit_per_shoe = (st.shoes ? (profit / double(st.shoes)) : 0.0);

    double err_mean_profit_per_shoe =
        (st.shoes ? (std::sqrt(std::fabs(profit)) / double(st.shoes)) : 0.0);



    if(sim == false){
        os << title << "\n"
        << "lost: " << st.lost
        << ", win: "  << st.win
        << ", asso: " << st.asso
        << ", push: " << st.push
        << ", tot: "  << st.total << "\n"
        << "P(win)  = " << st.p_win()  << "\n"
        << "P(lost) = " << st.p_lost() << "\n"
        << "P(asso) = " << st.p_asso() << "\n"
        << "P(push) = " << st.p_push() << "\n"
        << "EV  = "     << st.ev()     << "\n";
        auto old_prec = os.precision();
        os << "EV% = " << std::setprecision(3) << st.ev() * 100.0 << "\n";
        os.precision(old_prec);
        os << "var = "     << st.var()    << "\n"
        << "bet_frequency = " << bet_freq << "\n";
        os << std::setprecision(3) << "win/100 hands = " << win100_all << "\n";
        os.precision(old_prec);
    }else{
        os << title << "\n"
        << "lost: " << st.lost
        << ", win: "  << st.win
        << ", asso: " << st.asso
        << ", push: " << st.push
        << ", tot: "  << st.total << "\n"
        << "P(win)  = " << st.p_win()  << " +/- " << err_win << "\n"
        << "P(lost) = " << st.p_lost() << " +/- " << err_lost << "\n"
        << "P(asso) = " << st.p_asso() << " +/- " << err_asso << "\n"
        << "P(push) = " << st.p_push() << " +/- " << err_push << "\n"
        << "EV  = "     << st.ev()  << " +/- " << err_ev << "\n";
        auto old_prec = os.precision();
        os << "EV% = " << std::setprecision(3) << st.ev() * 100.0 << " +/- " << err_ev * 100 << "\n";
        os.precision(old_prec);
        os << "var = "     << st.var() <<  " +/- " << st.err_var() << "\n"
        << "bet_frequency = " << bet_freq << " +/- " << bet_freq_err << "\n";

        os << "win/100 bet hands = " << std::setprecision(3) << st.ev() * 100.0 << " +/- " << err_ev * 100 << "\n";

        os << "win/100 hands = " << win100_all << " +/- " << err_win100_all << "\n";

        os << "win/shoe = " << mean_profit_per_shoe << " +/- " << err_mean_profit_per_shoe <<"\n";
        os.precision(old_prec);
    }

    if (base) {
        const double eor = st.ev() - base->ev();
        const std::streamsize oldPrec = os.precision();
        const std::ios::fmtflags oldFlags = os.flags();

        os << "EOR% = " << (eor * 100.0) << "%\n";
        const double sys = eor * 10000.0;

        os << std::fixed << std::setprecision(1);
        os << "System1 = " << sys << "\n";
        os << "System2 = " << static_cast<long long>(std::llround(sys)) << "\n";

        os.flags(oldFlags);
        os.precision(oldPrec);
    }

    os << "\n";
}


void printSimulazione(std::ostream& os,
                      int mazzi,
                      int taglio,
                      int nGame,
                      const std::vector<bool>& rulesp,
                      const std::vector<bool>& rulesd,
                      Table& tab,
                      int playerIdx,
                      int punt_asso) {
    // const auto& p = tab.getPlayer(playerIdx);

    // const long long loss = p.getCounterLoss();
    // const long long win  = p.getCounterWin();
    // const long long asso = p.getCounterWinAsso();
    // const long long push = p.getCounterPush();
    // const long long tot  = loss + win + asso + push;

    // auto P = [&](long long x) -> double { return tot ? double(x) / double(tot) : 0.0; };

    // const double pw = P(win);
    // const double pl = P(loss);
    // const double pa = P(asso);
    // const double pp = P(push);

    // const double ev = (tot ? (double(win) - double(loss) + double(punt_asso) * double(asso)) / double(tot) : 0.0);
    // const double var = (tot ?
    //     pw * std::pow( 1.0 - ev, 2) +
    //     pl * std::pow(-1.0 - ev, 2) +
    //     pa * std::pow(double(punt_asso) - ev, 2) +
    //     pp * std::pow( 0.0 - ev, 2)
    //     : 0.0);

    os << "\n==========================\n=== SIMULAZIONE (Game) ===\n==========================\n";
    os << "shoe  = " << nGame << "\n";
    os << "mazzi  = " << mazzi << (mazzi == 1 ? " mazzo" : " mazzi") << "\n";
    os << "taglio = " << taglio << "\n";
    os << "punt_asso = " << punt_asso << "\n";

    os << "regole:\n"
        << "- player cambia se ha " << listaIndiciTrue1(rulesp) << ";\n"
        << "- dealer cambia se ha " << listaIndiciTrue1(rulesd) << ".\n\n";

    // os << "--------------------------------\n";
    // os << "win  = " << win  << "    P(win)  = " << pw << "\n";
    // os << "loss = " << loss << "    P(loss) = " << pl << "\n";
    // os << "asso = " << asso << "    P(asso) = " << pa << "\n";
    // os << "push = " << push << "    P(push) = " << pp << "\n";
    // os << "tot  = " << tot  << "    P(tot)  = " << (tot ? 1.0 : 0.0) << "\n";
    // os << "EV  = " << ev  << "\n";
    // os << "var = " << var << "\n\n";
}
