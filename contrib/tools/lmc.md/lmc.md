Logical Model Checking Parameter Modules
========================================

Here we define several modules with empty operators for the user to instantiate to their systems/tools.

Model Checking
--------------

-   `##m##` should be filled in as the module to do simulation of.
-   `##f##` shoudl be filled in as the term list of atomic propositions that will be used.

```maude
fmod META-LMC-PARAMETERS is
   protecting META-MODULE .

    op ##m## : ~> SModule  [memo] .
    op ##f## : ~> TermList [memo] .
endfm
```
