Module Transformations
======================

```maude
load purification.maude
load mconstruction.maude
load variables.maude
```

\newcommand{\R}{\mathcal{R}}

Theory transformations take advantage of Maude's *reflective* nature.
Assume throughout a rewrite theory $\R = (\Sigma, B \cup E, R)$.

Subtheory Abstraction
---------------------

When rewrite theory $\R$ protects sub-theory $(\Sigma_1, B_1 \cup E_1)$, define a new theory $(\Sigma, B \cup E, R')$ with:

-   TODO: Make `EqTheory` a sub-sort of `ModuleDeclSet`?
-   TODO: Make `Rule < CRule`, with `op _if_ : Rule Condition -> Rule [right id: true]` for easier matching.
-   TODO: Bring in code from `PURIFICATION` module.

```maude
fmod SUBTHEORY-ABSTRACTION is
   protecting CTERM-SET-SIMPLIFICATION .
   protecting MODULE-TEMPLATE .
   protecting PURIFICATION .

    var MD : ModuleDecl . var NeMDS : NeModuleDeclSet . vars M M' : Module .
    vars L L' R R' : Term . var C : Condition . var EqC : EqCondition . var AS : AttrSet .
    vars CL' CR' : Form .

    op unif : EqConj -> EqCondition .
    ---------------------------------

    op subtheoryAbstract : ModuleDeclSet Module Module -> [ModuleDeclSet] .
    -----------------------------------------------------------------------
    eq subtheoryAbstract(none,     M, M') = none .
   ceq subtheoryAbstract(MD,       M, M') = MD if not (MD :: Rule) .
    eq subtheoryAbstract(MD NeMDS, M, M') = subtheoryAbstract(MD, M, M') subtheoryAbstract(NeMDS, M, M') .

   ceq subtheoryAbstract(( rl L => R [AS] . ), M, M') = ( crl L' => R' if unif(CL' /\ CR') [AS] . )
    if L' st CL' := subtheoryPurify(asTemplate(M), M', L)
    /\ R' st CR' := subtheoryPurify(asTemplate(M), M', R) .

   ceq subtheoryAbstract(( crl L => R if C [AS] . ), M, M') = ( crl L' => R' if C /\ unif(CL' /\ CR') [AS] . )
    if L' st CL' := subtheoryPurify(asTemplate(M), M', L)
    /\ R' st CR' := subtheoryPurify(asTemplate(M), M', R) .

    op subtheoryAbstract : Module Module -> [Module] .
    --------------------------------------------------
    eq subtheoryAbstract(M, M') = fromTemplate(getName(M'), subtheoryAbstract(asTemplate(M'), M, M')) .
endfm
```

Note that when the sub-theory $(\Sigma_1, B_1 \cup E_1)$ is FVP, the above theory transformation enables variant unification to be used for narrowing.

Build-in Equations
------------------

If the equational fragment has a finitary unification algorithm, then we can "build" them into the rules.

-   TODO: Define `#eqnsModule` function.
-   TODO: Define `#variantsFrom` function.

```
fmod BUILDIN-EQUATIONS is
   protecting MODULE-TEMPLATE .
   protecting UNIFICATION .

    op #eqnsModule : Module -> [Module] .
    -------------------------------------

    op #buildinEqns : ModuleDeclSet Module -> [ModuleDeclSet] .
    -----------------------------------------------------------
    eq #buildinEqns(IS SDS SSDS MAS EQS, M) = IS SDS SSDS MAS EQS .
    eq #buildinEqns(NeMDS NeMDS',        M) = #buildinEqns(NeMDS, M) #buildinEqns(NeMDS', M) .

   ceq #buildinEqns(( crl L => R if C [AS] . ), M) = ( crl L => R if C [AS] . ) << SUBSTS
    if SUBSTS := #variantsFrom('_|_[L,R], #eqnsModule(M)) .
endfm
```

Removing Conditional Equations/Rules
------------------------------------

```maude
fmod UNCONDITIONALIZE is
   protecting META-LEVEL .
   protecting MODULE-TEMPLATE .
   protecting DETERMINISTIC-VARIABLES .

    vars Q OP : Qid . var M : Module . var FM : FModule . vars S S' CS : Sort .
    vars T T' C' : Term . var AS : AttrSet . var C : Condition . var V : Variable .
    var H : Header . var MDS : ModuleDeclSet .
    vars NeMDS NeMDS' : NeModuleDeclSet .
    var IS : ImportDeclSet . var SDS : SortDeclSet .
    var SSDS : SubsortDeclSet . var OPDS : OpDeclSet . var MAS : MembAxSet .
    var EQS : EquationSet . var RLS : RuleSet .

    op cSort : Sort -> Sort .
    -------------------------
    eq cSort(S) = qid("C" + string(S)) .

    op rmConditions : Sort Sort Qid ModuleDeclSet -> [ModuleDeclSet] .
    ------------------------------------------------------------------
    eq rmConditions(S, CS, OP, IS SDS SSDS OPDS MAS EQS) = IS SDS SSDS OPDS MAS EQS .
    eq rmConditions(S, CS, OP, NeMDS NeMDS')             = rmConditions(S, CS, OP, NeMDS) rmConditions(S, CS, OP, NeMDS') .
```

The following rules add an extra variable to each rule to capture the accumulated condition.
Currently, we assume syntax `'_/\_` exists to conjunct the conditions together (though the transformation could probably be made parametric in this).
Note that we must transform both conditional and unconditional rules, so that narrowing substitutions into the original term also instantiate variables in the condition.
This also assumes that the given theory is topmost.

```maude
   ceq rmConditions(S, CS, OP, rl T => T' [AS] .) = ( rl OP[T, V] => OP[T', V] [AS] . )
    if V := #var((T, T'), CS) .

   ceq rmConditions(S, CS, OP, crl T => T' if C [AS] .) = ( rl OP[T, V] => OP[T', '_/\_[V, C']] [AS] . )
    if C' := upTerm(C)
    /\ V  := #var((T, T', C'), CS) .

    op unconditionalize : Sort Sort Qid Qid ModuleDeclSet -> [ModuleDeclSet] .
    --------------------------------------------------------------------------
   ceq unconditionalize(S, CS, OP, Q, MDS)
     = ( pr Q . )
       ( sorts cSort(S) . )
       ( op OP : S CS -> cSort(S) [none] . )
       rmConditions(S, CS, OP, MDS)
    if S' := qid("C" + string(S)) .

    op unconditionalize : Sort Sort Qid Qid Module -> [Module] .
    ------------------------------------------------------------
    eq unconditionalize(S, CS, OP, Q, M) = fromTemplate(getName(M), unconditionalize(S, CS, OP, Q, asTemplate(M))) .

    op stripConditions : ModuleDeclSet -> [ModuleDeclSet] .
    -------------------------------------------------------
    eq stripConditions(IS SDS SSDS OPDS MAS EQS)    = IS SDS SSDS OPDS MAS EQS .
    eq stripConditions(NeMDS NeMDS')                = stripConditions(NeMDS) stripConditions(NeMDS') .
    eq stripConditions(  rl T => T'      [ AS ] . ) = ( rl T => T' [ AS ] . ) .
    eq stripConditions( crl T => T' if C [ AS ] . ) = ( rl T => T' [ AS ] . ) .
endfm
```
