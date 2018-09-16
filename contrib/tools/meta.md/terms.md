Term Handling
=============

Term Sets
---------

```maude
fmod TERM-SET is
   protecting META-TERM .

    sorts NeTermSet TermSet .
    -------------------------
    subsort Term < NeTermSet < TermSet .

    var T : Term . var TS : TermSet . var NeTL : NeTermList .

    op .TermSet : -> TermSet [ctor] .
    op _|_ : TermSet   TermSet ->   TermSet [ctor assoc comm id: .TermSet] .
    op _|_ : TermSet NeTermSet -> NeTermSet [ctor ditto] .
    ------------------------------------------------------
    eq T | T = T .

    op _in_ : Term TermSet -> Bool .
    --------------------------------
    eq T in (T | TS) = true .
    eq T in TS       = false [owise] .

    op asSet : TermList -> TermSet .
    --------------------------------
    eq asSet(empty)    = .TermSet .
    eq asSet(T)        = T .
    eq asSet((T,NeTL)) = T | asSet(NeTL) .
endfm
```

Substitution Sets
-----------------

```maude
fmod SUBSTITUTION-SET is
   protecting TERM-SET .

    sort SubstitutionSet NeSubstitutionSet .
    ----------------------------------------
    subsort Substitution < NeSubstitutionSet < SubstitutionSet .

    vars SUB : Substitution . var NeSUBS : NeSubstitutionSet . var SUBS : SubstitutionSet .
    var T : Term . var C : Constant . vars V V' : Variable . var Q : Qid .
    var NeTL : NeTermList . var TL : TermList .
    var NeTS : NeTermSet . var TS : TermSet .

    op .SubstitutionSet :                                   ->   SubstitutionSet [ctor] .
    op _|_              : SubstitutionSet   SubstitutionSet ->   SubstitutionSet [ctor assoc comm id: .SubstitutionSet] .
    op _|_              : SubstitutionSet NeSubstitutionSet -> NeSubstitutionSet [ctor ditto] .
    -------------------------------------------------------------------------------------------
    eq SUB | SUB = SUB .

    op _<<_ : Term Substitution -> Term .
    -------------------------------------
    eq    TL << none              = TL .
    eq     C << SUB               = C .
    eq     V << ((V' <- T) ; SUB) = if V == V' then T else V << SUB fi .
    eq     V << none              = V .
    eq Q[TL] << SUB               = Q[TL << SUB] .

    op _<<_ : TermList Substitution -> TermList .
    ---------------------------------------------
    eq empty     << SUB = empty .
    eq (T, NeTL) << SUB = (T << SUB, NeTL << SUB) .

    op _<<_ : TermSet SubstitutionSet -> TermSet .
    ----------------------------------------------
    eq TS         << .SubstitutionSet = .TermSet .
    eq .TermSet   << SUBS             = .TermSet .
    eq T          << (SUB | NeSUBS)   = (T << SUB)    | (T    << NeSUBS) .
    eq (T | NeTS) << NeSUBS           = (T << NeSUBS) | (NeTS << NeSUBS) .
endfm
```

Term Comparison
---------------

```maude
fmod TERM-COMPARISON is
   protecting SUBSTITUTION-SET .
   protecting META-LEVEL .

    vars N M M' : Nat . vars T T' : Term . var MOD : Module .
    var S : Substitution . var CTX : Context .

    op #unifiers : Module Term Term -> SubstitutionSet .
    op #unifiers : Module Term Term Nat Nat -> SubstitutionSet .
    ------------------------------------------------------------
    eq #unifiers(MOD, T, T')       = #unifiers(MOD, T, T', 0, 0) .
   ceq #unifiers(MOD, T, T', N, M) = S | #unifiers(MOD, T, T', s(N), M') if { S , M' } := metaUnify(MOD, T =? T', N, M) .
    eq #unifiers(MOD, T, T', N, M) = .SubstitutionSet [owise] .

    op #subsumesWith : Module Term Term -> SubstitutionSet .
    op #subsumesWith : Module Term Term Nat -> SubstitutionSet .
    ------------------------------------------------------------
    eq #subsumesWith(MOD, T, T')    = #subsumesWith(MOD, T, T', 0) .
   ceq #subsumesWith(MOD, T, T', N) = S | #subsumesWith(MOD, T, T', s(N)) if S := metaMatch(MOD, T, T', nil, N) .
    eq #subsumesWith(MOD, T, T', N) = .SubstitutionSet [owise] .

    op #subsumesXWith : Module Term Term -> SubstitutionSet .
    op #subsumesXWith : Module Term Term Nat -> SubstitutionSet .
    -------------------------------------------------------------
    eq #subsumesXWith(MOD, T, T')    = #subsumesXWith(MOD, T, T', 0) .
   ceq #subsumesXWith(MOD, T, T', N) = S | #subsumesXWith(MOD, T, T', s(N)) if { S , CTX } := metaXmatch(MOD, T, T', nil, 0, unbounded, N) .
    eq #subsumesXWith(MOD, T, T', N) = .SubstitutionSet [owise] .
endfm
```
