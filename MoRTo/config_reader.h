#ifndef CONFIG_READER_H
#define CONFIG_READER_H

#include <string>
#include <vector>

struct Config {
    int mazzi = 4;
    int taglio = 14;
    int nGame = 1000; // se nel file c'è "sabot:" lo uso come nGame
    int v_asso = 4;
    std::vector<bool> rulesp = {true,true,true,true,true,false,false,false,false,false};
    std::vector<bool> rulesd = {true,true,true,true,true,true,false,false,false,false};
    double sogliaTC1 = 7;
    double sogliaTC2 = 7;
};

// Legge il file e ritorna una Config popolata (con default per campi mancanti)
Config loadConfig(const std::string& path);

#endif
