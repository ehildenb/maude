Constrained Terms
=================

Constrained terms are a term coupled with a FO-formula (provided by `foform.maude`).

We should make the first part of a constrained term use Stephen Skeirik's pattern operations when it detects that the term falls in a module where that is safe.
The formulas present are being broken into different categories based on what we have decision procedures for, so it's only natural that we use the pattern operations when they are safe.

We also need to extend FOForm to handle terms which fall into CVC4's abilities directly.
This will require infrastructure to determine whether part of a formula downs correctly into a module.

```maude
load mtemplate.maude
load foform.maude

fmod CTERM-SET is
   protecting FOFORM-SUBSTITUTION + FOFORMSIMPLIFY-IMPL + FOFORM-DEFINEDOPS .
   protecting META-LEVEL-EXT .

    sorts CTerm NeCTermSet CTermSet CTermSet? .
    -------------------------------------------
    subsorts Term < CTerm < NeCTermSet < CTermSet < CTermSet? .
    subsort TermSet < CTermSet .

    var Q : Qid . vars S S' : Substitution . var SS : SubstitutionSet .
    var MOD : Module . var X : Variable . vars T T' : Term . vars TML? TML?' : [TermList] .
    vars CT CT' : CTerm . vars CTS CTS' : CTermSet . vars NeCTS NeCTS' : NeCTermSet .
    vars F F' F'' : FOForm . vars EqC EqC' : EqConj .

    op _|_ : Term FOForm -> CTerm [right id: tt prec 52] .
    ------------------------------------------------------
    eq  T        | ff                = .CTermSet .
    eq (T | EqC) | EqC'              = T | (EqC /\ EqC') .
    eq Q [ TML? , (T | EqC), TML?' ] = Q[TML?, T, TML?'] | EqC .

    op _<<_ : CTerm    Substitution    -> CTerm .
    op _<<_ : CTermSet SubstitutionSet -> CTermSet .
    ------------------------------------------------
    eq .CTermSet     << SS = .CTermSet .
    eq (CT ;; NeCTS) << SS = (CT << SS) ;; (NeCTS << SS) .
    eq CT << empty         = .CTermSet .
    eq CT << (S | S' | SS) = (CT << S) ;; (CT << S') ;; (CT << SS) .
   ceq (T | F)       << S  = (T << S) | (F << S)
    if not (F == tt) .

    op .CTermSet : -> CTermSet .
    op _;;_ : CTermSet CTermSet   -> CTermSet   [ctor assoc comm id: .CTermSet prec 60] .
    op _;;_ : CTermSet CTermSet?  -> CTermSet?  [ctor ditto] .
    op _;;_ : CTermSet NeCTermSet -> NeCTermSet [ctor ditto] .
    ----------------------------------------------------------
    eq NeCTS ;; NeCTS = NeCTS .

    op _[_] : CTermSet? Module -> [CTermSet] [prec 64] .
    ----------------------------------------------------
    eq CTS [ MOD ] = CTS .

    op _++_ : CTermSet? CTermSet? -> CTermSet? [assoc comm id: .CTermSet prec 61] .
    -------------------------------------------------------------------------------
    eq NeCTS ;; CTS ++ NeCTS ;; CTS'  = NeCTS ;; CTS ++ CTS' .
    eq NeCTS        ++ NeCTS' [ MOD ] = NeCTS ;; NeCTS' [owise] .

   ceq T | F ;; CTS ++ CT' ;; CTS' [ MOD ] = T | F'' ;; CTS ++ CTS' [ MOD ]
    if T' | F' := #varsApart(CT', T | F)
    /\ S | SS  := #subsumesWith(MOD, T, T')
    /\ F''     := F \/ (F' /\ #disjSubsts(S | SS)) .

    op _--_ : CTermSet? CTermSet? -> CTermSet? [right id: .CTermSet prec 62] .
    --------------------------------------------------------------------------
    eq .CTermSet    -- NeCTS          = .CTermSet .
    eq NeCTS ;; CTS -- NeCTS ;; CTS'  = CTS -- NeCTS ;; CTS' .
    eq CT ;; NeCTS  -- NeCTS' [ MOD ] = (CT -- NeCTS' [ MOD ]) ;; (NeCTS -- NeCTS' [ MOD ]) .
    eq NeCTS        -- NeCTS' [ MOD ] = NeCTS [owise] . --- Over-approximate when we can't simplify

   ceq CT    -- CT' ;; CTS'  [ MOD ] = .CTermSet
    if S | SS := #subsumesWith(MOD, CT', #varsApart(CT, CT')) .

   ceq T | F -- CT' ;; CTS'  [ MOD ] = CT -- CTS' [ MOD ]
    if T' | F' := #varsApart(CT', T | F)
    /\ S | SS  := #subsumesWith(MOD, T, T')
    /\ CT      := (T | F /\ (#disjSubsts(S | SS) => (~ F'))) .

   ceq CT    -- CT' ;; CTS   [ MOD ] = CT -- CTS' ;; CTS [ MOD ]
    if CTS' := #intersect(MOD, CT, CT') .

    op #intersect : Module CTerm CTerm -> CTermSet? .
    -------------------------------------------------
   ceq #intersect(MOD, T | F, CT') = (T | F /\ F') << (S | SS)
    if T' | F' := #varsApart(CT', T | F)
    /\ S | SS  := #unifiers(MOD, T, T') .
```

This should either be implemented, hooked up to an existing implementation, or we should convince ourselves it's not needed.

```maude
    op #varsApart : CTerm CTerm -> [CTerm] .
    ----------------------------------------

    op #disjSubsts : SubstitutionSet -> PosEqQFForm? .
    --------------------------------------------------
    eq #disjSubsts(empty)  = ff .
    eq #disjSubsts(S | SS) = #conjSubst(S) \/ #disjSubsts(SS) .

    op #conjSubst : Substitution -> PosEqConj? .
    --------------------------------------------
    eq #conjSubst(none)       = tt .
    eq #conjSubst(X <- T ; S) = X ?= T /\ #conjSubst(S) .

    --- TODO: This should eventually actually try to do what it claims.
    op #varsApart : CTerm CTerm -> CTerm .
    --------------------------------------
    eq #varsApart(CT, CT') = CT .
```

`EqConj` is extended to handle constrained terms.
It's not clear that we always want to separate a constraint from its term (the commented equations).

```maude
    op _?=_ : CTerm CTerm -> EqConj [ditto] .
    op _!=_ : CTerm CTerm -> EqConj [ditto] .
    -----------------------------------------
---    eq TM? ?= (TM | EqC) = (TM? ?= TM) /\ EqC .
---    eq TM? != (TM | EqC) = (TM? != TM) /\ EqC .
endfm
```

A trace of constrained terms is just a map from naturals to pairs constrained-term sets.
The first element of the pair are the states seen for the first time in that step, and the second element are the accumulated states up to that point.

```maude
fmod CTERM-TRACE is
   protecting CTERM-SET .

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

Module Intersection
===================

This module computes module intersections over `ModuleDeclSet`.
The intersection operation is lifted to `Module` via `asTemplate`.

```maude
fmod INTERSECTION is
   protecting MODULE-TEMPLATE .
   protecting DETERMINISTIC-VARIABLES .

    var B : Bool . vars TL : TypeList . var T : Type . var AS : AttrSet .
    var OPD : OpDecl . var OPDS : OpDeclSet .
    vars MOD MOD' M0 : Module . vars ME ME' : ModuleExpression .
    vars Q Q' : Qid . vars H H' : Header . vars S S' : Sort . vars SS SS' : SortSet .
    vars NeMDS NeMDS' : NeModuleDeclSet . vars MDS MDS' MDS'' : ModuleDeclSet .

    op intersect : ModuleDeclSet ModuleDeclSet -> ModuleDeclSet [assoc comm id: none] .
    -----------------------------------------------------------------------------------
    eq intersect( NeMDS     , NeMDS' )     = none [owise] .
    eq intersect( NeMDS MDS , NeMDS MDS' ) = NeMDS intersect( MDS , MDS' ) .
    eq intersect( (sorts S ; SS .) MDS , (sorts S ; SS' .) MDS' )
     = (sorts S .) intersect( (sorts SS .) MDS , (sorts SS' .) MDS' ) .

    op intersect : Module Module -> Module .
    op intersect : ModuleExpression ModuleExpression -> Module .
    ------------------------------------------------------------
    eq intersect(MOD, MOD') = fromTemplate(qid("INTERSECT{" + string(resolveNames(getName(MOD))) + "," + string(resolveNames(getName(MOD'))) + "}"), intersect(asTemplate(MOD), asTemplate(MOD'))) .
    eq intersect(ME, ME')   = intersect(upModule(ME, true), upModule(ME', true)) .
```

It will often be useful to know if a sort or an operator is in a `ModuleDeclSet`.

```maude
    op _inS_ : Sort ModuleDeclSet -> Bool .
    op _inO_ : Qid  ModuleDeclSet -> Bool .
    --------------------------------------
    eq S inS ( sorts SS . )            MDS = S in SS .
    eq Q inO ( op Q : TL -> T [AS] . ) MDS = true .
    eq S inS MDS = false [owise] .
    eq Q inO MDS = false [owise] .
```

Joint Sorts
-----------

Calculating the joint sort of a sort `S` in connected components `C1` and `C2` corresponding to modules `MOD` and `MOD'`:

-   If `S` is a sort of `C1 /\ C2`, return `S`;
-   If not, return the maximal sort in `C1 /\ C2`.

Right now we just take *any* maximal sort in `C1 /\ C2`, but we really need to take the correct maximal sort.

```maude
    op joint-sort : Sort ModuleDeclSet ModuleDeclSet -> Sort .
    ----------------------------------------------------------
    ceq joint-sort(S, MDS, MDS') = S if S inS intersect(MDS, MDS') .
    ceq joint-sort(S, MDS, MDS') = joint-sort(S, MDS', MDS) if (not S inS MDS) /\ S inS MDS' .
    ceq joint-sort(S, MDS, MDS') = if S inS MDS'' then S else #top-sort(MDS'') fi
                                if S inS MDS /\ MDS'' := intersect(connected-component(MDS, (sorts S .)), MDS') .

    op joint-sort : Sort Module           Module           -> Sort .
    op joint-sort : Sort ModuleExpression ModuleExpression -> Sort .
    ----------------------------------------------------------------
    eq joint-sort(S, ME,  ME')  = joint-sort(S, intersect(ME, ME'), intersect(ME, ME')) .
    eq joint-sort(S, MOD, MOD') = joint-sort(S, asTemplate(MOD), asTemplate(MOD')) .

    op #top-sort : ModuleDeclSet -> [Sort] .
    ----------------------------------------
    eq #top-sort( ( sorts S ; SS . )   MDS ) = top-sort( ( sorts S ; SS . )   MDS , S ) .
    eq #top-sort( ( subsort S < S' . ) MDS ) = top-sort( ( subsort S < S' . ) MDS , S ) .
```

Sometimes it's necessary to calculate a joint-variable of a term in the intersection of two modules.

```maude
    op joint-variable : ModuleExpression ModuleExpression Term -> Variable .
    ------------------------------------------------------------------------
    eq joint-variable(ME, ME', T) = joint-variable(upModule(ME, true), upModule(ME', true), T) .

    op joint-variable : Module Module Term -> Variable .
    ----------------------------------------------------
   ceq joint-variable(MOD, MOD', T) = #makeVariable(#string(T), joint-sort(leastSort(MOD, T),  MOD, MOD'))
                                   if wellFormed(MOD, T) .
   ceq joint-variable(MOD, MOD', T) = #makeVariable(#string(T) ,joint-sort(leastSort(MOD', T), MOD', MOD))
                                   if wellFormed(MOD', T) .
endfm
```

Breaking Equalities
===================

Breaking equality atoms means taking an equality atom between terms of different modules and reforming them:

-   `T ?= T'` goes to `x ?= T /\ x ?= T'` for `x` a variable of sort common to `T` and `T'`.
-   `T != T'` goes to `x ?= T /\ x != T'` for `x` a variable of sort greater than that `T'`.

```maude
fmod BREAK-EQATOMS is
   protecting INTERSECTION .
   protecting FOFORM .

    vars EqC EqC' : EqConj . vars MOD MOD' : Module . vars ME ME' : ModuleExpression .
    vars T T' : Term . var NV : Variable .

    op break-eqatoms : Module Module EqConj -> EqConj .
    op break-eqatoms : ModuleExpression ModuleExpression EqConj -> EqConj .
    -----------------------------------------------------------------------
    eq break-eqatoms(ME,  ME',  EqC)         = break-eqatoms(upModule(ME, true), upModule(ME', true), EqC) .
    eq break-eqatoms(MOD, MOD', EqC /\ EqC') = break-eqatoms(MOD, MOD', EqC) /\ break-eqatoms(MOD, MOD', EqC') .

    ceq break-eqatoms(MOD, MOD', T ?= T') = T ?= NV /\ T' ?= NV if not (T :: Variable or T' :: Variable)
                                                                /\ NV := joint-variable(MOD, MOD', T) .
    ceq break-eqatoms(MOD, MOD', T != T') = T ?= NV /\ T' != NV if not (T :: Variable or T' :: Variable)
                                                                /\ NV := joint-variable(MOD, MOD', T)
                                                                /\ sortLeq(MOD, leastSort(MOD, T), leastSort(MOD, NV)) .
    ceq break-eqatoms(MOD, MOD', T != T') = T ?= NV /\ T' != NV if not (T :: Variable or T' :: Variable)
                                                                /\ NV := joint-variable(MOD, MOD', T)
                                                                /\ sortLeq(MOD', leastSort(MOD', T), leastSort(MOD', NV)) .
endfm
```

Variable Abstraction
====================

Input:
:   One module `M`, and an Equational Conjunction: a conjunction of
    equations of the form `T ?= T'` and `T != T'`.

Output:
:   A new Equational Conjunction, where each Equational Atom is of the form
    `T1 = T2`, where either `T1` or `T2` (or both) is a variable, and the other
    is of the form `f(V1, ..., Vn)` for variables `V1` through `Vn`.

```maude
fmod VABS is
   protecting BREAK-EQATOMS .

    vars V FV : Variable . var C : Constant . var Q : Qid . vars T T' T'' : Term .
    vars TL TL' : TermList . vars NeTL NeTL' : NeTermList .
    var ME : ModuleExpression . var M : Module .
    var EqA : EqAtom . vars EqC EqC' : EqConj . var TA : TruthAtom .

    op vabs : ModuleExpression EqConj -> [EqConj] .
    -----------------------------------------------
    eq vabs(ME, EqC) = vabs(upModule(ME, true), EqC) .
```

`abstracted : Term -> Bool` returns true if the current is abstracted.

```maude
    op abstracted : Term -> Bool .
    ------------------------------
    eq  abstracted(T)             = true [owise] .
    ceq abstracted(Q[TL, T, TL']) = false if not (T :: Variable) .
```

The operation `vabs : Module EqConj -> EqConj` takes in an equational conjunction and variable abstracts all the equational atoms.

```maude
    op vabs : Module EqConj -> [EqConj] .
    -------------------------------------
    eq  vabs(M, EqC /\ EqC') = vabs(M, EqC) /\ vabs(M, EqC') .
    ceq vabs(M, V ?= T)      = V ?= T if abstracted(T) .
    ceq vabs(M, V != T)      = V != T if abstracted(T) .
```

If the top terms in the equality atom are both not variables and abstracted, then `break-eqatoms` is used to make it a conjunct of simple equalities.

```maude
    ceq vabs(M, T ?= T') = break-eqatoms(M, M, T ?= T') if not (T :: Variable or T' :: Variable) /\ abstracted(T) /\ abstracted(T') .
    ceq vabs(M, T != T') = break-eqatoms(M, M, T != T') if not (T :: Variable or T' :: Variable) /\ abstracted(T) /\ abstracted(T') .
```

If there is a non-variable subterm, it is extracted as an equality.
Both the remaining subterms and the new equality are variable abstracted.

```maude
    ceq vabs(M, Q[TL, T, TL'] ?= T') = vabs(M, Q[TL, FV, TL'] ?= T') /\ vabs(M, FV ?= T) if not (T :: Variable) /\ FV := joint-variable(M, M, T) .
    ceq vabs(M, Q[TL, T, TL'] != T') = vabs(M, Q[TL, FV, TL'] != T') /\ vabs(M, FV ?= T) if not (T :: Variable) /\ FV := joint-variable(M, M, T) .
endfm
```

Purification
============

Purification ensures that a conjunction of equational atoms has sub-conjuncts strictly formed of equational atoms from the individual theories.
The resulting formulas will always be of the form

-   `T ?= T'` or `T != T'` with `T` and `T'` in the same signature.
-   `x ?= T` or `x != T` with `x` a variable of appropriate sort in the signature of `T`.

```maude
fmod PURIFICATION is
   protecting META-LEVEL .
   protecting BREAK-EQATOMS .
   protecting CTERM-SET .

    var Q : Qid . var TA : TruthAtom . vars EqC EqC' : EqConj . var QFF : QFForm .
    vars ME ME' : ModuleExpression . vars M M' : Module . var MDS : ModuleDeclSet .
    vars FV : Variable . vars T T' T1 T2 : Term . var T? : [Term] .
    vars NeTL NeTL' : NeTermList . vars TL TL' : TermList . vars TL? TL?' : [TermList] .

    op _in_ : EqConj Module -> Bool .
    ---------------------------------
    eq TA            in M = true .
    eq (EqC /\ EqC') in M = (EqC in M) and (EqC' in M) .
    eq (T ?= T')     in M = wellFormed(M, T) and wellFormed(M, T') .
    eq (T != T')     in M = wellFormed(M, T) and wellFormed(M, T') .
```

Purifying Equational Conjunctions
---------------------------------

Purification first checks if the conjunction is well-formed in one of the modules.
If so, then it leaves it alone, otherwise more work is required on the equational atoms.

```maude
    op purify : ModuleExpression ModuleExpression EqConj -> [EqConj] .
    ------------------------------------------------------------------
    eq purify(ME, ME', EqC) = purify(upModule(ME, true), upModule(ME', true), EqC) .

    op purify : Module Module EqConj -> [EqConj] .
    ----------------------------------------------
    ceq purify(M, M', EqC)         = EqC if (EqC in M) or (EqC in M') .
    eq  purify(M, M', EqC /\ EqC') = purify(M, M', EqC) /\ purify(M, M', EqC') .
```

If one of the sides of the equality is not in the first module, purify it with respect to the first module.
Note that the equational atoms are commutative, which handles the reversed case.

```maude
    ceq purify(M, M', T1 ?= T2) = purify(M, M', purify(M, M', T1) ?= T2) if not wellFormed(M, T1) .
    ceq purify(M, M', T1 != T2) = purify(M, M', purify(M, M', T1) != T2) if not wellFormed(M, T1) .
```

Purifying Terms
---------------

Purifying a term means finding alien sub-terms and extracting them as equality constraints.
This is lifted to `TermList` in the normal way.
Note that we take advantage of the fact that generated constraints will bubble to the top.

```maude
    op purify : ModuleExpression ModuleExpression TermList -> [CTerm] .
    -------------------------------------------------------------------
    eq purify(ME, ME', TL) = purify(upModule(ME, true), upModule(ME', true), TL) .

    op purify : Module Module TermList -> [CTerm] .
    -----------------------------------------------
    eq purify(M, M', empty)          = empty .
    eq purify(M, M', (NeTL , NeTL')) = purify(M, M', NeTL) , purify(M, M', NeTL') .
```

If the term is well-formed in the first module, return it.
If the top symbol of the term is from the first module, purify the subterms.
Otherwise, generate an equality constraint at the top and purify with respect to the second module.

```maude
    ceq purify(M, M', T)     = T                       if wellFormed(M, T) .
    ceq purify(M, M', Q[TL]) = Q[purify(M, M', TL)]    if Q inO asTemplate(M) .
    ceq purify(M, M', Q[TL]) = FV | ((FV ?= T) /\ QFF) if (not Q inO asTemplate(M)) /\ Q inO asTemplate(M')
                                                       /\ T | QFF := purify(M', M, Q[TL])
                                                       /\ FV      := joint-variable(M', M, T) .
```

Sometimes, we need to make sure that a term contains only symbols from a given subtheory.

```maude
    op subtheoryPurify : ModuleDeclSet Module TermList -> [TermList] .
    ------------------------------------------------------------------
    eq subtheoryPurify(MDS, M, empty)         = empty .
    eq subtheoryPurify(MDS, M, (NeTL, NeTL')) = subtheoryPurify(MDS, M, NeTL) , subtheoryPurify(MDS, M, NeTL') .
   ceq subtheoryPurify(MDS, M, T)             = T
                                             if wellFormed(fromTemplate('TMP, MDS), T) .
   ceq subtheoryPurify(MDS, M, Q[TL])         = Q[subtheoryPurify(MDS, M, TL)]
                                             if Q inO MDS .
   ceq subtheoryPurify(MDS, M, Q[TL])         = FV | FV ?= Q[TL]
                                             if (not Q inO MDS)
                                             /\ FV := #var(Q[TL], leastSort(M, Q[TL])) .
endfm
```

Traversing Terms
================

```
fmod TERM-TRAVERSE is
   protecting FOFORMSIMPLIFY-IMPL .
   protecting META-LEVEL .
   protecting BREAK-EQATOMS .

    var TO TO' : TermOp . var C : Constant . var V : Variable . var Q : Qid .
    var T : Term . var NeTL NeTL' : NeTermList . var TL : TermList .

    sorts TermOp ModuleExpressionList .
    subsort ModuleExpression < ModuleExpressionList .

    op _;_  : TermOp TermOp -> TermOp [assoc] .
    op _[_] : TermOp TermList -> [TermList] .
    -----------------------------------------
    eq (TO ; TO')[T]    = TO'[TO[T]] .
    eq TO[NeTL , NeTL'] = TO[NeTL] , TO[NeTL'] .

    op onChildren : TermOp -> TermOp .
    ----------------------------------
    eq onChildren(TO)[C]     = C .
    eq onChildren(TO)[V]     = V .
    eq onChildren(TO)[Q[TL]] = Q[TO[TL]] .

    op traverseUp   : TermOp -> TermOp .
    op traverseDown : TermOp -> TermOp .
    ------------------------------------
    eq traverseUp  (TO)[T] = TO[onChildren(traverseUp(TO))[T]] .
    eq traverseDown(TO)[T] = onChildren(traverseDown(TO))[TO[T]] .

    op _;;;_ : ModuleExpressionList ModuleExpressionList -> ModuleExpressionList [assoc] .
    --------------------------------------------------------------------------------------

    op purify : ModuleExpressionList -> TermOp .
    --------------------------------------------
    ceq purify(ME ;;; MEL          [T]     = T                          if wellFormed(M, T) .
    ceq purify(ME ;;; MEL)         [Q[TL]] = Q[purify((ME ;; MEL))[TL]] if Q inO asTemplate(ME) .
---    ceq purify(ME ;;; ME' ;;; MEL) [Q[TL]] = FV | ((FV ?= T) /\ QFF)    if not Q inO asTemplate(ME)
---                                                                        /\ T | QFF := purify(ME' ;;; MEL ;;; ME) [Q[TL]]
---                                                                        /\ FV      := ??? .
endfm
```
