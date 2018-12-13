Module Transformations
======================

```maude
sload purification.maude
sload mconstruction.maude
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

Reversing Rules
---------------

This transformation will reverse both conditional and unconditional rules by swapping the LHS and the RHS.
It leaves conditions *unchanged* on conditional rules.

```maude
fmod REVERSE-RULES is
   protecting MODULE-TEMPLATE .

    var IDS : ImportDeclSet . var SDS : SortDeclSet . var SSDS : SubsortDeclSet .
    var OPDS : OpDeclSet . var MAS : MembAxSet . var EQS : EquationSet .
    vars NeMDS NeMDS' : NeModuleDeclSet .

    vars T T' : Term . var C : Condition . var AS : AttrSet .
    var M : Module .

    op reverseRules : ModuleDeclSet -> [ModuleDeclSet] .
    ----------------------------------------------------
    eq reverseRules(IDS SDS SSDS OPDS MAS EQS) = IDS SDS SSDS OPDS MAS EQS .
    eq reverseRules(NeMDS NeMDS')              = reverseRules(NeMDS) reverseRules(NeMDS') .

    eq reverseRules(  rl T => T'      [ AS ] . ) = (  rl T' => T      [ AS ] . ) .
    eq reverseRules( crl T => T' if C [ AS ] . ) = ( crl T' => T if C [ AS ] . ) .

    op reverseRules : Module -> [Module] .
    --------------------------------------
    eq reverseRules(M) = fromTemplate(getName(M), reverseRules(asTemplate(M))) .
endfm
```

Stripping Conditions
--------------------

Stripping conditions from the rules of a module creates an over-approximation of the transition system represented by them.
Every rule application that could happen in the original system still can happen, but they may apply in more ways (when the condition would have stopped the rule application).

```maude
fmod STRIP-CONDITIONS is
    protecting META-LEVEL .
    protecting MODULE-TEMPLATE .

    var M : Module . var Q : Qid . vars T T' : Term . var C : Condition . var AS : AttrSet .
    var IDS : ImportDeclSet . var SDS : SortDeclSet . var SSDS : SubsortDeclSet .
    var OPDS : OpDeclSet . var MAS : MembAxSet . var EQS : EquationSet .
    vars NeMDS NeMDS' : NeModuleDeclSet . var MDS : ModuleDeclSet .

    op stripConditions : ModuleDeclSet -> [ModuleDeclSet] .
    -------------------------------------------------------
    eq stripConditions(IDS SDS SSDS OPDS MAS EQS)   = IDS SDS SSDS OPDS MAS EQS .
    eq stripConditions(NeMDS NeMDS')                = stripConditions(NeMDS) stripConditions(NeMDS') .
    eq stripConditions(  rl T => T'      [ AS ] . ) = ( rl T => T' [ AS ] . ) .
    eq stripConditions( crl T => T' if C [ AS ] . ) = ( rl T => T' [ AS ] . ) .

    op stripConditions : Module -> [Module] .
    -----------------------------------------
    eq stripConditions(M) = fromTemplate(getName(M), stripConditions(asTemplate(M))) .

    op conditionFor : Qid ModuleDeclSet -> [Condition] .
    ----------------------------------------------------
    eq conditionFor(Q, crl T => T' if C [ label(Q) AS ] . MDS) = C   .
    eq conditionFor(Q,  rl T => T'      [ label(Q) AS ] . MDS) = nil .

    op conditionFor : Qid Module -> [Condition] .
    ---------------------------------------------
    eq conditionFor(Q, M) = conditionFor(Q, asTemplate(M)) .
endfm
```

Unconditionalize Transformation
-------------------------------

This operation will "internalize" the conditions of the rules in a conditional theory.
It assumes that the theory is topmost to begin with, and augments it with a new operator which allows storing a constraint as well as the original state.
Conditions are added to the right-hand-side of the rule as accumulated constraints.
This transformation is parametric in several operators and sorts (prefixed below with `#`) about the module which the conditions will be translated into.

```maude
fmod UNCONDITIONALIZE is
   protecting META-LEVEL .
   protecting MODULE-TEMPLATE .

    var M : Module . var Q : Qid . vars T T' C' : Term . vars EqC EqC' : EqCondition .
    var A : Attr . var AS : AttrSet . var CS : Sort . var V : Variable .
    var IDS : ImportDeclSet . var SDS : SortDeclSet . var SSDS : SubsortDeclSet .
    var OPDS : OpDeclSet . var MAS : MembAxSet . var EQS : EquationSet .
    vars NeRLS NeRLS' : NeRuleSet . var RLS : RuleSet .

    op unconditionalize : ModuleDeclSet -> [ModuleDeclSet] .
    --------------------------------------------------------
   ceq unconditionalize(IDS SDS SSDS OPDS MAS EQS RLS)
     = IDS  ( pr #cModule . )
       SDS  ( sorts CS . )
       SSDS
       OPDS ( op #cTerm : #tSort #cSort -> CS [ctor prec(57)] . )
       MAS
       EQS
       unconditionalizeRules(RLS)
    if CS := qid("C" + string(#tSort)) .

    op unconditionalize : Module -> [Module] .
    ------------------------------------------
    eq unconditionalize(M) = fromTemplate(getName(M), unconditionalize(asTemplate(M))) .

    op unconditionalizeRules : RuleSet -> [RuleSet] .
    -------------------------------------------------
    eq unconditionalizeRules(none)         = none .
    eq unconditionalizeRules(NeRLS NeRLS') = unconditionalizeRules(NeRLS) unconditionalizeRules(NeRLS') .

   ceq unconditionalizeRules(rl T => T' [AS] .) = ( rl #cTerm[T, V] => #cTerm[T', V] [narrowing #noNonExec(AS)] . )
    if V := qid("###COND###:" + string(#cSort)) .

   ceq unconditionalizeRules(crl T => T' if EqC [AS] .) = ( rl #cTerm[T, V] => #cTerm[T', C'] [narrowing #noNonExec(AS)] . )
    if V  := qid("###COND###:" + string(#cSort))
    /\ C' := #cConjunct[V, #mkCondition(EqC)] .

    op #noNonExec : AttrSet -> [AttrSet] .
    --------------------------------------
    eq #noNonExec(none) = none .
    eq #noNonExec(A AS) = if A == nonexec then none else A fi #noNonExec(AS) .
```

The parameters of the transformation are as follows:

-   `#tSort`: topmost sort of the rules from the conditional theory.
-   `#cModule`: module that the conditions (of sort `#cSort`) come from.
-   `#cSort`: sort of the conditions from the conditional theory.
-   `#cTrue`: true/top element of the conditional sort `#cSort`.
-   `#cFalse`: false/bottom element of the conditional sort `#cSort`.
-   `#cConjunct`: conjunction operator in condition module `#cModule`.
-   `#cTerm`: operator to join the a `#tSort` and a `#cSort` together (currently set to `_st_`).
-   `#mkCondition`: translation from an `EqCondition` atom to a term of sort `#cSort`.

```maude
    op #tSort : -> Sort .
    ---------------------

    op #cModule   : -> Qid .
    op #cSort     : -> Sort .
    op #cTrue     : -> Constant .
    op #cFalse    : -> Constant .
    op #cConjunct : -> Qid .
    ------------------------

    op #cTerm : -> Qid .
    --------------------
    eq #cTerm = '_st_ .

    op #mkCondition : EqCondition -> Term .
    ---------------------------------------
    eq #mkCondition(nil)         = #cTrue .
   ceq #mkCondition(EqC /\ EqC') = #cConjunct[#mkCondition(EqC), #mkCondition(EqC')]
                                if EqC =/= nil /\ EqC' =/= nil .
endfm
```
