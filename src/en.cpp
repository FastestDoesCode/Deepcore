#include "en.h"
#include "rn.h"

static En enBase(char ch, const std::string& nm, int hp, int at, int df, int xp, int r, int g, int b) {
    En e;
    e.ch = ch;
    e.nm = nm;
    e.hp = e.mh = hp;
    e.at = at;
    e.df = df;
    e.xp = xp;
    e.r = r;
    e.g = g;
    e.b = b;
    return e;
}

En enMake(int dp, int x, int y) {
    int r = rnI(0, 99);
    En e;
    if (dp >= 4 && r < dp * 3) e = enBase('D', "daemon", 12 + dp * 2, 4 + dp / 2, 2, 10, 224, 90, 138);
    else if (r < 55) e = enBase('t', "trojan", 6 + dp * 2, 2 + dp / 2, 1, 5, 232, 196, 106);
    else e = enBase('w', "worm", 3 + dp, 1 + dp / 2, 0, 3, 91, 227, 154);
    e.x = x;
    e.y = y;
    return e;
}

En enBoss(int x, int y) {
    En e = enBase('W', "warden", 50, 8, 3, 40, 255, 120, 90);
    e.x = x;
    e.y = y;
    return e;
}

En enPlayer() {
    En e = enBase('@', "you", 30, 5, 1, 0, 201, 214, 255);
    return e;
}
