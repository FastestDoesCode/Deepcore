#include "mp.h"
#include "rn.h"
#include "en.h"
#include "it.h"
#include <cstdlib>
#include <algorithm>

struct Rm { int x, y, w, h; };

static void mpH(Gs& g, int a, int b, int y) {
    for (int x = std::min(a, b); x <= std::max(a, b); x++) g.mp[y][x] = '.';
}

static void mpV(Gs& g, int a, int b, int x) {
    for (int y = std::min(a, b); y <= std::max(a, b); y++) g.mp[y][x] = '.';
}

bool mpOpen(const Gs& g, int x, int y) {
    if (x < 0 || y < 0 || x >= MW || y >= MH) return false;
    return g.mp[y][x] != '#';
}

int mpEnemyAt(const Gs& g, int x, int y) {
    for (size_t i = 0; i < g.en.size(); i++)
        if (g.en[i].alive && g.en[i].x == x && g.en[i].y == y) return static_cast<int>(i);
    return -1;
}

static bool mpTaken(const Gs& g, int x, int y) {
    if (g.pl.x == x && g.pl.y == y) return true;
    if (g.mp[y][x] != '.') return true;
    if (mpEnemyAt(g, x, y) >= 0) return true;
    for (auto& i : g.it) if (i.x == x && i.y == y) return true;
    return false;
}

static bool mpSpot(const Gs& g, const std::vector<Rm>& rm, int& x, int& y) {
    for (int t = 0; t < 200; t++) {
        const Rm& r = rm[rnI(1, static_cast<int>(rm.size()) - 1)];
        x = rnI(r.x, r.x + r.w - 1);
        y = rnI(r.y, r.y + r.h - 1);
        if (!mpTaken(g, x, y)) return true;
    }
    return false;
}

void mpGen(Gs& g) {
    std::vector<Rm> rm;
    while (rm.size() < 4) {
        rm.clear();
        g.mp.assign(MH, std::string(MW, '#'));
        for (int t = 0; t < 300 && rm.size() < 10; t++) {
            int w = rnI(5, 12), h = rnI(3, 6);
            int x = rnI(1, MW - w - 2), y = rnI(1, MH - h - 2);
            bool ok = true;
            for (auto& r : rm)
                if (x < r.x + r.w + 1 && x + w + 1 > r.x && y < r.y + r.h + 1 && y + h + 1 > r.y) { ok = false; break; }
            if (!ok) continue;
            for (int j = y; j < y + h; j++)
                for (int i = x; i < x + w; i++) g.mp[j][i] = '.';
            if (!rm.empty()) {
                const Rm& p = rm.back();
                int ax = p.x + p.w / 2, ay = p.y + p.h / 2, bx = x + w / 2, by = y + h / 2;
                if (rnI(0, 1)) { mpH(g, ax, bx, ay); mpV(g, ay, by, bx); }
                else { mpV(g, ay, by, ax); mpH(g, ax, bx, by); }
            }
            rm.push_back({x, y, w, h});
        }
    }
    g.seen.assign(MH, std::vector<int>(MW, 0));
    g.vis.assign(MH, std::vector<int>(MW, 0));
    g.en.clear();
    g.it.clear();
    g.pl.x = rm[0].x + rm[0].w / 2;
    g.pl.y = rm[0].y + rm[0].h / 2;
    const Rm& e = rm.back();
    int ex = e.x + e.w / 2, ey = e.y + e.h / 2;
    g.mp[ey][ex] = g.dp >= DMAX ? 'C' : '>';
    int x = 0, y = 0;
    int ne = 3 + g.dp + rnI(0, 2);
    for (int i = 0; i < ne; i++) if (mpSpot(g, rm, x, y)) g.en.push_back(enMake(g.dp, x, y));
    if (g.dp >= DMAX) {
        for (int d = 1; d < 4; d++) {
            if (g.mp[ey][ex - d] == '.' && mpEnemyAt(g, ex - d, ey) < 0) { g.en.push_back(enBoss(ex - d, ey)); break; }
            if (g.mp[ey][ex + d] == '.' && mpEnemyAt(g, ex + d, ey) < 0) { g.en.push_back(enBoss(ex + d, ey)); break; }
        }
    }
    int np = 2 + rnI(0, 1), ns = 3 + g.dp / 2 + rnI(0, 2);
    for (int i = 0; i < np; i++) if (mpSpot(g, rm, x, y)) g.it.push_back({x, y, '+'});
    for (int i = 0; i < ns; i++) if (mpSpot(g, rm, x, y)) g.it.push_back({x, y, '$'});
    if (rnI(0, 1) && mpSpot(g, rm, x, y)) g.it.push_back({x, y, '!'});
}

static bool mpLine(const Gs& g, int x0, int y0, int x1, int y1) {
    int dx = std::abs(x1 - x0), dy = -std::abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1, sy = y0 < y1 ? 1 : -1;
    int er = dx + dy;
    while (true) {
        if (x0 == x1 && y0 == y1) return true;
        if (g.mp[y0][x0] == '#' && !(x0 == g.pl.x && y0 == g.pl.y)) return false;
        int e2 = 2 * er;
        if (e2 >= dy) { er += dy; x0 += sx; }
        if (e2 <= dx) { er += dx; y0 += sy; }
    }
}

void mpFov(Gs& g) {
    const int R = 7;
    for (auto& r : g.vis) std::fill(r.begin(), r.end(), 0);
    for (int y = g.pl.y - R; y <= g.pl.y + R; y++) {
        for (int x = g.pl.x - R; x <= g.pl.x + R; x++) {
            if (x < 0 || y < 0 || x >= MW || y >= MH) continue;
            int dx = x - g.pl.x, dy = y - g.pl.y;
            if (dx * dx + dy * dy > R * R + R) continue;
            if (mpLine(g, g.pl.x, g.pl.y, x, y)) { g.vis[y][x] = 1; g.seen[y][x] = 1; }
        }
    }
}
