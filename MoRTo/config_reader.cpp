#include "config_reader.h"

#include <fstream>
#include <algorithm>
#include <cctype>
#include <stdexcept>

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
        [](unsigned char ch){ return !std::isspace(ch); }));
}
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
        [](unsigned char ch){ return !std::isspace(ch); }).base(), s.end());
}
static inline std::string trim(std::string s) {
    ltrim(s); rtrim(s); return s;
}
static inline std::string tolower_str(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return (char)std::tolower(c); });
    return s;
}

static std::vector<bool> parseBoolList(std::string v) {
    v = trim(v);
    if (v.empty()) return {};

    // Accetta {true,false,...}
    if (v.front() == '{' && v.back() == '}') {
        v = v.substr(1, v.size() - 2);
    }

    std::vector<bool> out;
    std::string token;
    for (size_t i = 0; i <= v.size(); ++i) {
        if (i == v.size() || v[i] == ',') {
            token = tolower_str(trim(token));
            if (token == "true" || token == "1") out.push_back(true);
            else if (token == "false" || token == "0") out.push_back(false);
            else if (!token.empty())
                throw std::runtime_error("Valore booleano non valido: '" + token + "'");
            token.clear();
        } else {
            token.push_back(v[i]);
        }
    }
    return out;
}

Config loadConfig(const std::string& path) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Impossibile aprire file: " + path);

    Config cfg;
    std::string line;

    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // ignora righe decorative tipo "=====" o simili
        if (!line.empty() && line[0] == '=') continue;

        // (opzionale) se vuoi supportare commenti:
        // auto cpos = line.find('#');
        // if (cpos != std::string::npos) line = trim(line.substr(0, cpos));

        auto pos = line.find(':');
        if (pos == std::string::npos) continue;

        std::string key = tolower_str(trim(line.substr(0, pos)));
        std::string val = trim(line.substr(pos + 1));

        if (key == "mazzi") {
            cfg.mazzi = std::stoi(val);
        } else if (key == "taglio") {
            cfg.taglio = std::stoi(val);
        } else if (key == "sabot" || key == "ngame") {
            cfg.nGame = std::stoi(val);
        } else if (key == "vincita_asso") {
            cfg.v_asso = std::stoi(val);
        } else if (key == "rulesp") {
            cfg.rulesp = parseBoolList(val);
        } else if (key == "rulesd") {
            cfg.rulesd = parseBoolList(val);
        } else if (key == "sogliatc1") {
            cfg.sogliaTC1 = std::stod(val);
        } else if (key == "sogliatc2") {
            cfg.sogliaTC2 = std::stod(val);
        }
        // chiavi sconosciute: ignorate
    }

    return cfg;
}
