# PESADILLA DEL '94 / NIGHTMARE OF '94

Source code of the game for MSDOS Club podcast contest.

Go [here](https://onepopcorn.github.io/pesadilla94/) and read the manual to know how to play. RTFM!

To play the game download the [Spanish version](https://github.com/onepopcorn/pesadilla94/releases/latest/download/pes94_es.zip) or [English Version](https://github.com/onepopcorn/pesadilla94/releases/latest/download/pes94_en.zip) and play it in your emulator of choice or on a real machine (I haven't test it on a real machine). For simplicity, I recommend downloading the game and use [this](https://www.neilb.net/doswasmx/) online emulator

This game has been developed using [DJGPP](https://www.delorie.com/djgpp/) compiler under a Windows machine. In order to compile the game you will need to have DJGPP installed correctly on your system.

If you prefer, you can get a pre-built DJGPP version from [here](https://github.com/andrewwutw/build-djgpp?tab=readme-ov-file)

This project also depends on [MikMod](https://mikmod.sourceforge.net/) library. You must download, compile and properly link the library in order to compile the game.

## Get the code

This project uses git submodules for tools dependencies, to get the source code with its dependencies clone the repository using `git clone --recursive`.

## Compile the code

Use `make all` to compile the game and all its dependencies. Use `make clean all` to delete any previous compilation of the game before compiling again. and `make clean_all all` to delete both game compiled files and tools compiles files before compiling again.

The game can be compiled with some useful flags as follows:

-   `make all CHEATS=on` Enables cheats to debug and test the game. `S` will skip the level `K` will give the player a gift.
-   `make all LANG={value}` Will compile the game in given language. Possible values:
    -   `es` Spanish (default)
    -   `en` English
-   `make all DEBUG={value}` Enables debugging tools. Tools have a performance cost. Possible values:
    -   `collisions` Shows a border box around collision hitboxes.
    -   `tiles` Highlights tiles that are maked as "dirty" and will be repainted in the next frame.
    -   `all` Enables all previous debug tools at the same time.
