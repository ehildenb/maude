Module Transformations
======================

```maude
load cterms.maude
load mtemplate.maude
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
   protecting CTERM-SET .
   protecting MODULE-TEMPLATE .
   --- protecting PURIFICATION .

    var IS : ImportDeclSet . var SDS : SubsortDeclSet . vars SSDS : SubsortDeclSet .
    var MAS : MembAxSet . var EQS : EquationSet . var RLS : RuleSet .
    vars NeMDS NeMDS' : NeModuleDeclSet . var M : Module .
    vars L L' R R' : Term . vars C CL' CR' : Constraint . var AS : AttrSet .

    op #purifyIn : Term          Module -> [CTerm] .
    op #purifyIn : ModuleDeclSet Module -> [ModuleDeclSet] .
    --------------------------------------------------------
    eq #purifyIn(IS SDS SSDS MAS EQS, M) = IS SDS SSDS MAS EQS .
    eq #purifyIn(NeMDS NeMDS',        M) = #purifyIn(NeMDS, M) #purifyIn(NeMDS', M) .

   ceq #purifyIn(( rl L => R [AS] . ), M) = ( crl L' => R' if CL' /\ CR' [AS] . )
    if L' | CL' := #purifyIn(L, M)
    /\ R' | CR' := #purifyIn(R, M) .

   ceq #purifyIn(( crl L => R if C [AS] . ), M) = ( crl L' => R' if C /\ CL' /\ CR' [AS] . )
    if L' | CL' := #purifyIn(L, M)
    /\ R' | CR' := #purifyIn(R, M) .
endfm
```

Note that when the sub-theory $(\Sigma_1, B_1 \cup E_1)$ is FVP, the above theory transformation enables variant unification to be used for narrowing.

Build-in Equations
------------------

If the equational fragment has a finitary unification algorithm, then we can "build" them into the rules.

-   TODO: Define `#eqnsModule` function.
-   TODO: Define `#variantsFrom` function.

```maude
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

    var M : Module . var FM : FModule . var S : Sort .
    vars T T' C' : Term . var AS : AttrSet . var C : Condition . var V : Variable .
    var H : Header .
    vars NeMDS NeMDS' : NeModuleDeclSet .
    var IS : ImportDeclSet . var SDS : SortDeclSet .
    var SSDS : SubsortDeclSet . var OPDS : OpDeclSet . var MAS : MembAxSet .
    var EQS : EquationSet . var RLS : RuleSet .

    op cSort : Sort -> Sort .
    -------------------------
    eq cSort(S) = qid("C" + string(S)) .

    op ctermOp : Sort -> OpDecl .
    -----------------------------
    eq ctermOp(S) = (op '_|_ : S 'Condition -> cSort(S) [none] .) .

    op rmConditions : Sort ModuleDeclSet -> [ModuleDeclSet] .
    ---------------------------------------------------------
    eq rmConditions(S, none)                = none .
    eq rmConditions(S, IS SDS SSDS MAS EQS) = IS SDS SSDS MAS EQS .
    eq rmConditions(S, NeMDS NeMDS')        = rmConditions(S, NeMDS) rmConditions(S, NeMDS') .

   ceq rmConditions(S, rl T => T' [AS] .) = ( rl '_|_[T, V] => '_|_[T', V] [AS] . )
    if V := #var((T, T'), 'Condition) .

   ceq rmConditions(S, crl T => T' if C [AS] .) = ( rl '_|_[T, V] => '_|_[T', '_/\_[V, C']] [AS] . )
    if C' := upTerm(C)
    /\ V  := #var((T, T', C'), 'Condition) .

    op unconditionalize : Sort ModuleDeclSet -> [ModuleDeclSet] .
    -------------------------------------------------------------
    eq unconditionalize(S, MDS) = ( pr 'META-LEVEL . ) cTermOp(S) rmConditions(S, MDS) .
endfm
```