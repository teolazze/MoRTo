#include "player.h"
#include <utility>
#include <vector>
using std::vector;

Player::Player(int I)
    : Id(I), card(), counter_loss(0), counter_win(0), counter_win_asso(0), counter_push(0), rules() {}

int Player::getId() const { return Id; }
int Player::getCounterLoss() const { return counter_loss; }
int Player::getCounterWin() const { return counter_win; }
int Player::getCounterWinAsso() const { return counter_win_asso; }
int Player::getCounterPush() const { return counter_push; }
bool Player::getRule(int i) const { return rules[i]; }
Card Player::getCard() const { return card; }

void Player::AssignRule(vector<bool> rule) { rules = rule; }

void Player::AssignCard(Deck& myDeck, RunningCount& rc, const TagTable& tags) {
    card = estraiEConta(myDeck, rc, tags);
}

void Player::swapCard(Player& other) { std::swap(card, other.card); }

void Player::Loss() { counter_loss += 1; }
void Player::Win()  { counter_win += 1; }
void Player::WinAsso() { counter_win_asso += 1; }
void Player::Push() { counter_push += 1; }
