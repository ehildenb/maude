QLock Mutual Exclusion
======================

With definitions:

-   `Si`: inactive processes
-   `Sw`: processes waiting to use a shared resource
-   `Sc`: processes currently using the shared resource (this should satisfy `| Sc | <= 1`)

we want to show that:

-   Property to prove:  `Sc` is either a singleton or empty
-   Auxiliary property: `Si`, `Sw`, and `Sc` contain no repeated elements
-   Initial state:      everything empty except for `Si`

```maude
set include BOOL off .
```

Booleans and Numbers
--------------------

**TODO**: Use standard booleans/numbers.

```maude
fmod QLOCK-STATE is
  sort Bool* Nat* .
  op  tt   :           -> Bool* [ctor] .
  ops 1 0  :           -> Nat*  [ctor] .
  op  _:+_ : Nat* Nat* -> Nat*  [assoc comm ctor id: 0] .

  sort Soup .
  subsort Nat* < Soup .
  op mt :              -> Soup [ctor] .
  op __ : Soup Soup    -> Soup [ctor assoc comm id: mt] .

  sort NeQueue Queue .
  subsort Nat* < NeQueue < Queue .
  op nil :                 -> Queue   [ctor] .
  op _@_ : NeQueue NeQueue -> NeQueue [ctor assoc] .

  sort Pred .
  op true    : -> Pred          [ctor] .
  op _=/=_ : Pred Pred -> Bool* [ctor] .
  op _in_  : Nat* Soup -> Pred  [ctor] .
  op dupl  : Soup -> Pred       [ctor] .
  var N : Nat* . var S : Soup .
  eq N in N      = true [variant] .
  eq N in N S    = true [variant] .
  eq dupl(N N)   = true [variant] .
  eq dupl(N N S) = true [variant] .
endfm
```

QLock Protocol
--------------

```maude
mod QLOCK is
  pr QLOCK-STATE .

  sort Conf State .
  op _|_|_|_ : Soup Soup Soup Queue -> Conf  [ctor] .
  op <_>     : Conf                 -> State [ctor] .
  op [_]     : Conf                 -> State [ctor] .

  var Si Sw Sc : Soup . var Q : NeQueue . var N N' : Nat* . var C : Conf .

  --- inactive to waiting
  rl [i2w]  : < Si N | Sw   | Sc   | Q      > => < Si   | Sw N | Sc   | Q @ N  > .
  rl [i2w]  : < Si N | Sw   | Sc   | nil    > => < Si   | Sw N | Sc   | N      > .

  --- waiting  to critical
 crl [w2c]  : < Si   | Sw N | Sc   | N' @ Q > => < Si   | Sw   | Sc N | N' @ Q > if N = N' .
 crl [w2c]  : < Si   | Sw N | Sc   | N'     > => < Si   | Sw   | Sc N | N'     > if N = N' .

  --- critical to inactive
  rl [c2i]  : < Si   | Sw   | Sc N | N' @ Q > => < Si N | Sw   | Sc   | Q      > .
  rl [c2i]  : < Si   | Sw   | Sc N | N'     > => < Si N | Sw   | Sc   | nil    > .

  --- join/leave
 crl [join] : < Si   | Sw   | Sc N | Q      > => < Si N | Sw   | Sc   | Q      > if dupl(Si N) =/= true = tt [nonexec] .
  rl [exit] : < Si N | Sw   | Sc   | Q      > => < Si   | Sw   | Sc   | Q      > .

  --- terminate
  rl [term] : < C > => [ C ] .
endm
```
