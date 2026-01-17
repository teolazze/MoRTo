#pragma once

#include "table.h"
#include "deck.h"
#include "counting.h"      // RunningCount
#include "rangen/random.h"

Table& Round(Table& tab, Random& rnd, Deck& myDeck, const TagTable& tags, RunningCount& rc);

Table& Game(Table& tab, Random& rnd, int mazzi, int taglio,
            const TagTable& tags,
            double sogliaTC1, double sogliaTC2,
            Stats& st_all, Stats& st_tc1, Stats& st_tc2);
