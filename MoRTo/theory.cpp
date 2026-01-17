#include "theory.h"

Stats calcolaStats(Deck &myDeck, int p_asso, std::vector<bool> rulesp, std::vector<bool> rulesd) {
    Stats s;
    const int N = myDeck.Decksize();

    s.punt_asso = p_asso;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (j == i) continue;

            for (int k = 0; k < N; ++k) {
                if (k == i || k == j) continue;

                Card p = myDeck.getCard(i);
                Card d = myDeck.getCard(j);

                // Player: scambia se < 6, vietato se dealer==10
                if (rulesp[p.getValue()-1] == true /*&& p.getValue() > 1*/ && d.getValue() != 10) {
                    // perde subito se riceverebbe una carta più bassa
                    if (d.getValue() < p.getValue()) {
                        s.lost++; s.total++;
                        continue;
                    }
                    std::swap(p, d);
                }

                // Dealer: cambia se < 7 usando k, altrimenti tiene d
                Card dFinal = d;
                if (rulesd[d.getValue()-1] == true) dFinal = myDeck.getCard(k);

                // Esito
                if (p.getValue() < dFinal.getValue()) s.lost++;
                else if (p.getValue() > dFinal.getValue()) s.win++;
                else {
                    if (p.getValue() == 1) s.asso++;
                    else s.push++;
                }

                s.total++;
            }
        }
    }
    return s;
}
