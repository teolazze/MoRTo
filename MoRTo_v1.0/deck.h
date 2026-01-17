#pragma once
#include <vector>
#include <string>
#include "./rangen/random.h"
#include "card.h"

class Deck {
public:
    Deck(Random& rnd_, int nDecks = 1);

    int Decksize() const;

    // ATTENZIONE: lasciata com'era nel tuo file (nessuna "correzione" su RandInt)
    Card ExtractRandom();

    Card getCard(int i) const;

    bool RemoveOne(const std::string& suit, int value);
    bool RemoveOne(const Card& c);

private:
    std::vector<Card> cards;
    Random& rnd;
};
