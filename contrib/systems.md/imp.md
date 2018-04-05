IMP Language
============

IMP is a simple imperative language with expression, variables assignment/lookup, and structured control flow.

```maude
set include BOOL off .

load ../tools/varsat/numbers.maude
```

Data Structures
---------------

The variant-satisfiability data-structures are used here.

```maude
fmod IMP-DATA is
   protecting MULT-NAT* + NUMBERS .

    sort Id .
    ---------

    sorts Env WrappedEnv .
    ----------------------

    op _|->_  : Id Nat* -> Env [ctor prec 50] .
    -------------------------------------------

    op mt  :         -> Env [ctor] .
    op _*_ : Env Env -> Env [ctor prec 51 assoc comm id: mt] .
    ----------------------------------------------------------

    op {_}    : Env -> WrappedEnv [ctor] .
    --------------------------------------
endfm
```

IMP Syntax
----------

```maude
fmod IMP-SYNTAX is
   protecting IMP-DATA .

    sort AExp BExp Exp Stmt Ids Value .
    -----------------------------------
    subsort Id Nat* < AExp .
    subsort Bool* < BExp .
    subsorts BExp AExp < Exp .
    subsorts Bool* Nat* < Value < Exp .

    var A A' : AExp . var B B' : BExp .

    --- ctors
    --- -----

    --- Arithmetic Expressions
    op !_    : BExp      -> BExp [ctor] .
    op _+:_  : AExp AExp -> AExp [ctor] .
    op _-:_  : AExp AExp -> AExp [ctor] .
    op _*:_  : AExp AExp -> AExp [ctor] .
    op _<:_  : AExp AExp -> BExp [ctor] .
    op _&&:_ : BExp BExp -> BExp [ctor] .

    --- Statements
    op __              : Stmt Stmt      -> Stmt [ctor prec 42 gather (E e)] .
    op if (_) _ else _ : BExp Stmt Stmt -> Stmt [ctor] .
    op while (_) _     : BExp Stmt      -> Stmt [ctor] .
    op {_}             : Stmt           -> Stmt [ctor] .
    op {}              :                -> Stmt [ctor] .
    op _=_;            : Id AExp        -> Stmt [ctor] .

    --- defined
    --- -------

    op val? : Exp -> Bool* .
    ------------------------
    eq val?(Q:Id)     = ff [variant] .
    eq val?(A +: A')  = ff [variant] .
    eq val?(A *: A')  = ff [variant] .
    eq val?(A -: A')  = ff [variant] .
    eq val?(! B)      = ff [variant] .
    eq val?(A <: A')  = ff [variant] .
    eq val?(B &&: B') = ff [variant] .
    eq val?(V:Value)  = tt [variant] .
endfm
```

Contex Holes
------------

IMP will be given semantics in terms of context-evaluations.
Here we provide the generic context/hole infrastructure.

```maude
fmod IMP-HOLE-SYNTAX is
   protecting IMP-SYNTAX .

    sorts !AExp !BExp !Stmt .
    -------------------------

    --- !AExp
    op []+:_ : AExp -> !AExp [ctor] .
    op _+:[] : AExp -> !AExp [ctor] .
    op []*:_ : AExp -> !AExp [ctor] .
    op _*:[] : AExp -> !AExp [ctor] .
    op []-:_ : AExp -> !AExp [ctor] .
    op _-:[] : AExp -> !AExp [ctor] .

    --- !BExp
    op ![]    : -> !BExp      [ctor] .
    op []<:_  : AExp -> !BExp [ctor] .
    op _<:[]  : Nat* -> !BExp [ctor] .
    op []&&:_ : BExp -> !BExp [ctor] .

    --- !Stmt
    op if ([]) _ else _ : Stmt Stmt -> !Stmt [ctor] .
    op while ([]) _     : Stmt      -> !Stmt [ctor] .
    op _=[];            : Id        -> !Stmt [ctor] .

    sorts RedContext Continuation Redex .
    ------------------------------------
    subsorts Stmt Exp !AExp !BExp !Stmt < Redex .

    --- RedContext
    op <_|_>  : Continuation Env -> RedContext [ctor] .

    --- Continuation
    op done   :                    -> Continuation [ctor] .
    op _~>_   : Redex Continuation -> Continuation [ctor prec 43] .
endfm
```

IMP Semantics
-------------

```maude
mod IMP-SEMANTICS is
   protecting IMP-HOLE-SYNTAX .

    var K : Continuation .
    var E : Env .
    var S S' : Stmt .
    var Q : Id .
    var BE BE' : BExp .
    var AE AE' : AExp .
    var N M : Nat* .
    var B : Bool* .

    --- Structural Rules
    --- ----------------

    --- Heating
   crl [#if]       : < if (BE) S else S' ~> K | E >  => < BE ~> if ([]) S else S' ~> K | E > if val?(BE) = ff .
   crl [#assign]   : < (Q = AE ;)        ~> K | E >  => < AE ~> Q = [];           ~> K | E > if val?(AE) = ff .
   crl [#add-lft]  : < AE +:  AE'        ~> K | E >  => < AE ~> [] +: AE'         ~> K | E > if val?(AE) = ff .
   crl [#add-rght] : < N  +:  AE         ~> K | E >  => < AE ~> N  +: []          ~> K | E > if val?(AE) = ff .
   crl [#mul-lft]  : < AE *:  AE'        ~> K | E >  => < AE ~> [] *: AE'         ~> K | E > if val?(AE) = ff .
   crl [#mul-rght] : < N  *:  AE         ~> K | E >  => < AE ~> N  *: []          ~> K | E > if val?(AE) = ff .
   crl [#sub-lft]  : < AE -:  AE'        ~> K | E >  => < AE ~> [] -: AE'         ~> K | E > if val?(AE) = ff .
   crl [#sub-rght] : < N  -:  AE         ~> K | E >  => < AE ~> N  -: []          ~> K | E > if val?(AE) = ff .
   crl [#and]      : < BE &&: BE'        ~> K | E >  => < BE ~> [] &&: BE'        ~> K | E > if val?(BE) = ff .
   crl [#lt-lft]   : < AE <:  AE'        ~> K | E >  => < AE ~> [] <: AE'         ~> K | E > if val?(AE) = ff .
   crl [#lt-rght]  : < N  <:  AE         ~> K | E >  => < AE ~> N  <: []          ~> K | E > if val?(AE) = ff .
   crl [#not]      : < ! BE              ~> K | E >  => < BE ~> ! []              ~> K | E > if val?(BE) = ff .

    --- Cooling
    rl [@if]       : < B ~> if ([]) S else S' ~> K | E > => < if (B) S else S' ~> K | E > .
    rl [@assign]   : < N ~> Q = [];           ~> K | E > => < (Q = N ;)        ~> K | E > .
    rl [@add-lft]  : < N ~> [] +: AE          ~> K | E > => < N  +: AE         ~> K | E > .
    rl [@add-rght] : < M ~> N  +: []          ~> K | E > => < N  +: M          ~> K | E > .
    rl [@sub-lft]  : < N ~> [] -: AE          ~> K | E > => < N  -: AE         ~> K | E > .
    rl [@sub-rght] : < M ~> N  -: []          ~> K | E > => < N  -: M          ~> K | E > .
    rl [@and]      : < B ~> [] &&: BE         ~> K | E > => < B &&: BE         ~> K | E > .
    rl [@lt-lft]   : < N ~> [] <: AE          ~> K | E > => < N <: AE          ~> K | E > .
    rl [@lt-rght]  : < M ~> N  <: []          ~> K | E > => < N <: M           ~> K | E > .
    rl [@not]      : < B ~> ! []              ~> K | E > => < ! B              ~> K | E > .

    --- Semantic Rules
    --- --------------

    --- Stmts
    rl [emp-block] : < {}                ~> K | E > => <                                       K | E > .
    rl [stmtlist]  : < S S'              ~> K | E > => < S ~> S'                            ~> K | E > .
    rl [block]     : < {S}               ~> K | E > => < S                                  ~> K | E > .
    rl [if-true]   : < if (tt) S else S' ~> K | E > => < S                                  ~> K | E > .
    rl [if-false]  : < if (ff) S else S' ~> K | E > => < S'                                 ~> K | E > .
    rl [while]     : < while (BE) {S}    ~> K | E > => < if (BE) {S while (BE) {S}} else {} ~> K | E > .

    --- Assignemnt/lookup rules assume memory locations exist and are unique
    rl [assign] : < (Q = N ;) ~> K | E * Q |-> M > => <      K | E * Q |-> N > .
    rl [lookup] : < Q         ~> K | E * Q |-> N > => < N ~> K | E * Q |-> N > .

    --- Expressions
    rl [add]       : < N +: M    ~> K | E > => < N :+ M   ~> K | E > .
    rl [mul]       : < N *: M    ~> K | E > => < N :* M   ~> K | E > .
    rl [sub]       : < N -: M    ~> K | E > => < sd(N,M)  ~> K | E > .
    rl [lt]        : < N <: M    ~> K | E > => < N <Nat M ~> K | E > .
    rl [not-1]     : < ! tt      ~> K | E > => < ff       ~> K | E > .
    rl [not-2]     : < ! ff      ~> K | E > => < tt       ~> K | E > .
    rl [and-true]  : < tt &&: BE ~> K | E > => < BE       ~> K | E > .
    rl [and-false] : < ff &&: BE ~> K | E > => < ff       ~> K | E > .
endm
```

By adding this module, you get (confluent) non-determinism in the resulting executions.
This can be used to test partial-order reduction engines.

```maude
mod NONSEQ-IMP-SEMANTICS is
   protecting IMP-SEMANTICS .

    var AE AE' : AExp .
    var E : Env .
    var N : Nat* .
    var K : Continuation .

   crl [@add-rght] : < AE +: AE'      ~> K | E > => < AE' ~> AE +: [] ~> K | E > if val?(AE) = ff .
    rl [#add-rght] : < N  ~> AE +: [] ~> K | E > => < AE +: N         ~> K | E > .
endm
```

IMP Programs
------------

Here several simple IMP programs are provided as primitives which desugar into their equivalent code.

```maude
mod IMP-PROGRAMS is
   protecting IMP-SEMANTICS + IDENTIFIER .

    vars N M O P Q R S T X Y Z : Id .
    vars XS YS ZS : Ids .

    op sum : Id Id -> Stmt .
    ------------------------
    eq sum(N,S)
     = S = 0 ;
       while (0 <: N) {
           S = S +: N ;
           N = N -: 1 ;
       } .

    op swap : Id Id Id -> Stmt .
    ----------------------------
    eq swap(X,Y,T)
     = T = X ;
       X = Y ;
       Y = T ; .

    op swap-in-place : Id Id -> Stmt .
    ----------------------------------
    eq swap-in-place(X, Y)
     = X = X +: Y ;
       Y = X -: Y ;
       X = X -: Y ; .

    op swap-sort : Ids -> Stmt .
    ----------------------------
    eq swap-sort(X)          = {} .
    eq swap-sort(X ; Y)      = if (X <: Y) {} else { swap-in-place(X, Y) } .
    eq swap-sort(X ; Y ; ZS) = swap-sort(X ; Y)
                               swap-sort(X ; ZS)
                               swap-sort(Y ; ZS) .
endm
```
