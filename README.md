# PESADILLA DEL '94 / NIGHTMARE OF '94

Source code of the game for MSDOS Club podcast contest.

Play the game [here](https://onepopcorn.github.io/pesadilla94/) and read the manual to know how to play. RTFM!

This game has been developed using [DJGPP](https://www.delorie.com/djgpp/) compiler under a Windows machine. In order to compile the game you will need to have DJGPP installed correctly on your system.

If you prefer, you can get a pre-built DJGPP version from [here](https://github.com/andrewwutw/build-djgpp?tab=readme-ov-file)

This project also depends on [MikMod](https://mikmod.sourceforge.net/) library. You must download, compile and properly link the library in order to compile the game.

## Get the code

This project uses git submodules for tools dependencies, to get the source code with its dependencies clone the repository using `git clone --recursive`.

## Compile the code

Use `make run` to compile the game and all its dependencies. Use `make clean run` to delete any previous compilation of the game before compiling again and `make clean_all run` to delete both game compiled files and tools compiles files before compiling again.
