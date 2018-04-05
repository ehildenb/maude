Token Ring
==========

A group of processes pass a token around the ring.
Several versions are provided.

```maude
set include BOOL off .
```

Version 1 - RL
--------------

```maude
mod 2TOKEN is
  sorts Name Proc Token Conf State .
  subsorts Proc Token < Conf .
  op {_}      : Conf      -> State [ctor] .
  op none     :           -> Conf  [ctor] .
  ops * $     :           -> Token [ctor] .
  ops a b     :           -> Name  [ctor] .
  op __       : Conf Conf -> Conf  [assoc comm id: none] .
  op [_,wait] : Name      -> Proc  [ctor] .
  op [_,crit] : Name      -> Proc  [ctor] .
  var C : Conf .
  rl [a-enter] : { $ [a,wait] C } => { [a,crit]   C } [narrowing] .
  rl [b-enter] : { * [b,wait] C } => { [b,crit]   C } [narrowing] .
  rl [a-exit]  : {   [a,crit] C } => { [a,wait] * C } [narrowing] .
  rl [b-exit]  : {   [b,crit] C } => { [b,wait] $ C } [narrowing] .
endm

mod 2TOKEN-stop is protecting 2TOKEN .
  op [_] : Conf -> State .
  var C  : Conf .
  rl [done] : { C } => [ C ] .
endm
```

Version 2 - LMC
---------------

This version is modified from *All about Maude* p390.

```maude
fmod TOKEN-CONF is
  sorts Number .
  op 0 : -> Number [ctor] .
  op s : -> Number [ctor] .
  op __ : Number Number -> Number [ctor comm assoc id: 0] .

  sorts Token Mode ModeWait .
  subsorts ModeWait < Mode .
  op {_} : Number -> Token [ctor] .
  op crit : -> Mode [ctor] .
  op wait : -> ModeWait [ctor] .

  sorts Proc ProcWait .
  subsort ProcWait < Proc .
  op [_,_] : Number Mode -> Proc [ctor] .
  op [_,_] : Number ModeWait -> ProcWait [ctor] .

  sort Conf ConfTWait ConfWait .
  subsort Proc < Conf .
  subsort Token < ConfTWait .
  subsorts ProcWait < ConfWait < ConfTWait < Conf .
  op none : -> ConfWait [ctor] .
  op __ : Conf Conf -> Conf [ctor comm assoc id: none] .
  op __ : ConfWait ConfWait -> ConfWait [ctor ditto] .
  op __ : ConfTWait ConfTWait -> ConfTWait [ctor ditto] .
endfm

mod TOKEN-MUTEX is
  pr TOKEN-CONF .

  sort Top .
  op <_,_> : Number Conf -> Top .

--- NOTE : we do not impose N > M in the enter rule,
---   since it make unifications nontrivial and we do *not* need it
---   to prove the invariant.

  vars N M : Number . var CF : Conf .

  rl [enter] : < N,         {M} [M, wait] CF >
            => < N,             [M, crit] CF > [narrowing] .
  rl [exit1] : < s s N M,       [M, crit] CF >
            => < s s N M, {s M} [M, wait] CF > [narrowing] .
  rl [exit2] : <     s M,       [M, crit] CF >
            => <     s M,   {0} [M, wait] CF > [narrowing] .
endm
```
