#pragma once
#include <ostream>
#include <string>
#include <vector>
#include "stats.h"
#include "table.h"
#include <sstream>
#include <string>

void printStats(std::ostream& os,
                const std::string& title,
                const Stats& st,
                const Stats* base = nullptr,
                const Stats* allHands = nullptr,
                bool sim = false);

void printSimulazione(std::ostream& os,
                      int mazzi,
                      int taglio,
                      int nGame,
                      const std::vector<bool>& rulesp,
                      const std::vector<bool>& rulesd,
                      Table& tab,
                      int playerIdx = 0,
                      int punt_asso = 4);


static inline std::string listaIndiciTrue1(const std::vector<bool>& r) {
    std::ostringstream oss;
    oss << "{";
    bool first = true;
    for (size_t i = 0; i < r.size(); ++i) {
        if (r[i]) {
            if (!first) oss << ",";
            oss << (i + 1);
            first = false;
        }
    }
    oss << "}";
    return oss.str();
}