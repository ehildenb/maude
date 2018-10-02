Narrowing
=========

Several narrowing utilities/wrappers are provided here.
Much of this code is extracted from Santiago Escobar's [unification.md].

```maude
load cterms.maude
```

```maude
load terms.maude
```

Variant Sets
------------

```maude
fmod VARIANT-SET is
   protecting META-LEVEL .
   protecting TERM-SET .

    var N N' : Nat . var P : Parent . var B : Bool .
    var M : Module . var T : Term . var SUB : Substitution .
    vars V V' : Variant . var VS : VariantSet .

    sort VariantSet .
    -----------------
    subsort Variant < VariantSet .

    op .VariantSet :                       -> VariantSet [ctor] .
    op _#_         : VariantSet VariantSet -> VariantSet [ctor assoc comm id: .VariantSet prec 65 format (d d n d)] .
    -----------------------------------------------------------------------------------------------------------------
    eq V # V = V .

    op variants : Module Term -> VariantSet .
    -----------------------------------------
    eq variants(M, T) = allVariants(M, T, 0) .

    op allVariants : Module Term Nat -> [VariantSet] .
    --------------------------------------------------
    eq allVariants(M, T, N) = .VariantSet [owise].
   ceq allVariants(M, T, N) = V # allVariants(M, T, N + 1)
    if V := metaGetIrredundantVariant(M, T, empty, 0, N) .

    op getTerms : VariantSet -> TermSet .
    -------------------------------------
    eq getTerms(.VariantSet)            = .TermSet .
    eq getTerms({T, SUB, N, P, B} # VS) = T | getTerms(VS) .
endfm
```

Narrowing using Core Maude
--------------------------

```maude
fmod NARROWING is
   protecting VARIANT-SET .

    sorts NarrowStepResult NeNarrowStepResults NarrowStepResults .
    --------------------------------------------------------------
    subsort NarrowStepResult < NeNarrowStepResults < NarrowStepResults .

    op {_:_,_} : Qid Term Substitution -> NarrowStepResult .
    --------------------------------------------------------

    op .NarrowStepResults :                                       ->   NarrowStepResults .
    op _||_               : NarrowStepResults   NarrowStepResults ->   NarrowStepResults [assoc comm id: .NarrowStepResults] .
    op _||_               : NarrowStepResults NeNarrowStepResults -> NeNarrowStepResults [ditto] .
    ----------------------------------------------------------------------------------------------

    var N : Nat . vars Q RL : Qid .
    var TYPE : Type . var CTX : Context .
    var M : Module . vars T T' : Term . vars SUBST TSUBST RLSUBST : Substitution .

    op allNarrowSteps : Module Term Nat -> [NarrowStepResults] .
    ------------------------------------------------------------
    eq allNarrowSteps(M, T, N) = .NarrowStepResults [owise] .
   ceq allNarrowSteps(M, T, N) = { RL : T' , TSUBST } || allNarrowSteps(M, T, N + 1)
    if { T' , TYPE , CTX , RL , TSUBST , RLSUBST , Q } := metaNarrowingApply(M, T, empty, '#, N) .

    op narrowSteps : Module Term -> NarrowStepResults .
    ---------------------------------------------------
    eq narrowSteps(M, T) = allNarrowSteps(M, T, 0) .
endfm
```
