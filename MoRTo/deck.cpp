#include "deck.h"
#include <stdexcept>
#include <algorithm>

Deck::Deck(Random& rnd_, int nDecks) : rnd(rnd_) {
    const std::string suits[4] = {"Cuori", "Quadri", "Fiori", "Picche"};

    cards.reserve(40 * nDecks);

    for (int t = 0; t < nDecks; ++t) {
        for (const auto& s : suits) {
            for (int v = 1; v <= 10; ++v) {
                cards.emplace_back(s, v);
            }
        }
    }
    // a fine costruttore: cards.size() == 40 * nDecks
}

int Deck::Decksize() const { return (int)cards.size(); }

Card Deck::getCard(int i) const { return cards.at(i); }

Card Deck::ExtractRandom() {
    if (cards.empty()) {
        throw std::runtime_error("Errore: mazzo vuoto in ExtractRandom()");
    }

    // COME NEL TUO FILE: assumiamo RandInt(a,b) con b NON "aggiustato" qui.
    int nRand = rnd.RandInt(0, (int)cards.size());

    Card selection = cards.at(nRand);
    cards.erase(cards.begin() + nRand);
    return selection;
}

bool Deck::RemoveOne(const std::string& suit, int value) {
    for (auto it = cards.begin(); it != cards.end(); ++it) {
        if (it->getSuit() == suit && it->getValue() == value) {
            cards.erase(it);
            return true;
        }
    }
    return false;
}

bool Deck::RemoveOne(const Card& c) {
    return RemoveOne(c.getSuit(), c.getValue());
}
