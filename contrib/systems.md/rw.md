Readers and Writers
===================

Yet another mutual exclusion algorithm, at most one writer should be present but as many readers as you want is ok.
Several variants provided.

```maude
load ../tools/varsat/numbers.maude

set include BOOL off .
```

Version 1 - RL
--------------

```maude
mod R&W is
  sorts Nat Config .
  op 0 : -> Nat [ctor] .
  op s : Nat -> Nat [ctor] .
  op <_,_> : Nat Nat -> Config [ctor] . --- readers/writers
  op [_,_] : Nat Nat -> Config [ctor] . --- terminating states
  op done  : -> Config .

  vars R W : Nat .
  rl [write+] : < 0    , 0    > => < 0    , s(0) > .
  rl [write-] : < R    , s(W) > => < R    , W    > .
  rl [read+ ] : < R    , 0    > => < s(R) , 0    > .
  rl [read- ] : < s(R) , W    > => < R    , W    > .
endm

mod R&W-stop is pr R&W .
  vars R W : Nat .
  rl [finish] : < R , W > => [ R , W ] .
endm
```

Version 2 - LMC
---------------

This comes from *All About Maude - A High-Performance Logical Framework*.

```maude
mod READERS-WRITERS is
  sort Nat .
  op 0 : -> Nat [ctor] .
  op s : Nat -> Nat [ctor] .
  sort Config .
  op <_,_> : Nat Nat -> Config [ctor] .

  vars R W : Nat .

  rl [enter-w] : < 0    , 0    > => < 0    , s(0) > [narrowing] .
  rl [leave-w] : < R    , s(W) > => < R    , W    > [narrowing] .
  rl [enter-r] : < R    , 0    > => < s(R) , 0    > [narrowing] .
  rl [leave-r] : < s(R) , W    > => < R    , W    > [narrowing] .
endm
```

Version 3 - LMC RW-SHARED
-------------------------

This comes from *Constrained Monotonic Abstraction, P.A. Abdulla, et. al.*.

```maude
fmod RW-CONF is
  sort Natural .
  op 0 : -> Natural [ctor] .
  op s : Natural -> Natural [ctor] .

  sort Truth .
  ops true false : -> Truth [ctor] .

  sorts ProcIdle ProcRead ProcWrite Proc .
  subsort ProcIdle ProcRead ProcWrite < Proc .
  op think : -> ProcIdle [ctor] .
  ops read : -> ProcRead[ctor] .
  ops write : -> ProcWrite [ctor] .

  sorts ProcIdleSet ProcReadSet ProcWriteSet ProcSet .
  subsort ProcIdle < ProcIdleSet .
  subsort ProcRead < ProcReadSet .
  subsort ProcWrite < ProcWriteSet .
  subsort Proc < ProcSet .
  subsort ProcIdleSet < ProcReadSet ProcWriteSet < ProcSet .
  op none : -> ProcIdleSet [ctor] .
  op _;_ : ProcIdleSet ProcIdleSet -> ProcIdleSet [ctor comm assoc id: none] .
  op _;_ : ProcReadSet ProcReadSet -> ProcReadSet [ctor ditto] .
  op _;_ : ProcWriteSet ProcWriteSet -> ProcWriteSet [ctor ditto] .
  op _;_ : ProcSet ProcSet -> ProcSet [ctor ditto] .

  sort Conf .
  op <_,_|_> : Natural Truth ProcSet -> Conf [ctor] .
endfm

mod RW is
  protecting RW-CONF .

  var N : Natural . var B : Truth . var PS : ProcSet .

  rl [r1]: < 0       , true  | think ; PS > => < s(0)    , false | read  ; PS > [narrowing] .
  rl [r2]: < s(N)    , B     | think ; PS > => < s(s(N)) , B     | read  ; PS > [narrowing] .
  rl [r3]: < s(s(N)) , B     | read  ; PS > => < s(N)    , B     | think ; PS > [narrowing] .
  rl [r4]: < s(0)    , false | read  ; PS > => < 0       , true  | think ; PS > [narrowing] .
  rl [w1]: < N       , true  | think ; PS > => < N       , false | write ; PS > [narrowing] .
  rl [w2]: < N       , false | write ; PS > => < N       , true  | think ; PS > [narrowing] .
endm
```
