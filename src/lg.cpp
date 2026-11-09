#include "lg.h"
#include "tm.h"

void lgAdd(Gs& g, const std::string& c, const std::string& t) {
    g.log.push_back(c + t + rs());
    if (g.log.size() > 4) g.log.erase(g.log.begin());
}
