Prelude Extensions
==================

This file extends the basic datatypes in the prelude with many additional operations

Extensions of BOOL, NAT, QID, and STRING
----------------------------------------

```maude
fmod MAYBE-BOOL is
  pr EXT-BOOL .
  pr QID-LIST .

  sort NoBool MaybeBool .
  subsort Bool NoBool < MaybeBool .
  op nobool :         -> NoBool [ctor] .
  op errb   : QidList -> [Bool] [ctor] .

  op _and_      : MaybeBool MaybeBool -> MaybeBool [assoc comm prec 55] .
  op _or_       : MaybeBool MaybeBool -> MaybeBool [assoc comm prec 59] .
  op _xor_      : MaybeBool MaybeBool -> MaybeBool [assoc comm prec 57] .
  op not_       : MaybeBool           -> MaybeBool [prec 53] .
  op _implies_  : MaybeBool MaybeBool -> MaybeBool [gather (e E) prec 61] .
  op _and-then_ : MaybeBool MaybeBool -> MaybeBool [strat (1 0) gather (e E) prec 55] .
  op _or-else_  : MaybeBool MaybeBool -> MaybeBool [strat (1 0) gather (e E) prec 59] .

  var A : MaybeBool . var QL : QidList .

  eq nobool and A = nobool .
  eq nobool xor A = nobool .
  eq not nobool = nobool .
  eq nobool or A = nobool .
  eq nobool implies A = nobool .
  eq A implies nobool = nobool .
  eq nobool and-then A = nobool .
  eq nobool or-else A = nobool .

  op maybeTrue? : MaybeBool -> Bool .
  eq maybeTrue?(true)     = true .
  eq maybeTrue?(nobool)   = true .
  eq maybeTrue?(B:[Bool]) = false [owise] .

  op boolErrMsg : [Bool] -> QidList .
  eq boolErrMsg(errb(QL)) = QL .
  eq boolErrMsg(B:[Bool]) = nil [owise] .
endfm

fmod BOUND-REFINEMENT is
  pr META-LEVEL .
  sort NzBound .
  subsort NzNat < NzBound < Bound .
  op unbounded : -> NzBound [ctor] .

  op dec : NzBound -> Bound .
  eq dec(unbounded) = unbounded .
  eq dec(s(N:Nat)) = N:Nat .
endfm

fmod MAYBE-QID is
  pr QID .
  sort MaybeQid .
  subsort Qid < MaybeQid .
  op noqid : -> MaybeQid [ctor] .
endfm

fmod QID-JOIN is
  pr QID-SET * (op empty to none, op _,_ to _;_ [prec 43]) .
  pr QID-LIST .
  pr CONVERSION .
  op joinl  : QidList QidList -> QidList .
  op join   : QidList         -> Qid .
  op join   : QidList String  -> Qid .
  op $join  : QidList String  -> String .
  op tolist : QidSet          -> QidList .
  var Q : Qid . var QL QL' : NeQidList . var QS : QidSet . var S : String .
  eq joinl(Q QL,QL') = Q QL' joinl(QL,QL') .
  eq joinl(Q,QL')    = Q .
  eq joinl(nil,QL')  = nil .

  eq join(nil)       = qid("") .
  eq join(QL)        = qid($join(QL,"")) .
  eq join(nil,S)     = qid("") .
  eq join(QL,S)      = qid($join(QL,S)) .

  eq $join(Q,S)      = string(Q) .
  eq $join(Q QL,S)   = string(Q) + S + $join(QL,S) .
  eq $join(nil,S)    = "" .

  eq tolist(Q ; QS)  = Q tolist(QS) .
  eq tolist(none)    = nil .
endfm

fmod QIDTUPLESET is
  pr QID .
  sort EmptyQidTupleSet QidPairItem QidPairSet QidTripleItem QidTripleSet .
  subsort QidPairItem      < QidPairSet   .
  subsort QidTripleItem    < QidTripleSet .
  subsort EmptyQidTupleSet < QidPairSet QidTripleSet .
  op qp   : Qid Qid                           -> QidPairItem      [ctor] .
  op qt   : Qid Qid Qid                       -> QidTripleItem    [ctor] .
  op _|_  : QidPairSet   QidPairSet           -> QidPairSet       [ctor assoc comm id: none] .
  op _|_  : QidTripleSet QidTripleSet         -> QidTripleSet     [ctor assoc comm id: none] .
  op _|_  : EmptyQidTupleSet EmptyQidTupleSet -> EmptyQidTupleSet [ctor assoc comm id: none] .
  op none :                                   -> EmptyQidTupleSet [ctor] .
endfm

fmod STRING-EXTRA is
  pr STRING .
  pr NAT .

  op insert    : String Nat Nat String -> String .

  op replace   : String String String ~> String .
  op replace   : String String String Nat ~> String .
  op $replace1 : String String Nat String Nat FindResult Nat Nat -> String .
  op $replace? : String String Nat String Nat Nat Nat Nat -> String .

  var S O R : String .
  var N L1 L2 P1 P2 I C : Nat .

  --- INP: String1 Nat1 Nat2 String2
  --- PRE: None
  --- OUT: Replaces substr(String1,Nat1,sd(Nat2,Nat1)) with String2
  eq insert(S,P1,P2,R) = substr(S,0,P1) + R + substr(S,P2,length(S)) .

  --- INP: String0 String1 String2 [Nat]
  --- PRE: None
  --- OUT: Replaces first [Nat] occurences of String1 with String2 in String0
  eq replace(S,O,R) = replace(S,O,R,length(S)) .
  ceq replace(S,O,R,N) =
    if N == 0 then
      S
    else
      $replace1(S,O,length(O),R,length(R),find(S,O,0),0,s(N))
    fi
  if O =/= "" .
  eq $replace1(S,O,L1,R,L2,notFound,C,N) = S .
  eq $replace1(S,O,L1,R,L2,I,C,N) = $replace?(insert(S,I,I + L1,R),O,L1,R,L2,I,s(C),N) .
  eq $replace?(S,O,L1,R,L2,I,C,N) =
    if C < N then
      $replace1(S,O,L1,R,L2,find(S,O,I + L2),C,N)
    else
      S
    fi .
endfm

fmod STRING-PAIR is
  pr STRING .

  sort StringPair .
  op ((_,_)) : String String -> StringPair [ctor] .

  var S S'   : String .
  var B L    : Nat .

  op split   : String String ~> StringPair .
  op rsplit  : String String ~> StringPair .
  op split   : String Nat Nat -> StringPair .

  eq  split(S,S')  = split(S, find(S,S',0),length(S')) .
  eq rsplit(S,S')  = split(S,rfind(S,S',length(S)),length(S')) .
  eq  split(S,B,L) = (substr(S,0,B),substr(S,B + L,length(S))) .
endfm
```

Data Structures: Typed Dictionary, Map, Stream, Stream Tupling, and N-Tree
---------------------------------------------------------------------------

NOTE: these are used for creating views from the empty theory
      into the empty module so that Maude will automatically
      rename sorts for us in parameterized modules
```maude
fmod EMPTY-MOD is endfm
fth  EMPTY     is endfth
```

NOTE: this module is declared over an empty theory
      because that allows us to create isomorphic copies
      of this sort and the only operator which will
      fail the sensibility criterion is the reference
      operator since it has a shared domain Qid

NOTE: after this module is instantiated, the user
      will need to add injections into the value sort, e.g.
      for a view called MyView, we will need:
      op ty : Type -> TypedVal{MyView} .
      for each Type we want to put into the dictionary.
      If wanted, can also add:
      op @ty : TypedDict{MyView} NamedValRef{X} -> Type .
      Note any injections/projections with the same name
      on isormorphic copies of Dict will clash.

```maude
fmod TYPED-DICTIONARY{X :: EMPTY} is
  pr QID .
  sort TypedVal{X} NamedVal{X} NamedRef{X} .
  sort NamedValRef{X} .
  sort TypedDict{X} .
  subsort NamedVal{X} NamedRef{X} < NamedValRef{X} .
  subsort NamedVal{X} < TypedDict{X} .

  ---# Define typed dictionary ctors
  op @_ : Qid -> NamedRef{X} [ctor] .
  op _:_ : Qid TypedVal{X} -> NamedVal{X} [ctor] .
  op _,_ : TypedDict{X} TypedDict{X} -> TypedDict{X} [ctor assoc comm id: none] .
  op none : -> TypedDict{X} [ctor] .

  ---# Declare typed dictionary basic operations
  op set : TypedDict{X} NamedVal{X} -> TypedDict{X} .
  op get : TypedDict{X} NamedValRef{X} -> [TypedVal{X}] .
  op getdef : TypedDict{X} NamedVal{X} -> TypedVal{X} .

  ---# Declare variables
  var D : TypedDict{X} . var K : Qid . var V V' : TypedVal{X} .
  eq set((D,K : V),K : V') = D,K : V .
  eq get(D,K : V) = V .
  eq get((D,K : V),@ K) = V .
  eq getdef((D,K : V),K : V') = V .
  eq getdef(D,K : V') = V' [owise] .
endfm

fmod MAP-EXTRA{X :: TRIV, Y :: TRIV} is
  pr MAP{X,Y} .
  pr NAT .
  --- basic operations only defined over maps
  op _[_,_]     : Map{X,Y} X$Elt Y$Elt -> Y$Elt .
  op remove     : X$Elt Map{X,Y} -> Map{X,Y} .
  op haskey     : X$Elt Map{X,Y} -> Bool .
  op size       : Map{X,Y} -> Nat .
  op $size      : Map{X,Y} Nat -> Nat .
  op wellFormed : Map{X,Y} -> Bool .

  var M : Map{X,Y} .
  var E : Entry{X,Y} .
  var K : X$Elt .
  var V V' : Y$Elt .
  var N : Nat .

  --- Default Value Lookup
  --- Returns V if K is not in the map
  eq M [K,V] =
          if not $hasMapping(M,K)
          then V else M [ K ] fi .
  --- Remove an entry from the map
  eq remove(K, (M, K |-> V)) = M .
  eq remove(K, M) = M [owise] .
  --- Check if a key exists in the map
  eq haskey(K, M) = $hasMapping(M,K) .
  --- Get the size of the map
  eq size(M) = $size(M,0) .
  eq $size((M , E), N) = $size(M,s(N)) .
  eq $size(empty,N) = N .
  --- Checks if each key is only mapped once
  eq wellFormed((M, K |-> V, K |-> V')) = false .
  eq wellFormed(M) = true [owise] .
endfm

fmod STREAM{X :: TRIV} is
  pr META-LEVEL .
  --- SORTS
  sort Stream{X} .
  --- CTOR
  op _&_ : X$Elt [Stream{X}] -> Stream{X} [strat (1 0) ctor] .
  op _|_ : Stream{X} Stream{X} -> Stream{X} [strat (1 0) ctor] .
  op end : -> Stream{X} [ctor] .
  --- DEF
  op take  : Bound Stream{X} -> Stream{X} .
  op $take : Bound Stream{X} Stream{X} -> Stream{X} .
  op pick! : Nat Stream{X} ~> X$Elt .
  op rest  : Nat Stream{X} -> Stream{X} .
  op last  : Stream{X} -> Stream{X} .
  op last! : Stream{X} ~> X$Elt .
  --- VAR
  var X Y  : X$Elt .
  var S S' : [Stream{X}] .
  var N : Nat .
  var B : Bound .
  --- IMPL
  eq end     | S               = S .
  eq (X & S) | S'              = X & (S | S') .
  eq take(B,S)                 = $take(B,S,end) .
  eq $take(unbounded,X & S,S') = $take(unbounded,S,X & S') .
  eq $take(unbounded,end,S)    = S .
  eq $take(s(N),X & S,S')      = $take(N,S,X & S') .
  eq $take(0,X & S,S')         = S' .
  eq $take(N,end,S')           = S' .
  eq pick!(s(N),X & S)         = pick!(N,S) .
  eq pick!(0,X & S)            = X .
  eq rest(s(N),X & S)          = rest(N,S) .
  eq rest(0,X & S)             = X & S .
  eq rest(N,end)               = end .
  eq last!(X & S)              = last!(S) .
  eq last!(X & end)            = X .
  eq last(X & Y & S)           = last(Y & S) .
  eq last(X & end)             = X & end .
  eq last(end)                 = end .
  --- EXTRA
  op size : Stream{X} -> Nat .
  op $size : Nat Stream{X} -> Nat .
  eq size(S) = $size(0,S) .
  eq $size(N,X & S) = $size(s(N),S) .
  eq $size(N,end) = N .
endfm

fth UNIT-LIST is
  sort Elt List .
  subsort Elt < List .
  op _;_ : List List -> List [id: empty] .
  op empty : -> List .
endfth

view UnitList from TRIV to UNIT-LIST is
  sort Elt to List .
endv

fmod LAZY-TUPLE{X :: UNIT-LIST} is
  pr STREAM{UnitList}{X} .
  pr LIST{UnitList}{X} .

  op tuples : List{UnitList}{X}                     -> Stream{UnitList}{X} .
  op ns     : List{UnitList}{X} Stream{UnitList}{X} -> Stream{UnitList}{X} .
  op ones   : X$List                                -> Stream{UnitList}{X} .
  op adds   : X$List X$List Stream{UnitList}{X}     -> Stream{UnitList}{X} .

  var L C C' : X$List .
  var OL     : List{UnitList}{X} .
  var E      : X$Elt .
  var S      : Stream{UnitList}{X} .

  --- INP: List{UnitList{X}}
  --- PRE: None
  --- OUT: Stream{UnitList{X}} where each UnitList in the stream
  ---      represents one possible tuple generated by selecting an
  ---      element from each input UnitList{X} in order
  eq tuples(nil)          = end .
  eq tuples(OL L)         = ns(OL,ones(L)) .
  eq ns(OL L,S)           = ns(OL,adds(L,L,S)) .
  eq ns(nil,S)            = S .
  eq ones(E ; L)          = E & ones(L) .
  eq ones(empty)          = end .
  eq adds(C',E ; C,L & S) = E ; L & adds(C',C,L & S) .
  eq adds(C',empty,L & S) = adds(C',C',S) .
  eq adds(C,C,end)        = end .
endfm

fmod TREE-INDEX{X :: TRIV} is
  pr LIST{X} * (sort List{X} to TreeIndex{X}, sort NeList{X} to NeTreeIndex{X}, op nil to noind,
                op __ to _:_, op append to tiappend, op tail to titail, op front to tifront, op reverse to tireverse, op $reverse to $tireverse) .
endfm

fmod N-TREE{X :: TRIV,Y :: TRIV} is
  pr EXT-BOOL . --- and-then definition
  pr TREE-INDEX{X} .

  sort Tree{X,Y} Tree?{X,Y}              .
  sort Branch{X,Y} BranchSet{X,Y}        .
  subsort Branch{X,Y} < BranchSet{X,Y}   .
  subsort Tree{X,Y}   < Tree?{X,Y}       .

  op (_:_:_)    : X$Elt Y$Elt Tree{X,Y}             -> Branch{X,Y}    [ctor] .
  op _|_        : BranchSet{X,Y} BranchSet{X,Y}     -> BranchSet{X,Y} [ctor assoc comm id: nobranch prec 51] .
  op nobranch   :                                   -> BranchSet{X,Y} [ctor] .
  op [_]        : BranchSet{X,Y}                    -> Tree{X,Y}      [ctor] .
  op notree     :                                   -> Tree?{X,Y}     [ctor] .

  --- an empty tree/check for leaves
  op isleaf?    : Branch{X,Y}                       -> Bool .
  op emptree    :                                   -> Tree{X,Y} .
  --- check that no index is duplicated in the tree
  op wellFormed : Tree{X,Y}                         -> Bool .
  op wellFormed : BranchSet{X,Y}                    -> Bool .
  --- extract a subtree/prune at treeindex/prune at depth
  op find       : Tree{X,Y} TreeIndex{X}            -> Tree?{X,Y} .     --- extract subtree at named index
  op prune      : Tree{X,Y} TreeIndex{X}            -> Tree{X,Y}  .     --- cutoff  subtree at named index
  op cut        : Tree{X,Y} Nat                     -> Tree{X,Y}  .     --- extract subtree upto depth
  op cut        : BranchSet{X,Y} BranchSet{X,Y} Nat -> Tree{X,Y}  .
  op graft      : Tree{X,Y} TreeIndex{X} Tree{X,Y}  -> Tree{X,Y}  .     --- insert tree into other tree at treeindex
  op frontier   : Tree{X,Y}                         -> BranchSet{X,Y} . --- extract edge of tree
  op frontier   : BranchSet{X,Y}                    -> BranchSet{X,Y} .
  op depth      : Tree{X,Y}                         -> Nat .            --- find depth of tree
  op depth      : BranchSet{X,Y} Nat                -> Nat .
  ---
  var X : X$Elt . var XS : TreeIndex{X} . var Y Y' : Y$Elt . var T T' : Tree{X,Y} . var BS BS' : BranchSet{X,Y} . var N : Nat .
  ---
  eq wellFormed([X : Y : T | X : Y' : T' | BS]) = false .
  eq wellFormed([BS])                           = wellFormed(BS) [owise] .
  eq wellFormed(X : Y : T | BS)                 = wellFormed(T) and-then wellFormed(BS) .
  eq wellFormed(nobranch)                       = true .
  ---
  eq emptree                           = [nobranch] .
  eq isleaf?(X : Y : T)                = T == emptree .
  ---
  eq find([X : Y : T | BS],X : XS)     = find(T,XS) .
  eq find(T,noind)                     = T .
  eq find(T,XS)                        = notree [owise] .
  ---
  eq prune([X : Y : T | BS],X : XS)    = [X : Y : prune(T,XS) | BS] .
  eq prune(T,noind)                    = emptree .
  eq prune(T,XS)                       = T [owise] .
  ---
  eq cut([BS],s(N))                    = cut(BS,nobranch,N) .
  eq cut(T,0)                          = emptree .
  eq cut(X : Y : T | BS,BS',N)         = cut(BS,BS' | X : Y : cut(T,N),N) .
  eq cut(nobranch,BS',N)               = [BS'] .
  ---
  eq graft([X : Y : T | BS],X : XS,T') = [X : Y : graft(T,XS,T') | BS] .
  eq graft(T,noind,T')                 = T' .
  eq graft(T,XS,T')                    = T [owise] .
  ---
  eq frontier([BS])                    = frontier(BS) .
  eq frontier(X : Y : T | BS)          = if T == emptree then X : Y : T  else frontier(T) fi | frontier(BS) .
  eq frontier(nobranch)                = nobranch .
  ---
  eq depth([nobranch])                 = 0 .
  eq depth([BS])                       = s(depth(BS,0)) [owise] .
  eq depth(X : Y : T | BS,N)           = depth(BS,max(depth(T),N)) .
  eq depth(nobranch,N)                 = N .
endfm
```

Miscellaneous
-------------

Prints string messsage for debugging use. Example:

```
eq foo(...) = case1() .
...
eq foo(...) = caseN() .
ceq foo(X) = any-value if debug-print("My message") [owise] .
```

where X is a totally generic variable tuple at the kind.

By owise, foo(X) will only be called if none of the other
equations apply. Since it is fully general, it will always
execute. But it will generate an error message when it is
executed and then fail to evaluate.

NOTE: this may break if a function defined earlier is later
      extended with more cases any any of those cases has
      the owise property, in which case, the message will
      print but evaluation will continue.

```maude
fmod DEBUG-PRINT is
  pr STRING .
  op debug-print : String -> Bool .
  eq debug-print(S:String) = false [print S:String] .
endfm
```
