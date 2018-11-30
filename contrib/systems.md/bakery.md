Bakery Protocol
===============

The Bakery protocol is a simple scheduling protocol for multiple processes.
The details differ among implementations, but the same basic ingredients are the same.
Processes wait for their turn to enter the critical section, synchronizing using some token or external state.
An implementation which allows multiple processes into the critical section simultaneously is buggy.

```maude
load ../tools/fvp/numbers.maude

set include BOOL off .
set include NAT  off .
```

Two Process Bakery
------------------

This bakery protocol has only two processes which keep track of one counter each.

```maude
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

```maude
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

Arbitrary Processes Bakery
--------------------------

This version comes from the book *All About Maude* [@all-about-maude].

```maude
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
