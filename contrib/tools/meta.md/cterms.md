Constrained Terms
=================

Constrained terms are a term coupled with a FO-formula (provided by `foform.maude`).

We should make the first part of a constrained term use Stephen Skeirik's pattern operations when it detects that the term falls in a module where that is safe.
The formulas present are being broken into different categories based on what we have decision procedures for, so it's only natural that we use the pattern operations when they are safe.

We also need to extend FOForm to handle terms which fall into CVC4's abilities directly.
This will require infrastructure to determine whether part of a formula downs correctly into a module.

```maude
load foform.maude
load terms.maude
```

Constrained Terms
-----------------

```maude
fmod CTERM-SET is
   protecting FOFORM-SUBSTITUTION + FOFORMSIMPLIFY-IMPL + FOFORM-DEFINEDOPS .
   protecting TERM-COMPARISON .

    sorts CTerm NeCTermSet CTermSet .
    ---------------------------------
    subsorts Term < CTerm < NeCTermSet < CTermSet .
    subsort  NeTermSet < NeCTermSet .
    subsort    TermSet <   CTermSet .

    var Q : Qid . var S : Substitution . var SS : SubstitutionSet . var NeSS : NeSubstitutionSet .
    vars T T' : Term . vars TML? TML?' : [TermList] .
    var CT : CTerm . var NeCTS : NeCTermSet .
    var F : FOForm . vars EqC EqC' : EqConj .

    op _st_ : Term FOForm -> CTerm [right id: tt prec 52] .
    -------------------------------------------------------
    eq  T         st ff               = .TermSet .
    eq (T st EqC) st EqC'             = T st (EqC /\ EqC') .
    eq Q [ TML? , (T st EqC), TML?' ] = Q[TML?, T, TML?'] st EqC .

    op _|_ : CTermSet   CTermSet ->   CTermSet [ctor ditto] .
    op _|_ : CTermSet NeCTermSet -> NeCTermSet [ctor ditto] .
    ---------------------------------------------------------
    eq CT | CT = CT .

    op _<<_ : CTerm    Substitution    -> CTerm .
    op _<<_ : CTermSet SubstitutionSet -> CTermSet .
    ------------------------------------------------
   ceq (T st F) << S = (T << S) st (F << S)
                    if not (F == tt) .

    eq CT           << .SubstitutionSet = .TermSet .
    eq CT           << (S | NeSS)       = (CT << S) | (CT << NeSS) .
    eq (CT | NeCTS) << SS               = (CT << SS) | (NeCTS << SS) .
```

`EqConj` is extended to handle constrained terms.

```maude
    op _?=_ : CTerm CTerm -> [FOForm] [ditto] .
    op _!=_ : CTerm CTerm -> [FOForm] [ditto] .
    -------------------------------------------
   ceq CT ?= (T' st F) = (CT ?= T') /\ F if F =/= tt .
   ceq CT != (T' st F) = (CT != T') /\ F if F =/= tt .
endfm
```

Constrained terms can be simplified with respect to a given module.

```maude
fmod CTERM-SET-SIMPLIFICATION is
   protecting CTERM-SET .

    var Q : Qid . vars S S' : Substitution . var SS : SubstitutionSet . var NeSS : NeSubstitutionSet .
    var MOD : Module . var X : Variable . vars T T' : Term . vars TML? TML?' : [TermList] .
    vars CT CT' : CTerm . vars CTS CTS' : CTermSet . vars NeCTS NeCTS' : NeCTermSet .
    vars F F' F'' : FOForm .

    sort CTermSet? .
    ----------------
    subsort CTermSet < CTermSet? .

    op _[_] : CTermSet? Module -> [CTermSet] [prec 64] .
    ----------------------------------------------------
    eq CTS [ MOD ] = CTS .

    op _++_ : CTermSet? CTermSet? -> CTermSet? [assoc comm id: .TermSet prec 61] .
    ------------------------------------------------------------------------------
    eq NeCTS | CTS ++ NeCTS | CTS'   = NeCTS | CTS ++ CTS' .
    eq NeCTS       ++ NeCTS' [ MOD ] = NeCTS | NeCTS' [owise] .

   ceq (T st F) | CTS ++ CT' | CTS' [ MOD ] = (T st F'') | CTS ++ CTS' [ MOD ]
    if T' st F' := #varsApart(CT', T st F)
    /\ S | SS   := #subsumesWith(MOD, T, T')
    /\ F''      := F \/ (F' /\ #disjSubsts(S | SS)) .

    op _--_ : CTermSet? CTermSet? -> CTermSet? [right id: .TermSet prec 62] .
    -------------------------------------------------------------------------
    eq .TermSet    -- NeCTS          = .TermSet .
    eq NeCTS | CTS -- NeCTS | CTS'   = CTS -- NeCTS | CTS' .
    eq CT | NeCTS  -- NeCTS' [ MOD ] = (CT -- NeCTS' [ MOD ]) | (NeCTS -- NeCTS' [ MOD ]) .
    eq NeCTS       -- NeCTS' [ MOD ] = NeCTS [owise] . --- Over-approximate when we can't simplify

   ceq CT -- CT' | CTS' [ MOD ] = .TermSet
                               if S | SS := #subsumesWith(MOD, CT', #varsApart(CT, CT')) .

   ceq T  st F -- CT' | CTS' [ MOD ] = CT -- CTS' [ MOD ]
                                    if T' st F' := #varsApart(CT', T st F)
                                    /\ S | SS   := #subsumesWith(MOD, T, T')
                                    /\ CT       := (T st F /\ (#disjSubsts(S | SS) => (~ F'))) .

   ceq CT -- CT' | CTS [ MOD ] = CT -- CTS' | CTS [ MOD ]
                              if CTS' := #intersect(MOD, CT, CT') .

    op #intersect : Module CTerm CTerm -> CTermSet? .
    -------------------------------------------------
   ceq #intersect(MOD, T st F, CT') = (T st F /\ F') << (S | SS)
                                   if T' st F' := #varsApart(CT', T st F)
                                   /\ S | SS   := #unifiers(MOD, T, T') .
```

This should either be implemented, hooked up to an existing implementation, or we should convince ourselves it's not needed.

```maude
    op #varsApart : CTerm CTerm -> [CTerm] .
    ----------------------------------------

    op #disjSubsts : SubstitutionSet -> PosEqQFForm? .
    --------------------------------------------------
    eq #disjSubsts(.SubstitutionSet) = ff .
    eq #disjSubsts(S | SS)           = #conjSubst(S) \/ #disjSubsts(SS) .

    op #conjSubst : Substitution -> PosEqConj? .
    --------------------------------------------
    eq #conjSubst(none)       = tt .
    eq #conjSubst(X <- T ; S) = X ?= T /\ #conjSubst(S) .

    --- TODO: This should eventually actually try to do what it claims.
    op #varsApart : CTerm CTerm -> CTerm .
    --------------------------------------
    eq #varsApart(CT, CT') = CT .
endfm
```

A trace of constrained terms is just a map from naturals to pairs constrained-term sets.
The first element of the pair are the states seen for the first time in that step, and the second element are the accumulated states up to that point.

```maude
fmod CTERM-TRACE is
   protecting CTERM-SET-SIMPLIFICATION .

    sorts CTermSetPair CTermSetPairMap CTermSetTrace .

    op <_,_> : CTermSet CTermSet -> CTermSetPair .
    ----------------------------------------------

    op .CTermSetPairMap : -> CTermSetPairMap .
    op _|->_ : Nat CTermSetPair -> CTermSetPairMap [prec 64] .
    op __    : CTermSetPairMap CTermSetPairMap -> CTermSetPairMap [assoc comm id: .CTermSetPairMap prec 65 format(d n d)] .
    -----------------------------------------------------------------------------------------------------------------------

    op .CTermSetTrace : -> CTermSetTrace .
    op _|_            : Nat CTermSetPairMap -> CTermSetTrace [prec 66] .
    --------------------------------------------------------------------
    eq .CTermSetTrace = 0 | .CTermSetPairMap .
endfm
```
