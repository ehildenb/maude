Logical Model Checking Parameter Modules
========================================

Here we define several modules with empty operators for the user to instantiate to their systems/tools.

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
