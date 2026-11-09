#include "tm.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <cctype>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <conio.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#else
#include <termios.h>
#include <unistd.h>
static termios tmOld;
static bool tmRawOn = false;
#endif

static int tmSpeed = 1;

void tmEnd() {
#ifndef _WIN32
    if (tmRawOn) { tcsetattr(STDIN_FILENO, TCSANOW, &tmOld); tmRawOn = false; }
#endif
    std::cout << "\x1b[0m\x1b[?25h\n" << std::flush;
}

void tmInit() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD m = 0;
    if (GetConsoleMode(h, &m)) SetConsoleMode(h, m | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
#else
    if (tcgetattr(STDIN_FILENO, &tmOld) == 0) {
        termios t = tmOld;
        t.c_lflag &= ~(ICANON | ECHO);
        t.c_cc[VMIN] = 1;
        t.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
        tmRawOn = true;
    }
#endif
    if (std::getenv("DC_FAST")) tmSpeed = 0;
    std::atexit(tmEnd);
    std::cout << "\x1b[?25l" << std::flush;
}

void tmSleep(int ms) {
    if (tmSpeed && ms > 0) std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void tmClear() {
    std::cout << "\x1b[2J\x1b[H" << std::flush;
}

void tmType(const std::string& s, int ms) {
    bool esc = false;
    for (char c : s) {
        std::cout << c;
        if (c == '\x1b') esc = true;
        else if (esc) { if (c == 'm') esc = false; }
        else if ((static_cast<unsigned char>(c) & 0xC0) != 0x80) { std::cout << std::flush; tmSleep(ms); }
    }
    std::cout << std::flush;
}

static int tmNorm(int c) {
    if (c < 0) return -1;
    c = std::tolower(c);
    if (c == 'k') return 'w';
    if (c == 'h') return 'a';
    if (c == 'j') return 's';
    if (c == 'l') return 'd';
    return c;
}

int tmKey() {
    std::cout << std::flush;
#ifdef _WIN32
    int c = _getch();
    if (c == 0 || c == 224) {
        int d = _getch();
        if (d == 72) return 'w';
        if (d == 80) return 's';
        if (d == 75) return 'a';
        if (d == 77) return 'd';
        return 0;
    }
    return tmNorm(c);
#else
    unsigned char c = 0;
    if (read(STDIN_FILENO, &c, 1) != 1) return -1;
    if (c == 27) {
        unsigned char a = 0, b = 0;
        if (read(STDIN_FILENO, &a, 1) == 1 && a == '[' && read(STDIN_FILENO, &b, 1) == 1) {
            if (b == 'A') return 'w';
            if (b == 'B') return 's';
            if (b == 'D') return 'a';
            if (b == 'C') return 'd';
        }
        return 0;
    }
    return tmNorm(c);
#endif
}

std::string fg(int r, int g, int b) {
    return "\x1b[38;2;" + std::to_string(r) + ";" + std::to_string(g) + ";" + std::to_string(b) + "m";
}

std::string rs() { return "\x1b[0m"; }
std::string bd() { return "\x1b[1m"; }
