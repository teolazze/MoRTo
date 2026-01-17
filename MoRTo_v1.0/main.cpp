#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>
#include <limits>
#include <cmath>
#include <cstdlib>

#include "./rangen/random.h"
#include "deck.h"
#include "stats.h"
#include "counting.h"
#include "printing.h"
#include "game.h"
#include "theory.h"
#include "config_reader.h"

int main(int argc, char* argv[]) {
    Random rnd = generaRand(1);

    const std::string outName = (argc > 1) ? argv[1] : "risultati.txt";
    std::ofstream out(outName);
    if (!out) {
        std::cerr << "Errore: impossibile aprire " << outName << "\n";
        return 1;
    }
    out << std::fixed << std::setprecision(6);

    Config cfg;
    try {
        cfg = loadConfig("input.txt");
    } catch (const std::exception& e) {
        std::cerr << "Errore: " << e.what() << "\n";
        return 1;
    }

    const int mazzi  = cfg.mazzi;
    const int taglio = cfg.taglio;
    const int nGame  = cfg.nGame;

    int v_asso = cfg.v_asso;
    std::vector<bool> rulesp = cfg.rulesp;
    std::vector<bool> rulesd = cfg.rulesd;

    double sogliaTC1 = cfg.sogliaTC1;
    double sogliaTC2 = cfg.sogliaTC2;

    Deck baseDeck(rnd, mazzi);
    Stats baseStats = calcolaStats(baseDeck, v_asso, rulesp, rulesd);

    out << "==========================\n=== CALCOLO COMBINATORIO ===\n==========================\n";
    out << "mazzi = " << mazzi << (mazzi == 1 ? " mazzo" : " mazzi") << "\n";
    out << "punt_asso = " << v_asso << "\n";
    out << "regole:\n"
        << "- player cambia se ha " << listaIndiciTrue1(rulesp) << ";\n"
        << "- dealer cambia se ha " << listaIndiciTrue1(rulesd) << ".\n\n";

    const std::string titoloBase = "=== COMPLETO ===";
    printStats(out, titoloBase, baseStats);

    TagTable tags{};

    for (int v = 1; v <= 10; ++v) {
        Deck deck(rnd, mazzi);
        Card remCard("Cuori", v);
        deck.RemoveOne(remCard);

        Stats st = calcolaStats(deck, v_asso, rulesp, rulesd);
        tags[v] = tagDaStatsRaw(st, baseStats);

        std::string title = "=== REMOVED " + std::to_string(remCard.getValue()) +
                            " " + remCard.getSuit() + " ===";
        printStats(out, title, st, &baseStats);
    }

    normalizzaEAggiustaTags(tags, 1, 10);

    // === SIMULAZIONE ===
    Table tab(2, 0, rulesp, rulesd);

    Stats st_all, st_tc1, st_tc2;
    st_all.punt_asso = v_asso;
    st_tc1.punt_asso = v_asso;
    st_tc2.punt_asso = v_asso;

    st_all.shoes = nGame;
    st_tc1.shoes = nGame;
    st_tc2.shoes = nGame;

    for (int j = 0; j < nGame; ++j) {
        tab = Game(tab, rnd, mazzi, taglio, tags, sogliaTC1, sogliaTC2,
                   st_all, st_tc1, st_tc2);
    }

    printSimulazione(out, mazzi, taglio, nGame, rulesp, rulesd, tab, 0, baseStats.punt_asso);

    out << "\n=== TAGS (System1/System2) ===\n";
    for (int v = 1; v <= 10; ++v) {
        out << "v=" << v
            << "  S1=" << tags[v].s1
            << "  S2=" << tags[v].s2
            << "\n";
    }
    out << "\n";

    printStats(out, "=== SIM (tutte le mani) ===", st_all, nullptr, &st_all, true);
    printStats(out, "=== SIM (solo se TC System1 > " + std::to_string(sogliaTC1) + ") ===", st_tc1, nullptr, &st_all, true);
    printStats(out, "=== SIM (solo se TC System2 > " + std::to_string(sogliaTC2) + ") ===", st_tc2, nullptr, &st_all, true);

    std::cout << "Scritto tutto su: " << outName << "\n";
    return 0;
}
