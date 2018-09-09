FVP Naturals, Integers, and Instantiation to EqForm
===================================================

```maude
set include BOOL off .
set include NAT  off .
set include INT  off .
```

FVP Natural Numbers
-------------------

The following implements FVP natural numbers as bags of `1`s.

```maude
fmod FVP-NAT-SORT is
    sorts Nat .
endfm

fmod FVP-NAT-CTOR is
    including FVP-NAT-SORT .

    sorts NzNat .
    -------------
    subsorts NzNat < Nat .

    op 0   :           ->   Nat [ctor] .
    op 1   :           -> NzNat [ctor] .
    op _+_ : Nat   Nat ->   Nat [ctor assoc comm id: 0] .
    op _+_ : Nat NzNat -> NzNat [ctor assoc comm id: 0] .
    -----------------------------------------------------
endfm

fmod FVP-NAT is
   protecting FVP-NAT-CTOR .

    vars N M : Nat .

    op s : Nat -> Nat .
    -------------------
    eq s(N) = N + 1 [variant] .

    op _monus_ : Nat Nat -> Nat .
    -----------------------------
    eq  N      monus N + M = 0 [variant].
    eq (N + M) monus N     = M [variant] .

    op sd : Nat Nat -> Nat [comm] .
    -------------------------------
    eq sd( N , N + M ) = M [variant] .

    op min : Nat Nat -> Nat [comm] .
    op max : Nat Nat -> Nat [comm] .
    --------------------------------
    eq min( N , N + M ) = N     [variant] .
    eq max( N , N + M ) = N + M [variant] .
endfm

fmod MULT-FVP-NAT is
   protecting FVP-NAT .

    var    N      :   Nat .
    vars NzN NzN' : NzNat .

    op _*_ : Nat Nat -> Nat [assoc comm] .
    --------------------------------------
    eq N * 0 =  0 [variant] .
    eq N * 1 =  N [variant] .

    eq N * (NzN + NzN') = (N * NzN) + (N * NzN') .
endfm

fmod FVP-NUMBERS is
   protecting FVP-NAT .

   ops 2 3 4 5 6 7 8 9 10 : -> NzNat .
   -----------------------------------
    eq 2  = s(1) .
    eq 3  = s(2) .
    eq 4  = s(3) .
    eq 5  = s(4) .
    eq 6  = s(5) .
    eq 7  = s(6) .
    eq 8  = s(7) .
    eq 9  = s(8) .
    eq 10 = s(9) .
endfm
```

FVP Integer Extension
---------------------

```maude
fmod FVP-INT is
   protecting MULT-FVP-NAT .

    sorts NzNeg Int NzInt .
    -----------------------
    subsort    Nat               < Int .
    subsorts NzNat NzNeg < NzInt < Int .

    vars   N   N' :   Nat .
    vars NzN NzN' : NzNat .
    vars   I   I' :   Int .
    vars NzI NzI' : NzInt .

    op -_ :   Int ->   Int .
    op -_ : NzNat -> NzNeg [ctor] .
    -------------------------------
    eq - 0     = 0 [variant] .
    eq - (- I) = I [variant] .

    op _+_ : Int Int -> Int [ditto] .
    ---------------------------------
    eq (- NzN) + (- NzN')    = - (NzN + NzN') [variant] .
    eq    NzN  + - (NzN + N) = - N            [variant] .

    op _-_ : Int Int -> Int .
    -------------------------
    eq I - I' = I + (- I') [variant] .

    op _*_ : Int Int -> Int [ditto] .
    ---------------------------------
    eq I * 0 = 0 [variant] .
    eq I * 1 = I [variant] .

    eq I * (NzI + NzI') = (I * NzI) + (I * NzI') .
endfm
```

Predicates over Numbers
-----------------------

```
fmod FVP-NAT-PRED is
   protecting FVP-NAT .

    vars   N N' :   Nat .
    var  NzN    : NzNat .

    op _==_  : Nat Nat -> Bool* [comm] .
    op _=/=_ : Nat Nat -> Bool* [comm] .
    ------------------------------------
    eq N       == N = tt [variant] .
    eq N + NzN == N = ff [variant] .

    eq N =/= N' = ~ (N == N') [variant] .

    op _<_ : Nat Nat -> Bool* .
    ---------------------------
    eq N      < N + NzN = tt [variant] .
    eq N + N' < N       = ff [variant] .

    op _<=_ : Nat Nat -> Bool* .
    ----------------------------
    eq N + NzN <= N      = ff [variant] .
    eq N       <= N + N' = tt [variant] .
endfm
```

```
fmod FVP-INT-PRED is
   protecting FVP-INT .
   protecting FVP-NAT-PRED .

    op _<_  : Int Int -> Bool* [ditto] .
    op _<=_ : Int Int -> Bool* [ditto] .
    ------------------------------------
    eq I <  I = ff [variant] .
    eq I <= I = tt [variant] .

    eq - NzN <  N' = tt [variant] .
    eq - NzN <= N' = tt [variant] .

    eq N <  - NzN' = ff [variant] .
    eq N <= - NzN' = ff [variant] .
endfm
```
