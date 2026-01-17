#pragma once
#include <array>
#include <cmath>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <iostream>
#include <cstdlib>
#include "card.h"
#include "deck.h"
#include "stats.h"

// peso per valore (1..10)
struct Tag {
    double s1 = 0.0; // System1 (decimale)
    int    s2 = 0;   // System2 (intero)
};

// index 1..10 (0 ignorato)
using TagTable = std::array<Tag, 11>;

struct RunningCount {
    double rc1 = 0.0;  // somma System1
    long long rc2 = 0; // somma System2
};

static inline void aggiornaConteggio(const Card& c, RunningCount& rc, const TagTable& tags) {
    const int v = c.getValue(); // 1..10
    if (v < 1 || v > 10) return;
    rc.rc1 += tags[v].s1;
    rc.rc2 += tags[v].s2;
}

static inline Card estraiEConta(Deck& d, RunningCount& rc, const TagTable& tags) {
    Card c = d.ExtractRandom();
    aggiornaConteggio(c, rc, tags);
    return c;
}

// Da Stats -> pesi grezzi (come prima)
static inline Tag tagDaStatsRaw(const Stats& st, const Stats& base) {
    const double eor  = st.ev() - base.ev();
    const double sys1 = eor * 10000.0;
    const int    sys2 = (int)std::llround(sys1);

    Tag t;
    t.s1 = sys1;
    t.s2 = sys2;
    return t;
}

// Post-processing: normalizza per il sys2 più frequente, poi forza sum(sys2)=0
static inline void normalizzaEAggiustaTags(TagTable& tags, int vMin = 1, int vMax = 10) {
    // 1) Mode di sys2 (escludo 0 perché non si può dividere)
    std::unordered_map<long long, int> freq;
    for (int v = vMin; v <= vMax; ++v) {
        freq[(long long)tags[v].s2] += 1;
    }

    long long mode = 0;
    int bestCount = -1;
    for (const auto& kv : freq) {
        const long long val = kv.first;
        const int count = kv.second;
        if (val == 0) continue;

        // tie-break deterministico: a parità di count prendo quello con |val| più grande
        if (count > bestCount || (count == bestCount && std::llabs(val) > std::llabs(mode))) {
            bestCount = count;
            mode = val;
        }
    }

    //     // DEBUG: stampa frequenze sys2
    // std::cout << "Frequenze sys2 (valore -> count):\n";
    // for (const auto& kv : freq) {
    //     std::cout << "  " << kv.first << " -> " << kv.second << "\n";
    // }
    // std::cout << std::endl;

    long long div = std::llabs(mode);
    if (div == 0) div = 1; // caso patologico: tutti 0

    // std::cout << div << std::endl;

    // 2) Divido sys1 per div e ricalcolo sys2 come prima (llround di sys1)
    for (int v = vMin; v <= vMax; ++v) {
        tags[v].s1 /= (double)div;
        tags[v].s2 = (int)std::llround(tags[v].s1);
    }

    // 3) Controllo somma sys2
    long long sum = 0;
    for (int v = vMin; v <= vMax; ++v) sum += (long long)tags[v].s2;

    auto err = [&](int v) -> double {
        // quanto è lontano sys1 dall'intero attuale sys2
        return tags[v].s1 - (double)tags[v].s2;
    };

    // 4) Se sum != 0, aggiusto sys2 scegliendo quelli "più lontani dall'intero"
    // - se sum < 0 => devo AUMENTARE qualche sys2 (+1) finché arrivo a 0
    //   conviene scegliere quello con err più grande (vicino a +0.5)
    // - se sum > 0 => devo DIMINUIRE qualche sys2 (-1)
    //   conviene scegliere quello con err più piccolo (vicino a -0.5)
    while (sum != 0) {
        int bestV = vMin;

        if (sum < 0) {
            double bestE = -std::numeric_limits<double>::infinity();
            for (int v = vMin; v <= vMax; ++v) {
                const double e = err(v);
                if (e > bestE) { bestE = e; bestV = v; }
            }
            tags[bestV].s2 += 1;
            sum += 1;
        } else { // sum > 0
            double bestE = std::numeric_limits<double>::infinity();
            for (int v = vMin; v <= vMax; ++v) {
                const double e = err(v);
                if (e < bestE) { bestE = e; bestV = v; }
            }
            tags[bestV].s2 -= 1;
            sum -= 1;
        }
    }
}


// decks rimanenti "esatti"
static inline double decksRemExact(int cardsRem) {
    return double(cardsRem) / 40.0;
}

// decks rimanenti arrotondati "al mezzo" (come i tuoi esempi: 3.4->3.0, 3.2->3.0)
// cioè TRONCO al mezzo mazzo più vicino per difetto: floor(x*2)/2
static inline double decksRemHalfDown(int cardsRem) {
    double d = double(cardsRem) / 40.0;
    d = std::floor(d * 2.0) / 2.0;
    return std::max(0.5, d); // evita divisione per 0 quando sei quasi a fine mazzo
}

static inline double trueCountS1(double rc1, int cardsRem) {
    const double denom = decksRemExact(cardsRem);
    return (denom > 0.0) ? (rc1 / denom) : 0.0;
}

static inline double trueCountS2(long long rc2, int cardsRem) {
    const double denom = decksRemHalfDown(cardsRem);
    return (denom > 0.0) ? (double(rc2) / denom) : 0.0;
}
