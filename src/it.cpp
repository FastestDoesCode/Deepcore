#include "it.h"
#include "lg.h"
#include "tm.h"
#include <algorithm>

void itPick(Gs& g) {
    for (size_t i = 0; i < g.it.size();) {
        It& t = g.it[i];
        if (t.x != g.pl.x || t.y != g.pl.y) { i++; continue; }
        if (t.ch == '+') {
            int h = 10 + g.dp;
            g.pl.hp = std::min(g.pl.mh, g.pl.hp + h);
            lgAdd(g, fg(91, 227, 154), "> patch applied :: +" + std::to_string(h) + " hp");
        } else if (t.ch == '$') {
            g.sh++;
            lgAdd(g, fg(127, 162, 240), "> data shard recovered");
        } else if (t.ch == '!') {
            g.oc = 15;
            lgAdd(g, fg(232, 196, 106), "> overclock :: +3 atk for 15 turns");
        }
        g.it.erase(g.it.begin() + static_cast<long>(i));
    }
}
