Labelled Graph Search and Analysis
==================================

Here is a labelled graph search algorithm and its instantiation to narrowing.

Abstract Graph Search Algorithms
--------------------------------

```maude
load ../meta/narrowing.maude
load ../meta/cterms.maude
load ../meta/mtransform.maude
load lmc.maude

set include BOOL off .
```

### Data Structures

Sort `LGraph` is an edge-labelled graph between `Node`s.

```maude
fmod LABELLED-GRAPH is

    sorts Node NeNodeSet NodeSet .
    ------------------------------
    subsort Node < NeNodeSet < NodeSet .

    vars N N' : Node . var NeNS : NeNodeSet .

    op .NodeSet : -> NodeSet .
    op _;_      : NodeSet   NodeSet ->   NodeSet [assoc comm id: .NodeSet] .
    op _;_      : NodeSet NeNodeSet -> NeNodeSet [ditto] .
    ------------------------------------------------------
    eq NeNS ; NeNS = NeNS .

    sorts Label LEdge NeLGraph LGraph .
    subsorts LEdge < NeLGraph < LGraph .
    ------------------------------------
    var L : Label . var NeLG : NeLGraph .

    op .LGraph : -> LGraph .
    op __      :   LGraph LGraph ->   LGraph [assoc comm id: .LGraph prec 55 format(d n d)] .
    op __      : NeLGraph LGraph -> NeLGraph [ditto] .
    --------------------------------------------------
    eq NeLG NeLG = NeLG .

    op _-[_]->_ : Node Label Node -> LEdge [prec 50] .
    --------------------------------------------------

    sorts Transition NeTransitionSet TransitionSet .
    ------------------------------------------------
    subsorts Transition < NeTransitionSet < TransitionSet .

    var TS : TransitionSet . vars NeTS NeTS' : NeTransitionSet .

    op .TransitionSet : -> TransitionSet .
    op _,_ :   TransitionSet TransitionSet ->   TransitionSet [assoc comm id: .TransitionSet] .
    op _,_ : NeTransitionSet TransitionSet -> NeTransitionSet [ditto] .
    -------------------------------------------------------------------
    eq NeTS , NeTS = NeTS .

    op <_,_> : Label Node -> Transition .
    op _->_  : Node TransitionSet -> LGraph [prec 50] .
    ---------------------------------------------------
    eq N -> .TransitionSet = .LGraph .
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
    extending LABELLED-GRAPH .

    vars D D' : Depth . var B : Bound .
    vars NS NS' NS'' : NodeSet . vars NeNS NeNS' NeNS'' : NeNodeSet .
    vars N N' N'' : Nat . var NID NID' NID'' : NodeId . vars ND ND' : Node .
    var L : Label . vars LG LG' LG'' LG''' : LGraph . vars NeLG NeLG' : NeLGraph .

    sort Fold .
    -----------
    vars F F' : Fold . var F? : [Fold] .

    op .Fold : -> Fold .
    op fold  : Node Node -> [Fold] .
    --------------------------------
    eq fold(ND, ND) = .Fold .

    op step : Node -> [TransitionSet] .
    -----------------------------------

    op all-step : NodeSet -> [LGraph] .
    -----------------------------------
    eq all-step(ND)           = ND -> step(ND) .
    eq all-step(.NodeSet)     = .LGraph .
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

    sorts FLGraph FLGraph? .
    ------------------------
    subsort FLGraph < FLGraph? .

    var FLG : FLGraph . var FLG? : FLGraph? .

    op _|_|_ : LGraph NodeMap Nat -> FLGraph [format(d n d n d d)] .
    ----------------------------------------------------------------

    op _|_ : FLGraph NodeSet -> FLGraph? [right id: .NodeSet format(d n d d)] .
    ---------------------------------------------------------------------------
    eq FLG | NeNS | NeNS' = FLG | NeNS ; NeNS' .

    op insert : NodeSet FLGraph? -> FLGraph? .
    op insert : LGraph  FLGraph? -> FLGraph? .
    ------------------------------------------
    eq insert(.NodeSet,     FLG?)        = FLG? .
    eq insert(NeNS ; NeNS', FLG?)        = insert(NeNS, insert(NeNS', FLG?)) .
    eq insert(NeNS,         FLG | NeNS') = insert(NeNS, FLG) | NeNS' .

    eq insert(.LGraph,    FLG?)        = FLG? .
    eq insert(NeLG NeLG', FLG?)        = insert(NeLG, insert(NeLG', FLG?)) .
    eq insert(NeLG,       FLG | NeNS') = insert(NeLG, FLG) | NeNS' .

   ceq insert(ND, LG | NM | N) = LG | NM' | s N | NS
                              if { NM' , NID , NS } := insert(N |-> ND, NM) .

   ceq insert(ND -[ L ]-> ND', LG | NM | N) = (NID' -[ L ]-> NID'' LG) | NM'' | s s N | NS' ; NS''
                                           if { NM'  , NID'  , NS'  } := insert(N   |-> ND,  NM)
                                           /\ { NM'' , NID'' , NS'' } := insert(s N |-> ND', NM') .

    op .FLGraph : -> FLGraph .
    --------------------------
    eq .FLGraph = .LGraph | .NodeMap | 0 .

    op nodes : FLGraph? -> [NodeSet] .
    ----------------------------------
    eq nodes(LG | NM | N | NS) = nodes(NM) .

    op extend : NodeSet  -> [FLGraph?] .
    op extend : FLGraph? -> [FLGraph?] .
    ------------------------------------
    eq extend(NS) = extend(insert(NS, .FLGraph)) .

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
    vars NS NS' : NodeSet . var NeNS : NeNodeSet .
    var FLG : FLGraph .

    op  bfs : NodeSet       -> [FLGraph] .
    op  bfs : NodeSet Bound -> [FLGraph] .
    op #bfs : Bound FLGraph -> [FLGraph] .
    --------------------------------------
    eq bfs(NS)    = bfs(NS, unbounded) .
    eq bfs(NS, B) = #bfs(B, insert(NS, .FLGraph)) .

    eq #bfs(B,         FLG)        = FLG .
    eq #bfs(0,         FLG |   NS) = FLG | NS .
    eq #bfs(s N,       FLG | NeNS) = #bfs(N,         extend(FLG | NeNS)) .
    eq #bfs(unbounded, FLG | NeNS) = #bfs(unbounded, extend(FLG | NeNS)) .

    --- TODO: uniform command language (to perform multiple computations).
    op invariant : NodeSet -> [Bool] .
    ----------------------------------
    eq invariant(NS) = nodes(bfs(NS, 1)) <= NS .

    op check_stable in_ : NodeSet NodeSet -> [Bool] .
    -------------------------------------------------
    eq check NS stable in NS' = NS <= NS' and-then invariant(NS') .
endfm
```

Instantiation to Narrowing
--------------------------

### Unconditional Narrowing

```maude
fmod FVP-NARROWING-GRAPH is
   protecting NARROWING2 .
    extending GRAPH-FOLDING-SEARCH .
    extending META-LMC-PARAMETERS .

    var TYPE : Type . vars T T' : Term . var M : Module . var SUB : Substitution .
    vars SR SR' : StepResult . var SRS : StepResultSet . var RS : RuleSet .

    op label : RuleSet Substitution -> Label .
    ------------------------------------------

    op transition : StepResultSet -> [TransitionSet] .
    --------------------------------------------------
    eq transition({T , TYPE , SUB / RS }) = < label(RS, SUB) , state(T) > .
    eq transition(.StepResultSet)         = .TransitionSet .
    eq transition(SR || SR' || SRS)       = transition(SR) , transition(SR') , transition(SRS) .

    op state : Term         -> Node .
    op fold  : Substitution -> Fold .
    ---------------------------------
    eq step(state(T))            = transition(metaNarrow2(##m##, T)) .
   ceq fold(state(T), state(T')) = fold(SUB) if SUB := metaMatch(##m##, T', T, nil, 0) .
endfm
```

### Conditional Narrowing

```maude
fmod FVP-NARROWING-MODULO-T-GRAPH is
   protecting CTERM-SET .
   protecting SUBSTITUTIONSET .
   protecting FVP-NARROWING-GRAPH .
   protecting UNCONDITIONALIZE .

    vars T T' : Term . vars F F' : FOForm . var C : Condition . var AS : AttrSet . var Q : Qid .
    var TYPE : Type . var SUB : Substitution . var RS : RuleSet . vars SR SR' : StepResult . var SRS : StepResultSet .

    op side-condition : Condition -> FOForm .

    op ##m-stripped## : ~> SModule [memo] .
    ---------------------------------------
    eq ##m-stripped## = fromTemplate(qid(string(getName(##m##)) + "-NO-CONDITIONS"), stripConditions(asTemplate(##m##))) .

    op getLabel : RuleSet -> Qid .
    ------------------------------
    eq getLabel( rl T => T'      [label(Q) AS] .) = Q .
    eq getLabel(crl T => T' if C [label(Q) AS] .) = Q .

    op getCondition : RuleSet -> FOForm .
    -------------------------------------
    eq getCondition( rl T => T'      [AS] .) = tt .
    eq getCondition(crl T => T' if C [AS] .) = side-condition(C) .

    op lookupCondition : RuleSet -> FOForm .
    ----------------------------------------
    eq lookupCondition(RS) = getCondition(getRls(getLabel(RS), ##m##)) .

    op transition : StepResultSet FOForm -> [TransitionSet] .
    ---------------------------------------------------------
    eq transition({T , TYPE , SUB / RS }, F) = < label(RS, SUB) , state(T | F /\ getCondition(RS)) > .
    eq transition(.StepResultSet,         F) = .TransitionSet .
    eq transition(SR || SR' || SRS,       F) = transition(SR, F) , transition(SR', F) , transition(SRS, F) .

    op state : CTerm -> Node .
    --------------------------
    eq step(state(T | F)) = transition(metaNarrow2(##m-stripped##, T), F) .
   --- ceq fold(state(T | C), state(T' | C')) = fold(SUB) if SUB := metaMatch(##m##, T', T, nil, 0) .
endfm

eof

    sorts Constraint CState .
    subsort State < CState .
    ------------------------
    vars NeTS NeTS' : NeTransitionSet . var T : Term . var N : Nat .
    vars C C' : Constraint . var M : Module . vars S S' : State .
    var SUBST : Substitution . var SUBSTS : SubstitutionSet .

    op downConstraintError : -> [Constraint] .
    op _<<_ : Constraint Substitution -> Constraint .
    -------------------------------------------------
   ceq C << SUBST = C'
    if T  := upTerm(C) << SUBST
    /\ C' := downTerm(T, downConstraintError) .

    op unsatis? : Constraint            -> [Bool] .
    op entails? : Constraint Constraint -> [Bool] .
    -----------------------------------------------

    op _|_ : State Constraint -> CState .
    op <_,_> : Step CState -> Transition .
    --------------------------------------

    op step : Module CState -> [TransitionSet] .
    --------------------------------------------
    eq step(M, S | C) = updateTransitions(C, step(M, S)) .

    op updateTransitions : Constraint TransitionSet -> [TransitionSet] .
    --------------------------------------------------------------------
    eq updateTransitions(C, .TransitionSet) = .TransitionSet .
    eq updateTransitions(C, (NeTS , NeTS')) = updateTransitions(C, NeTS) , updateTransitions(C, NeTS') .

   ceq updateTransitions(C, < SUBST , S >) = .TransitionSet             if C' := C << SUBST /\ unsatis?(C') .
    eq updateTransitions(C, < SUBST , S >) = < SUBST , S | C << SUBST > [owise] .

    op allMatches : Module State State Nat -> [SubstitutionSet] .
    -------------------------------------------------------------
    eq allMatches(M, S, S', N) = none [owise] .
   ceq allMatches(M, S, S', N) = SUBST | allMatches(M, S, S', N + 1)
    if SUBST := metaMatch(M, upTerm(S'), upTerm(S), nil, N) .

    op fold : Module CState CState -> [Fold] .
    ------------------------------------------
   ceq fold(M, S | C, S' | C') = SUBST
    if SUBST | SUBSTS := allMatches(M, S, S', 0)
    /\ entails?(C', C << SUBST) .
endfm
```
