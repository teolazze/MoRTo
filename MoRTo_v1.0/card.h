#pragma once
#include <string>

class Card {
public:
    Card() : suit(""), value(0) {}
    Card(const std::string& s, int v) : suit(s), value(v) {}

    int getValue() const { return value; }
    std::string getSuit() const { return suit; }

private:
    std::string suit;
    int value;
};
