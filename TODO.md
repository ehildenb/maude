Next Release
============

-   Get Maude to allow loading from subdirectories of `MAUDE_LIB`. Then we can
    point `MAUDE_LIB` at `contrib/tools`, and people can say
    `load base/full-maude.maude`, or `load rl/rl.maude`.

-   Make `./build` smarter about when it needs to build CVC4 or not. Perhaps
    include building that in Automake process? Not sure how well Automake
    interacts with git submodules. Check [this Stack Overflow post] for
    inspiration.

[this Stack Overflow post]: https://stackoverflow.com/questions/11776382/how-to-handle-subprojects-with-autotools

Bugs
====

-   Figure out why `load contrib/tools/rl/rl.maude` causes an
    `ins nested too deeply` error in Maude. The function is `includeFile` in
    file `src/Mixfix/lexerAux.cc`. Somehow when `full-maude.maude` is loaded,
    even though the variable `inStackPtr` is only at 6, it gets bumped to the
    maximum (which is 10), which causes an advisory.

Features
========

-   Get `./build` script to warn about missing packages/dependencies, and
    suggest ways to fix it in common cases (eg. `libtecla`, `libbdd`). Get list
    of dependencies that Stephen collected.

Amorphous
=========

-   Better loading semantics. Right now Maude is horrendously slow loading a
    large chain of files.

-   Split full-maude up into many different components. Often tools are only
    depending on a small subset of the modules in there.

-   Enforce a coding style standard over the C++ code to make it clearer to
    read.

-   Enforce a coding style standard over the Maude code to make it easier to
    switch between Maude packages.

-   Increase inter-opability of Maude files.


Nelson Oppen
============

- Generalize to multiple modules
