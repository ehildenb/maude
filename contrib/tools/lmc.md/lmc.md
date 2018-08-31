Logical Model Checking Parameter Modules
========================================

Here we define several modules with empty operators for the user to instantiate to their systems/tools.

```maude
load ../meta/mtransform.maude
```

Model Checking
--------------

-   `#M` should be filled in as the module to do simulation of.
-   `#FS` shoudl be filled in as the term list of atomic propositions that will be used.

```maude
fmod META-LMC-PARAMETERS is
   protecting META-MODULE .

    op #M  : ~> SModule  [memo] .
    op #FS : ~> TermList [memo] .
endfm
```

Conditional Model Checking
--------------------------

-   `#MO` should be filled as the module with the original system.
-   `#MC` should be filled as the module to resolve conditions in.
-   `#T` should be filled as the topmost rewriting sort of `#M` from `META-LMC-PARAMETERS`.
-   `#C` should be filled as the sort of conditions.
-   `#ST` should be filled as a suitable operator for holding a tuple of `#T` and `#C` (which won't clash with operators from `#M` or `#MC`).

```maude
fmod META-CONDITIONAL-LMC-PARAMETERS is
   protecting META-LMC-PARAMETERS .
   protecting UNCONDITIONALIZE .

    op #MC : ~> SModule [memo] .
    op #MO : ~> SModule [memo] .
    op #T  : ~> Sort    [memo] .
    op #C  : ~> Sort    [memo] .
    op #ST : ~> Qid     [memo] .
    ----------------------------

    op #M : ~> SModule [memo] .
    ---------------------------
    eq #M = unconditionalize(#T, #C, #ST, getName(#MC), #MO) .
endfm
```
