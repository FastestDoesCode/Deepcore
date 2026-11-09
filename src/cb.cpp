#include "cb.h"
#include "rn.h"
#include "lg.h"
#include "tm.h"
#include <algorithm>

static int cbDmg(int at, int df) {
    return std::max(1, at + rnI(-1, 2) - df);
}

void cbLevel(Gs& g) {
    while (g.xp >= g.nx) {
        g.xp -= g.nx;
        g.lv++;
        g.nx += 8 * g.lv;
        g.pl.mh += 6;
        g.pl.hp = std::min(g.pl.mh, g.pl.hp + 12);
        g.pl.at += 1;
        if (g.lv % 2 == 0) g.pl.df += 1;
        lgAdd(g, fg(232, 196, 106), "> core sync :: level " + std::to_string(g.lv));
    }
}

void cbPlayerHit(Gs& g, En& e) {
    int d = cbDmg(g.pl.at + (g.oc > 0 ? 3 : 0), e.df);
    e.hp -= d;
    if (e.hp <= 0) {
        e.alive = false;
        g.kills++;
        g.xp += e.xp;
        lgAdd(g, fg(91, 227, 154), "> " + e.nm + " purged (+" + std::to_string(e.xp) + " xp)");
        cbLevel(g);
    } else {
        lgAdd(g, fg(127, 162, 240), "> you hit the " + e.nm + " for " + std::to_string(d));
    }
}

void cbEnemyHit(Gs& g, En& e) {
    int d = cbDmg(e.at, g.pl.df);
    g.pl.hp -= d;
    lgAdd(g, fg(224, 90, 138), "> the " + e.nm + " hits you for " + std::to_string(d));
    if (g.pl.hp <= 0) { g.pl.hp = 0; g.over = true; }
}
