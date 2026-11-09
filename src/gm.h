#pragma once
#include <string>
#include <vector>

const int MW = 60;
const int MH = 21;
const int DMAX = 8;

struct En {
    int x = 0, y = 0, hp = 0, mh = 0, at = 0, df = 0, xp = 0;
    char ch = '?';
    std::string nm;
    int r = 255, g = 255, b = 255;
    bool alive = true;
};

struct It {
    int x = 0, y = 0;
    char ch = '?';
};

struct Gs {
    std::vector<std::string> mp;
    std::vector<std::vector<int>> seen, vis;
    std::vector<En> en;
    std::vector<It> it;
    std::vector<std::string> log;
    En pl;
    int dp = 1, sh = 0, lv = 1, xp = 0, nx = 10, oc = 0, kills = 0, turns = 0;
    bool over = false, won = false, qt = false;
};

void gmRun();
