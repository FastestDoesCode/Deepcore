#pragma once
#include <string>
void tmInit();
void tmEnd();
void tmSleep(int ms);
void tmClear();
void tmType(const std::string& s, int ms);
int tmKey();
std::string fg(int r, int g, int b);
std::string rs();
std::string bd();
