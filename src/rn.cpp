#include "rn.h"
#include <chrono>
#include <cstdlib>

static std::mt19937 rnEngine;

void rnSeed() {
    const char* e = std::getenv("DC_SEED");
    if (e) { rnEngine.seed(static_cast<unsigned>(std::atoi(e))); return; }
    std::random_device rd;
    unsigned t = static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count());
    rnEngine.seed(rd() ^ t);
}

int rnI(int a, int b) {
    std::uniform_int_distribution<int> d(a, b);
    return d(rnEngine);
}

std::mt19937& rnG() { return rnEngine; }
