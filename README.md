Maude
=====

This directory contains the Maude 2 interpreter source tree.
Copyright 1997-2014 SRI International, Menlo Park, CA 94025, USA.

The Maude 2 interpreter is free software.
See the file COPYING for copying permission.

For documentation and examples, see the Maude website: <http://maude.cs.uiuc.edu/>

For help using Maude 2:	<maude-help@maude.cs.uiuc.edu>
Report bugs to:	<maude-bugs@maude.cs.uiuc.edu>

Contributing
============

There are two main trunk branches:

master:
:   History of master branch never changes, and it should always be in a "good" state.
    Only receives merges from the develop branch, and only with commits that have been stable for at least 14 days.

develop:
:   History of develop branch can change sometimes, tests should always pass on the develop branch.
    Feature branches are merged into the develop branch only *after*:

    1.  The feature branch is rebased on develop.
    2.  All tests in the repository pass.
    3.  Code review has approved the changes.

Each feature/topic branch should follow this naming prefix scheme:

core/FEATURE-NAME:
:   Any branch that touches code in `src/` (modifying Core Maude).

tools/FEATURE-NAME:
:   Branches which work in the `contrib/tools` directory.

systems/FEATURE-NAME:
:   Branches which add systems to the `contrib/systems` directory.

doc/FEATURE-NAME:
:   Branches which work on repository documentation.
