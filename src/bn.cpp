#include "bn.h"
#include <map>

static std::vector<std::string> bnGlyph(char c) {
    static std::map<char, std::vector<std::string>> f = {
        {'A', {" ### ", "#   #", "#####", "#   #", "#   #"}},
        {'B', {"#### ", "#   #", "#### ", "#   #", "#### "}},
        {'C', {" ####", "#    ", "#    ", "#    ", " ####"}},
        {'D', {"#### ", "#   #", "#   #", "#   #", "#### "}},
        {'E', {"#####", "#    ", "#### ", "#    ", "#####"}},
        {'G', {" ####", "#    ", "#  ##", "#   #", " ####"}},
        {'H', {"#   #", "#   #", "#####", "#   #", "#   #"}},
        {'K', {"#   #", "#  # ", "###  ", "#  # ", "#   #"}},
        {'L', {"#    ", "#    ", "#    ", "#    ", "#####"}},
        {'O', {" ### ", "#   #", "#   #", "#   #", " ### "}},
        {'P', {"#### ", "#   #", "#### ", "#    ", "#    "}},
        {'R', {"#### ", "#   #", "#### ", "#  # ", "#   #"}},
        {'Y', {"#   #", " # # ", "  #  ", "  #  ", "  #  "}},
        {' ', {"     ", "     ", "     ", "     ", "     "}}
    };
    auto it = f.find(c);
    if (it == f.end()) return f[' '];
    return it->second;
}

std::vector<std::string> bnRender(const std::string& w) {
    std::vector<std::string> out(5);
    for (char c : w) {
        auto g = bnGlyph(c);
        for (int r = 0; r < 5; r++) {
            for (char p : g[r]) out[r] += (p == '#') ? "█" : " ";
            out[r] += " ";
        }
    }
    return out;
}
