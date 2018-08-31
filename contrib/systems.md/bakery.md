Bakery Protocol
===============

The Bakery protocol is a simple scheduling protocol for multiple processes.
The details differ among implementations, but the same basic ingredients are the same.
Processes wait to receive some token, and upon receiving the token are scheduled until they give up the token.

```maude
load ../tools/varsat/numbers.maude
```

Version 1 - RL
--------------

```maude
fmod BAKERY-STATE is
  pr NAT* .
  sort Conf .
  sort Mode ModeWait ModeIdle .
  subsort ModeIdle ModeWait < Mode .
  sort ProcIdle ProcWait Proc ProcSet .
  subsort ProcIdle ProcWait < Proc < ProcSet .

  op idle  :                   -> ModeIdle [ctor] .
  op wait  : Nat*              -> ModeWait [ctor] .
  op crit  : Nat*              -> Mode     [ctor] .
  op [_,_] : Nat* ModeIdle     -> ProcIdle  [ctor] .
  op [_,_] : Nat* ModeWait     -> ProcWait  [ctor] .
  op [_,_] : Nat* Mode         -> Proc      [ctor] .
  op none  :                   -> ProcSet   [ctor] .
  op __    : ProcSet ProcSet   -> ProcSet   [ctor assoc comm id: none] .

  op _;_;_ : Nat* Nat* ProcSet -> Conf  [ctor] .
endfm

mod REVERSE-BAKERY is
  pr BAKERY-STATE .
  sort State .

  op <_>   : Conf              -> State [ctor] .
  op [_]   : Conf              -> State [ctor] .

  var N M I : Nat* .
  var S : ProcSet .

  rl [wake]: < N :+ 1 ; M      ; [I,wait(N)] S > => < N ; M ; [I,idle   ] S > .
  rl [crit]: < N      ; M      ; [I,crit(M)] S > => < N ; M ; [I,wait(M)] S > .
  rl [exit]: < N      ; M :+ 1 ; [I,idle   ] S > => < N ; M ; [I,crit(M)] S > .
  rl [term]: < C:Conf > => [ C:Conf ] .
endm
```

Version 2 - LMC
---------------

This version comes from the book *All About Maude - A High-Performance Logical Framework*.

```maude
fmod BAKERY-SYNTAX is
  sort Name .
  op 0 : -> Name [ctor] .
  op s : -> Name [ctor] .
  op __ : Name Name -> Name [ctor comm assoc id: 0] .

  sorts ModeIdle ModeWait ModeCrit Mode .
  subsorts ModeIdle ModeWait ModeCrit < Mode .
  sorts ProcIdle ProcWait Proc ProcIdleSet ProcWaitSet ProcSet .
  subsorts ProcIdle < ProcIdleSet .
  subsorts ProcWait < ProcWaitSet .
  subsorts ProcIdle ProcWait < Proc < ProcSet .
  subsorts ProcIdleSet < ProcWaitSet < ProcSet .

  op idle : -> ModeIdle .
  op wait : Name -> ModeWait .
  op crit : Name -> ModeCrit .
  op `[_`] : ModeIdle -> ProcIdle .
  op `[_`] : ModeWait -> ProcWait .
  op `[_`] : Mode -> Proc .
  op none : -> ProcIdleSet .
  op __ : ProcIdleSet ProcIdleSet -> ProcIdleSet [assoc comm id: none] .
  op __ : ProcWaitSet ProcWaitSet -> ProcWaitSet [assoc comm id: none] .
  op __ : ProcSet ProcSet -> ProcSet [assoc comm id: none] .

  sort Conf .
  op _;_;_ : Name Name ProcSet -> Conf .
endfm

mod BAKERY is
  protecting BAKERY-SYNTAX .

  var PS : ProcSet .  vars N M : Name .

  rl [wake] : N ; M ; [idle]    PS => s N ; M   ; [wait(N)] PS [narrowing] .
  rl [crit] : N ; M ; [wait(M)] PS => N   ; M   ; [crit(M)] PS [narrowing] .
  rl [exit] : N ; M ; [crit(M)] PS => N   ; s M ; [idle]    PS [narrowing] .
endm
```

Version 3 - FVP
---------------

This version specifically has the Finite Variant Property.

```maude
set include BOOL off .

fmod NAT> is

    sorts Zero> NzNat> Nat> Bool> .
    -------------------------------
    subsorts Zero> NzNat> < Nat> .

    vars n m : Nat> . vars n' m' : NzNat> .

   ops tt ff : -> Bool>  [ctor] .
    op 0     : -> Zero>  [ctor] .
    op 1     : -> NzNat> [ctor] .
    -----------------------------

    op _+_ : Nat>   Nat>   -> Nat>   [ctor assoc comm id: 0] .
    op _+_ : NzNat> Nat>   -> NzNat> [ctor assoc comm id: 0] .
    op _+_ : Nat>   NzNat> -> NzNat> [ctor assoc comm id: 0] .
    op _+_ : NzNat> NzNat> -> NzNat> [ctor assoc comm id: 0] .
    ----------------------------------------------------------

    op _>_ : Nat> Nat> -> Bool> .
    -----------------------------
    eq n + n' > n     = tt [variant] .
    eq n      > n + m = ff [variant] .
endfm

mod BAKERY-FVP is
   protecting NAT> .

    sorts Mode NcMode BState .
    --------------------------
    subsort NcMode < Mode .

    vars P Q : Mode . vars X Y : Nat> .

    op sleep : -> NcMode [ctor] .
    op wait  : -> NcMode [ctor] .
    op crit  : -> Mode   [ctor] .
    -----------------------------

    op <_,_,_,_> : Mode Nat> Mode Nat> -> BState [ctor] .
    -----------------------------------------------------
    rl [p1_sleep] : < sleep, X, Q, Y > => <  wait, Y + 1, Q, Y > .
    rl [p1_wait]  : <  wait, X, Q, 0 > => <  crit, X,     Q, 0 > .
   crl [p1_wait]  : <  wait, X, Q, Y > => <  crit, X,     Q, Y > if  X > Y = ff .
    rl [p1_crit]  : <  crit, X, Q, Y > => < sleep, 0,     Q, Y > .

    rl [p2_sleep] : < P, X, sleep, Y > => < P, X,  wait, X + 1  > .
    rl [p2_wait]  : < P, 0,  wait, Y > => < P, 0,  crit, Y      > .
   crl [p2_wait]  : < P, X,  wait, Y > => < P, X,  crit, Y      > if X > Y = tt .
    rl [p2_crit]  : < P, X,  crit, Y > => < P, X, sleep, 0      > .
endm
```

After constructor-decomposition of `BAKERY-FVP`, you should get the following theory.

**TODO**: deduplicate rule names

```maude
mod BAKERY-FVP-CTOR is
   protecting NAT> .

    sorts Mode NcMode BState .
    --------------------------
    subsort NcMode < Mode .

    vars P Q : Mode . vars X Y : Nat> .

    op sleep : -> NcMode [ctor] .
    op wait  : -> NcMode [ctor] .
    op crit  : -> Mode   [ctor] .
    -----------------------------

    op <_,_,_,_> : Mode Nat> Mode Nat> -> BState [ctor] .
    -----------------------------------------------------
    rl [p1_sleep] : < sleep, X,       Q, Y                 > => <  wait, Y + 1,   Q, Y                 > [narrowing] .
    rl [p1_wait]  : <  wait, X,       Q, 0                 > => <  crit, X,       Q, 0                 > [narrowing] .
    rl [p1_wait]  : <  wait, V1:Nat>, Q, V1:Nat> + V2:Nat> > => <  crit, V1:Nat>, Q, V1:Nat> + V2:Nat> > [narrowing] .
    rl [p1_crit]  : <  crit, X,       Q, Y                 > => < sleep, 0,       Q, Y                 > [narrowing] .

    rl [p2_sleep] : < P, X,                   sleep, Y       > => < P, X,                    wait, X + 1   > [narrowing] .
    rl [p2_wait]  : < P, 0,                    wait, Y       > => < P, 0,                    crit, Y       > [narrowing] .
    rl [p2_wait]  : < P, V1:Nat> + V2:NzNat>,  wait, V1:Nat> > => < P, V1:Nat> + V2:NzNat>,  crit, V1:Nat> > [narrowing] .
    rl [p2_crit]  : < P, X,                    crit, Y       > => < P, X,                   sleep, 0       > [narrowing] .
endm
```
