Labelled Graph Search and Analysis
==================================

Here is a labelled graph search algorithm and its instantiation to narrowing.

Abstract Graph Search Algorithms
--------------------------------

```maude
load ../meta/narrowing.maude
load ../meta/cterms.maude
load ../meta/mtransform.maude
load ../meta/variables.maude
load lmc.maude

set include BOOL off .
```

### Data Structures

Sort `LabeledGraph` is an edge-labelled graph between `Node`s.

```maude
fmod LABELED-GRAPH is

    sorts Node NeNodeSet NodeSet .
    ------------------------------
    subsort Node < NeNodeSet < NodeSet .

    --- TODO: change variables N, N' to ND, ND'
    vars N N' : Node . vars NeNS NeNS' : NeNodeSet .

    op .NodeSet : -> NodeSet .
    op _;_      : NodeSet   NodeSet ->   NodeSet [assoc comm id: .NodeSet] .
    op _;_      : NodeSet NeNodeSet -> NeNodeSet [ditto] .
    ------------------------------------------------------
    eq NeNS ; NeNS = NeNS .

    sorts Label LabeledEdge NeLabeledGraph LabeledGraph .
    subsorts LabeledEdge < NeLabeledGraph < LabeledGraph .
    ------------------------------------------------------
    var L : Label . var NeLG : NeLabeledGraph .

    op .LabeledGraph :                             ->   LabeledGraph .
    op __            :   LabeledGraph LabeledGraph ->   LabeledGraph [assoc comm id: .LabeledGraph prec 55 format(d n d)] .
    op __            : NeLabeledGraph LabeledGraph -> NeLabeledGraph [ditto] .
    --------------------------------------------------------------------------
    eq NeLG NeLG = NeLG .

    op _-[_]->_ : Node Label Node -> LabeledEdge [prec 50] .
    --------------------------------------------------------

    sorts Transition NeTransitionSet TransitionSet .
    ------------------------------------------------
    subsorts Transition < NeTransitionSet < TransitionSet .

    var TS : TransitionSet . vars NeTS NeTS' : NeTransitionSet .

    op .TransitionSet :                               ->   TransitionSet .
    op _,_            :   TransitionSet TransitionSet ->   TransitionSet [assoc comm id: .TransitionSet] .
    op _,_            : NeTransitionSet TransitionSet -> NeTransitionSet [ditto] .
    ------------------------------------------------------------------------------
    eq NeTS , NeTS = NeTS .

    op <_,_> : Label Node -> Transition .
    op _->_  : Node TransitionSet -> LabeledGraph [prec 50] .
    ---------------------------------------------------------
    eq N -> .TransitionSet = .LabeledGraph .
    eq N -> < L , N' >     = N -[ L ]-> N' .
    eq N -> NeTS , NeTS'   = N -> NeTS N -> NeTS' .
endfm
```

### Folding Search Command

```maude
fmod GRAPH-FOLDING-SEARCH is
   protecting NAT .
   protecting EXT-BOOL .
   protecting BOUND .
    extending LABELED-GRAPH .

    vars D D' : Depth . var B : Bound .
    vars NS NS' NS'' : NodeSet . vars NeNS NeNS' NeNS'' : NeNodeSet .
    vars N N' N'' : Nat . var NID NID' NID'' : NodeId . vars ND ND' : Node .
    var L : Label . vars LG LG' LG'' LG''' : LabeledGraph . vars NeLG NeLG' : NeLabeledGraph .

    sort Fold .
    -----------
    vars F F' : Fold . var F? : [Fold] .

    op .Fold : -> Fold .
    op fold  : Node Node -> [Fold] .
    --------------------------------
    eq fold(ND, ND) = .Fold .

    op step : Node -> [TransitionSet] .
    -----------------------------------

    op all-step : NodeSet -> [LabeledGraph] .
    -----------------------------------------
    eq all-step(ND)           = ND -> step(ND) .
    eq all-step(.NodeSet)     = .LabeledGraph .
    eq all-step(NeNS ; NeNS') = all-step(NeNS) all-step(NeNS') .

    sort NodeId .
    -------------
    subsorts Nat < NodeId < Node .

    --- TODO: add operator _~=_ for NodeSet equality
    --- TODO: extract this and common functionality from bae-symbolic branch

    op _<_  : NodeSet NodeSet -> [Bool] [ditto] .
    op _<=_ : NodeSet NodeSet -> [Bool] [ditto] .
    ---------------------------------------------
    eq NS             <  NS'        = NS =/= NS' and-then NS <= NS' .
    eq NS             <= NS         = true .
    eq .NodeSet       <= NeNS       = true .
    eq NeNS           <= .NodeSet   = false .
    eq (NeNS ; NeNS') <= NeNS''     = NeNS <= NeNS'' and-then NeNS' <= NeNS'' .
   ceq ND             <= (ND' ; NS) = true if fold(ND, ND') :: Fold .

    op intersect : NodeSet NodeSet -> Bool [comm] .
    -----------------------------------------------
    eq intersect(ND, ND)           = true .
    eq intersect(.NodeSet, NS)     = false .
    eq intersect(NeNS ; NeNS', NS) = intersect(NeNS, NS) or intersect(NeNS', NS) .

    --- TODO: Sometimes this may speed things up? Check performance.
   ceq intersect(ND, ND') = true if ND <= ND' .

    op _[_] : Nat Fold -> NodeId [right id: .Fold prec 20] .
    --------------------------------------------------------
   ceq N[F] <= N'[F'] = N <= N' if F =/= .Fold or F' =/= .Fold .

    sorts NodeAlias NeNodeMap NodeMap NodeMap? .
    --------------------------------------------
    subsorts NodeAlias < NeNodeMap < NodeMap .

    var NA : NodeAlias . vars NM NM' NM'' : NodeMap . vars NeNM NeNM' : NeNodeMap .

    op _|->_ : Nat Node -> NodeAlias [prec 55] .
    --------------------------------------------

    op .NodeMap : -> NodeMap .
    op __ : NeNodeMap NodeMap -> NeNodeMap [assoc id: .NodeMap prec 60 format(d n d)] .
    op __ :   NodeMap NodeMap ->   NodeMap [ditto] .
    ------------------------------------------------
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

    op extend : NodeSet             -> [FoldedLabeledGraph?] .
    op extend : FoldedLabeledGraph? -> [FoldedLabeledGraph?] .
    ----------------------------------------------------------
    eq extend(NS) = extend(flgraph(NS)) .

    eq extend(FLG)                = FLG .
    eq extend(LG | NM | N | NeNS) = insert(all-step(NM [ NeNS ]), LG | NM | N) .
endfm
```

### Graph Analysis

```maude
fmod GRAPH-ANALYSIS is
   protecting GRAPH-FOLDING-SEARCH .
   protecting EXT-BOOL .

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
    eq NS =>[ B ] NS' = intersect(NS, NS') or-else extend(NS) =>[ decrement(B) ] NS' .

    eq  FLG?        =>[ B ] .NodeSet = false .
    eq  FLG         =>[ B ] NeNS'    = false .
    eq (FLG | NeNS) =>[ B ] NeNS'    = intersect(NeNS', frontier(FLG | NeNS)) or-else extend(FLG | NeNS) =>[ decrement(B) ] NeNS' .
endfm
```

Instantiation to Narrowing
--------------------------

### Unconditional Narrowing

```maude
fmod FVP-NARROWING-GRAPH is
   protecting NARROWING .
   protecting VARIABLE-NUMBERS .
   protecting SUBSTITUTION-SET .
    extending GRAPH-FOLDING-SEARCH .
    extending META-LMC-PARAMETERS .

    vars T T' : Term . var M : Module .
    var SUB : Substitution . var SUBS : SubstitutionSet .
    vars NSR NSR' : NarrowStepResult . var NSRS : NarrowStepResults . var RL : Qid .

    op label : Qid Substitution -> Label .
    --------------------------------------

    op transition : NarrowStepResults -> [TransitionSet] .
    ------------------------------------------------------
    eq transition({RL : T , SUB })     = < label(RL, SUB) , state(T) > .
    eq transition(.NarrowStepResults)  = .TransitionSet .
    eq transition(NSR || NSR' || NSRS) = transition(NSR) , transition(NSR') , transition(NSRS) .

    op state : Term         -> Node .
    op fold  : Substitution -> Fold .
    ---------------------------------
    eq step(state(T))            = transition(narrowSteps(#M, T)) .
   ceq fold(state(T), state(T')) = fold(SUB) if SUB := metaMatch(#M, T', T, nil, 0) .

    --- Unification based intersection
    eq intersect(state(T), state(T')) = metaVariantDisjointUnify(#M, T =? T', empty, metaHighestVar((T, T')), 0) :: UnificationTriple .
endfm
```

### Conditional Narrowing

```maude
fmod FVP-CONDITIONAL-NARROWING-GRAPH is
   protecting FVP-NARROWING-GRAPH .
   protecting META-CONDITIONAL-LMC-PARAMETERS .

    vars ND ND' : Node . vars T T' C C' : Term .
    var Q : Qid . var SUB : Substitution . var N : Nat .

    op foldAny : Node Node Nat -> [Fold] .
    --------------------------------------
    eq fold(ND, ND') = foldAny(ND, ND', 0) [owise] .

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
endfm
```
