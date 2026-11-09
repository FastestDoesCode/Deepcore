#include "gm.h"
#include "tm.h"
#include "rn.h"
#include "mp.h"
#include "en.h"
#include "it.h"
#include "ai.h"
#include "cb.h"
#include "lg.h"
#include "rd.h"
#include <iostream>
#include <fstream>
#include <vector>

static const std::string SAVE = "dc.sav";

static int gmBest() {
    std::ifstream f(SAVE);
    int b = 0;
    if (f >> b) return b;
    return 0;
}

static int gmScore(const Gs& g) {
    return g.sh * 10 + g.kills * 5 + g.dp * 100 + (g.won ? 1000 : 0);
}

static void gmDescend(Gs& g) {
    g.dp++;
    mpGen(g);
    mpFov(g);
    if (g.dp >= DMAX) lgAdd(g, fg(255, 210, 120), "> depth " + std::to_string(g.dp) + " :: the core hums nearby. the warden waits.");
    else lgAdd(g, fg(201, 214, 255), "> depth " + std::to_string(g.dp) + " :: the corruption thickens");
}

static bool gmAct(Gs& g, int k) {
    int dx = 0, dy = 0;
    if (k == 'w') dy = -1;
    else if (k == 's') dy = 1;
    else if (k == 'a') dx = -1;
    else if (k == 'd') dx = 1;
    else if (k == '.' || k == ' ') return true;
    else if (k == '>') {
        if (g.mp[g.pl.y][g.pl.x] == '>') { gmDescend(g); return false; }
        lgAdd(g, fg(84, 112, 180), "> no gate here");
        return false;
    } else return false;
    int nx = g.pl.x + dx, ny = g.pl.y + dy;
    int e = mpEnemyAt(g, nx, ny);
    if (e >= 0) { cbPlayerHit(g, g.en[e]); return true; }
    if (!mpOpen(g, nx, ny)) return false;
    g.pl.x = nx;
    g.pl.y = ny;
    itPick(g);
    char c = g.mp[ny][nx];
    if (c == '>') lgAdd(g, fg(201, 214, 255), "> a gate down :: press > to descend");
    if (c == 'C') g.won = true;
    return true;
}

static void gmEnd(Gs& g) {
    tmClear();
    int sc = gmScore(g);
    int best = gmBest();
    if (g.won) {
        rdBanner("DEEPCORE");
        tmType("  " + fg(91, 227, 154) + "THE CORE IS PURGED :: THE MAINFRAME IS YOURS" + rs() + "\n", 25);
    } else {
        std::cout << "\n";
        tmType("  " + fg(224, 90, 138) + "PROCESS TERMINATED AT DEPTH " + std::to_string(g.dp) + rs() + "\n", 25);
    }
    std::string D = fg(84, 112, 180), L = fg(201, 214, 255);
    std::cout << "\n";
    std::cout << "  " << D << "score   " << L << sc << rs() << "\n";
    std::cout << "  " << D << "depth   " << L << g.dp << "/" << DMAX << rs() << "\n";
    std::cout << "  " << D << "purged  " << L << g.kills << rs() << "\n";
    std::cout << "  " << D << "shards  " << L << g.sh << rs() << "\n";
    if (sc > best) {
        std::ofstream f(SAVE);
        f << sc;
        std::cout << "  " << fg(232, 196, 106) << "new record" << rs() << "\n";
    } else {
        std::cout << "  " << D << "record  " << L << best << rs() << "\n";
    }
    std::cout << "\n  " << D << "[ any key ]" << rs() << std::flush;
    tmKey();
}

static void gmPlay() {
    Gs g;
    g.pl = enPlayer();
    mpGen(g);
    mpFov(g);
    lgAdd(g, fg(127, 162, 240), "> uplink stable :: descend to depth " + std::to_string(DMAX) + " and purge the core");
    tmClear();
    while (true) {
        rdDraw(g);
        int k = tmKey();
        if (k < 0 || k == 'q') return;
        if (!gmAct(g, k)) continue;
        g.turns++;
        if (g.oc > 0) g.oc--;
        if (g.won) { rdDraw(g); tmSleep(400); gmEnd(g); return; }
        aiTurn(g);
        mpFov(g);
        if (g.over) { rdDraw(g); tmSleep(700); gmEnd(g); return; }
    }
}

static void gmHow() {
    tmClear();
    std::string D = fg(84, 112, 180), L = fg(201, 214, 255), A = fg(127, 162, 240);
    std::vector<std::string> T = {
        "you are a process sent down into a corrupted mainframe.",
        "reach depth " + std::to_string(DMAX) + ", get past the warden and touch the core.",
        "",
        "move with wasd, arrows or hjkl. walk into enemies to attack.",
        "wait with . and take the gate down with > when standing on it.",
        "",
        "w worm      weak and common",
        "t trojan    tougher, hits harder",
        "D daemon    deep levels only, dangerous",
        "W warden    guards the core",
        "",
        "+ patch     restores hp",
        "$ shard     score",
        "! overclock +3 attack for 15 turns",
        "",
        "purging enemies grants xp. levels raise hp, attack and defence."
    };
    std::cout << "\n  " << bd() << L << "MANUAL" << rs() << "\n\n";
    for (auto& t : T) std::cout << "  " << (t.size() > 1 && t[1] == ' ' ? A : L) << t << rs() << "\n";
    std::cout << "\n  " << D << "[ any key ]" << rs() << std::flush;
    tmKey();
}

void gmRun() {
    tmInit();
    rnSeed();
    while (true) {
        tmClear();
        rdBanner("DEEPCORE");
        std::string D = fg(84, 112, 180), L = fg(201, 214, 255), A = fg(127, 162, 240);
        std::cout << "  " << D << "a mainframe descent roguelike  ::  (c) 2026 Fastest" << rs() << "\n";
        std::cout << "  " << D << "record " << L << gmBest() << rs() << "\n\n";
        std::cout << "  " << A << "[n] " << L << "new descent" << rs() << "\n";
        std::cout << "  " << A << "[m] " << L << "manual" << rs() << "\n";
        std::cout << "  " << A << "[q] " << L << "disconnect" << rs() << "\n" << std::flush;
        int k = tmKey();
        if (k < 0 || k == 'q') break;
        if (k == 'n') gmPlay();
        else if (k == 'm') gmHow();
    }
    tmClear();
}
