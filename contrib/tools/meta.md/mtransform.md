Module Transformations
======================

```maude
load purification.maude
load mconstruction.maude
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

    vars Q OP : Qid . var M : Module . var FM : FModule . vars S S' CS : Sort .
    vars T T' C' : Term . vars A A' : Attr . var AS : AttrSet .
    var C : Condition . var V : Variable .
    var H : Header . var MD : ModuleDecl . var MDS : ModuleDeclSet .
    vars NeMDS NeMDS' : NeModuleDeclSet .
    var IDS : ImportDeclSet . var SDS : SortDeclSet .
    var SSDS : SubsortDeclSet . var OPDS : OpDeclSet . var MAS : MembAxSet .
    var EQS : EquationSet . var RLS : RuleSet . vars EqC EqC' : EqCondition .
    vars NeRLS NeRLS' : NeRuleSet .
```

The following allow for simple manipulation of modules with coonditions, including removing and retrieving conditions for rules.

```maude
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
```

### Unconditionalize

The general algorithm for unconditionalizing rules with equational conditions follows.
It's parametric in several operators and sorts (prefixed below with `#`) about the module which the conditions will be translated into.

```maude
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
