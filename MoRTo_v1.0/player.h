#pragma once
#include "card.h"
#include "deck.h"
#include "counting.h"
#include <vector>
using std::vector;

class Player {
public:
    explicit Player(int I);

    int getId() const;
    int getCounterLoss() const;
    int getCounterWin() const;
    int getCounterWinAsso() const;
    int getCounterPush() const;
    bool getRule(int i) const;
    Card getCard() const;

    void AssignRule(vector<bool> rule);
    void AssignCard(Deck& myDeck, RunningCount& rc, const TagTable& tags);

    void swapCard(Player& other);

    void Loss();
    void Win();
    void WinAsso();
    void Push();

private:
    int Id;
    Card card;
    int counter_loss;
    int counter_win;
    int counter_win_asso;
    int counter_push;
    vector<bool> rules;
};
