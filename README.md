# dw4-ps2-ai: Digimon World 4 (PS2), AI-written decompilation

Work-in-progress matching decompilation of **Digimon World 4** (USA, `SLUS-20836`, `VER = 1.01`):
C/C++ that compiles to the same bytes as the original game. The long-term goals are mods and a
PC port.

This repository contains **no game data**. You must provide your own legally obtained copy.

Looking for mods you can play today? See [dw4-mods](https://github.com/Namek025/dw4-mods).

## How this is made

**Most of the C/C++ here was written by an AI agent (Claude)**, one function at a time, through
`tools/agent_loop.py`, under my supervision. I am not an experienced reverse engineer. Repetitive
registration code comes from generators (`tools/meta_src.py`, `tools/trivial_src.py`).

What that means for you:

- A matching function is proven to compile to the original bytes. That says nothing about how
  readable it is, and it does not prove every name or type is right.
- Many functions and fields still have placeholder names (`func_XXXXXXXX`, `unk1C`).
- `src/lua/`, `src/zlib/` and `src/libpng/` are the libraries' original source, with small changes
  marked `port:`. Those weren't written by the agent.
- Functions under `#ifdef NON_MATCHING` do not match: the build uses the original asm for them.

## Progress

Share of `.text` bytes decompiled, from `tools/progress.py`:

| Region | Done | Size |
|---|---|---|
| Game (`be*`) | 35.8% | 936 KB |
| Alchemy engine, zlib and libpng | 22.8% | 1977 KB |
| Lua 4.0.1 | 99.5% | 75 KB |
| CRI middleware | 0.9% | 380 KB |
| Sony SDK and libc | 0% | 197 KB |
| **Total** | **24.2%** | 3568 KB |

The Sony and CRI code is the lowest priority, because a PC port would replace it. The build
reproduces the program's code and data, but not yet the full ELF file (header and sections).

## Target

| File | SHA-1 |
|---|---|
| `SLUS_208.36` | `d6c8bbcd6b3a0b3f135f7559d098af088ba06e2a` |

## Setup (Linux / WSL)

```sh
sudo apt install build-essential git python3-venv ninja-build binutils-mips-linux-gnu
python3 -m venv .venv
. .venv/bin/activate
pip install -r requirements.txt

# EE-GCC 2.96 (the game's compiler). Its binaries are 32-bit i386.
sudo dpkg --add-architecture i386 && sudo apt update && sudo apt install libc6:i386
mkdir -p tools/ee-gcc2.96
curl -L https://github.com/decompme/compilers/releases/download/compilers/ee-gcc2.96.tar.xz \
  | tar -xJ -C tools/ee-gcc2.96

# MWCC 3.0b52 (some game units are CodeWarrior C++), run through wibo.
curl -Lo tools/wibo https://github.com/decompals/wibo/releases/download/1.2.0/wibo-x86_64
chmod +x tools/wibo
mkdir -p tools/mwcps2/3.0b52-030722
curl -L https://github.com/decompme/compilers/releases/download/compilers/mwcps2-3.0b52-030722.tar.gz \
  | tar -xz -C tools/mwcps2/3.0b52-030722

# MWCC 3.0b38 (zlib and libpng were built with this older version).
mkdir -p tools/mwcps2/3.0b38-030307
curl -L https://github.com/decompme/compilers/releases/download/compilers/mwcps2-3.0b38-030307.tar.gz \n  | tar -xz -C tools/mwcps2/3.0b38-030307

# objdiff CLI (per-function diff against the original).
curl -Lo tools/objdiff-cli https://github.com/encounter/objdiff/releases/download/v3.8.2/objdiff-cli-linux-x86_64
chmod +x tools/objdiff-cli
```

Put the extracted disc in `disc/` so that `disc/SLUS_208.36` exists, then check it:

```sh
sha1sum -c config/checksum.sha1
```

Build:

```sh
python3 configure.py   # split with splat (first time) and write build.ninja
ninja                  # build and compare against the original
ninja progress         # how much is decompiled (functions / bytes per unit)
python3 tools/fcmp.py build/src/script_bind.mw.o NAME   # diff one function
tools/objdiff-cli diff -u src/script_bind NAME          # same, interactive (objdiff TUI)
tools/objdiff-cli report generate -o build/report.json  # objdiff progress report
```

`configure.py` also writes `objdiff.json`: per unit, our object against the original
rebuilt from splat's asm (`build/expected/`). The objdiff GUI can open the repo too.

## Contributing

Start with `CLAUDE.md` (compiler flags, memory map, conventions) and
[`docs/matching_tricks.md`](docs/matching_tricks.md). A function only counts as matching if the build still
passes its SHA-1 check. Never commit game data, the ELF or generated asm.

Findings about the game are in [`docs/`](docs/).

## Related projects

- [dw4-gc](https://github.com/ivanno4317/dw4-gc): separate decompilation of the GameCube version
- [dw4-mods](https://github.com/Namek025/dw4-mods): patches and modding tools built from this research

## Contact

Open an issue on this repository, or find me on Discord: `namek`.

## License and legal

Unofficial fan project, not affiliated with the game's rights holders. Digimon, Digimon World 4
and related names belong to their respective owners. This repository grants no license for the
reconstructed game and engine code. Third-party libraries keep their own licenses: Lua 4.0.1
(`src/lua/COPYRIGHT`), zlib 1.1.3 (`src/zlib/zlib.h`) and libpng 1.0.8 (`src/libpng/png.h`).
