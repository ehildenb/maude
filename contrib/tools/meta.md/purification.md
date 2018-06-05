Module Intersection
===================

```maude
load mtemplate.maude
load variables.maude
load eqform.maude
```

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
    var MD : ModuleDecl . var TM : Term .

    op moduleIntersect : ModuleDeclSet ModuleDeclSet -> ModuleDeclSet [assoc comm id: none] .
    -----------------------------------------------------------------------------------------
    eq moduleIntersect( NeMDS  , NeMDS' )     = none [owise] .
    eq moduleIntersect( MD MDS , MD MDS' )    = MD moduleIntersect( MDS , MDS' ) .
    eq moduleIntersect( (sorts S ; SS .) MDS , (sorts S ; SS' .) MDS' )
     = (sorts S .) moduleIntersect( (sorts SS .) MDS , (sorts SS' .) MDS' ) .

    op moduleIntersect : Module           Module           -> Module .
    op moduleIntersect : ModuleExpression ModuleExpression -> Module .
    ------------------------------------------------------------------
    eq moduleIntersect(ME, ME')   = moduleIntersect(upModule(ME, true), upModule(ME', true)) .
    eq moduleIntersect(MOD, MOD') = fromTemplate(qid("INTERSECT{" + string(resolveNames(getName(MOD))) + "," + string(resolveNames(getName(MOD'))) + "}"), moduleIntersect(asTemplate(MOD), asTemplate(MOD'))) .
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
    ceq joint-sort(S, MDS, MDS') = S if S inS moduleIntersect(MDS, MDS') .
    ceq joint-sort(S, MDS, MDS') = joint-sort(S, MDS', MDS) if (not S inS MDS) /\ S inS MDS' .
    ceq joint-sort(S, MDS, MDS') = if S inS MDS'' then S else #top-sort(MDS'') fi
                                if S inS MDS /\ MDS'' := moduleIntersect(connected-component(MDS, (sorts S .)), MDS') .

    op joint-sort : Sort Module           Module           -> Sort .
    op joint-sort : Sort ModuleExpression ModuleExpression -> Sort .
    ----------------------------------------------------------------
    eq joint-sort(S, ME,  ME')  = joint-sort(S, upModule(ME), upModule(ME')) .
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
    eq joint-variable(ME, ME', TM) = joint-variable(upModule(ME, true), upModule(ME', true), TM) .

    op joint-variable : Module Module Term -> Variable .
    ----------------------------------------------------
   ceq joint-variable(MOD, MOD', TM) = #makeVariable(#string(TM), joint-sort(leastSort(MOD, TM),  MOD, MOD'))
                                    if wellFormed(MOD, TM) .
   ceq joint-variable(MOD, MOD', TM) = #makeVariable(#string(TM), joint-sort(leastSort(MOD', TM), MOD', MOD))
                                    if wellFormed(MOD', TM) .
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
   protecting EQFORM .

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
    vars EqC EqC' : EqConj .

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

    vars Q Q' : Qid . vars F F' : Form .
    vars ME ME' : ModuleExpression . vars M M' : Module . var MDS : ModuleDeclSet .
    vars FV : Variable . vars T T' T1 T2 : Term . var T? : [Term] . var CT : CTerm .
    vars NeTL NeTL' : NeTermList . vars TL TL' : TermList . vars TL? TL?' : [TermList] .

    op _in_ : Form Module -> Bool .
    -------------------------------
    eq tt        in M = true .
    eq ff        in M = true .
    eq (~ F)     in M = F in M .
   ceq (F /\ F') in M = (F in M) and (F' in M) if F =/= tt /\ F' =/= tt .
   ceq (F \/ F') in M = (F in M) and (F' in M) if F =/= ff /\ F' =/= ff .
    eq (T ?= T') in M = wellFormed(M, T) and wellFormed(M, T') .
    eq (T != T') in M = wellFormed(M, T) and wellFormed(M, T') .
```

Purifying Equational Conjunctions
---------------------------------

Since in the purification process the order of the modules does not matter, we define `modulePair` a helper to abstract this.

```maude
    sort ModulePair .

    op modulePair : Module Module -> ModulePair [ctor comm] .
    ---------------------------------------------------------
```

Purification first checks if the conjunction is well-formed in one of the modules.
If so, then it leaves it alone, otherwise more work is required on the equational atoms.

```maude
    op purify : ModuleExpression ModuleExpression Form -> [Form] .
    --------------------------------------------------------------
    eq purify(ME, ME', F) = purify(modulePair(upModule(ME, true), upModule(ME', true)), F) .

    op purify : ModulePair Form -> [Form] .
    ---------------------------------------
   ceq purify(modulePair(M, M'), F)       = F if (F in M) .
    eq purify(modulePair(M, M'), ~ F)     = ~ purify(modulePair(M, M'), F) .
   ceq purify(modulePair(M, M'), F /\ F') = purify(modulePair(M, M'), F) /\ purify(modulePair(M, M'), F')
                                         if F =/= tt /\ F' =/= tt .
   ceq purify(modulePair(M, M'), F \/ F') = purify(modulePair(M, M'), F) \/ purify(modulePair(M, M'), F')
                                         if F =/= ff /\ F' =/= ff .
```

If a term in a (dis)equality is not `wellFormed` in either `Module`, then we purify it.
If a (dis)equality has terms pure in the same theory, then it is pure.
If a (dis)equality has terms pure in different theories then we separate it into two
contraints each pure in one of the theories, and forced to be (dis)equal via a `joint-variable`.

TODO: Abstract ?= vs !=

```maude
   ceq purify(modulePair(M, M'), Q[TL] ?= T2) = purify(modulePair(M, M'), purify(M, M', Q[TL]) ?= T2) if not wellFormed(M, Q[TL]) and not wellFormed(M', Q[TL]) and Q inO asTemplate(M) .
   ceq purify(modulePair(M, M'), Q[TL] != T2) = purify(modulePair(M, M'), purify(M, M', Q[TL]) != T2) if not wellFormed(M, Q[TL]) and not wellFormed(M', Q[TL]) and Q inO asTemplate(M) .
   ceq purify(modulePair(M, M'), T1 ?= T2) = break-eqatoms(M, M', T1 ?= T2)                           if     wellFormed(M, T1) and wellFormed(M', T2) and not wellFormed(M, T2) .
   ceq purify(modulePair(M, M'), T1 != T2) = break-eqatoms(M, M', T1 != T2)                           if     wellFormed(M, T1) and wellFormed(M', T2) and not wellFormed(M, T2) .
   ceq purify(modulePair(M, M'), T1 ?= T2) = T1 ?= T2                                                 if     wellFormed(M, T1) and wellFormed(M, T2) .
   ceq purify(modulePair(M, M'), T1 != T2) = T1 != T2                                                 if     wellFormed(M, T1) and wellFormed(M, T2) .
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
    eq purify(M, M', empty)      = empty .
    eq purify(M, M', (T , NeTL)) = purify(M, M', T) , purify(M, M', NeTL) .
   ceq purify(M, M', T st F)     = purify(M, M', T) st purify(modulePair(M, M'), F) if F =/= tt .
```

If the term is well-formed in the first module, return it.
If the top symbol of the term is from the first module, purify the subterms.
Otherwise, generate an equality constraint at the top and purify with respect to the second module.

```maude
   ceq purify(M, M', T)     = T                    if wellFormed(M, T) .
   ceq purify(M, M', T)     = FV st (FV ?= T)      if wellFormed(M', T) /\ FV := joint-variable(M', M, T) .
   ceq purify(M, M', Q[TL]) = Q[purify(M, M', TL)] if not wellFormed(M, Q[TL]) /\ not wellFormed(M', Q[TL]) /\      Q inO asTemplate(M) .
   ceq purify(M, M', Q[TL]) = purify(M, M', CT)    if not wellFormed(M, Q[TL]) /\ not wellFormed(M', Q[TL]) /\ not (Q inO asTemplate(M)) /\ Q inO asTemplate(M')
                                                   /\ CT := purify(M', M, Q[TL]) .
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
   ceq subtheoryPurify(MDS, M, Q[TL])         = FV st FV ?= Q[TL]
                                             if (not Q inO MDS)
                                             /\ FV := #var(Q[TL], leastSort(M, Q[TL])) .
endfm
```

Traversing Terms
================

```
fmod TERM-TRAVERSE is
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
---    ceq purify(ME ;;; ME' ;;; MEL) [Q[TL]] = FV st ((FV ?= T) /\ QFF)    if not Q inO asTemplate(ME)
---                                                                        /\ T st QFF := purify(ME' ;;; MEL ;;; ME) [Q[TL]]
---                                                                        /\ FV      := ??? .
endfm
```
