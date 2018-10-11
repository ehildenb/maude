Labelled Graph Search and Analysis
==================================

Here is a labelled graph search algorithm and its instantiation to narrowing.

```maude
sload ../meta/narrowing.maude
sload ../meta/cterms.maude
sload ../meta/mtransform.maude
sload ../meta/variables.maude
sload lmc.maude

set include BOOL off .
```

Data Structures
---------------

### Node Maps

Sort `NodeMap` can store a natural-number indexed set of nodes which support `fold`ing.
Insertion returns the `NodeId`, which can either be a new natural number or an old natural number with an associated `fold`.

```maude
fmod FOLDING-NODEMAP is
   protecting NAT .
   protecting EXT-BOOL .
```

Natural numbers can be interpreted as `NodeId`s, which makes it possible to refer to nodes more compactly.

```maude
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
```

A `Fold` is a "witness" that a specified node is less general than another node.
This allows building a `NodeId` which consists of a `Nat` and a `Fold`, indicating how the less general `Node` is subsumed in the more general.

```maude
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
The insert operation is aware of `Fold`s, and will not create new entries for new nodes that are less general than existing ones.

```maude
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
    eq nodes(N |-> NID)  = .NodeSet .
    eq nodes(N |-> ND)   = ND .
    eq nodes(NeNM NeNM') = nodes(NeNM) ; nodes(NeNM') .

    op _[_] : NodeMap NodeSet -> [NodeSet] .
    ----------------------------------------
    eq (NID |-> ND NM) [ NID' ] = if NID == NID' then ND else NM [ NID' ] fi .

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
endfm
```

### Folding Edge-labeled Graphs

Sort `LabeledGraph` is an edge-labelled graph between `Node`s.
The `Label` used is left abstract.

```maude
fmod FOLDING-LABELED-GRAPH is
   protecting FOLDING-NODEMAP .

    var N : Nat . vars NID NID' NID'' : NodeId . vars ND ND' : Node .
    vars NeNS NeNS' : NeNodeSet . var NS NS' NS'' : NodeSet .
    vars NM NM' NM'' : NodeMap .

    sorts Label LabeledEdge NeLabeledGraph LabeledGraph .
    -----------------------------------------------------
    subsorts LabeledEdge < NeLabeledGraph < LabeledGraph .

    var L : Label . var LG : LabeledGraph . vars NeLG NeLG' : NeLabeledGraph .

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

```maude
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
    op frontier : FoldedLabeledGraph? -> [NodeSet] .
    ------------------------------------------------
    eq nodes   (LG | NM | N | NS) = nodes(NM) .
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
endfm
```

Folding Graph Searches
----------------------

### Single search step

```maude
fmod FOLDING-LABELED-GRAPH-STEP is
   protecting FOLDING-LABELED-GRAPH .

    vars N N' : Nat . vars ND ND' : Node .
    vars NS : NodeSet . vars NeNS NeNS' : NeNodeSet . var NM : NodeMap .
    var L : Label . var LG : LabeledGraph . var FLG : FoldedLabeledGraph .
```

A `Transition` can be used to represent a set of outgoing edges from a given `Node` compactly.

```maude
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

The following operators must be instantiated to the given graph structure which is being searched.

-   `step` *must* return the complete set of outgoing edges (`TransitionSet`).
-   `prune` *can* optionally be instantiated to prune given transitions generated by `step`.

**TODO**: `step` loses information about *which* `Node` resulted in each `Transition`.

```maude
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

```maude
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

### Full graph analysis

```maude
fmod FOLDING-LABELED-GRAPH-SEARCH is
   protecting FOLDING-LABELED-GRAPH-STEP .
   protecting BOUND .

    var N : Nat . var B : Bound .
    vars NS NS' : NodeSet . var NeNS NeNS' : NeNodeSet .
    var FLG : FoldedLabeledGraph . var FLG? : FoldedLabeledGraph? .

    op  bfs : NodeSet                  -> [FoldedLabeledGraph] .
    op  bfs : NodeSet Bound            -> [FoldedLabeledGraph] .
    op #bfs : Bound FoldedLabeledGraph -> [FoldedLabeledGraph] .
    ------------------------------------------------------------
    eq bfs(NS)    = bfs(NS, unbounded) .
    eq bfs(NS, B) = #bfs(B, flgraph(NS)) .

    eq #bfs(B, FLG)        = FLG .
    eq #bfs(0, FLG?)       = FLG? .
    eq #bfs(B, FLG | NeNS) = #bfs(decrement(B), extend(FLG | NeNS)) .

    --- TODO: uniform command language (to perform multiple computations).
    op invariant : NodeSet -> [Bool] .
    ----------------------------------
    eq invariant(NS) = nodes(extend(NS)) <= NS .

    op check_stable in_ : NodeSet NodeSet -> [Bool] .
    -------------------------------------------------
    eq check NS stable in NS' = NS <= NS' and-then invariant(NS') .

    --- TODO: produce a `ReachPath` instead of a `Bool`
    op _=>*_   : NodeSet                  NodeSet -> Bool .
    op _=>[_]_ : NodeSet            Bound NodeSet -> Bool .
    op _=>[_]_ : FoldedLabeledGraph Bound NodeSet -> Bool .
    -------------------------------------------------------
    eq NS =>*     NS' = NS =>[ unbounded ] NS' .
    eq NS =>[ B ] NS' = (not isEmpty?(intersect(NS, NS'))) or-else extend(NS) =>[ decrement(B) ] NS' .

    eq  FLG?        =>[ B ] .NodeSet = false .
    eq  FLG         =>[ B ] NeNS'    = false .
    eq (FLG | NeNS) =>[ B ] NeNS'    = (not isEmpty?(intersect(NeNS', frontier(FLG | NeNS)))) or-else extend(FLG | NeNS) =>[ decrement(B) ] NeNS' .
endfm
```

Instantiation to Narrowing
--------------------------

### Common Narrowing

**TODO**: Support partial folds?
          Would need a pattern like `T /\ not (T_1 \/ ... \/ T_i)` for `T_1 \/ ... \/ T_i` the folded component.

```maude
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
    eq step(state(T))            = transition(narrowSteps(#M, T)) .
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

### Unconditional Narrowing

```maude
fmod NARROWING-GRAPH is
    including NARROWING-GRAPH-COMMON .

    vars T T' : Term .

   ceq intersect(state(T), state(T')) = .NodeSet
    if noUnifier := metaVariantDisjointUnify(#M, renameTmpVar(#M, T) =? renameTmpVar(#M, T'), empty, 0, 0) .
endfm
```

### Conditional Narrowing

```maude
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
```

-   `implies?` is supplied by the user and should check that the models of the first condition are contained in those of the second.

```maude
    --- TODO: Measure performance of this with `memo` on `implies?` vs not.
    --- Probably theory specific, maybe best to leave the choice of `memo` to each individual theory.
    op implies? : Term Term -> [Bool] .
    -----------------------------------
    eq implies?(C, C)  = true .
   ceq implies?(C, C') = true if C' = #cTrue  .
   ceq implies?(C, C') = true if C  = #cFalse .

   ceq intersect(state(Q[T,C]), state(Q[T',C'])) = .NodeSet
    if Q = #cTerm /\ noUnifier := metaVariantDisjointUnify(#M, renameTmpVar(#M, T) =? renameTmpVar(#M, T'), empty, 0, 0) .
endfm
```
