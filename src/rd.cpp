#include "rd.h"
#include "tm.h"
#include "bn.h"
#include "mp.h"
#include <iostream>
#include <sstream>

std::string rdBar(int v, int m, int n) {
    int f = m > 0 ? v * n / m : 0;
    std::string s;
    for (int i = 0; i < n; i++) s += i < f ? "█" : "░";
    return s;
}

void rdBanner(const std::string& w) {
    auto ln = bnRender(w);
    int sh[5][3] = {{201, 214, 255}, {170, 190, 250}, {127, 162, 240}, {95, 128, 210}, {60, 92, 170}};
    std::cout << "\n";
    for (int i = 0; i < 5; i++) std::cout << "  " << fg(sh[i][0], sh[i][1], sh[i][2]) << ln[i] << rs() << "\n";
    std::cout << "\n";
}

static std::string rdCell(const Gs& g, int x, int y) {
    bool v = g.vis[y][x], s = g.seen[y][x];
    if (!s) return " ";
    if (g.pl.x == x && g.pl.y == y) return bd() + fg(g.pl.r, g.pl.g, g.pl.b) + "@" + rs();
    if (v) {
        int e = mpEnemyAt(g, x, y);
        if (e >= 0) { const En& n = g.en[e]; return bd() + fg(n.r, n.g, n.b) + std::string(1, n.ch) + rs(); }
    }
    for (auto& i : g.it) {
        if (i.x != x || i.y != y) continue;
        if (i.ch == '+') return (v ? fg(91, 227, 154) : fg(40, 100, 70)) + "+" + rs();
        if (i.ch == '$') return (v ? fg(127, 162, 240) : fg(50, 70, 120)) + "$" + rs();
        return (v ? fg(232, 196, 106) : fg(110, 92, 50)) + "!" + rs();
    }
    char c = g.mp[y][x];
    if (c == '#') return (v ? fg(60, 90, 170) : fg(28, 42, 84)) + "█" + rs();
    if (c == '>') return bd() + (v ? fg(201, 214, 255) : fg(84, 112, 180)) + ">" + rs();
    if (c == 'C') return bd() + (v ? fg(255, 210, 120) : fg(140, 110, 60)) + "◆" + rs();
    return (v ? fg(84, 112, 180) : fg(32, 46, 88)) + "·" + rs();
}

void rdDraw(const Gs& g) {
    std::ostringstream o;
    std::string A = fg(127, 162, 240), L = fg(201, 214, 255), D = fg(84, 112, 180), N = fg(26, 48, 99);
    o << "\x1b[H";
    o << "  " << bd() << L << "DEEPCORE" << rs() << D << "  ::  depth " << L << g.dp << D << "/" << DMAX
      << "  ::  lv " << L << g.lv << D << "  ::  shards " << L << g.sh << D << "  ::  turn " << L << g.turns << rs() << "\x1b[K\n";
    std::string hc = g.pl.hp * 3 > g.pl.mh * 2 ? fg(91, 227, 154) : (g.pl.hp * 3 > g.pl.mh ? fg(232, 196, 106) : fg(224, 90, 138));
    o << "  " << D << "hp " << hc << rdBar(g.pl.hp, g.pl.mh, 20) << " " << L << g.pl.hp << D << "/" << g.pl.mh
      << "   atk " << L << g.pl.at << (g.oc > 0 ? fg(232, 196, 106) + "+3" : "") << D << "   def " << L << g.pl.df
      << D << "   xp " << A << rdBar(g.xp, g.nx, 10) << rs() << "\x1b[K\n";
    o << "  " << N;
    for (int i = 0; i < MW; i++) o << "━";
    o << rs() << "\x1b[K\n";
    for (int y = 0; y < MH; y++) {
        o << "  ";
        for (int x = 0; x < MW; x++) o << rdCell(g, x, y);
        o << "\x1b[K\n";
    }
    o << "  " << N;
    for (int i = 0; i < MW; i++) o << "━";
    o << rs() << "\x1b[K\n";
    for (size_t i = 0; i < 4; i++) o << "  " << (i < g.log.size() ? g.log[i] : "") << "\x1b[K\n";
    o << "  " << N << "[wasd/arrows] move  [.] wait  [>] descend  [q] quit" << rs() << "\x1b[K\x1b[J";
    std::cout << o.str() << std::flush;
}
