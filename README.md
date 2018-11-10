branch: pr

Maude
=====

This directory contains the Maude 2 interpreter source tree.
Copyright 1997-2014 SRI International, Menlo Park, CA 94025, USA.

The Maude 2 interpreter is free software.
See the file COPYING for copying permission.

For more documentation and examples, see the Maude website: <http://maude.cs.uiuc.edu/>

Installing
==========

The script `./build` will help with building and installing Maude.
Run `./build help` for instructions on how to use.

### Dependencies

-   [GNU gcc](http://gcc.gnu.org/)
-   [GNU bison](http://www.gnu.org/software/bison/)
-   [GNU flex](http://www.gnu.org/software/flex/)

It is recommended that you use the latest versions of bison and flex since Maude is known to tickle bugs in certain older versions.
You should use gcc 3.1 or later as there are code generation issues with the 3.0.* series.
Maude requires the following packages:

On Ubuntu, you can run:

```sh
sudo apt install gcc bison flex
```

Additionally, you'll need:

-   [GNU gmp](http://www.swox.com/gmp/)
-   [GNU libsigsegv](http://libsigsegv.sourceforge.net/)
-   [Tecla](http://www.astro.caltech.edu/~mcs/tecla/)
-   [BuDDY](http://sourceforge.net/projects/buddy)

On Ubuntu, you can run:

```sh
sudo apt install libgmp-dev libsigsegv-dev libtecla-dev libbdd-dev
```

### Optional Dependencies

-   [CVC4](http://cvc4.cs.nyu.edu/web/)
-   [Yices2](https://github.com/SRI-CSL/yices2.git)
-   [GNU gperf (for Yices2)](https://www.gnu.org/software/gperf/)

**NOTE**: There is not an Ubuntu package for Yices2 yet.

On Ubuntu, you can run:

```sh
sudo apt install cvc4 gperf
```

### Building

First build the dependencies:

```sh
./build deps
```

Then build Maude (with or without an SMT solver):

```sh
./build maude           # No SMT Solver
./build maude-cvc4      # CVC4 SMT Solver
./build maude-yices2    # Yices2 SMT Solver
```

Contributing
============

Merging into `master` will only occur when the following have been done:

1.  The feature branch is rebased on `master`.
2.  All tests in the repository pass.
3.  Code review has approved the changes.

Each feature/topic branch should use the following naming prefix scheme.
If your branch falls into two categories, pick the earlier one in this list.

-   `build/FEATURE-NAME`:   Branches which modify/update the build system.
-   `core/FEATURE-NAME`:    Any branch that touches code in `src/` (modifying Core Maude).
-   `tools/FEATURE-NAME`:   Branches which work in the `contrib/tools` directory.
-   `systems/FEATURE-NAME`: Branches which add systems to the `contrib/systems` directory.
-   `doc/FEATURE-NAME`:     Branches which work on repository documentation.

The actual merge into `master` will be performed using the `merge-pr` script in `contrib/scripts`.

```sh
./contrib/scripts/merge-pr BRANCH 'PULL-REQUEST-NAME'
git push upstream master
```

with:

-   `BRANCH` as the branch to be merged.
-   `PULL-REQUEST-NAME` as the GitHub pull-request name.

This creates an empty merge commit which marks the feature addition/bug-fix.
It's assumed that the remote you're pushing to is named `upstream`.
