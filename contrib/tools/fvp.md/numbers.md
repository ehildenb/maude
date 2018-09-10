FVP Naturals, Integers, and Instantiation to EqForm
===================================================

```maude
set include BOOL off .
set include NAT  off .
set include INT  off .
```

FVP Booleans
------------

```maude
fmod FVP-BOOL-SORT is
    sort Bool .
endfm

fmod FVP-BOOL-CTOR is
   protecting FVP-BOOL-SORT .

   ops tt ff : -> Bool [ctor] .
   ----------------------------
endfm

fmod FVP-BOOL is
   protecting FVP-BOOL-CTOR .

    var B : Bool .

    op _/\_ : Bool Bool -> Bool [assoc comm] .
    op _\/_ : Bool Bool -> Bool [assoc comm] .
    ------------------------------------------
    eq tt /\ B = B [variant] .
    eq ff \/ B = B [variant] .

    eq ff /\ B = ff [variant] .
    eq tt \/ B = tt [variant] .

    op ~_ : Bool -> Bool .
    ----------------------
    eq ~ tt = ff [variant] .
    eq ~ ff = tt [variant] .
endfm
```

FVP Natural Numbers
-------------------

The following implements FVP natural numbers as bags of `1`s.

```maude
fmod FVP-NAT-SORT is
    sorts Nat NzNat .
    -----------------
    subsort NzNat < Nat .
endfm

fmod FVP-NAT-CTOR is
   protecting FVP-NAT-SORT .

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
   protecting FVP-NAT .

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

    op min : Int Int -> Int [ditto] .
    op max : Int Int -> Int [ditto] .
    ---------------------------------
    eq min(- NzN, N') = - NzN [variant] .
    eq max(- NzN, N') =   N'  [variant] .

    eq min(- NzN, - NzN') = - max(NzN, NzN') [variant] .
    eq max(- NzN, - NzN') = - min(NzN, NzN') [variant] .
endfm
```

Predicates over Numbers
-----------------------

```maude
fmod FVP-NAT-PRED is
   protecting FVP-NAT .
   protecting FVP-BOOL .

    vars   N N' :   Nat .
    var  NzN    : NzNat .

    op _==_  : Nat Nat -> Bool [comm] .
    op _=/=_ : Nat Nat -> Bool [comm] .
    -----------------------------------
    eq N       == N = tt [variant] .
    eq N + NzN == N = ff [variant] .

    eq N =/= N' = ~ (N == N') [variant] .

    op _<_ : Nat Nat -> Bool .
    --------------------------
    eq N      < N + NzN = tt [variant] .
    eq N + N' < N       = ff [variant] .

    op _<=_ : Nat Nat -> Bool .
    ---------------------------
    eq N + NzN <= N      = ff [variant] .
    eq N       <= N + N' = tt [variant] .
endfm
```

```maude
fmod FVP-INT-PRED is
   protecting FVP-INT .
   protecting FVP-NAT-PRED .

    vars   N   N' :   Nat .
    var  NzN NzN' : NzNat .
    vars   I   I' :   Int .

    op _==_  : Int Int -> Bool [ditto] .
    op _=/=_ : Int Int -> Bool [ditto] .
    ------------------------------------
    eq - NzN ==   NzN' = ff          [variant] .
    eq - NzN == - NzN' = NzN == NzN' [variant] .

    eq I =/= I' = ~ (I == I') [variant] .

    op _<_  : Int Int -> Bool [ditto] .
    op _<=_ : Int Int -> Bool [ditto] .
    -----------------------------------
    eq - NzN <  N' = tt [variant] .
    eq - NzN <= N' = tt [variant] .

    eq N <  - NzN' = ff [variant] .
    eq N <= - NzN' = ff [variant] .

    eq - NzN <  - NzN' = NzN' <  NzN [variant] .
    eq - NzN <= - NzN' = NzN' <= NzN [variant] .
endfm
```

Locally FVP Operators
---------------------

```maude
fmod FVP-NAT-MULT is
   protecting FVP-NAT .

    var    N      :   Nat .
    vars NzN NzN' : NzNat .

    op _*_ : Nat Nat -> Nat [assoc comm] .
    --------------------------------------
    eq N * 0 =  0 [variant] .
    eq N * 1 =  N [variant] .

    eq N * (NzN + NzN') = (N * NzN) + (N * NzN') .
endfm

fmod FVP-INT-MULT is
   protecting FVP-NAT-MULT .
   protecting FVP-INT .

    vars   I   I' :   Int .
    vars NzI NzI' : NzInt .

    op _*_ : Int Int -> Int [ditto] .
    ---------------------------------
    eq I * 0 = 0 [variant] .
    eq I * 1 = I [variant] .

    eq I * (NzI + NzI') = (I * NzI) + (I * NzI') .
endfm
```
