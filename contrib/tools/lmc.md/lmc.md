Logical Model Checking Parameter Modules
========================================

Here we define several modules with empty operators for the user to instantiate to their systems/tools.

```maude
load ../meta/mtransform.maude
```

Model Checking
--------------

-   `#M` should be filled in as the module to do simulation of.
-   `#FS` should be filled in as the term list of atomic propositions that will be used.

```maude
fmod META-LMC-PARAMETERS is
   protecting META-MODULE .

    op #M  : -> [SModule]  [memo] .
    op #FS : -> [TermList] [memo] .
    -------------------------------
endfm
```

Conditional Model Checking
--------------------------

-   `#MO` should be filled as the original module with conditional rules instead of `#M`.
-   The parameters of module `UNCONDITIONALIZE` should be filled in as well.

```maude
fmod META-CONDITIONAL-LMC-PARAMETERS is
   protecting META-LMC-PARAMETERS .
   protecting UNCONDITIONALIZE .

    op #MO : -> [SModule] [memo] .
    ------------------------------
    eq #M = unconditionalize(#MO) .
endfm
```
