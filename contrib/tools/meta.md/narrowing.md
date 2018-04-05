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

Single Narrowing Steps
----------------------

```maude
fmod NARROWING-STEP-SET is
   protecting RESULT-CONTEXT-SET .

    sorts StepResult StepResult? .
    ------------------------------
    subsort StepResult < StepResult? .

    op failure   : -> StepResult? [ctor] .
    op {_,_,_/_} : Term Type Substitution RuleSet -> StepResult [ctor] .
    --------------------------------------------------------------------

    sort StepResultSet .
    --------------------
    subsort StepResult < StepResultSet .

    var M : Module . var N : Nat . vars S S' : Substitution .
    vars T TS CtTS : Term . var TL : TermList . var TP : Type . var R : Rule .
    var Tr : TraceNarrow . var B : Flags . vars Ct CtS : Context .
    var SR : StepResult . var RTS : ResultContextSet . var SRS : StepResultSet .

    op .StepResultSet :                             -> StepResultSet [ctor] .
    op _||_           : StepResultSet StepResultSet -> StepResultSet [ctor assoc comm id: .StepResultSet prec 65] .
    ---------------------------------------------------------------------------------------------------------------
    eq SR || SR = SR .

    op to4Tuple : Module ResultContextSet -> StepResult .
    -----------------------------------------------------
    eq to4Tuple(M, empty) = .StepResultSet .
    eq to4Tuple(M, {T,TP,S,S',Ct,CtS,TS,CtTS,N,Tr,B} | RTS )
     = {CtTS, leastSort(M,CtTS), S .. S' / getRuleSet(Tr)} || to4Tuple(M, RTS) .

    op _|>_ : StepResult TermList -> StepResult .
    ---------------------------------------------
    eq .StepResultSet        |> TL = .StepResultSet .
    eq ({T,TP,S / R} || SRS) |> TL = {T,TP,S |> TL / R} || (SRS |> TL) .
    eq (failure      || SRS) |> TL = failure || (SRS |> TL) .

    op getRuleSet : TraceNarrow -> RuleSet .
    ----------------------------------------
    eq getRuleSet(nil)           = none .
    eq getRuleSet({T,S,TP,R} Tr) = R getRuleSet(Tr) .
endfm
```

Narrowing using Santiago Escobar's Code
---------------------------------------

```maude
fmod NARROWING is
   protecting VARIANT-SET * ( op getTerms to getVariantTerms ) .
   protecting NARROWING-STEP-SET .
   protecting META-NARROWING-SEARCH .

    vars N N' : Nat . var M : Module . var T : Term . var V : Variant .

    op metaNarrow2 : Module Term -> StepResult .
    --------------------------------------------
    eq metaNarrow2(M,T) = to4Tuple(M,metaENarrowShowAll(M,T,1,full noStrategy BuiltIn-unify)) |> T .
endfm
```

Narrowing using Core Maude
--------------------------

```maude
fmod NARROWING2 is
   protecting VARIANT-SET .
   protecting NARROWING-STEP-SET .
   protecting SUBSTITUTIONSET .

    var N : Nat . vars Q RL : Qid .
    var TYPE : Type . var CTX : Context .
    var M : Module . vars T T' : Term . vars SUBST TSUBST RLSUBST : Substitution .

    op allNarrowSteps : Module Term Nat -> [StepResultSet] .
    --------------------------------------------------------
    eq allNarrowSteps(M, T, N) = .StepResultSet [owise] .
   ceq allNarrowSteps(M, T, N) = { T' , TYPE , TSUBST / getRls(RL, M) } || allNarrowSteps(M, T, N + 1)
    if { T' , TYPE , CTX , RL , TSUBST , RLSUBST , Q } := metaNarrowingApply(M, T, empty, '#, N) .

    op metaNarrow2 : Module Term -> StepResult .
    --------------------------------------------
    eq metaNarrow2(M,T) = allNarrowSteps(M, T, 0) .
endfm
```
