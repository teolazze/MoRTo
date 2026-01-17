#pragma once
#include <vector>
#include <stdexcept>
#include <algorithm>
#include "player.h"

class Table {
public:
    Table(int num, int l, const std::vector<bool>& rulesp, const std::vector<bool>& rulesd);

    Player& getPlayer(int k);
    const Player& getPlayer(int k) const;

    void swapCardPlayers(int i, int j);

private:
    int Num;
    std::vector<Player> players;
};
