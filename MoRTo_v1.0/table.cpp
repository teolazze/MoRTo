#include "table.h"

Table::Table(int num, int /*l*/, const std::vector<bool>& rulesp, const std::vector<bool>& rulesd) {
    if ((int)rulesp.size() != 10 || (int)rulesd.size() != 10) {
        throw std::invalid_argument("Errore: rules.size() diverso da num");
    }

    Num = num;
    players.reserve(num);

    for (int i = 0; i < num; i++) {
        players.emplace_back(i + 1);
    }
    players[0].AssignRule(rulesp);
    players[1].AssignRule(rulesd);
}

Player& Table::getPlayer(int k) { return players[k]; }
const Player& Table::getPlayer(int k) const { return players[k]; }

void Table::swapCardPlayers(int i, int j) { players[i].swapCard(players[j]); }
