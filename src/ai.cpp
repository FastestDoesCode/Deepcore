#include "ai.h"
#include "mp.h"
#include "cb.h"
#include "rn.h"
#include <cstdlib>
#include <algorithm>

static bool aiFree(const Gs& g, int x, int y) {
    if (!mpOpen(g, x, y)) return false;
    if (g.pl.x == x && g.pl.y == y) return false;
    return mpEnemyAt(g, x, y) < 0;
}

void aiTurn(Gs& g) {
    const int DX[4] = {1, -1, 0, 0};
    const int DY[4] = {0, 0, 1, -1};
    for (auto& e : g.en) {
        if (!e.alive || g.over) continue;
        int dx = g.pl.x - e.x, dy = g.pl.y - e.y;
        int dist = std::abs(dx) + std::abs(dy);
        if (dist == 1) { cbEnemyHit(g, e); continue; }
        bool hunt = g.vis[e.y][e.x] || dist <= 4;
        if (hunt) {
            int best = -1, bd = dist;
            for (int k = 0; k < 4; k++) {
                int nx = e.x + DX[k], ny = e.y + DY[k];
                if (!aiFree(g, nx, ny)) continue;
                int nd = std::abs(g.pl.x - nx) + std::abs(g.pl.y - ny);
                if (nd < bd) { bd = nd; best = k; }
            }
            if (best >= 0) { e.x += DX[best]; e.y += DY[best]; }
        } else if (rnI(0, 9) < 3) {
            int k = rnI(0, 3);
            if (aiFree(g, e.x + DX[k], e.y + DY[k])) { e.x += DX[k]; e.y += DY[k]; }
        }
    }
}
