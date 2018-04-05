Choice
======

AC Choice
---------

This very simple system simply takes advantage of AC data-structures to make a non-deterministic choice.

```maude
set include BOOL off .

fmod CHOICE-DATA is
  sorts Nat MSet State Pred .
  subsorts Nat < MSet .
  op zero : -> Nat [ctor] .
  op s    : Nat -> Nat [ctor] .
  op __   : MSet MSet -> MSet [ctor assoc comm] .
  op {_}  : MSet -> State [ctor] .
  op tt   : -> Pred [ctor] .
  op _=C_ : MSet MSet -> Pred [ctor] .  *** MSet containment
  vars U V : MSet . var N : Nat .
  eq U =C U = tt .
  eq U =C U V = tt .
endfm

mod CHOICE is pr CHOICE-DATA .
  vars U V : MSet .
  rl [choice] : {U V} => {U} .
endm
```

Scheduler Choice
----------------

This simple system takes a set of tasks and non-deterministically chooses the order to execute them in.

```maude
fmod SCHEDULER is
    sorts Task NeTasks Tasks .
    subsorts Task < NeTasks < Tasks .
    ---------------------------------
    vars NeTS : NeTasks . vars TS TS' : Tasks . var T : Task .

    sort State .

    op .Tasks : -> Tasks .
    op __ : NeTasks Tasks -> NeTasks [assoc comm id: .Tasks] .
    op __ : Tasks   Tasks -> Tasks   [ditto] .
    ------------------------------------------
    eq NeTS NeTS = NeTS .

    ops ta tb tc : -> Task .
    op {_} : Tasks -> State .
    op _|_ : Task Tasks -> State .

    op addTasks    : Tasks -> Task .
    op removeTasks : Tasks -> Task .
    --------------------------------
    eq addTasks(TS)      | TS'   = {TS TS'} .
    eq removeTasks(T TS) | T TS' = removeTasks(T TS) | TS' .
    eq removeTasks(TS)   | TS'   = {TS'} [owise] .
endfm

mod NONDET-SCHEDULER is
    protecting SCHEDULER .
    var T : Task . var TS : Tasks .

    rl { T TS } => T | TS .
    rl T | TS   => { TS } .
endm
```
