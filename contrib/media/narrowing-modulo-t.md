---
title: Narrowing Modulo T
geometry: margin=2.5cm
---

Abstract
--------

Introduction
============

Background
==========

Methodology
===========

Module Transformations
----------------------

``` {.maude}
load purification.maude
load mconstruction.maude
```

\newcommand{\R}{\ensuremath{\mathcal{R}}}
\newcommand{\rto}{\ensuremath{\Rightarrow}}
\newcommand{\rif}{\ensuremath{\ \texttt{if}\ }}
\newcommand{\st}{\ensuremath{\ st\ }}
Theory transformations take advantage of Maude's *reflective* nature.
Assume throughout a rewrite theory $\ensuremath{\mathcal{R}}= (\Sigma, B, E, T, R)$ with $\Sigma = (S, <)$ the signature.

### Subtheory Abstraction

When rewrite theory $\ensuremath{\mathcal{R}}$ protects sub-theory $(\Sigma_1, B_1 \cup E_1)$, define a new theory $(\Sigma, B \cup E, R')$ with:

-   TODO: Make `EqTheory` a sub-sort of `ModuleDeclSet`?
-   TODO: Make `Rule < CRule`, with `op _if_ : Rule Condition -> Rule [right id: true]` for easier matching.
-   TODO: Bring in code from `PURIFICATION` module.

``` {.maude}
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

### Build-in Equations

If the equational fragment has a finitary unification algorithm, then we can "build" them into the rules.

-   TODO: Define `#eqnsModule` function.
-   TODO: Define `#variantsFrom` function.

<!-- -->

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

### Reversing Rules

``` {.maude}
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
```

This transformation allows execution and/or analysis in the reversed theory $\overleftarrow{\ensuremath{\mathcal{R}}} = (\Sigma, B, E, T, R')$.
Each rule $l_i \to r_i \ensuremath{\ \texttt{if}\ }\varphi_i \in R$ is transformed into the rule $r_i \to l_i \ensuremath{\ \texttt{if}\ }\varphi_i \in R'$.
It leaves conditions *unchanged* on conditional rules.

``` {.maude}
    eq reverseRules(  rl T => T'      [ AS ] . ) = (  rl T' => T      [ AS ] . ) .
    eq reverseRules( crl T => T' if C [ AS ] . ) = ( crl T' => T if C [ AS ] . ) .

    op reverseRules : Module -> [Module] .
    --------------------------------------
    eq reverseRules(M) = fromTemplate(getName(M), reverseRules(asTemplate(M))) .
endfm
```

### Stripping Conditions

Stripping conditions from the rules of a module creates an over-approximation of the transition system represented by them.
Every rule application that could happen in the original system still can happen, but they may apply in more ways (when the condition would have stopped the rule application).

``` {.maude}
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

### Unconditionalize Transformation

``` {.maude}
fmod UNCONDITIONALIZE is
   protecting META-LEVEL .
   protecting MODULE-TEMPLATE .

    var M : Module . var Q : Qid . vars T T' C' : Term . vars EqC EqC' : EqCondition .
    var A : Attr . var AS : AttrSet . var CS : Sort . var V : Variable .
    var IDS : ImportDeclSet . var SDS : SortDeclSet . var SSDS : SubsortDeclSet .
    var OPDS : OpDeclSet . var MAS : MembAxSet . var EQS : EquationSet .
    vars NeRLS NeRLS' : NeRuleSet . var RLS : RuleSet .
```

Given a topmost conditional theory $(\Sigma, B, E, T, R)$, this transformation produces the unconditional theory $\ensuremath{\mathcal{R}}' = (\Sigma', B, E, \varnothing, R')$ over pattern predicates.
This transformation preserves executability, both concretely and symbolically.

The new signature $\Sigma' = (S \cup \{s'\}, < \cup \{s < s'\})$ adds a new supersort $s'$ of the top sort $s$ of rules $R$.
Another operator $\_ \ensuremath{\ st\ }\_ : s b \to s'$ is added, conjoining an element of $s$ with a constraint from $T$.

``` {.maude}
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
```

Each rule $l_i \to r_i \ensuremath{\ \texttt{if}\ }\varphi_i \in R$ is replaced with the rule $l_i \ensuremath{\ st\ }V \to r_i \ensuremath{\ st\ }V \land \varphi_i$ (for $V : b$ fresh) over pattern predicates.
This transformation preserves executability.

**Theorem** (Completeness): If an execution step $t \ensuremath{\Rightarrow}t'$ is possible in $\ensuremath{\mathcal{R}}$, then a corresponding execution step $t \ensuremath{\ st\ }\top \ensuremath{\Rightarrow}t' \ensuremath{\ st\ }\phi$ is possible in $\ensuremath{\mathcal{R}}'$.

**Proof**: If $\ensuremath{\mathcal{R}}\proves t \ensuremath{\Rightarrow}t'$, then for some $l_i \ensuremath{\Rightarrow}r_i \ensuremath{\ st\ }\varphi_i \in R$ we have that $l_i \alpha =_{E/B} t \alpha$ (with $\alpha$ the *unifier*) such that $T \models \varphi_i \alpha$.
In this case, the generated rule $l_i \ensuremath{\ st\ }V \ensuremath{\Rightarrow}r_i \ensuremath{\ st\ }V \land \varphi_i$ can be applied, as $(l_i \ensuremath{\ st\ }V) \alpha' =_{E/B} (t \ensuremath{\ st\ }\top) \alpha'$ (with unifier $\alpha' = \alpha \cup \{ V \mapsto \top \}$).
Thus, $\ensuremath{\mathcal{R}}' \proves t \ensuremath{\ st\ }\top \ensuremath{\Rightarrow}t' \ensuremath{\ st\ }\varphi_i \alpha$.

``` {.maude}
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

-   $s$ (`#tSort`): top sort of the rules from the conditional theory.
-   $T$ (`#cModule`): module that the conditions (of sort `#cSort`) come from.
-   $b$ (`#cSort`): top sort of the conditions from the conditional theory.
-   $\top$ (`#cTrue`): true/top element of the conditional sort `#cSort`.
-   $\bot$ (`#cFalse`): false/bottom element of the conditional sort `#cSort`.
-   $\land$ (`#cConjunct`): conjunction operator in condition module `#cModule`.
-   $\ensuremath{\ st\ }$ (`#cTerm`): operator to conjoin a `#tSort` a `#cSort` together with (currently set to `_st_`).

``` {.maude}
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
```

`#mkCondition` is a book-keeping operator to make it possible to use Maude's built-in boolean side-conditions as stand-in for the theory $T$.
You must provide translation from an `EqCondition` atom to a term of sort `#cSort`.

``` {.maude}
    op #mkCondition : EqCondition -> Term .
    ---------------------------------------
    eq #mkCondition(nil)         = #cTrue .
   ceq #mkCondition(EqC /\ EqC') = #cConjunct[#mkCondition(EqC), #mkCondition(EqC')]
                                if EqC =/= nil /\ EqC' =/= nil .
endfm
```

Labelled Graph Search and Analysis
----------------------------------

Here is a labelled graph search algorithm and its instantiation to narrowing.

``` {.maude}
load ../meta/narrowing.maude
load ../meta/cterms.maude
load ../meta/mtransform.maude
load ../meta/variables.maude
load lmc.maude

set include BOOL off .
```

### Data Structures

#### Node Maps

Sort `NodeMap` can store a natural-number indexed set of nodes which support `fold`ing.
Insertion returns the `NodeId`, which can either be a new natural number or an old natural number with an associated `fold`.

``` {.maude}
fmod FOLDING-NODEMAP is
   protecting NAT .
   protecting EXT-BOOL .
```

Natural numbers can be interpreted as `NodeId`s, which makes it possible to refer to nodes more compactly.

``` {.maude}
    sorts NodeId Node NeNodeSet NodeSet .
    -------------------------------------
    subsort Nat < NodeId < Node < NeNodeSet < NodeSet .

    vars N N' : Nat . vars NID NID' : NodeId .
    vars ND ND' : Node . vars NeNS NeNS' NeNS'' : NeNodeSet . vars NS NS' : NodeSet .

    op .NodeSet : -> NodeSet .
    op _;_      : NodeSet   NodeSet ->   NodeSet [assoc comm id: .NodeSet] .
    op _;_      : NodeSet NeNodeSet -> NeNodeSet [ditto] .
    ------------------------------------------------------
    eq NeNS ; NeNS = NeNS .

    op _\_ : NodeSet NodeSet -> NodeSet .
    -------------------------------------
    eq       NS  \       NS'  = NS [owise] .
    eq (ND ; NS) \ (ND ; NS') = NS \ (ND ; NS') .
```

A `Fold` is a "witness" that a specified node is less general than another node.
The user can specify the equations for `fold` to allow subsumption when possible.

``` {.maude}
    sort Fold NeFoldSet FoldSet .
    -----------------------------
    subsort Fold < NeFoldSet < FoldSet .

    vars F F' : Fold . var F? : [Fold] .

    op .Fold : -> Fold .
    --------------------

    op .FoldSet :                   ->   FoldSet .
    op __       : FoldSet   FoldSet ->   FoldSet [assoc comm id: .FoldSet] .
    op __       : FoldSet NeFoldSet -> NeFoldSet [ditto] .
    ------------------------------------------------------
    eq F F = F .

    op fold : NodeSet Node -> [FoldSet] .
    -------------------------------------
    eq fold(.NodeSet  , ND') = .Fold .
    eq fold(ND        , ND)  = .Fold .
   ceq fold(ND ; NeNS , ND') = F F' if F  := fold(ND, ND')
                                    /\ F' := fold(NeNS, ND') .

    op _[_] : Nat Fold -> NodeId [right id: .Fold prec 20] .
    --------------------------------------------------------

    --- TODO: add operator _~=_ for NodeSet equality
    --- TODO: extract this and common functionality from bae-symbolic branch

    op _<_  : NodeSet NodeSet -> [Bool] [ditto] .
    op _<=_ : NodeSet NodeSet -> [Bool] [ditto] .
    ---------------------------------------------
    eq NS             <  NS'        = NS =/= NS' and-then NS <= NS' .
    eq ND             <= ND         = true .
   ceq ND             <= ND'        = fold(ND, ND') :: Fold if not (ND :: NodeId) /\ not (ND' :: NodeId) .
    eq .NodeSet       <= NS         = true .
    eq NeNS           <= .NodeSet   = false .
    eq (NeNS ; NeNS') <= NeNS''     = NeNS <= NeNS'' and-then NeNS' <= NeNS'' .
   ceq ND             <= (ND' ; NS) = true if fold(ND, ND') :: Fold .

    op intersect : NodeSet NodeSet -> [NodeSet] [comm] .
    ----------------------------------------------------
    eq intersect(N[F], N') = if N == N' then N[F] else .NodeSet fi .
   ceq intersect(ND, ND')  = ND if not (ND :: NodeId) /\ not (ND' :: NodeId) /\ ND <= ND' .

    eq intersect(.NodeSet    , NS) = .NodeSet .
    eq intersect(NeNS ; NeNS', NS) = intersect(NeNS, NS) ; intersect(NeNS', NS) .

    op isEmpty? : NodeSet -> [Bool] .
    ---------------------------------
    eq isEmpty?(.NodeSet) = true .
```

A `NodeMap` is a lookup table of `Nat |-> Node`, which supports a lookup `_[_]` and `insert` operation.
The insert operation is aware of the user defined `Fold`s, and will not create new entries for new nodes that are less general than existing ones.

``` {.maude}
    sorts NodeAlias NeNodeMap NodeMap NodeMap? .
    --------------------------------------------
    subsorts NodeAlias < NeNodeMap < NodeMap .

    var NA : NodeAlias . vars NM NM' NM'' : NodeMap . vars NeNM NeNM' : NeNodeMap .

    op _|->_ : Nat Node -> NodeAlias [prec 55] .
    --------------------------------------------

    op .NodeMap :                   ->   NodeMap .
    op __       : NeNodeMap NodeMap -> NeNodeMap [assoc id: .NodeMap prec 60 format(d n d)] .
    op __       :   NodeMap NodeMap ->   NodeMap [ditto] .
    ------------------------------------------------------
    eq NeNM NeNM = NeNM .

    op nodes : NodeMap -> [NodeSet] .
    ---------------------------------
    eq nodes(.NodeMap)   = .NodeSet .
    eq nodes(N |-> ND)   = if ND :: NodeId then .NodeSet else ND fi .
    eq nodes(NeNM NeNM') = nodes(NeNM) ; nodes(NeNM') .

    op keys : NodeMap -> [NodeSet] .
    -------------------------------
    eq keys(.NodeMap)   = .NodeSet .
    eq keys(N |-> ND)   = N .
    eq keys(NeNM NeNM') = keys(NeNM) ; keys(NeNM') .

    op _[_] : NodeMap NodeSet -> [NodeSet] .
    ----------------------------------------
    eq (NID |-> ND NM) [ NID' ] = if NID == NID' then ND else NM [ NID' ] fi .

    eq .NodeMap [ NS ] = .NodeSet .

    eq NM [ .NodeSet     ] = .NodeSet .
    eq NM [ NeNS ; NeNS' ] = (NM [ NeNS ]) ; (NM [ NeNS' ]) .

    op {_,_,_} : NodeMap NodeId NodeSet -> NodeMap? .
    -------------------------------------------------

    op  insert : NodeAlias NodeMap         -> NodeMap? .
    op #insert : NodeAlias NodeMap NodeMap -> NodeMap? .
    ----------------------------------------------------
    eq insert(NA, NM) = #insert(NA, .NodeMap, NM) .

    eq #insert(N |-> ND , NM , .NodeMap)       = { NM N |-> ND , N , N } .
   ceq #insert(N |-> ND , NM , N' |-> ND' NM') = if (F? :: Fold) then { NM N' |-> ND' NM' , N'[F?] , .NodeSet }
                                                                 else #insert(N |-> ND, NM N' |-> ND', NM')
                                                 fi
                                              if F? := fold(ND, ND') .
```

We can `restrict` a given `NodeMap` to a set of keys, and get the set of `NodeId` which `intersect` with a given nodeset.

``` {.maude}
    op restrict : NodeMap NodeSet -> [NodeMap] .
    --------------------------------------------
    eq restrict(NM, .NodeSet)     = .NodeMap .
   ceq restrict(NM, N [ F ] ; NS) = restrict(NM, N ; NS)      if F =/= .Fold .
   ceq restrict(NM, N       ; NS) = N |-> ND restrict(NM, NS) if ND       := NM [ N ] .
   ceq restrict(NM, N       ; NS) =          restrict(NM, NS) if .NodeSet := NM [ N ] .

    op intersects-with : NodeMap NodeSet -> [NodeSet] .
    ---------------------------------------------------
    eq intersects-with(.NodeMap, NS) = .NodeSet .
    eq intersects-with(NM, .NodeSet) = .NodeSet .

    eq intersects-with(N |-> ND NM , NeNS) = if intersect(ND, NeNS) =/= .NodeSet then N else .NodeSet fi ; intersects-with(NM, NeNS) .
endfm
```

#### Folding Edge-labeled Graphs

Sort `LabeledGraph` is an edge-labelled graph between `Node`s.
The `Label` used is left abstract.

``` {.maude}
fmod FOLDING-LABELED-GRAPH is
   protecting FOLDING-NODEMAP .

    vars N N' : Nat . vars NID NID' NID'' : NodeId . vars ND ND' ND'' : Node .
    vars NeNS NeNS' : NeNodeSet . var NS NS' NS'' : NodeSet .
    vars NM NM' NM'' : NodeMap .
    vars F F' : Fold .

    sorts Label LabeledEdge NeLabeledGraph LabeledGraph .
    -----------------------------------------------------
    subsorts LabeledEdge < NeLabeledGraph < LabeledGraph .

    var L : Label . vars LG LG' : LabeledGraph . vars NeLG NeLG' : NeLabeledGraph .

    op .LabeledGraph :                             ->   LabeledGraph .
    op __            :   LabeledGraph LabeledGraph ->   LabeledGraph [assoc comm id: .LabeledGraph prec 55 format(d n d)] .
    op __            : NeLabeledGraph LabeledGraph -> NeLabeledGraph [ditto] .
    --------------------------------------------------------------------------
    eq NeLG NeLG = NeLG .

    op _-[_]->_ : Node Label Node -> LabeledEdge [prec 50] .
    --------------------------------------------------------

    op nodes : LabeledGraph -> [NodeSet] .
    --------------------------------------
    eq nodes(.LabeledGraph)      = .NodeSet .
    eq nodes(ND -[ L ]-> ND' LG) = ND ; ND' ; nodes(LG) .

    op withoutFolds : LabeledGraph -> [LabeledGraph] .
    --------------------------------------------------
    eq withoutFolds(.LabeledGraph)      = .LabeledGraph .
    eq withoutFolds(NeLG NeLG')         = withoutFolds(NeLG) withoutFolds(NeLG') .
    eq withoutFolds(N[F] -[L]-> N'[F']) = N -[L]-> N' .
```

We can access a `FoldingLabeledGraph?` in various ways, including the `pred` and `succ` relations, and transitive closures of them.

``` {.maude}
    op succ : LabeledGraph NodeSet -> [NodeSet] .
    op pred : LabeledGraph NodeSet -> [NodeSet] .
    ---------------------------------------------
    eq succ(ND -[ L ]-> ND' LG, ND ; NS) = ND' ; succ(LG, ND ; NS) .
    eq succ(                LG,      NS) = .NodeSet [owise] .

    eq pred(ND -[ L ]-> ND' LG, ND' ; NS) = ND ; pred(LG, ND' ; NS) .
    eq pred(                LG,       NS) = .NodeSet [owise] .

    op restrict : LabeledGraph NodeSet -> [LabeledGraph] .
    ------------------------------------------------------
    eq restrict(LG,                        NS) = .LabeledGraph [owise] .
   ceq restrict(ND -[ L ]-> ND' LG, ND'' ; NS) = ND -[ L ]-> ND' restrict(LG, ND ; ND' ; ND'' ; NS)
                                              if intersect(ND ; ND', ND'') =/= .NodeSet .

    op restrictBackwards : LabeledGraph NodeSet -> [LabeledGraph] .
    ---------------------------------------------------------------
    eq restrictBackwards(LG,                        NS) = .LabeledGraph [owise] .
   ceq restrictBackwards(ND -[ L ]-> ND' LG, ND'' ; NS) = ND -[ L ]-> ND' restrictBackwards(LG, ND ; ND'' ; NS)
                                                       if intersect(ND', ND'') =/= .NodeSet .
```

A `FoldedLabeledGraph` will use the `NodeMap` to store nodes, instead of directly storing them as nodes.
This will allow for "folding" the graph when a certain node is subsumed by another.
In the data structure `FoldedLabeledGraph?`, we additionally maintain a `frontier` of nodes which have not been explored for successor states yet.

``` {.maude}
    sorts FoldedLabeledGraph FoldedLabeledGraph? .
    ----------------------------------------------
    subsort FoldedLabeledGraph < FoldedLabeledGraph? .

    var FLG : FoldedLabeledGraph . var FLG? : FoldedLabeledGraph? .

    op .FoldedLabeledGraph :                          -> FoldedLabeledGraph .
    op _|_|_               : LabeledGraph NodeMap Nat -> FoldedLabeledGraph [format(d n d n d d)] .
    -----------------------------------------------------------------------------------------------
    eq .FoldedLabeledGraph = .LabeledGraph | .NodeMap | 0 .

    op _|_ : FoldedLabeledGraph NodeSet -> FoldedLabeledGraph? [right id: .NodeSet format(d n d d)] .
    -------------------------------------------------------------------------------------------------
    eq FLG | NeNS | NeNS' = FLG | NeNS ; NeNS' .

    op nodes    : FoldedLabeledGraph? -> [NodeSet] .
    op keys     : FoldedLabeledGraph? -> [NodeSet] .
    op frontier : FoldedLabeledGraph? -> [NodeSet] .
    ------------------------------------------------
    eq nodes   (LG | NM | N | NS) = nodes(NM) .
    eq keys    (LG | NM | N | NS) = keys(NM) .
    eq frontier(LG | NM | N | NS) = NM [ NS ] .

    op insert : NodeSet      FoldedLabeledGraph? -> FoldedLabeledGraph? .
    op insert : LabeledGraph FoldedLabeledGraph? -> FoldedLabeledGraph? .
    ---------------------------------------------------------------------
    eq insert(.NodeSet,     FLG?)        = FLG? .
    eq insert(NeNS ; NeNS', FLG?)        = insert(NeNS, insert(NeNS', FLG?)) .
    eq insert(NeNS,         FLG | NeNS') = insert(NeNS, FLG) | NeNS' .

    eq insert(.LabeledGraph, FLG?)        = FLG? .
    eq insert(NeLG NeLG',    FLG?)        = insert(NeLG, insert(NeLG', FLG?)) .
    eq insert(NeLG,          FLG | NeNS') = insert(NeLG, FLG) | NeNS' .

   ceq insert(ND, LG | NM | N) = LG | NM' | s N | NS
                              if { NM' , NID , NS } := insert(N |-> ND, NM) .

   ceq insert(ND -[ L ]-> ND', LG | NM | N) = (NID' -[ L ]-> NID'' LG) | NM'' | s s N | NS' ; NS''
                                           if { NM'  , NID'  , NS'  } := insert(  N |-> ND,  NM)
                                           /\ { NM'' , NID'' , NS'' } := insert(s N |-> ND', NM') .

    op flgraph : NodeSet -> FoldedLabeledGraph? .
    ---------------------------------------------
    eq flgraph(NS) = insert(NS, .FoldedLabeledGraph) .
```

Function `restrict` is lifted to a `FoldedLabeledGraph?`:

``` {.maude}
    op restrict : FoldedLabeledGraph? NodeSet -> [FoldedLabeledGraph?] .
    --------------------------------------------------------------------
   ceq restrict(LG | NM | N | NS, NS') = LG' | restrict(NM, NS'') | N | intersect(NS, NS'')
                                      if LG'  := restrict(LG, intersects-with(NM, NS'))
                                      /\ NS'' := nodes(LG') .

    op restrictBackwards : FoldedLabeledGraph? NodeSet -> [FoldedLabeledGraph?] .
    -----------------------------------------------------------------------------
   ceq restrictBackwards(LG | NM | N | NS, NS') = LG' | restrict(NM, NS'') | N | intersect(NS, NS'')
                                               if LG'  := restrictBackwards(LG, intersects-with(NM, NS'))
                                               /\ NS'' := nodes(LG') .
endfm
```

#### TikZ Rendering of Folded Labeled Graphs

To ease the presentation of these systems, a primitive TikZ rendering engine is supplied.
Note that this does not attempt to do any placement of the nodes, so the result must still be tuned to produce a readable graph.

``` {.maude}
fmod FOLDING-LABELED-GRAPH-RENDER-TIKZ is
   protecting FOLDING-LABELED-GRAPH .
   protecting CONVERSION .

    sort Lines .
    ------------
    subsort String < Lines .

    sorts TikZLine TikZGrid .
    -------------------------
    subsort TikZLine < TikZGrid .

    vars N N' : Nat . vars ND ND' : Node . var NID : NodeId . var NeNS : NeNodeSet . vars NS NS' NS'' : NodeSet .
    var F : Fold . var L : Label . vars STR STR' STR'' : String .
    vars NeNM NeNM' : NeNodeMap . var NM : NodeMap .
    vars NeLG NeLG' : NeLabeledGraph . var LG : LabeledGraph .
    vars TL TL' : TikZLine . var TG : TikZGrid .

    op .Lines :             -> Lines .
    op __     : Lines Lines -> Lines [assoc id: .Lines format(d n d)] .
    -------------------------------------------------------------------

    op TikZNode : NodeId Node -> Node .
    op TikZLine :     NodeSet -> TikZLine .
    ---------------------------------------

    op .TikZGrid :                   -> TikZGrid .
    op __        : TikZGrid TikZGrid -> TikZGrid [assoc id: .TikZGrid format(d n d)] .
    ----------------------------------------------------------------------------------

    op makeTikZNodes : NodeSet NodeMap -> [NodeSet] .
    -------------------------------------------------
    eq makeTikZNodes(.NodeSet , NM) = .NodeSet .
    eq makeTikZNodes(ND ; NS  , NM) = TikZNode(ND, NM [ ND ]) ; makeTikZNodes(NS, NM) .

    op makeTikZGrid : LabeledGraph NodeMap NodeSet         -> [TikZGrid] .
    op makeTikZGrid : LabeledGraph NodeMap NodeSet NodeSet -> [TikZGrid] .
    ----------------------------------------------------------------------
    eq makeTikZGrid(LG, NM, NS) = makeTikZGrid(LG, NM, NS, .NodeSet) .

    eq makeTikZGrid(LG, NM, .NodeSet, NS') = .TikZGrid .
   ceq makeTikZGrid(LG, NM, NeNS, NS')     = TikZLine(ND ; NS) makeTikZGrid(LG, NM, NS'', NS' ; NS'')
                                          if ND ; NS := makeTikZNodes(NeNS, NM)
                                          /\ NS''    := succ(LG, NeNS) \ NS' .

    op renderTikZLabel : Label -> String .
    op renderTikZNode  : Node  -> String .
    --------------------------------------
    eq renderTikZNode(TikZNode(N, ND)) = "    \node[elliptic state] (s_" + string(N, 10) + ") {" + renderTikZNode(ND) + "};" .

    op renderTikZAbsNode : Nat Nat Node -> String .
    -----------------------------------------------
    eq renderTikZAbsNode(N, N', TikZNode(NID, ND)) = "    \node[elliptic state] (s_" + string(NID, 10) + ") at (" + string(N, 10) + ",-" + string(N', 10) + ") {" + renderTikZNode(ND) + "};" .

    op renderTikZAbsLine : Nat Nat TikZLine -> Lines .
    --------------------------------------------------
    eq renderTikZAbsLine(N, N', TikZLine(.NodeSet)) = .Lines .
    eq renderTikZAbsLine(N, N', TikZLine(ND ; NS))  = renderTikZAbsNode(N    , N'       , ND)
                                                      renderTikZAbsLine(N + 2, sd(N', 1), TikZLine(NS)) .

    op renderTikZAbsGrid : Nat TikZGrid -> Lines .
    ----------------------------------------------
    eq renderTikZAbsGrid(N, .TikZGrid) = .Lines .
    eq renderTikZAbsGrid(N, TL TG)     = renderTikZAbsLine(0, N, TL)
                                         renderTikZAbsGrid(N + 4, TG) .

    op renderTikZPaths : LabeledGraph -> Lines .
    --------------------------------------------
    eq renderTikZPaths(.LabeledGraph) = .Lines .
    eq renderTikZPaths(NeLG NeLG')    = renderTikZPaths(NeLG) renderTikZPaths(NeLG') .

   ceq renderTikZPaths(N -[ L ]-> N'[F]) = "    \\path[" + STR + "] (s_" + string(N, 10) + ") edge [" + STR' + "] node [pos=0.3," + STR'' + "] {" + renderTikZLabel(L) + "} (s_" + string(N', 10) + ");"
                                        if STR STR' STR'' := if F == .Fold and N =/= N' then  "->" "bend left=10"               "right"
                                                        else if                N  == N' then "->>" "loop below,min distance=25" "left"
                                                                                        else "->>" "bend left=10"               "left"
                                                             fi fi .

    op renderTikZGraph : NodeSet FoldedLabeledGraph? -> Lines .
    -----------------------------------------------------------
    eq renderTikZGraph(NS, LG | NM | N | NS')
     = "\tikzset{elliptic state/.style={draw,ellipse}}"
       "\begin{tikzpicture}[shorten >=1pt,on grid,transform shape]"
       "\footnotesize"
       renderTikZAbsGrid(10, makeTikZGrid(withoutFolds(LG), NM, NS, NS))
       renderTikZPaths(LG)
       "\\end{tikzpicture}" .
endfm
```

### Folding Graph Searches

Over these data-structures, we can define abstract graph-search algorithms.

#### Single search step

``` {.maude}
fmod FOLDING-LABELED-GRAPH-STEP is
   protecting FOLDING-LABELED-GRAPH .

    vars N N' : Nat . vars ND ND' : Node .
    vars NS : NodeSet . vars NeNS NeNS' : NeNodeSet . var NM : NodeMap .
    var L : Label . var LG : LabeledGraph . var FLG : FoldedLabeledGraph .
```

A `Transition` can be used to represent a set of outgoing edges from a given `Node` compactly.

``` {.maude}
    sorts Transition NeTransitionSet TransitionSet .
    ------------------------------------------------
    subsorts Transition < NeTransitionSet < TransitionSet .

    var T : Transition . vars NeTS NeTS' : NeTransitionSet .

    op .TransitionSet :                               ->   TransitionSet .
    op _,_            :   TransitionSet TransitionSet ->   TransitionSet [assoc comm id: .TransitionSet] .
    op _,_            : NeTransitionSet TransitionSet -> NeTransitionSet [ditto] .
    ------------------------------------------------------------------------------
    eq NeTS , NeTS = NeTS .

    op <_,_> : Label Node -> Transition .
    op _->_  : Node TransitionSet -> LabeledGraph [prec 50] .
    ---------------------------------------------------------
    eq ND -> .TransitionSet = .LabeledGraph .
    eq ND -> < L , ND' >    = ND -[ L ]-> ND' .
    eq ND -> NeTS , NeTS'   = ND -> NeTS ND -> NeTS' .

    op nodes : TransitionSet -> [NodeSet] .
    ---------------------------------------
    eq nodes(.TransitionSet) = .NodeSet .
    eq nodes(< L , ND >)     = ND .
    eq nodes((NeTS , NeTS')) = nodes(NeTS) ; nodes(NeTS') .
```

The following are system-specific (user-specified).

-   `step` *must* return the complete set of outgoing edges (`TransitionSet`).
-   `prune` *may* optionally be instantiated to prune given transitions generated by `step` (note the fallback `owise` case).

**TODO**: `step` loses information about *which* `Node` resulted in each `Transition`.

``` {.maude}
    op step : NodeSet -> [TransitionSet] .
    --------------------------------------
    eq step(.NodeSet)     = .TransitionSet .
    eq step(NeNS ; NeNS') = step(NeNS) , step(NeNS') .

    op prune : TransitionSet -> [TransitionSet] .
    ---------------------------------------------
    eq prune(T)              = T [owise] .
    eq prune(.TransitionSet) = .TransitionSet .
    eq prune(NeTS , NeTS')   = prune(NeTS) , prune(NeTS') .
```

-   `all-step` will generate the labeled graph associated with doing a single step on a set of initial nodes.
-   `extend` uses all the above functionality to explore the existing frontier of the current `FoldedLabeledGraph?`.

``` {.maude}
    op all-step : NodeSet -> [LabeledGraph] .
    -----------------------------------------
    eq all-step(ND)           = ND -> step(ND) .
    eq all-step(.NodeSet)     = .LabeledGraph .
    eq all-step(NeNS ; NeNS') = all-step(NeNS) all-step(NeNS') .

    op extend : NodeSet             -> [FoldedLabeledGraph?] .
    op extend : FoldedLabeledGraph? -> [FoldedLabeledGraph?] .
    ----------------------------------------------------------
    eq extend(NS) = extend(flgraph(NS)) .

    eq extend(FLG)                = FLG .
    eq extend(LG | NM | N | NeNS) = insert(all-step(NM [ NeNS ]), LG | NM | N) .
endfm
```

#### Full graph analysis

After the user has instantiated the abstract graph search machinery, various analysis are provided.

``` {.maude}
fmod FOLDING-LABELED-GRAPH-SEARCH is
   protecting FOLDING-LABELED-GRAPH-STEP .
   protecting BOUND .

    var N : Nat . var B : Bound .
    vars NS NS' : NodeSet . var NeNS NeNS' : NeNodeSet . var NM : NodeMap .
    var LG : LabeledGraph . var FLG : FoldedLabeledGraph . var FLG? : FoldedLabeledGraph? .
```

`bfs` allows doing a (possible bounded) breadth-first search from a specified set of nodes.

``` {.maude}
    op  bfs : NodeSet                  -> [FoldedLabeledGraph] .
    op  bfs : NodeSet Bound            -> [FoldedLabeledGraph] .
    op #bfs : Bound FoldedLabeledGraph -> [FoldedLabeledGraph] [memo] .
    -------------------------------------------------------------------
    eq bfs(NS)    = bfs(NS, unbounded) .
    eq bfs(NS, B) = #bfs(B, flgraph(NS)) .

    eq #bfs(B, FLG)        = FLG .
    eq #bfs(0, FLG?)       = FLG? .
    eq #bfs(B, FLG | NeNS) = #bfs(decrement(B), extend(FLG | NeNS)) .
```

`invariant` will check that a given `NodeSet` is invariant by extending it with one step and checking subsumption of the new nodes into the original set.

``` {.maude}
    op invariant : NodeSet -> [Bool] .
    ----------------------------------
    eq invariant(NS) = nodes(extend(NS)) <= NS .

    op check_stable in_ : NodeSet NodeSet -> [Bool] .
    -------------------------------------------------
    eq check NS stable in NS' = NS <= NS' and-then invariant(NS') .
```

The search operators `_=>[_]_` will produce the reach-graph from the given initial states to the final states (up to a specified bound).
Before presenting the reach-graph to the user, it's pruned to only include paths which end at the final state.

``` {.maude}
    op _=>*_   : NodeSet                   NodeSet -> [FoldedLabeledGraph?] .
    op _=>[_]_ : NodeSet             Bound NodeSet -> [FoldedLabeledGraph?] .
    op _=>[_]_ : FoldedLabeledGraph? Bound NodeSet -> [FoldedLabeledGraph?] .
    -------------------------------------------------------------------------
    eq NS =>*     NS' = NS =>[ unbounded ] NS' .
    eq NS =>[ B ] NS' = flgraph(NS) =>[ B ] NS' .

    eq LG | NM | N | NeNS =>[ B ] NS' = if intersect(NM [ NeNS ], NS') == .NodeSet
                                            then restrictBackwards(LG | NM | N | NeNS, NS')
                                            else extend(LG | NM | N | NeNS) =>[ decrement(B) ] NS'
                                        fi .
endfm
```

### Instantiation to Narrowing

#### Common Narrowing

The above algorithms are abstract, but must be instantiated to a particular problem type.
Here, they are instantiated it to two theories of interest for Maude: narrowing and narrowing modulo $T$.

The parameters are instantiated as follows:

-   A `Label` is a tuple of the rule identifier enabling the transition, and the substitution used to take the rule.
-   A `Node` is a Maude meta-level `Term`, taking a `step` means calling out to Maude's meta-level narrowing functionality.
-   A `Fold` is a substitution which enables subsumption, and Maude's meta-level matching functionality is used to compute folds.

**TODO**: Support partial folds?
Would need a pattern like `T /\ not (T_1 \/ ... \/ T_i)` for `T_1 \/ ... \/ T_i` the folded component.

``` {.maude}
fmod NARROWING-GRAPH-COMMON is
   protecting NARROWING .
   protecting RENAME-METAVARS .
   protecting SUBSTITUTION-SET .
    extending FOLDING-LABELED-GRAPH-SEARCH .
    extending META-LMC-PARAMETERS .

    vars T T' : Term . var NeTS : NeTermSet . var M : Module . var SUB : Substitution . var TYPE : Type .
    vars NSR NSR' : NarrowStepResult . var NeNSRS : NeNarrowStepResults . var NSRS : NarrowStepResults . var RL : Qid .

    op label : Qid Substitution -> Label .
    --------------------------------------

    op transition : NarrowStepResults -> [TransitionSet] .
    ------------------------------------------------------
   ceq transition({RL : T , SUB })    = prune(< label(RL, SUB) , state(T') >) if { T' , TYPE } := metaReduce(#M, T) .
    eq transition(.NarrowStepResults) = .TransitionSet .
    eq transition(NSR || NeNSRS)      = transition(NSR) , transition(NeNSRS) .

    op fold : SubstitutionSet -> Fold .
    -----------------------------------
   ceq fold(state(T), state(T')) = fold(SUB) if SUB := metaMatch(#M, T', T, nil, 0) .

    op state : Term -> Node .
    -------------------------
    eq step(state(T)) = transition(narrowSteps(#M, T)) .

    op states : TermSet -> NodeSet .
    --------------------------------
    eq states(.TermSet) = .NodeSet .
    eq states(T)        = state(T) .
    eq states(T | NeTS) = state(T) ; states(NeTS) .
endfm
```

#### Unconditional Narrowing

For unconditional narrowing, we additionally define:

-   `intersect` should assume empty intersection if there are no variant unifiers between the states.

``` {.maude}
fmod NARROWING-GRAPH is
    including NARROWING-GRAPH-COMMON .

    vars T T' : Term .

   ceq intersect(state(T), state(T')) = .NodeSet
    if noUnifier := metaVariantDisjointUnify(#M, renameTmpVar(#M, T) =? renameTmpVar(#M, T'), empty, 0, 0) .
endfm
```

#### Conditional Narrowing

For conditional narrowing, we additionally define:

-   `prune` can opportunistically remove states where the constraint is exactly the identified false term.
-   `fold` must also take the side-condition into account to ensure subsumption, and can look through multiple matching subsumptions on the term to do so.
-   `implies?` is supplied by the user and should check that the models of the first condition are contained in those of the second.
-   `intersect` can assume no intersection only if there are no variant unifiers on the *state* component, not taking into account the *constraint*.

``` {.maude}
fmod CONDITIONAL-NARROWING-GRAPH is
    including NARROWING-GRAPH-COMMON
            + META-CONDITIONAL-LMC-PARAMETERS .

    vars ND ND' : Node . vars T T' C C' : Term . var F : Fold . var L : Label .
    vars Q RL : Qid . var SUB : Substitution . var SUB? : Substitution? . var N : Nat .
    vars NSR NSR' : NarrowStepResult . var NSRS : NarrowStepResults .

   ceq prune(< L , state(Q[T, C]) >) = .TransitionSet if Q = #cTerm /\ C = #cFalse .
   ---------------------------------------------------------------------------------

   ceq fold(ND, ND') = F if F := foldAny(ND, ND', 0) .
   ---------------------------------------------------

    op foldAny : Node Node Nat -> [Fold] .
    --------------------------------------
   ceq foldAny(ND, ND', N) = if implies?(C << SUB, C' << SUB) then fold(SUB) else foldAny(ND, ND', s N) fi
                          if state(Q[T  , C ]) := ND  /\ Q == #cTerm
                          /\ state(Q[T' , C']) := ND' /\ Q == #cTerm
                          /\ SUB := metaMatch(#M, T', T, nil, N) .

    op implies? : Term Term -> [Bool] .
    -----------------------------------
    eq implies?(C, C)  = true .
   ceq implies?(C, C') = true if C' = #cTrue  .
   ceq implies?(C, C') = true if C  = #cFalse .

   ceq intersect(state(Q[T,C]), state(Q[T',C'])) = .NodeSet
    if Q = #cTerm /\ noUnifier := metaVariantDisjointUnify(#M, renameTmpVar(#M, T) =? renameTmpVar(#M, T'), empty, 0, 0) .
endfm
```

Evaluation
==========

Thermostat
----------

A simple thermostat in Maude, which switches between on/off based on some threshold temperatures.
The state components of the thermostate are the current time, the temperature, and the mode (on, off, turning on, turning off).

### Parametric Thermostat

#### Unital Ordered Rings

The thermostat is parametric in an ordered ring with unit (which will be the data for the temperature).

``` {.maude}
fth UNITAL-ORDERED-RING is

    sorts RingBool Ring .
    ---------------------
    vars R R' : Ring .

    ops 0 1 : -> Ring [ctor] .
    --------------------------

    op _+_ : Ring Ring -> Ring [ctor assoc comm id: 0] .
    ----------------------------------------------------

    op -_ : Ring -> Ring [ctor] .
    -----------------------------
    eq - 0       = 0 [nonexec] .
    eq R + (- R) = 0 [nonexec] .

    op _-_ : Ring Ring -> Ring .
    ----------------------------
    eq R - R' = R + (- R') [nonexec] .

    ops true false : -> RingBool [ctor] .
    -------------------------------------

    op _<_ : Ring Ring -> RingBool .
    --------------------------------
endfth
```

#### Thermostat Definition

When discussing the thermostat, `TIME` and `TMP` will be used as the units of measure.

``` {.maude}
mod THERMOSTAT{TIME :: UNITAL-ORDERED-RING, TMP :: UNITAL-ORDERED-RING} is

    sorts DelayMode InMode Mode .
    -----------------------------
    subsorts DelayMode InMode < Mode .

    sort Conf .
    -----------
    vars TIME TIME' : TIME$Ring . vars TMP TMP' : TMP$Ring .
    var MODE : Mode . var IM : InMode . var DM : DelayMode .

   ops on off :        -> InMode    [ctor] .
    op delay  : InMode -> DelayMode [ctor] .
    ----------------------------------------

    op <_,_,_> : TIME$Ring TMP$Ring Mode -> Conf [ctor] .
    op {_,_,_} : TIME$Ring TMP$Ring Mode -> Conf [ctor] .
    -----------------------------------------------------
    rl [tick] : < TIME , TMP                  , MODE >
             => { TIME , heat-rate(MODE, TMP) , MODE } .

   crl [turning-off] : { TIME            , TMP , on         }
                    => < time-until(off) , TMP , delay(off) >
                    if max < TMP + bound = true .

   crl [inmode-on] : { TIME , TMP , on }
                  => < TIME , TMP , on >
                  if max < TMP + bound = false .

   crl [turning-on] : { TIME           , TMP , off       }
                   => < time-until(on) , TMP , delay(on) >
                   if TMP < min + bound = true .

   crl [inmode-off] : { TIME , TMP , off }
                   => < TIME , TMP , off >
                   if TMP < min + bound = false .

   crl [delaying] : { TIME     , TMP , DM }
                 => < TIME - 1 , TMP , DM >
                 if 0 < TIME = true .

    rl [delay-over] : { 0 , TMP , delay(IM) }
                   => < 0 , TMP , IM        > .
```

The thermostat `heat-rate` is determined as a function of the thermostat parameters.

``` {.maude}
    op heat-rate : Mode TMP$Ring -> TMP$Ring .
    ------------------------------------------
    eq heat-rate(MODE, TMP) = (TMP + source(MODE)) - drain(TMP) .
```

The following are parameters which must be filled in for your particular thermostat.

``` {.maude}
    ops min max bound : -> TMP$Ring .
    ---------------------------------

    op time-until : InMode -> TIME$Ring .
    -------------------------------------

    op source : Mode -> TMP$Ring .
    ------------------------------

    op drain : TMP$Ring -> TMP$Ring .
    ---------------------------------
endm
```

### Thermostat Instantiation

#### Instantiation to FVP-INT

``` {.maude}
load ../tools/fvp/numbers.maude

view RingInt from UNITAL-ORDERED-RING to FVP-INT-PRED is
   sort RingBool to Bool .
   sort Ring     to Int  .
endv

mod THERMOSTAT-INT is protecting THERMOSTAT{RingInt, RingInt} . endm
```

The following is an example thermostat over the integers.

``` {.maude}
mod THERMOSTAT-INT-COMFORTABLE is
    extending THERMOSTAT-INT + FVP-NUMBERS .
    var TMP : Int .
```

Here we setup a thermostat which tries to stay between `18 TMP` and `26 TMP`, and will switch states when `3 TMP` away from a boundary temperature.

``` {.maude}
    eq min   = 10 + 8      [variant] .
    eq max   = 10 + 10 + 6 [variant] .
    eq bound = 3           [variant] .
```

The thermostat takes `4 TIME` to turn on, and `2 TIME` to turn off.

``` {.maude}
    eq time-until(on)  = 4 [variant] .
    eq time-until(off) = 2 [variant] .
```

When turning on/off, the heater is effective for `2 TMP/TIME`.
When on, the heater produces `5 TMP/TIME`, and when off produces nothing.
The room looses heat at a rate of `3 TMP/TIME` to the environment.

``` {.maude}
    eq source(on)         = 5 [variant] .
    eq source(off)        = 0 [variant] .
    eq source(delay(on))  = 2 [variant] .
    eq source(delay(off)) = 2 [variant] .

    eq drain(TMP) = 3 [variant] .
endm
```

#### Instantiation to REAL

``` {.maude}
load ../tools/base/smt.maude

view RingReal from UNITAL-ORDERED-RING to REAL is
    sort RingBool to Boolean .
    sort Ring     to Real    .

    op 0 to term 0/1 .
    op 1 to term 1/1 .
endv

mod THERMOSTAT-REAL is protecting THERMOSTAT{RingReal, RingReal} . endm
```

The following is an example thermostat over the reals.
The bound and timing parameters are the same as for the integers, just interpereted over the reals instead.

``` {.maude}
mod THERMOSTAT-REAL-COMFORTABLE is
    extending THERMOSTAT-REAL .
    extending FVP-NUMBERS .

    vars TIME TMP TMP' : Real .
    var IM : InMode . var MD : DelayMode . var MODE : Mode .

    eq min   = 18/1 [variant] .
    eq max   = 26/1 [variant] .
    eq bound = 3/1  [variant] .

    eq time-until(on)  = 4/1 [variant] .
    eq time-until(off) = 2/1 [variant] .

    eq source(on)         = 5/1 [variant] .
    eq source(off)        = 0/1 [variant] .
    eq source(delay(on))  = 2/1 [variant] .
    eq source(delay(off)) = 2/1 [variant] .
```

However, the drain to the outside is now inverse linear with the current temperature.

``` {.maude}
    eq drain(TMP) = 1/10 * (2/1 - TMP) [variant] .
endm
```

#### Instantiation to RAT

``` {.maude}
view RingRat from UNITAL-ORDERED-RING to RAT is
    sort RingBool to Bool .
    sort Ring     to Rat  .

    op 1 to term s(0) .
endv

mod THERMOSTAT-RAT is protecting THERMOSTAT{RingRat, RingRat} . endm
```

The following is an example thermostat over the rats.
The bound and timing parameters are the same as for the integers, just interpereted over the rats instead.

``` {.maude}
mod THERMOSTAT-RAT-COMFORTABLE is
    extending THERMOSTAT-RAT .

    vars TIME TMP TMP' : Rat .
    var IM : InMode . var MD : DelayMode . var MODE : Mode .

    eq min   = 18 [variant] .
    eq max   = 26 [variant] .
    eq bound = 3  [variant] .

    eq time-until(on)  = 4 [variant] .
    eq time-until(off) = 2 [variant] .

    eq source(on)         = 5 [variant] .
    eq source(off)        = 0 [variant] .
    eq source(delay(on))  = 2 [variant] .
    eq source(delay(off)) = 2 [variant] .
```

However, the drain to the outside is now inverse linear with the current temperature.

``` {.maude}
    eq drain(TMP) = 1/10 * (2 - TMP) [variant] .
endm
```

### Example Output

```
load ../../../contrib/systems/thermostat.maude
load ../../../contrib/tools/lmc/lgraph-search.maude
load ../../../contrib/tools/meta/variables.maude

set include BOOL off .

--- ### Thermostat Search Instantiation

mod THERMOSTAT-COMMON-INIT is
   protecting META-TERM .
   protecting RENAME-METAVARS .
    extending CONDITIONAL-NARROWING-GRAPH + FOLDING-LABELED-GRAPH-SEARCH .

    vars TIME TMP TMP' TMP'' T C : Term .
    vars RL Q : Qid . var SUB : Substitution .

    op #nSort : -> Qid .
    op #nTrue : -> Qid .
    --------------------

--- In all cases, the top-sort of the Thermostat parameterized module will be `Conf`, which we instantiate here.

    eq #tSort = 'Conf .
    -------------------

--- Operators `lt`, `le`, `min`, `max`, and `inRange` are macros for specifying meta-level side-conditions compactly/easily for this theory.

   ops lt le : Term Term -> [Term] .
   ---------------------------------
    eq lt(TMP, TMP') = '_?=_[ '_<_[TMP, TMP'], #nTrue] .
    eq le(TMP, TMP') = '_?=_['_<=_[TMP, TMP'], #nTrue] .

   ops min max : -> Term .
   -----------------------
    eq min = qid("min." + string(#nSort)) .
    eq max = qid("max." + string(#nSort)) .

    op inRange : Term Term Term -> [Term] .
    ---------------------------------------
    eq inRange(TMP, TMP', TMP'') = #cConjunct[le(TMP', TMP), le(TMP, TMP'')] .

--- These states capture "good" and "bad" states for the thermostat; `goodConf` has the temperature within bounds, while `badConf` has the temperature out of bounds.

    op goodConf : -> [Term] .
    op  badConf : -> [Term] .
    -------------------------
    eq goodConf = #cTerm['`{_`,_`,_`}[qid("T:" + string(#nSort)) , qid("TMP:" + string(#nSort)) , 'off.Mode ],     inRange(qid("TMP:" + string(#nSort)), min, max)] .
    eq badConf  = #cTerm['`{_`,_`,_`}[qid("T:" + string(#nSort)) , qid("TMP:" + string(#nSort)) , 'M:Mode   ], '~_[inRange(qid("TMP:" + string(#nSort)), min, max)]] .

--- **TODO**: Move this equation to somewhere more generic.
--- In order to take advantage of our ability to prune infeasible states, we extend the definition of `prune` to check if the side-condition is unsatisfiable.
--- For a specific theory, one can specify `isUnsat` so that it simplifies to `true` if you can prove infeasibility.

   ceq prune(< label(RL, SUB) , state(Q[T, C]) >) = .TransitionSet if Q = #cTerm /\ isUnsat(C) .
   ---------------------------------------------------------------------------------------------

    op isUnsat : Term -> [Bool] .
    -----------------------------
endm

--- ### Thermostat Search modulo FVP-INT

--- By importing `UNCONDITIONALIZE-FVP-BOOL`, we already have the relevant instantiation for the `FVP-BOOL` side-condition theory.
--- In addition, we tell the tool that the top sort in the original theory `#MO` is `Conf`.

mod THERMOSTAT-INT-NARROWING-GRAPH is
    extending THERMOSTAT-COMMON-INIT .
    extending UNCONDITIONALIZE-FVP-BOOL .

    var T : Term . var TS : TermSet . vars NeTS NeTS' : NeTermSet .
    vars V V' : Variant . var VS : VariantSet .
    var N : Nat . var P : Parent . var B : Bool . var SUB : Substitution .

    eq #nSort = 'Int .
    eq #nTrue = 'true.Bool .
    ------------------------

    eq #MO = upModule('THERMOSTAT-INT-COMFORTABLE, true) .
    ------------------------------------------------------

--- Here we check that a given constraint `isUnsat` by generating variants and checking whether they are all false or exactly a renaming.
--- **TODO**: Invoke varsat.

   ceq isUnsat(T) = false                       if T == #cTrue .
   ceq isUnsat(T) = true                        if T == #cFalse .
   ceq isUnsat(T) = filterFalse(TS) == .TermSet if not T == #cTrue /\ not T == #cFalse
                                                /\ TS := getTerms(filterRenaming(variants(#M, renameTmpVar(#M, T)))) .
   -------------------------------------------------------------------------------------------------------------------

    op filterFalse : TermSet -> [TermSet] .
    ---------------------------------------
    eq filterFalse(T) = if T == #cFalse then .TermSet else T fi .

    eq filterFalse(.TermSet)     = .TermSet .
    eq filterFalse(NeTS | NeTS') = filterFalse(NeTS) | filterFalse(NeTS') .
endm

reduce wellFormed(#M) .
reduce wellFormed(#M, goodConf) .
reduce wellFormed(#M, badConf) .

reduce bfs(state(goodConf), 2) .
reduce state(goodConf) =>[5] state(badConf) .

--- ### Thermostat Search modulo REAL

view Boolean from TRIV to REAL is sort Elt to Boolean . endv

fmod EQFORM-SMT-BOOLEAN is
   protecting EQFORM-IMPL{Boolean} + REAL-SIMPLIFICATION .
   protecting META-LEVEL * ( sort Nat to NatP
                           , op _+_             to _+P_
                           , op _=/=_           to _=/=P_
                           , op _==_            to _==P_
                           , op if_then_else_fi to if_then_else_fiP
                           , op min             to minP
                           , op max             to maxP
                           , op sd              to sdP
                           ) .

    vars NTFB NTFB' : NoTrueForm{Boolean} . vars NFFB NFFB' : NoFalseForm{Boolean} .
    vars FB FB' : Form{Boolean} . vars B B' : Boolean .

--- **TODO**: `git log` indicates that the behaviour of `metaCheck` is that { sat } => true { unsat unknown } => false.
---           This could make this simplification unsound in the case of `{ unknown } => false`

    op simplify : Form{Boolean} -> Form{Boolean} .
    ----------------------------------------------
    eq simplify(FB) = FB [owise] .
   ceq simplify(FB) = tt if B := toBoolean(~ FB) /\ false = metaCheck(upModule('REAL, true), upTerm(B)) .
   ceq simplify(FB) = ff if B := toBoolean(  FB) /\ false = metaCheck(upModule('REAL, true), upTerm(B)) .

    op toBoolean : Form{Boolean} -> Boolean .
    -----------------------------------------
    eq toBoolean(B ?= B') = B  === B' .
    eq toBoolean(B != B') = B =/== B' .
    eq toBoolean(~ FB)    = not toBoolean(FB) .

    eq toBoolean(NTFB /\ NTFB') = toBoolean(NTFB) and toBoolean(NTFB') .
    eq toBoolean(NTFB /\ NTFB') = toBoolean(NTFB) and toBoolean(NTFB') .
    eq toBoolean(NFFB \/ NFFB') = toBoolean(NFFB) or  toBoolean(NFFB') .
    eq toBoolean(NFFB \/ NFFB') = toBoolean(NFFB) or  toBoolean(NFFB') .

    eq toBoolean(FB => FB') = toBoolean(FB) implies toBoolean(FB') .
endfm

reduce simplify(false ?= 0/1 < 1/1) .
reduce simplify(true  ?= 0/1 < 1/1) .

reduce simplify((true ?= X:Real <= Y:Real) => (true ?= X:Real <  Y:Real)) .
reduce simplify((true ?= X:Real <  Y:Real) => (true ?= X:Real <= Y:Real)) .

reduce simplify((true ?= X:Real <= Y:Real) => (true ?= X:Real <= (Y:Real + 2/1))) .

--- For the real-numbered thermostat, we instantiat the parameter theory `#cModule` to `EQFORM-SMT-BOOLEAN` instead.
--- The top sort of the module remains the same (`Conf`), as it comes from the same parametric theory.

fmod UNCONDITIONALIZE-REAL is
   protecting DETERMINISTIC-VARIABLES .
    including UNCONDITIONALIZE .

    vars T T' : Term . var S : Sort . vars EqC EqC' : EqCondition .

    eq #cModule   = 'EQFORM-SMT-BOOLEAN .
    eq #cSort     = 'Form`{Boolean`} .
    eq #cTrue     = 'tt.TrueLit`{Boolean`} .
    eq #cFalse    = 'ff.FalseLit`{Boolean`} .
    eq #cConjunct = '_/\_ .
    -----------------------

    eq #mkCondition(T  = T') = '_?=_[T, T'] .
    eq #mkCondition(T := T') = '_?=_[T, T'] .
    eq #mkCondition(T : S)   = '_?=_[T, #var(T, S)] .
    -------------------------------------------------
endfm

mod THERMOSTAT-REAL-NARROWING-GRAPH is
    including UNCONDITIONALIZE-REAL .
    including THERMOSTAT-COMMON-INIT .

    vars T T' : Term . var TYPE : Type .

    eq #nSort = 'Real .
    eq #nTrue = 'true.Boolean .
    ---------------------------

    eq #MO = upModule('THERMOSTAT-REAL-COMFORTABLE, true) .
    -------------------------------------------------------

--- For the real numbers, we check `isUnsat` via Maude's meta-level hookup to an SMT solver.

   ceq isUnsat(T) = T' == #cFalse if { T' , TYPE } := metaReduce(upModule(#cModule, true), 'simplify[T]) .
   -------------------------------------------------------------------------------------------------------

   ceq implies?(T, T') = T' == #cTrue if { T' , TYPE } := metaReduce(upModule(#cModule, true), 'simplify['_=>_[T, T']]) .
   ----------------------------------------------------------------------------------------------------------------------
endm

reduce wellFormed(#M) .
reduce wellFormed(#M, goodConf) .
reduce wellFormed(#M, badConf) .

reduce isUnsat(lt('X:Real, 'Y:Real)) == false .
reduce isUnsat(lt('X:Real, 'X:Real)) == true .
reduce isUnsat(le('X:Real, 'Y:Real)) == false .
reduce isUnsat(le('X:Real, 'X:Real)) == false .

reduce isUnsat(#cConjunct[lt('X:Real, 'Y:Real), #cTrue]) == false .
reduce isUnsat(#cConjunct[lt('X:Real, 'X:Real), #cTrue]) == true .
reduce isUnsat(#cConjunct[le('X:Real, 'Y:Real), #cTrue]) == false .
reduce isUnsat(#cConjunct[le('X:Real, 'X:Real), #cTrue]) == false .

reduce isUnsat(#cConjunct[lt('X:Real, 'Y:Real), #cFalse]) == true .
reduce isUnsat(#cConjunct[lt('X:Real, 'X:Real), #cFalse]) == true .
reduce isUnsat(#cConjunct[le('X:Real, 'Y:Real), #cFalse]) == true .
reduce isUnsat(#cConjunct[le('X:Real, 'X:Real), #cFalse]) == true .

reduce isUnsat(#cConjunct[lt('X:Real, 'Y:Real), lt('Y:Real, 'X:Real)]) == true .
reduce isUnsat(#cConjunct[le('X:Real, 'Y:Real), le('Y:Real, 'X:Real)]) == false .

reduce implies?(lt('X:Real, 'Y:Real), le('X:Real, 'Y:Real)) .

reduce bfs(state(goodConf), 12) .
reduce state(goodConf) =>[5] state(badConf) .

--- ### Thermostat Search modulo REAL with RAT Concrete Represenattion

fmod EQFORM-RAT is protecting EQFORM-IMPL{Bool} + RAT . endfm

fmod UNCONDITIONALIZE-RAT is
   protecting DETERMINISTIC-VARIABLES .
    including UNCONDITIONALIZE .

    vars T T' : Term . var S : Sort . vars EqC EqC' : EqCondition .

    eq #cModule   = 'EQFORM-RAT .
    eq #cSort     = 'Form`{Bool`} .
    eq #cTrue     = 'tt.TrueLit`{Bool`} .
    eq #cFalse    = 'ff.FalseLit`{Bool`} .
    eq #cConjunct = '_/\_ .
    -----------------------

    eq #mkCondition(T  = T') = '_?=_[T, T'] .
    eq #mkCondition(T := T') = '_?=_[T, T'] .
    eq #mkCondition(T : S)   = '_?=_[T, #var(T, S)] .
    -------------------------------------------------
endfm

mod THERMOSTAT-RAT-NARROWING-GRAPH is
   protecting RAT-REAL-CONVERSION .
    including UNCONDITIONALIZE-RAT .
    including THERMOSTAT-COMMON-INIT .

    vars T T' : Term . var TYPE : Type .

    eq #nSort = 'Rat .
    eq #nTrue = 'true.Bool .
    ------------------------

    eq #MO = upModule('THERMOSTAT-RAT-COMFORTABLE, true) .
    ------------------------------------------------------

   ceq isUnsat(T) = T' == #cFalse if { T' , TYPE } := metaReduce(upModule('EQFORM-SMT-BOOLEAN, true), 'simplify[#Rat2Real(T)]) .
   -----------------------------------------------------------------------------------------------------------------------------

   ceq implies?(T, T') = T' == #cTrue if { T' , TYPE } := metaReduce(upModule('EQFORM-SMT-BOOLEAN, true), 'simplify['_=>_[#Rat2Real(T), #Rat2Real(T')]]) .
   -------------------------------------------------------------------------------------------------------------------------------------------------------
endm

reduce wellFormed(#M) .
reduce wellFormed(#M, goodConf) .
reduce wellFormed(#M, badConf) .

reduce isUnsat(lt('X:Rat, 'X:Rat)) == true .
reduce isUnsat(le('X:Rat, 'Y:Rat)) == false .
reduce isUnsat(le('X:Rat, 'X:Rat)) == false .

reduce isUnsat(#cConjunct[lt('X:Rat, 'Y:Rat), #cTrue]) == false .
reduce isUnsat(#cConjunct[lt('X:Rat, 'X:Rat), #cTrue]) == true .
reduce isUnsat(#cConjunct[le('X:Rat, 'Y:Rat), #cTrue]) == false .
reduce isUnsat(#cConjunct[le('X:Rat, 'X:Rat), #cTrue]) == false .

reduce isUnsat(#cConjunct[lt('X:Rat, 'Y:Rat), #cFalse]) == true .
reduce isUnsat(#cConjunct[lt('X:Rat, 'X:Rat), #cFalse]) == true .
reduce isUnsat(#cConjunct[le('X:Rat, 'Y:Rat), #cFalse]) == true .
reduce isUnsat(#cConjunct[le('X:Rat, 'X:Rat), #cFalse]) == true .

reduce isUnsat(#cConjunct[lt('X:Rat, 'Y:Rat), lt('Y:Rat, 'X:Rat)]) == true .
reduce isUnsat(#cConjunct[le('X:Rat, 'Y:Rat), le('Y:Rat, 'X:Rat)]) == false .

reduce implies?(lt('X:Real, 'Y:Real), le('X:Real, 'Y:Real)) .

reduce bfs(state(goodConf), 2) .
reduce state(goodConf) =>[5] state(badConf) .
```

Bakery Protocol
---------------

The Bakery protocol is a simple scheduling protocol for multiple processes.
The details differ among implementations, but the same basic ingredients are the same.
Processes wait for their turn to enter the critical section, synchronizing using some token or external state.
An implementation which allows multiple processes into the critical section simultaneously is buggy.

``` {.maude}
load ../tools/fvp/numbers.maude

set include BOOL off .
set include NAT  off .
```

### Two Process Bakery

This bakery protocol has only two processes which keep track of one counter each.

``` {.maude}
mod BAKERY-FVP is
   protecting FVP-NAT-PRED .

    sorts Mode NcMode BState .
    --------------------------
    subsort NcMode < Mode .

    vars P Q : Mode . vars X Y : Nat .

    op sleep : -> NcMode [ctor] .
    op wait  : -> NcMode [ctor] .
    op crit  : -> Mode   [ctor] .
    -----------------------------

    op <_,_,_,_> : Mode Nat Mode Nat -> BState [ctor] .
    ---------------------------------------------------
    rl [p1_sleep] : < sleep, X, Q, Y > => <  wait, Y + 1, Q, Y > .
    rl [p1_wait1] : <  wait, X, Q, 0 > => <  crit, X,     Q, 0 > .
   crl [p1_wait2] : <  wait, X, Q, Y > => <  crit, X,     Q, Y > if X <= Y = true .
    rl [p1_crit]  : <  crit, X, Q, Y > => < sleep, 0,     Q, Y > .

    rl [p2_sleep] : < P, X, sleep, Y > => < P, X,  wait, X + 1  > .
    rl [p2_wait1] : < P, 0,  wait, Y > => < P, 0,  crit, Y      > .
   crl [p2_wait2] : < P, X,  wait, Y > => < P, X,  crit, Y      > if Y < X = true .
    rl [p2_crit]  : < P, X,  crit, Y > => < P, X, sleep, 0      > .
endm
```

After constructor-decomposition of `BAKERY-FVP`, you should get the following theory.

**TODO**: deduplicate rule names

``` {.maude}
mod BAKERY-FVP-CTOR is
   protecting FVP-NAT-PRED .

    sorts Mode NcMode BState .
    --------------------------
    subsort NcMode < Mode .

    vars P Q : Mode . vars X Y : Nat .

    op sleep : -> NcMode [ctor] .
    op wait  : -> NcMode [ctor] .
    op crit  : -> Mode   [ctor] .
    -----------------------------

    op <_,_,_,_> : Mode Nat Mode Nat -> BState [ctor] .
    ---------------------------------------------------
    rl [p1_sleep] : < sleep, X,      Q, Y               > => <  wait, Y + 1,  Q, Y               > [narrowing] .
    rl [p1_wait1] : <  wait, X,      Q, 0               > => <  crit, X,      Q, 0               > [narrowing] .
    rl [p1_wait2] : <  wait, V1:Nat, Q, V1:Nat + V2:Nat > => <  crit, V1:Nat, Q, V1:Nat + V2:Nat > [narrowing] .
    rl [p1_crit]  : <  crit, X,      Q, Y               > => < sleep, 0,      Q, Y               > [narrowing] .

    rl [p2_sleep] : < P, X,                 sleep, Y      > => < P, X,                  wait, X + 1  > [narrowing] .
    rl [p2_wait1] : < P, 0,                  wait, Y      > => < P, 0,                  crit, Y      > [narrowing] .
    rl [p2_wait2] : < P, V1:Nat + V2:NzNat,  wait, V1:Nat > => < P, V1:Nat + V2:NzNat,  crit, V1:Nat > [narrowing] .
    rl [p2_crit]  : < P, X,                  crit, Y      > => < P, X,                 sleep, 0      > [narrowing] .
endm
```

### Arbitrary Processes Bakery

This version comes from the book *All About Maude* [@all-about-maude].

``` {.maude}
fmod BAKERY-SYNTAX is
    protecting FVP-NAT-PRED * ( sort Nat to Name
                              , op 1 to s
                              , op _+_ to __
                              ) .

    sorts ModeIdle ModeWait ModeCrit Mode .
    ---------------------------------------
    subsorts ModeIdle ModeWait ModeCrit < Mode .

    sorts Proc ProcIdle ProcWait .
    ------------------------------
    subsorts ProcIdle < ProcWait < Proc .

    sorts ProcIdleSet ProcWaitSet ProcSet .
    ---------------------------------------
    subsort ProcIdle < ProcIdleSet .
    subsort ProcWait < ProcWaitSet .
    subsort Proc     < ProcSet .
    subsorts ProcIdleSet < ProcWaitSet < ProcSet .

    op idle :      -> ModeIdle .
    op wait : Name -> ModeWait .
    op crit : Name -> ModeCrit .
    ----------------------------

    op `[_`] : ModeIdle -> ProcIdle .
    op `[_`] : ModeWait -> ProcWait .
    op `[_`] : Mode     -> Proc .
    -----------------------------

    op none :                         -> ProcIdleSet .
    op __   : ProcIdleSet ProcIdleSet -> ProcIdleSet [assoc comm id: none format(d s d)] .
    op __   : ProcWaitSet ProcWaitSet -> ProcWaitSet [ditto] .
    op __   :     ProcSet     ProcSet ->     ProcSet [ditto] .
    ----------------------------------------------------------
endfm

mod BAKERY is
    protecting BAKERY-SYNTAX .

    var PS : ProcSet . vars N M : Name .

    sort Conf .
    -----------

    op _;_;_ : Name Name ProcSet -> Conf [prec 56 format(d s s s s d)] .
    --------------------------------------------------------------------
    rl [wake] : N ; M ; [idle]    PS => (s N) ; M     ; [wait(N)] PS [narrowing] .
    rl [crit] : N ; M ; [wait(M)] PS => N     ; M     ; [crit(M)] PS [narrowing] .
    rl [exit] : N ; M ; [crit(M)] PS => N     ; (s M) ; [idle]    PS [narrowing] .
endm
```

### Example Output

```
set show timing off .

load ../../../contrib/systems/bakery.maude
load ../../../contrib/tools/lmc/lgraph-search.maude

--- ### Bakery System Instatiation

fmod BAKERY-NARROWING-GRAPH-COMMON is
    including UNCONDITIONALIZE-FVP-BOOL .
    including ( CONDITIONAL-NARROWING-GRAPH + FOLDING-LABELED-GRAPH-SEARCH ) .
    including FOLDING-LABELED-GRAPH-RENDER-TIKZ + LEXICAL .

    eq #tSort = 'Conf .
    -------------------

    vars T T' T'' : Term . var TL : TermList . var Q : Qid . var QL : QidList .
    var SUB : Substitution . vars STR STR' : String .

    op upIdle : -> [Term] .
    -----------------------
    eq upIdle = '`[_`]['idle.ModeIdle] .

    op upCrit : Term -> [Term] .
    ----------------------------
    eq upCrit(T) = '`[_`]['crit[T]] .

   ops lt le : Term Term -> [Term] .
   ---------------------------------
    eq lt(T, T') = '_?=_[ '_<_[T, T'], 'true.Bool] .
    eq le(T, T') = '_?=_['_<=_[T, T'], 'true.Bool] .

    op conf : Term Term Term -> [Term] .
    ------------------------------------
    eq conf(T, T', T'') = '_;_;_[T, T', T''] .

--- The following initial terms characterize our initialization and bad states.
---
--- -   `conf2Idle` has two idle processes.
--- -   `confManyIdle` has an arbitrary set of `idle` processes.
--- -   `confManyWait` has an arbitrary set of `wait` processes.
--- -   `confManyProc` has an arbitrary set of any processes.

    op conf2Idle    : Term Term -> [Term] .
    op confManyIdle : Term Term -> [Term] .
    op confManyWait : Term Term -> [Term] .
    op confManyProc : Term Term -> [Term] .
    ---------------------------------------
    eq conf2Idle   (T, T') = conf(T, T', '__[upIdle, upIdle]) .
    eq confManyIdle(T, T') = conf(T, T', 'PIS:ProcIdleSet)    .
    eq confManyWait(T, T') = conf(T, T', 'PWS:ProcWaitSet)    .
    eq confManyProc(T, T') = conf(T, T', 'PS:ProcSet)         .

--- -   `conf2Crit` is any constrained configuration which contains two `crit` processes.

    op conf2Crit : -> [Term] .
    --------------------------
    eq conf2Crit = #cTerm['_;_;_['N:Name, 'M:Name, '__['`[_`]['MC1:ModeCrit], '`[_`]['MC2:ModeCrit], 'PS:ProcSet]], #cTrue] .

    op stripVarTypes : QidList -> [QidList] .
    -----------------------------------------
    eq stripVarTypes(nil)  = nil .
    eq stripVarTypes(Q QL) = if (Q :: Variable) or (Q :: Constant) then getName(Q) else Q fi stripVarTypes(QL) .

    op renamingSubst : -> Substitution .
    ------------------------------------
    eq renamingSubst = '@1:Name <- 'X:Name ; '%1:Name <- 'N:Name
                     ; '@2:Name <- 'Y:Name ; '%2:Name <- 'M:Name
                     ; '@3:Name <- 'Z:Name
                     ; '@2:NzNat <- 'NzN:NzNat
                     ; '@2:ProcIdleSet <- 'PIS:ProcIdleSet
                     ; '@2:ProcWaitSet <- 'PWS2:ProcWaitSet
                     ; '@3:ProcWaitSet <- 'PWS3:ProcWaitSet .

    op renderDirect : Term -> String .
    ------------------------------------
    eq renderDirect(T) = printTokens(stripVarTypes(metaPrettyPrint(#M, T << renamingSubst))) .

    eq renderTikZLabel(label(Q , SUB)) = string(Q) .

    op renderTikZState : Term -> String .
    -------------------------------------
    eq renderTikZState(T) = "\texttt{" + renderDirect(T) + "}" .

    op renderTikZConstraint : Term -> String .
    ------------------------------------------
    eq renderTikZConstraint('_<=_[T, T']) = "\texttt{" + renderDirect(T) + "} \\le \texttt{" + renderDirect(T') + "}" .
    eq renderTikZConstraint('_<_ [T, T']) = "\texttt{" + renderDirect(T) + "} \\lt \texttt{" + renderDirect(T') + "}" .

    eq renderTikZConstraint('_?=_['true.Bool, T]) = renderTikZConstraint(T) .
    eq renderTikZConstraint('_?=_[T, 'true.Bool]) = renderTikZConstraint(T) .

   ceq renderTikZConstraint(T) = "" if T  := #cTrue .
    eq renderTikZConstraint(T) = "\texttt{" + renderDirect(T) + "}" [owise] .

   ceq renderTikZNode(state(Q[T,T'])) = "$" + renderTikZState(T) + STR' + "$"
                                     if Q := #cTerm
                                     /\ STR  := renderTikZConstraint(T')
                                     /\ STR' := if STR == "" then "" else "\\ st \\ " + STR fi .
endfm

reduce wellFormed(upModule('BAKERY, false), conf2Idle   ('X:Name, 'Y:Name)) .
reduce wellFormed(upModule('BAKERY, false), confManyIdle('X:Name, 'Y:Name)) .
reduce wellFormed(upModule('BAKERY, false), confManyWait('X:Name, 'Y:Name)) .
reduce wellFormed(upModule('BAKERY, false), confManyProc('X:Name, 'Y:Name)) .

--- ### Two Process Bakery

--- #### With Equational Abstraction

--- As a sanity check, results of section 3.1 of [@bae-escobar-meseguer-abstract-model-checking] are replicated.
--- This required an equational abstraction which reduced all the names in the configuration uniformly when they are all non-`0`.

--- **TODO**: Why is this system converging without the equational abstraction?

mod BAKERY-2PROC-EQUATIONAL-ABSTRACTION is
    including BAKERY .

    vars N M K L : Name . var PIS : ProcIdleSet .

---    eq s N ; s M ; [idle]      [idle]      = N ; M ; [idle]    [idle] .
---    eq s N ; s M ; [wait(s K)] [idle]      = N ; M ; [wait(K)] [idle] .
---    eq s N ; s M ; [crit(s K)] [idle]      = N ; M ; [crit(K)] [idle] .
---    eq s N ; s M ; [wait(s K)] [wait(s L)] = N ; M ; [wait(K)] [wait(L)] .
---    eq s N ; s M ; [wait(s K)] [crit(s L)] = N ; M ; [wait(K)] [crit(L)] .
---    eq s N ; s M ; [crit(s K)] [crit(s L)] = N ; M ; [crit(K)] [crit(L)] .
endm

--- Here we instantiate to the abstracted theory:

mod BAKERY-2PROC-EQUATIONAL-ABSTRACTION-NARROWING-GRAPH is
    extending BAKERY-NARROWING-GRAPH-COMMON .

    eq #MO = upModule('BAKERY-2PROC-EQUATIONAL-ABSTRACTION, true) .

--- The initial state has two `idle` processes and both counters `N`:

    op conf2IdleEq : -> [Term] .
    ----------------------------
    eq conf2IdleEq = #cTerm[conf2Idle('X:Name, 'X:Name), #cTrue] .
endm

reduce wellFormed(#M) .

reduce wellFormed(#M, conf2Crit) .
reduce wellFormed(#M, conf2IdleEq) .

--- Under this assumption, a finite state graph is achieved.
--- The resulting graph differs from [@bae-escobar-meseguer-abstract-model-checking] because here states which are $=_{B}$ are identified (with $B = AC$ for the process set).
--- In addition, edges which take advantage of the equational abstraction to fold into a prior state are not identified.

reduce renderTikZGraph(0, bfs(state(conf2IdleEq)))
    == "\tikzset{elliptic state/.style={draw,ellipse}}"
       "\begin{tikzpicture}[shorten >=1pt,on grid,transform shape]"
       "\footnotesize"
       "    \node[elliptic state] (s_0) at (0,-10) {$\texttt{X ; X ; [idle] [idle]}$};"
       "    \node[elliptic state] (s_2) at (0,-14) {$\texttt{s X ; X ; [idle] [wait(X)]}$};"
       "    \node[elliptic state] (s_4) at (0,-18) {$\texttt{s X ; X ; [idle] [crit(X)]}$};"
       "    \node[elliptic state] (s_6) at (2,-17) {$\texttt{s s X ; X ; [wait(X)] [wait(s X)]}$};"
       "    \node[elliptic state] (s_10) at (0,-22) {$\texttt{s s X ; X ; [crit(X)] [wait(s X)]}$};"
       "    \\path[->] (s_10) edge [bend left=10] node [pos=0.3,right] {exit} (s_2);"
       "    \\path[->] (s_6) edge [bend left=10] node [pos=0.3,right] {crit} (s_10);"
       "    \\path[->] (s_4) edge [bend left=10] node [pos=0.3,right] {wake} (s_10);"
       "    \\path[->>] (s_4) edge [bend left=10] node [pos=0.3,left] {exit} (s_0);"
       "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {wake} (s_6);"
       "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {crit} (s_4);"
       "    \\path[->] (s_0) edge [bend left=10] node [pos=0.3,right] {wake} (s_2);"
       "\\end{tikzpicture}" .

--- We can also search for a reach-path to a "bad" state in `conf2Crit`, and see that there is none:

--- **TODO**: Fix bug in this query.

reduce state(conf2IdleEq) =>* state(conf2Crit) .

--- #### With Constrained Bakery

--- Here the equational abstraction is avoided by taking advantage of constrained states.

mod BAKERY-2PROC-NARROWING-GRAPH is
    extending BAKERY-NARROWING-GRAPH-COMMON .

--- Instead we instantiate to the original theory:

    eq #MO = upModule('BAKERY, true) .
endm

reduce wellFormed(#M) .
reduce wellFormed(#M, conf2Crit) .

--- From an initial state with arbitrary counters, we are on course to have an infinite representation of the state space.

reduce renderTikZGraph(0, bfs(state(#cTerm[conf2Idle('X:Name, 'Y:Name), #cTrue]), 4))
    == "\tikzset{elliptic state/.style={draw,ellipse}}"
       "\begin{tikzpicture}[shorten >=1pt,on grid,transform shape]"
       "\footnotesize"
       "    \node[elliptic state] (s_0) at (0,-10) {$\texttt{X ; Y ; [idle] [idle]}$};"
       "    \node[elliptic state] (s_2) at (0,-14) {$\texttt{s X ; Y ; [idle] [wait(X)]}$};"
       "    \node[elliptic state] (s_4) at (0,-18) {$\texttt{s X ; X ; [idle] [crit(X)]}$};"
       "    \node[elliptic state] (s_6) at (2,-17) {$\texttt{s s Y ; X ; [wait(Y)] [wait(s Y)]}$};"
       "    \node[elliptic state] (s_10) at (0,-22) {$\texttt{s s X ; X ; [crit(X)] [wait(s X)]}$};"
       "    \node[elliptic state] (s_14) at (2,-21) {$\texttt{s s X ; s X ; [crit(s X)] [wait(X)]}$};"
       "    \node[elliptic state] (s_18) at (0,-26) {$\texttt{s s X ; s s X ; [idle] [wait(X)]}$};"
       "    \\path[->] (s_14) edge [bend left=10] node [pos=0.3,right] {exit} (s_18);"
       "    \\path[->>] (s_10) edge [bend left=10] node [pos=0.3,left] {exit} (s_2);"
       "    \\path[->] (s_6) edge [bend left=10] node [pos=0.3,right] {crit} (s_14);"
       "    \\path[->] (s_6) edge [bend left=10] node [pos=0.3,right] {crit} (s_10);"
       "    \\path[->] (s_4) edge [bend left=10] node [pos=0.3,right] {wake} (s_10);"
       "    \\path[->>] (s_4) edge [bend left=10] node [pos=0.3,left] {exit} (s_0);"
       "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {wake} (s_6);"
       "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {crit} (s_4);"
       "    \\path[->] (s_0) edge [bend left=10] node [pos=0.3,right] {wake} (s_2);"
       "\\end{tikzpicture}" .

--- Instead, we can refine the state-description by starting in states such that `Y <= X`, where `Y` is the critical counter and `X` is the next available number.
--- Here we see that a finite state graph is achieved, where some of the states have additional constraints:

reduce renderTikZGraph(0, bfs(state(#cTerm[conf2Idle('X:Name, 'Y:Name), le('Y:Name, 'X:Name)])))
    == "\tikzset{elliptic state/.style={draw,ellipse}}"
       "\begin{tikzpicture}[shorten >=1pt,on grid,transform shape]"
       "\footnotesize"
       "    \node[elliptic state] (s_0) at (0,-10) {$\texttt{X ; Y ; [idle] [idle]}\\ st \\ \texttt{Y} \\le \texttt{X}$};"
       "    \node[elliptic state] (s_2) at (0,-14) {$\texttt{s X ; Y ; [idle] [wait(X)]}\\ st \\ \texttt{Y} \\le \texttt{X}$};"
       "    \node[elliptic state] (s_4) at (2,-13) {$\texttt{s N M ; N ; [idle] [wait(N M)]}$};"
       "    \node[elliptic state] (s_6) at (0,-18) {$\texttt{s N ; N ; [idle] [crit(N)]}$};"
       "    \node[elliptic state] (s_8) at (2,-17) {$\texttt{s s Y ; X ; [wait(Y)] [wait(s Y)]}\\ st \\ \texttt{X} \\le \texttt{Y}$};"
       "    \node[elliptic state] (s_10) at (4,-16) {$\texttt{s s N M ; N ; [wait(N M)] [wait(s N M)]}$};"
       "    \node[elliptic state] (s_16) at (0,-22) {$\texttt{s X ; s X ; [idle] [idle]}$};"
       "    \node[elliptic state] (s_18) at (2,-21) {$\texttt{s s X ; X ; [crit(X)] [wait(s X)]}$};"
       "    \\path[->>] (s_18) edge [bend left=10] node [pos=0.3,left] {exit} (s_4);"
       "    \\path[->>] (s_16) edge [bend left=10] node [pos=0.3,left] {wake} (s_4);"
       "    \\path[->] (s_10) edge [bend left=10] node [pos=0.3,right] {crit} (s_18);"
       "    \\path[->>] (s_8) edge [bend left=10] node [pos=0.3,left] {crit} (s_18);"
       "    \\path[->] (s_6) edge [bend left=10] node [pos=0.3,right] {wake} (s_18);"
       "    \\path[->] (s_6) edge [bend left=10] node [pos=0.3,right] {exit} (s_16);"
       "    \\path[->>] (s_4) edge [bend left=10] node [pos=0.3,left] {wake} (s_10);"
       "    \\path[->>] (s_4) edge [bend left=10] node [pos=0.3,left] {crit} (s_6);"
       "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {wake} (s_10);"
       "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {wake} (s_8);"
       "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {crit} (s_6);"
       "    \\path[->] (s_0) edge [bend left=10] node [pos=0.3,right] {wake} (s_4);"
       "    \\path[->] (s_0) edge [bend left=10] node [pos=0.3,right] {wake} (s_2);"
       "\\end{tikzpicture}" .

--- #### Reversed Search for Coinvariants

mod BAKERY-REVERSE-NARROWING-GRAPH is
   protecting REVERSE-RULES .
    extending BAKERY-NARROWING-GRAPH-COMMON .

    eq #MO = reverseRules(upModule('BAKERY, true)) .
endm

reduce wellFormed(#M) .

reduce wellFormed(#M, conf2Crit) .

reduce state(conf2Idle)    =>[10] state(confBad) .
reduce state(confManyWait) =>[10] state(confBad) .

--- ### Arbitrary Process Bakery

--- #### With Equational Abstraction

--- As a sanity check, results of section 3.3 of [@bae-escobar-meseguer-abstract-model-checking] are replicated.
--- This required an equational abstraction which removed "extra" waiting processes, which was shown to be sound.

mod BAKERY-NPROC-EQUATIONAL-ABSTRACTION is
    including BAKERY .

    vars M L : Name . var PS : ProcSet .

    eq s s s L M ; M ; PS [wait(s L M)] [wait(s s L M)]
     =   s s L M ; M ; PS [wait(s L M)] .
endm

--- Here we instantiate to the abstracted theory:

mod BAKERY-NPROC-EQUATIONAL-ABSTRACTION-NARROWING-GRAPH is
    extending BAKERY-NARROWING-GRAPH-COMMON .

    eq #MO = upModule('BAKERY-NPROC-EQUATIONAL-ABSTRACTION, true) .

--- The initial state has all `idle` processes and equal counters.

    op confManyIdleEq : -> [Term] .
    -------------------------------
    eq confManyIdleEq = #cTerm[confManyIdle('X:Name, 'X:Name), #cTrue] .
endm

reduce wellFormed(#M) .
reduce wellFormed(#M, conf2Crit) .

--- Under this assumption, a finite state graph is achieved.
--- The resulting graph differs from [@bae-escobar-meseguer-abstract-model-checking] because here states which are $=_{B}$ are identified (with $B = AC$ for the process set).
--- In addition, edges which take advantage of the equational abstraction to fold into a prior state are not identified.

reduce renderTikZGraph(0, bfs(state(confManyIdleEq)))
    == "\tikzset{elliptic state/.style={draw,ellipse}}"
       "\begin{tikzpicture}[shorten >=1pt,on grid,transform shape]"
       "\footnotesize"
       "    \node[elliptic state] (s_0) at (0,-10) {$\texttt{X ; X ; PIS}$};"
       "    \node[elliptic state] (s_2) at (0,-14) {$\texttt{s X ; X ; PIS [wait(X)]}$};"
       "    \node[elliptic state] (s_4) at (0,-18) {$\texttt{s X ; X ; PIS [crit(X)]}$};"
       "    \node[elliptic state] (s_6) at (2,-17) {$\texttt{s s X ; X ; PIS [wait(X)] [wait(s X)]}$};"
       "    \node[elliptic state] (s_10) at (0,-22) {$\texttt{s s X ; X ; PIS [crit(X)] [wait(s X)]}$};"
       "    \\path[->>] (s_10) edge [loop below,min distance=25] node [pos=0.3,left] {wake} (s_10);"
       "    \\path[->>] (s_10) edge [bend left=10] node [pos=0.3,left] {exit} (s_2);"
       "    \\path[->>] (s_6) edge [loop below,min distance=25] node [pos=0.3,left] {wake} (s_6);"
       "    \\path[->>] (s_6) edge [bend left=10] node [pos=0.3,left] {crit} (s_10);"
       "    \\path[->] (s_4) edge [bend left=10] node [pos=0.3,right] {wake} (s_10);"
       "    \\path[->>] (s_4) edge [bend left=10] node [pos=0.3,left] {exit} (s_0);"
       "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {wake} (s_6);"
       "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {crit} (s_4);"
       "    \\path[->] (s_0) edge [bend left=10] node [pos=0.3,right] {wake} (s_2);"
       "\\end{tikzpicture}" .

--- #### With Constrained States and Lemmas

--- Similar to the two process case, an equational abstraction of the system can be avoided searching in a constraint system.

mod BAKERY-NPROC-RECFUNC is
   protecting BAKERY .
   protecting FVP-BOOL-EQFORM * ( sort Nat to Name
                                , op 1 to s
                                , op _+_ to __
                                ) .

    vars N N' M : Name . vars NzN NzN' : NzNat . vars EqF EqF' : EqForm{Bool} .
    var P : Proc . var PWS : ProcWaitSet . vars PS : ProcSet .

--- The predicate `ill-formed` encodes the cases when a queue at the bakery is not well formed.
--- The first name corresponds to the current customer being served, and the second to the next number someone will pull if they walk in.

    op ill-formed : Name ProcSet Name -> Bool .
    -------------------------------------------

--- Bakerys can be ill-formed because the pool of waiting processes has numbers which
---
--- -   should have already been called, or
--- -   remain to be called.

    eq ill-formed(N NzN, PS, N) = true .

    eq ill-formed(N NzN, [wait(N)] PS, M) = true .
    eq ill-formed(N NzN, [crit(N)] PS, M) = true .

    eq ill-formed(N, [wait(M N')] PS, M) = true .
    eq ill-formed(N, [crit(M N')] PS, M) = true .

--- Stating some recursive properties of ill-formed states assists when proving the implication in $T$ for state subsumption in $\R$.
--- Read right-to-left as implications, these are inductive properties about `ill-formed` states, that is:
---
--- -   adding `[idle]` processes does not change whether a state is `ill-formed`,
--- -   if a pool of processes `PS` has numbers amongst them down to `s N`, then adding a process with number `N` does not affect `ill-formed`, and
--- -   if a pool of processes `PS` has numbers amongst them less than `M`, adding a process wth number `s M` does not affect `ill-formed`.

    eq ill-formed(N, [idle] PS, M) = ill-formed(N, PS, M) .

    eq ill-formed(N, [crit(N)] PS, M) = ill-formed(s N, PS, M) .
    eq ill-formed(N, [wait(N)] PS, M) = ill-formed(s N, PS, M) .

    eq ill-formed(N, [wait(M)] PS, s M) = ill-formed(N, PS, M) .
    eq ill-formed(N, [crit(M)] PS, s M) = ill-formed(N, PS, M) .
endm

--- Note that `ill-formed` does not imply our overall property we wish to show for the system, as it does not bar states with two critical processes.
--- For instance, the following does not simplify to `true`:

reduce ill-formed(N, [crit(N)] [crit(s N)], s s N) .

--- but this does:

reduce ill-formed(N, [wait(N)] [wait(N)], s N) .

reduce (true ?= ill-formed(s N, PWS       , M))
    => (true ?= ill-formed(s N, PWS [idle], M)) .

reduce (true ?= ill-formed(s N, PWS, N'))
    => (true ?= ill-formed(s N, PWS [wait(N')] [wait(s N')], s s N')) .

reduce (true ?= ill-formed(s N, PWS, N'))
    => (true ?= ill-formed(s N, PWS [wait(N')], s N')) .

reduce (true ?= ill-formed(s N, PWS           , s M))
    => (true ?= ill-formed(s N, PWS [wait(M)] , s M)) .

reduce (true ?= ill-formed(s N, PWS,              M))
    => (true ?= ill-formed(s N, PWS [wait(M)] , s M)) .

reduce (true ?= ill-formed(s s N , PWS           , N))
    => (true ?= ill-formed(s M   , PWS [wait(N)] , s N)) .

fmod BAKERY-NPROC-RECFUNC-NARROWING-GRAPH is
    extending BAKERY-NARROWING-GRAPH-COMMON .

    vars C C' T T' T'' : Term . var TYPE : Type .

    eq #MO = upModule('BAKERY-NPROC-RECFUNC, true) .

    op ep : Term Term Term -> [Term] .
    ----------------------------------
    eq ep(T, T', T'') = '_!=_['true.Bool, 'ill-formed[T, T', T'']] .

   ceq implies?(C, C') = true if { T , TYPE } := metaReduce(#M, '_=>_[C, C'])
                              /\ T = #cTrue .
endfm

reduce wellFormed(#M) .

--- From an arbitrary state with idle processes, we see the ever-increasing state graph which was previously curbed with an equational abstraction of the system.

reduce renderTikZGraph(0, bfs(states(#cTerm[conf('X:Name, 'Y:Name, 'PIS:ProcIdleSet), #cTrue]), 3))
    == "\tikzset{elliptic state/.style={draw,ellipse}}"
       "\begin{tikzpicture}[shorten >=1pt,on grid,transform shape]"
       "\footnotesize"
       "    \node[elliptic state] (s_0) at (0,-10) {$\texttt{X ; Y ; PIS}$};"
       "    \node[elliptic state] (s_2) at (0,-14) {$\texttt{s X ; Y ; @3 [wait(X)]}$};"
       "    \node[elliptic state] (s_4) at (0,-18) {$\texttt{s X ; X ; PIS [crit(X)]}$};"
       "    \node[elliptic state] (s_6) at (2,-17) {$\texttt{s s Y ; X ; @3 [wait(Y)] [wait(s Y)]}$};"
       "    \node[elliptic state] (s_10) at (0,-22) {$\texttt{s s X ; X ; PIS [crit(X)] [wait(s X)]}$};"
       "    \node[elliptic state] (s_14) at (2,-21) {$\texttt{s s Y ; s Y ; @1 [crit(s Y)] [wait(Y)]}$};"
       "    \node[elliptic state] (s_16) at (4,-20) {$\texttt{s s s Z ; X ; PIS [wait(Z)] [wait(s Z)] [wait(s s Z)]}$};"
       "    \\path[->] (s_6) edge [bend left=10] node [pos=0.3,right] {wake} (s_16);"
       "    \\path[->>] (s_6) edge [bend left=10] node [pos=0.3,left] {crit} (s_10);"
       "    \\path[->] (s_6) edge [bend left=10] node [pos=0.3,right] {crit} (s_14);"
       "    \\path[->] (s_4) edge [bend left=10] node [pos=0.3,right] {wake} (s_10);"
       "    \\path[->>] (s_4) edge [bend left=10] node [pos=0.3,left] {exit} (s_0);"
       "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {wake} (s_6);"
       "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {crit} (s_4);"
       "    \\path[->] (s_0) edge [bend left=10] node [pos=0.3,right] {wake} (s_2);"
       "\\end{tikzpicture}" .

--- Equipped with our recursive predicate barring ill-formed bakeries, we are able to find an invariant set of three states which never has greater than one critical process.

reduce renderTikZGraph(0, bfs(states(#cTerm[conf('X:Name, 'Y:Name, 'PWS:ProcWaitSet), ep('Y:Name, 'PWS:ProcWaitSet, 'X:Name)]), 3))
    ==  "\tikzset{elliptic state/.style={draw,ellipse}}"
        "\begin{tikzpicture}[shorten >=1pt,on grid,transform shape]"
        "\footnotesize"
        "    \node[elliptic state] (s_0) at (0,-10) {$\texttt{X ; Y ; PWS}\\ st \\ \texttt{every-position(Y, PWS, X)}$};"
        "    \node[elliptic state] (s_2) at (0,-14) {$\texttt{X ; Y ; PWS3 [crit(Y)]}\\ st \\ \texttt{every-position(s Y, PWS3, X)}$};"
        "    \node[elliptic state] (s_8) at (0,-18) {$\texttt{s X ; Z ; PWS2 [crit(Z)] [wait(X)]}\\ st \\ \texttt{every-position(s Z, PWS2, X)}$};"
        "    \\path[->>] (s_8) edge [bend left=10] node [pos=0.3,left] {wake} (s_2);"
        "    \\path[->>] (s_8) edge [bend left=10] node [pos=0.3,left] {exit} (s_0);"
        "    \\path[->] (s_2) edge [bend left=10] node [pos=0.3,right] {wake} (s_8);"
        "    \\path[->>] (s_2) edge [bend left=10] node [pos=0.3,left] {exit} (s_0);"
        "    \\path[->>] (s_0) edge [loop below,min distance=25] node [pos=0.3,left] {wake} (s_0);"
        "    \\path[->] (s_0) edge [bend left=10] node [pos=0.3,right] {crit} (s_2);"
        "\\end{tikzpicture}" .

--- Module FVP-BAKERY
--- =================

fmod FVP-BAKERY-INIT is
   protecting META-TERM .
   protecting UNCONDITIONALIZE-FVP-BOOL .
    including CONDITIONAL-NARROWING-GRAPH + FOLDING-LABELED-GRAPH-SEARCH .

    op init : Nat -> [Term] .
    -------------------------
    eq init(0) = '<_`,_`,_`,_>['sleep.NcMode ,'_+_['1.NzNat,'1.NzNat,'1.NzNat],'wait.NcMode,'_+_['1.NzNat,'1.NzNat]] .
    eq init(1) = '<_`,_`,_`,_>[ 'PNC:NcMode   , 'X1:Nat   , 'QC:Mode      , 'X2:Nat   ] .
    eq init(2) = '<_`,_`,_`,_>[ 'PC:Mode      , 'Y1:Nat   , 'QNC:NcMode   , 'Y2:Nat   ] .
    eq init(3) = '<_`,_`,_`,_>[ 'sleep.NcMode , 'X0:Nat   , 'sleep.NcMode , 'X0:Nat   ] .
    eq init(4) = '<_`,_`,_`,_>[ 'sleep.NcMode , 'X0:Nat   , 'sleep.NcMode , 'X1:Nat   ] .
    eq init(5) = '<_`,_`,_`,_>[ 'PNC:NcMode   , 'X1:Nat   , 'QC:Mode      , 'X2:NzNat ] .
    eq init(6) = '<_`,_`,_`,_>[ 'PC:Mode      , 'Y1:NzNat , 'QNC:NcMode   , 'Y2:Nat   ] .
    eq init(7) = '<_`,_`,_`,_>[ 'wait.NcMode  , 'Y1:Nat   , 'QC:Mode      , 'Y2:NzNat ] .
    eq init(8) = '<_`,_`,_`,_>[ 'QC:Mode      , 'Y1:NzNat , 'wait.NcMode  , 'Y2:Nat   ] .
    eq init(9) = '<_`,_`,_`,_>[ 'crit.Mode    , 'X2:Nat   , 'crit.Mode    , 'X3:Nat   ] .
endfm

--- Look for safe starting states
--- -----------------------------

mod BAKERY-FVP-CONDITIONAL-NARROWING-GRAPH is
    including FVP-BAKERY-INIT .

    var N : Nat . vars T T' : Term .

    eq #tSort = 'BState .
    eq #MO    = upModule('BAKERY-FVP, true) .

    op fvpTT : -> Term .
    --------------------
    eq fvpTT = 'tt.TrueLit`{Bool`} .

    op inj : Nat -> Term .
    ----------------------
    eq inj(N) = '_st_[init(N), fvpTT] .

    op neq : Term Term -> Term .
    ----------------------------
endm

reduce wellFormed(#M) .
reduce wellFormed(#M, inj(1)) .
reduce wellFormed(#M, inj(2)) .

--- proposed invariant:
reduce state(inj(1)) ; state(inj(2)) .

--- actually can produce a double-crit mode in 2 steps:
reduce (state(inj(1)) ; state(inj(2))) =>* state(inj(9)) .
reduce bfs((state(inj(1)) ; state(inj(2))), 2) .

--- perhaps we can start from a smaller initial state:
reduce state(inj(4)) =>* state(inj(9)) .

--- Is this execution in our proposed invariant?
reduce nodes(bfs(state(inj(4)))) <= (state(inj(1)) ; state(inj(2))) .
reduce bfs(state(inj(4))) .

reduce (state(inj(5)) ; state(inj(6))) =>* state(inj(9)) .

--- Reverse the rules, search from bad state to init states
--- -------------------------------------------------------

mod BAKERY-FVP-CONDITIONAL-NARROWING-GRAPH is
   protecting REVERSE-RULES .
    including FVP-BAKERY-INIT .

    var N : Nat .

    eq #tSort = 'BState .
    eq #MO    = reverseRules(upModule('BAKERY-FVP, true)) .

    op fvpTT : -> Term .
    --------------------
    eq fvpTT = 'tt.TrueLit`{Bool`} .

    op inj : Nat -> Term .
    ----------------------
    eq inj(N) = '_st_[init(N), fvpTT] .

    op neq : Term Term -> Term .
    ----------------------------
endm

--- shows we can reach one of these initial states from the bad final states
reduce states(inj(9)) =>* states(inj(1) | inj(2)) .

--- but we cannot reach a state where both are asleep
reduce states(inj(9)) =>* states(inj(4)) .

--- BFS from the bad final state
reduce bfs(states(inj(4)), 2) .
reduce bfs(states(inj(4)), 2) .

mod BAKERY-FVP-CTOR-NARROWING-GRAPH is
   protecting FVP-BAKERY-INIT .
    extending NARROWING-GRAPH + FOLDING-LABELED-GRAPH-SEARCH .

    eq #M = upModule('BAKERY-FVP-CTOR, true) .
endm

***(Desired initial states and invariant:
   vars X0 X Y X' Y' : Nat .

Set of initial states:  < sleep, X0, sleep, X0 > .

  vars P Q : Mode .    vars P' Q' : NcMode .  vars

Mutex Invariant:  < P', X, Q, Y > \/ < P, X', Q', Y' > .
             or: state(init(1)) \/ state(init(2)) .
)


reduce wellFormed(#M, init(0)) .
reduce wellFormed(#M, init(1)) .
reduce wellFormed(#M, init(2)) .
reduce wellFormed(#M, init(3)) .
reduce wellFormed(#M, init(4)) .
reduce wellFormed(#M, init(5)) .
reduce wellFormed(#M, init(6)) .
reduce wellFormed(#M, init(7)) .
reduce wellFormed(#M, init(8)) .
reduce wellFormed(#M, init(9)) .

reduce step(state(init(0))) .
reduce step(state(init(1))) .
reduce step(state(init(2))) .
reduce step(state(init(3))) .

reduce state(init(3)) <= states(init(1) | init(2)) .
reduce state(init(1)) <= states(init(1) | init(2)) .
reduce state(init(2)) <= state(init(1)) .

reduce state('wait.NcMode)           <= state('PC:Mode) .
reduce state('_+_['1.NzNat,'V3:Nat]) <= state('Y1:NzNat) .
reduce state('V2:NcMode)             <= state('QNC:NcMode) .
reduce state('V3:Nat)                <= state('Y2:Nat) .

--- TODO: It would be nice to be able to prove the following
reduce state('Y1:NzNat) <= state('_+_['1.NzNat,'V3:Nat]) .

reduce state('<_`,_`,_`,_>[ 'wait.NcMode , '_+_['1.NzNat,'V3:Nat] ,  'V2:NcMode , 'V3:Nat ])
    <= state('<_`,_`,_`,_>[ 'PC:Mode     , 'Y1:NzNat              , 'QNC:NcMode , 'Y2:Nat ]) .


reduce state('sleep.NcMode) <= state('PNC:NcMode) .
reduce state('0.Nat)        <= state('X1:Nat) .
reduce state('V2:NcMode)    <= state('QC:Mode) .
reduce state('V3:Nat)       <= state('X2:NzNat) .

reduce state('<_`,_`,_`,_>[ 'sleep.NcMode , '0.Nat  , 'V2:NcMode , 'V3:Nat   ])
    <= state('<_`,_`,_`,_>[ 'PNC:NcMode   , 'X1:Nat , 'QC:Mode   , 'X2:NzNat ]) .

reduce   state('<_`,_`,_`,_>[ 'sleep.NcMode , '0.Nat    , 'V2:NcMode    , 'V3:Nat   ])
    <= ( state('<_`,_`,_`,_>[ 'PC:Mode      , 'Y1:NzNat , 'QNC:NcMode   , 'Y2:Nat   ])
       ; state('<_`,_`,_`,_>[ 'PNC:NcMode   , 'X1:Nat   , 'QC:Mode      , 'X2:NzNat ])
       ; state('<_`,_`,_`,_>[ 'sleep.NcMode , '0.Nat    , 'sleep.NcMode , '0.Nat    ])
       ) .

reduce intersect(state('V2:NzNat), state('V3:Nat)) .
reduce intersect(state('V2:Nat), state('V3:Nat)) .
reduce intersect( state('<_`,_`,_`,_>['V1:Mode,'0.Nat,'crit.Mode,'V2:Nat])
                , state('<_`,_`,_`,_>['V1:Mode,'_+_['V2:Nat,'V3:NzNat],'crit.Mode,'V2:Nat])
                ) .

--- both initial states yield finite state-graphs
reduce bfs(state(init(1))) .
reduce bfs(state(init(2))) .
reduce bfs(state(init(3))) .
reduce bfs(states(init(1) | init(2))) .

reduce nodes(bfs(state(init(1)))) .
reduce nodes(bfs(state(init(2)))) .
reduce nodes(bfs(state(init(3)))) .
reduce nodes(bfs(states(init(1) | init(2)))) .

reduce nodes(bfs(states(init(1) | init(2)))) <= (nodes(bfs(state(init(1)))) ; nodes(bfs(state(init(2))))) .
reduce (nodes(bfs(state(init(1)))) ; nodes(bfs(state(init(2))))) <= nodes(bfs(states(init(1) | init(2)))) .

reduce state(init(3))                                               <= states(init(1) | init(2)) .
reduce state(init(3))                                               <= nodes(bfs(states(init(1) | init(2)))) .
reduce states(init(1) | init(2))                                    <= nodes(extend(states(init(1) | init(2)))) .
reduce states(init(1) | init(2))                                    <= nodes(bfs(states(init(1) | init(2)))) .
reduce nodes(extend(states(init(1) | init(2))))                     <= states(init(1) | init(2)) .
reduce nodes(extend(states(init(1) | init(2))))                     <= nodes(bfs(states(init(1) | init(2)))) .
reduce nodes(extend(states(init(5) | init(6) | init(7) | init(8)))) <= states(init(5) | init(6) | init(7) | init(8)) .

--- state init(3) stays within init(1) ; init(2)
reduce nodes(bfs(state(init(3)))) <= states(init(1) | init(2)) .

--- even though in general init(1) ; init(2) is *not* an invariant
reduce invariant(states(init(1) | init(2))) .
reduce invariant(states(init(4) | init(5) | init(6))) .
reduce invariant(states(init(5) | init(6) | init(7) | init(8))) .
reduce nodes(bfs(states(init(1) | init(2)))) <= states(init(1) | init(2)) .
reduce check state(init(3)) stable in states(init(1) | init(2)) .

--- state 3 never reaches the bad state 9, but states 1 ; 2 can
reduce state(init(3))            =>* state(init(9)) .
reduce states(init(1) | init(2)) =>* state(init(9)) .
```

Conclusion
==========

