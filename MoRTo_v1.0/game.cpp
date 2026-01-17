#include "game.h"

#include <cassert>

enum class Outcome { Win, Loss, Asso, Push, Unknown };

struct PlayerSnap {
    long long win, loss, asso, push;
};

static inline PlayerSnap snapPlayer(Table& tab, int idx) {
    auto& p = tab.getPlayer(idx);
    return PlayerSnap{
        p.getCounterWin(),
        p.getCounterLoss(),
        p.getCounterWinAsso(),
        p.getCounterPush()
    };
}

static inline Outcome deduceOutcome(const PlayerSnap& b, const PlayerSnap& a) {
    const long long dW = a.win  - b.win;
    const long long dL = a.loss - b.loss;
    const long long dA = a.asso - b.asso;
    const long long dP = a.push - b.push;

    const long long sum = dW + dL + dA + dP;
    if (sum != 1) return Outcome::Unknown;   // Round dovrebbe incrementare esattamente UNO

    if (dW == 1) return Outcome::Win;
    if (dL == 1) return Outcome::Loss;
    if (dA == 1) return Outcome::Asso;
    if (dP == 1) return Outcome::Push;
    return Outcome::Unknown;
}

static inline void addOutcome(Stats& st, Outcome o) {
    if (o == Outcome::Unknown) return;
    st.total++;
    if (o == Outcome::Win)  st.win++;
    if (o == Outcome::Loss) st.lost++;
    if (o == Outcome::Asso) st.asso++;
    if (o == Outcome::Push) st.push++;
}

Table& Round(Table& tab, Random& rnd, Deck& myDeck, const TagTable& tags, RunningCount& rc) {

    // assegna carte
    tab.getPlayer(0).AssignCard(myDeck, rc, tags);
    tab.getPlayer(1).AssignCard(myDeck, rc, tags);

    bool re = false;
    bool lastCantSwapWithDeck = false;

    int vi = tab.getPlayer(0).getCard().getValue();

    if (tab.getPlayer(0).getRule(vi-1)==true) {
        int vnext = tab.getPlayer(1).getCard().getValue();

        if (vnext != 10) {
            tab.swapCardPlayers(0, 1);

            if (vnext < vi) lastCantSwapWithDeck = true;

        } else {
            re = true;
        }

        if (!re && !lastCantSwapWithDeck) {
            tab.getPlayer(1).AssignCard(myDeck, rc, tags);
        }

    } else {
        if (tab.getPlayer(1).getRule(tab.getPlayer(1).getCard().getValue() - 1) == true) {
            tab.getPlayer(1).AssignCard(myDeck, rc, tags);
        }
    }

    // ===== controllo pareggio DOPO tutte le mosse =====
    int v0 = tab.getPlayer(0).getCard().getValue();
    int v1 = tab.getPlayer(1).getCard().getValue();

    if(v0 > v1){
        tab.getPlayer(0).Win();
        tab.getPlayer(1).Loss();

        return tab;
    }

    if(v0 < v1){
        tab.getPlayer(1).Win();
        tab.getPlayer(0).Loss();

        return tab;
    }

    if (v0 == v1) {
        
        if (v0 == 1){
            tab.getPlayer(0).WinAsso();
            tab.getPlayer(1).Loss();
            return tab;
        }

        // if (v0 == 7){
        //     tab.getPlayer(1).Win();
        //     tab.getPlayer(0).Loss();
        //     return tab;
        // }

        tab.getPlayer(1).Push();
        tab.getPlayer(0).Push();

        return tab;
    }
    return tab;

}


Table& Game(Table& tab, Random& rnd, int mazzi, int taglio,
            const TagTable& tags,
            double sogliaTC1, double sogliaTC2,
            Stats& st_all, Stats& st_tc1, Stats& st_tc2)
{
    Deck myDeck(rnd, mazzi);
    RunningCount rc; // rc.rc1, rc.rc2

    // burn NON contato: lascia pure solo ExtractRandom() se lo fai
    // Card burn = myDeck.ExtractRandom();

    auto doOneRound = [&]() {
        const int cardsRem = myDeck.Decksize();

        const double tc1 = trueCountS1(rc.rc1, cardsRem);
        const double tc2 = trueCountS2(rc.rc2, cardsRem);

        const bool ok1 = (tc1 > sogliaTC1);
        const bool ok2 = (tc2 > sogliaTC2);

        const PlayerSnap before = snapPlayer(tab, 0);

        tab = Round(tab, rnd, myDeck, tags, rc);   // qui aggiorni RC sulle carte della mano

        const PlayerSnap after = snapPlayer(tab, 0);
        const Outcome outc = deduceOutcome(before, after);

        addOutcome(st_all, outc);
        if (ok1) addOutcome(st_tc1, outc);
        if (ok2) addOutcome(st_tc2, outc);
    };

    while (myDeck.Decksize() > taglio) {
        doOneRound();
    }
    doOneRound(); // iterazione extra come facevi tu

    return tab;
}
