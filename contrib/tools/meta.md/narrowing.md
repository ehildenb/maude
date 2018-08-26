Narrowing
=========

Several narrowing utilities/wrappers are provided here.

```maude
load unification.maude
```

Variant Sets
------------

```maude
fmod VARIANT-SET is
    protecting RENAMING .

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

    op metaGenVariant : Module Term -> VariantSet .
    -----------------------------------------------
    eq metaGenVariant(M, T) = metaGenVariant(M, T, highestVar(T)) .

    op metaGenVariant : Module Term Nat -> VariantSet .
    ---------------------------------------------------
    eq metaGenVariant(M, T, N) = metaGenVariant2(M, T, N, 0) .

    op metaGenVariant2 : Module Term Nat Nat -> VariantSet .
    --------------------------------------------------------
    eq metaGenVariant2(M, T, N', N) = .VariantSet [owise] .
   ceq metaGenVariant2(M, T, N', N) = V # metaGenVariant2(M,T,N',s N)
    if V := metaGetVariant(M,T,empty,N',N) .

    op getTerms : VariantSet -> TermList .
    --------------------------------------
    eq getTerms(.VariantSet) = empty .
    eq getTerms(V # V' # VS) = getTerms(V) , getTerms(V') , getTerms(VS) .

    eq getTerms( { T , SUB , N , P , B } ) = T .
endfm
```

Narrowing using Core Maude
--------------------------

```maude
fmod NARROWING is
   protecting VARIANT-SET .
   protecting SUBSTITUTIONSET .

    sorts NarrowStepResult NarrowStepResults .
    ------------------------------------------
    subsort NarrowStepResult < NarrowStepResults .

    op .NarrowStepResults :                                     -> NarrowStepResults .
    op _||_               : NarrowStepResults NarrowStepResults -> NarrowStepResults [assoc comm id: .NarrowStepResults] .
    ----------------------------------------------------------------------------------------------------------------------

    op {_:_,_} : Qid Term Substitution -> NarrowStepResult .
    --------------------------------------------------------

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
