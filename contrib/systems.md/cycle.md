Cycle
=====

These systems give very simple finite cycles for testing search algorithms with.

```maude
load ../tools/fvp/numbers.maude

mod CYCLE is
    sorts PreState State .

    ops a b c : -> PreState .
    ops f g : PreState -> State .
    -----------------------------
    rl [a2b] : f(a) => f(b) [narrowing] .
    rl [a2c] : f(a) => g(c) [narrowing] .
    rl [b2c] : f(b) => f(c) [narrowing] .
    rl [c2a] : f(c) => f(a) [narrowing] .
endm

mod EXT-CYCLE is
    extending CYCLE .

    op g : PreState -> State .
    --------------------------
    rl [a2c] : g(a) => g(c) [narrowing] .
    rl [c2a] : g(c) => g(a) [narrowing] .
endm
```

Though the following can have an arbitrarily large cycle, it will still always be finite.

```maude
mod BOUNDED-CYCLE is
   protecting FVP-NAT .

    sort State .
    ------------
    vars N M L : Nat .

    op {_,_} : Nat Nat -> State .
    -----------------------------
    rl [add] : { N         , L + 1 + N + M }
            => { N + L + 1 , L + 1 + N + M }
       [narrowing] .

    rl [rm]  : { N + L + 1 , M }
            => { N         , M }
       [narrowing] .
endm
```
