Maude
=====

This directory contains the Maude 2 interpreter source tree.
Copyright 1997-2014 SRI International, Menlo Park, CA 94025, USA.

The Maude 2 interpreter is free software.
See the file COPYING for copying permission.

For documentation and examples, see the Maude website: <http://maude.cs.uiuc.edu/>

For help using Maude 2:	<maude-help@maude.cs.uiuc.edu>
Report bugs to:	<maude-bugs@maude.cs.uiuc.edu>

Installing
==========

The script `./build` will help with building and installing Maude.
Run `./build help` for instructions on how to use.

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
