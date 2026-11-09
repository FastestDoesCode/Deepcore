# DEEPCORE

A roguelike descent into a corrupted mainframe. Fight down through eight procedurally generated depths, get past the warden and touch the core. One life, no saves.

```
████  █████ █████ ████   ████  ███  ████  █████
█   █ █     █     █   █ █     █   █ █   █ █
█   █ ████  ████  ████  █     █   █ ████  ████
█   █ █     █     █     █     █   █ █  █  █
████  █████ █████ █      ████  ███  █   █ █████
```

## Controls

| Key | Action |
|-----|--------|
| `w a s d`, arrows or `h j k l` | move, or attack by walking into an enemy |
| `.` or space | wait a turn |
| `>` | take the gate down when standing on it |
| `q` | quit to menu |

## The mainframe

| Glyph | What it is |
|-------|------------|
| `@` | you |
| `w` | worm, weak and common |
| `t` | trojan, tougher and hits harder |
| `D` | daemon, deep levels only and dangerous |
| `W` | warden, guards the core |
| `+` | patch, restores hp |
| `$` | data shard, score |
| `!` | overclock, +3 attack for 15 turns |
| `>` | gate to the next depth |
| `◆` | the core |

Purging enemies grants xp. Levelling up raises hp, attack and defence. Your best score is saved between runs.

## Build

**Windows**

1. Install a C++ compiler. Open PowerShell and run:

   ```
   winget install -e --id BrechtSanders.WinLibs.POSIX.UCRT
   ```

2. Close and reopen PowerShell so it finds the new compiler.
3. Download this repo (green **Code** button, then **Download ZIP**) and unzip it.
4. Double-click `build.bat`. It ends with `Built deepcore.exe`.
5. Double-click `deepcore.exe` to play. It looks best in Windows Terminal.

If `build.bat` says no compiler was found, reopen PowerShell or restart your PC and try again. It also works with Visual Studio's compiler when run from a Developer Command Prompt.

**Linux and macOS**

```
make
./deepcore
```

**CMake (any platform)**

```
cmake -B build
cmake --build build
```

Needs a C++17 compiler and a terminal with colour and UTF-8 support. Windows Terminal works best.

## License

MIT, copyright (c) 2026 Fastest. See [LICENSE](LICENSE).
