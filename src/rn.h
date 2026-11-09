#pragma once
#include <random>
#include <vector>
#include <string>
void rnSeed();
int rnI(int a, int b);
std::mt19937& rnG();
template <class T> T& rnPick(std::vector<T>& v) { return v[rnI(0, static_cast<int>(v.size()) - 1)]; }
