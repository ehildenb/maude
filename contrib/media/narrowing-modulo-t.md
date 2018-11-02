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

Theory transformations take advantage of Maude's *reflective* nature.
Assume throughout a rewrite theory $\ensuremath{\mathcal{R}}= (\Sigma, B, E, T, R)$ with $\Sigma = (S, <)$ the signature.

### Reversing Rules

``` {.maude}
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
```

### Unconditionalize Transformation

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

Labelled Graph Search and Analysis
----------------------------------

#### Node Maps

A `Fold` is a "witness" that a specified node is less general than another node.
The user can specify the equations for `fold` to allow subsumption when possible.

``` {.maude}
    op fold : NodeSet Node -> [FoldSet] .
    -------------------------------------
```

A `NodeMap` is a lookup table of `Nat |-> Node`, which supports a lookup `_[_]` and `insert` operation.
The insert operation is aware of the user defined `Fold`s, and will not create new entries for new nodes that are less general than existing ones.

#### Folding Edge-labeled Graphs

Sort `LabeledGraph` is an edge-labelled graph between `Node`s.
The `Label` used is left abstract.

``` {.maude}
    sorts Label LabeledEdge NeLabeledGraph LabeledGraph .
    -----------------------------------------------------
    subsorts LabeledEdge < NeLabeledGraph < LabeledGraph .

    op .LabeledGraph :                             ->   LabeledGraph .
    op __            :   LabeledGraph LabeledGraph ->   LabeledGraph [assoc comm id: .LabeledGraph prec 55 format(d n d)] .
    op __            : NeLabeledGraph LabeledGraph -> NeLabeledGraph [ditto] .
    --------------------------------------------------------------------------
    eq NeLG NeLG = NeLG .

    op _-[_]->_ : Node Label Node -> LabeledEdge [prec 50] .
    --------------------------------------------------------
```

A `FoldedLabeledGraph` will use the `NodeMap` to store nodes, instead of directly storing them as nodes.
This will allow for "folding" the graph when a certain node is subsumed by another.
In the data structure `FoldedLabeledGraph?`, we additionally maintain a `frontier` of nodes which have not been explored for successor states yet.

``` {.maude}
    sorts FoldedLabeledGraph FoldedLabeledGraph? .
    ----------------------------------------------
    subsort FoldedLabeledGraph < FoldedLabeledGraph? .


    op .FoldedLabeledGraph :                          -> FoldedLabeledGraph .
    op _|_|_               : LabeledGraph NodeMap Nat -> FoldedLabeledGraph [format(d n d n d d)] .
    -----------------------------------------------------------------------------------------------
    eq .FoldedLabeledGraph = .LabeledGraph | .NodeMap | 0 .

    op _|_ : FoldedLabeledGraph NodeSet -> FoldedLabeledGraph? [right id: .NodeSet format(d n d d)] .
    -------------------------------------------------------------------------------------------------
    eq FLG | NeNS | NeNS' = FLG | NeNS ; NeNS' .
```

### Folding Graph Searches

Over these data-structures, we can define abstract graph-search algorithms.

#### Single search step

The following are system-specific (user-specified).

-   `step` *must* return the complete set of outgoing edges (`TransitionSet`).
-   `prune` *may* optionally be instantiated to prune given transitions generated by `step` (note the fallback `owise` case).

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

#### Full graph analysis

After the user has instantiated the abstract graph search machinery, various analysis are provided.

`bfs` allows doing a (possible bounded) breadth-first search from a specified set of nodes.

``` {.maude}
    op  bfs : NodeSet                  -> [FoldedLabeledGraph] .
    op  bfs : NodeSet Bound            -> [FoldedLabeledGraph] .
    -------------------------------------------------------------------
```

`invariant` will check that a given `NodeSet` is invariant by extending it with one step and checking subsumption of the new nodes into the original set.

``` {.maude}
    op invariant : NodeSet -> [Bool] .
    ----------------------------------
    eq invariant(NS) = nodes(extend(NS)) <= NS .
```

The search operators `_=>[_]_` will produce the reach-graph from the given initial states to the final states (up to a specified bound).
Before presenting the reach-graph to the user, it's pruned to only include paths which end at the final state.

``` {.maude}
    op _=>*_   : NodeSet                   NodeSet -> [FoldedLabeledGraph?] .
    op _=>[_]_ : NodeSet             Bound NodeSet -> [FoldedLabeledGraph?] .
    op _=>[_]_ : FoldedLabeledGraph? Bound NodeSet -> [FoldedLabeledGraph?] .
    -------------------------------------------------------------------------
```

### Instantiation to Narrowing

#### Common Narrowing

The above algorithms are abstract, but must be instantiated to a particular problem type.
Here, they are instantiated it to two theories of interest for Maude: narrowing and narrowing modulo $T$.

The parameters are instantiated as follows:

-   A `Label` is a tuple of the rule identifier enabling the transition, and the substitution used to take the rule.
-   A `Node` is a Maude meta-level `Term`, taking a `step` means calling out to Maude's meta-level narrowing functionality.
-   A `Fold` is a substitution which enables subsumption, and Maude's meta-level matching functionality is used to compute folds.

``` {.maude}
    op label : Qid Substitution -> Label .

    op fold : SubstitutionSet -> Fold .
    -----------------------------------
   ceq fold(state(T), state(T')) = fold(SUB) if SUB := metaMatch(#M, T', T, nil, 0) .

    op state : Term -> Node .
    -------------------------
    eq step(state(T)) = transition(narrowSteps(#M, T)) .
```

#### Unconditional Narrowing

For unconditional narrowing, we additionally define:

-   `intersect` should assume empty intersection if there are no variant unifiers between the states.

``` {.maude}
   ceq intersect(state(T), state(T')) = .NodeSet
    if noUnifier := metaVariantDisjointUnify(#M, renameTmpVar(#M, T) =? renameTmpVar(#M, T'), empty, 0, 0) .
```

#### Conditional Narrowing

For conditional narrowing, we additionally define:

-   `prune` can opportunistically remove states where the constraint is exactly the identified false term.
-   `fold` must also take the side-condition into account to ensure subsumption, and can look through multiple matching subsumptions on the term to do so.
-   `implies?` is supplied by the user and should check that the models of the first condition are contained in those of the second.
-   `intersect` can assume no intersection only if there are no variant unifiers on the *state* component, not taking into account the *constraint*.

``` {.maude}
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

### Example Output

### Thermostat Search Instantiation

In all cases, the top-sort of the Thermostat parameterized module will be `Conf`, which we instantiate here.

```
    eq #tSort = 'Conf .
    -------------------
```

Operators `lt`, `le`, `min`, `max`, and `inRange` are macros for specifying meta-level side-conditions compactly/easily for this theory.

```
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
```

These states capture "good" and "bad" states for the thermostat; `goodConf` has the temperature within bounds, while `badConf` has the temperature out of bounds.

```
    op goodConf : -> [Term] .
    op  badConf : -> [Term] .
    -------------------------
    eq goodConf = #cTerm['`{_`,_`,_`}[qid("T:" + string(#nSort)) , qid("TMP:" + string(#nSort)) , 'off.Mode ],     inRange(qid("TMP:" + string(#nSort)), min, max)] .
    eq badConf  = #cTerm['`{_`,_`,_`}[qid("T:" + string(#nSort)) , qid("TMP:" + string(#nSort)) , 'M:Mode   ], '~_[inRange(qid("TMP:" + string(#nSort)), min, max)]] .
```

**TODO**: Move this equation to somewhere more generic.
In order to take advantage of our ability to prune infeasible states, we extend the definition of `prune` to check if the side-condition is unsatisfiable.
For a specific theory, one can specify `isUnsat` so that it simplifies to `true` if you can prove infeasibility.

```
   ceq prune(< label(RL, SUB) , state(Q[T, C]) >) = .TransitionSet if Q = #cTerm /\ isUnsat(C) .
   ---------------------------------------------------------------------------------------------

    op isUnsat : Term -> [Bool] .
    -----------------------------
```

### Thermostat Search modulo FVP-INT

By importing `UNCONDITIONALIZE-FVP-BOOL`, we already have the relevant instantiation for the `FVP-BOOL` side-condition theory.
In addition, we tell the tool that the top sort in the original theory `#MO` is `Conf`.

```
    eq #MO = upModule('THERMOSTAT-INT-COMFORTABLE, true) .
    ------------------------------------------------------
```

Here we check that a given constraint `isUnsat` by generating variants and checking whether they are all false or exactly a renaming.
**TODO**: Invoke varsat.

```
   ceq isUnsat(T) = false                       if T == #cTrue .
   ceq isUnsat(T) = true                        if T == #cFalse .
   ceq isUnsat(T) = filterFalse(TS) == .TermSet if not T == #cTrue /\ not T == #cFalse
                                                /\ TS := getTerms(filterRenaming(variants(#M, renameTmpVar(#M, T)))) .
   -------------------------------------------------------------------------------------------------------------------

reduce bfs(state(goodConf), 2) .
reduce state(goodConf) =>[5] state(badConf) .
```

### Thermostat Search modulo REAL

For the real-numbered thermostat, we instantiat the parameter theory `#cModule` to `EQFORM-SMT-BOOLEAN` instead.
The top sort of the module remains the same (`Conf`), as it comes from the same parametric theory.

```
    eq #cModule   = 'EQFORM-SMT-BOOLEAN .

    eq #MO = upModule('THERMOSTAT-REAL-COMFORTABLE, true) .
    -------------------------------------------------------
```

For the real numbers, we check `isUnsat` via Maude's meta-level hookup to an SMT solver.

```
   ceq isUnsat(T) = T' == #cFalse if { T' , TYPE } := metaReduce(upModule(#cModule, true), 'simplify[T]) .
   -------------------------------------------------------------------------------------------------------

   ceq implies?(T, T') = T' == #cTrue if { T' , TYPE } := metaReduce(upModule(#cModule, true), 'simplify['_=>_[T, T']]) .
   ----------------------------------------------------------------------------------------------------------------------

reduce bfs(state(goodConf), 2) .
reduce state(goodConf) =>[5] state(badConf) .
```

Bakery Protocol
---------------

The Bakery protocol is a simple scheduling protocol for multiple processes.
The details differ among implementations, but the same basic ingredients are the same.
Processes wait for their turn to enter the critical section, synchronizing using some token or external state.
An implementation which allows multiple processes into the critical section simultaneously is buggy.

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
--- ### Bakery System Instatiation

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

--- ### Two Process Bakery

--- #### With Equational Abstraction

--- As a sanity check, results of section 3.1 of [@bae-escobar-meseguer-abstract-model-checking] are replicated.
--- This required an equational abstraction which reduced all the names in the configuration uniformly when they are all non-`0`.

--- **TODO**: Why is this system converging without the equational abstraction?



---    eq s N ; s M ; [idle]      [idle]      = N ; M ; [idle]    [idle] .
---    eq s N ; s M ; [wait(s K)] [idle]      = N ; M ; [wait(K)] [idle] .
---    eq s N ; s M ; [crit(s K)] [idle]      = N ; M ; [crit(K)] [idle] .
---    eq s N ; s M ; [wait(s K)] [wait(s L)] = N ; M ; [wait(K)] [wait(L)] .
---    eq s N ; s M ; [wait(s K)] [crit(s L)] = N ; M ; [wait(K)] [crit(L)] .
---    eq s N ; s M ; [crit(s K)] [crit(s L)] = N ; M ; [crit(K)] [crit(L)] .

--- Here we instantiate to the abstracted theory:

    eq #MO = upModule('BAKERY-2PROC-EQUATIONAL-ABSTRACTION, true) .

--- The initial state has two `idle` processes and both counters `N`:

    op conf2IdleEq : -> [Term] .
    ----------------------------
    eq conf2IdleEq = #cTerm[conf2Idle('X:Name, 'X:Name), #cTrue] .

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

--- Instead we instantiate to the original theory:

    eq #MO = upModule('BAKERY, true) .

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

--- ### Arbitrary Process Bakery

--- #### With Equational Abstraction

--- As a sanity check, results of section 3.3 of [@bae-escobar-meseguer-abstract-model-checking] are replicated.
--- This required an equational abstraction which removed "extra" waiting processes, which was shown to be sound.

    eq s s s L M ; M ; PS [wait(s L M)] [wait(s s L M)]
     =   s s L M ; M ; PS [wait(s L M)] .

--- Here we instantiate to the abstracted theory:

    eq #MO = upModule('BAKERY-NPROC-EQUATIONAL-ABSTRACTION, true) .

--- The initial state has all `idle` processes and equal counters.

    op confManyIdleEq : -> [Term] .
    -------------------------------
    eq confManyIdleEq = #cTerm[confManyIdle('X:Name, 'X:Name), #cTrue] .

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

--- Note that `ill-formed` does not imply our overall property we wish to show for the system, as it does not bar states with two critical processes.
--- For instance, the following does not simplify to `true`:

reduce ill-formed(N, [crit(N)] [crit(s N)], s s N) .

--- but this does:

reduce ill-formed(N, [wait(N)] [wait(N)], s N) .

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
```

Conclusion
==========

