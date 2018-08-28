Bank Account
============

The Bank Account models a single account in a bank.
The account can receive deposits and withdrawals, and should never go into overdraft under proper usage.

FVP Numbers
-----------

Finite Variant Property numbers are supplied.

**TODO**: Use some standard FVP numbers.

```maude
set include BOOL off .

fmod BOOL-FVP is

    sort Truth .
    ------------
    vars B X Y Z : Truth .

    ops tt ff : -> Truth [ctor] .
    -----------------------------

    op ~ : Truth -> Truth .
    -----------------------
    eq ~(tt) = ff [variant] .
    eq ~(ff) = tt [variant] .

    op _/\_ : Truth Truth -> Truth .
    op _\/_ : Truth Truth -> Truth .
    --------------------------------
    eq tt /\ B = B  [variant] .
    eq ff /\ B = ff [variant] .
    eq ff \/ B = B  [variant] .
    eq tt \/ B = tt [variant] .
endfm

fmod NAT-PRES-MONUS is
   protecting BOOL-FVP .

    sort Nat .
    ----------
    vars n n' m x y x' y' : Nat . vars b b' : Truth .

   ops 0 1 :         -> Nat [ctor] .
    op _+_ : Nat Nat -> Nat [ctor assoc comm id: 0] .
    -------------------------------------------------

    op _>_  : Nat Nat -> Truth .
    op _>=_ : Nat Nat -> Truth .
    ----------------------------
    eq m + n + 1 > n     = tt [variant] .
    eq n         > n + m = ff [variant] .

    eq m + n >= n         = tt [variant] .
    eq n     >= m + n + 1 = ff [variant] .

    op _==_ : Nat Nat -> Truth [comm] .
    -----------------------------------
    eq n == n         = tt [variant] .
    eq n == n + m + 1 = ff [variant] .

    op _-_ : Nat Nat -> Nat .  *** monus
    ------------------------------------
    eq n       - (n + m) = 0 [variant] .
    eq (n + m) - n       = m [variant] .
endfm
```

Bank Account
------------

Here is the high-level specification of the bank account system.

```maude
mod BANK-ACCOUNT is
   protecting NAT-PRES-MONUS .

    sorts Msg MsgConf .
    -------------------
    subsort Msg < MsgConf .

    op withdraw : Nat -> Msg [ctor] .
    op mt       : -> MsgConf [ctor] .
    op _,_ : MsgConf MsgConf -> MsgConf [ctor assoc comm id: mt] .

    sort Account .
    ------------

    op < bal:_pend:_overdraft:_> : Nat Nat Truth -> Account [ctor] .
    ----------------------------------------------------------------

    sorts State StatePair .
    -----------------------
    vars s s' : State .

    --- TODO: Should [_,_,_] be frozen?
    op _#_ : Account MsgConf -> State [ctor] .
    op [_,_,_] : Truth State State -> State [frozen] .  *** if-then-else
    --------------------------------------------------------------------
    eq [tt,s,s'] = s  [variant] .
    eq [ff,s,s'] = s' [variant] .

    op [_,_] : State State -> StatePair [ctor] .  *** will be used to compute variants of rules
    -------------------------------------------------------------------------------------------

    vars n n' m x y x' y' : Nat . vars b b' : Truth .
    var msgs : MsgConf .

    *** requesting to draw money having sufficient funds; the amount requested is
    *** added to the amount of pending withdraw requests

    rl < bal: n + m + x pend: x     overdraft: ff > #               msgs
    => < bal: n + m + x pend: x + m overdraft: ff > # withdraw(m) , msgs .

    *** actual withdrawing of money from account

    rl < bal: n pend: x overdraft: ff > # withdraw(m) , msgs
    => [ m > n , < bal: n       pend: x       overdraft: tt > # msgs
               , < bal: (n - m) pend: (x - m) overdraft: ff > # msgs
       ] .

    *** more money can at any time be deposited in the account if it is not in overdraft

    rl < bal: n     pend: x overdraft: ff > # msgs
    => < bal: n + m pend: x overdraft: ff > # msgs .
endm
```

After performing constructor decomposition, you should arrive at this specification of the bank account system.

```maude
mod BANK-ACCOUNT-CTOR is
   protecting NAT-PRES-MONUS .

    sorts Msg MsgConf .
    -------------------
    subsort Msg < MsgConf .

    op withdraw : Nat -> Msg [ctor] .
    op mt       : -> MsgConf [ctor] .
    op _,_ : MsgConf MsgConf -> MsgConf [ctor assoc comm id: mt] .

    sort Account .
    --------------

    op < bal:_pend:_overdraft:_> : Nat Nat Truth -> Account [ctor] .
    ----------------------------------------------------------------

    sorts State StatePair .
    -----------------------
    vars s s' : State .

    --- TODO: Should [_,_,_] be frozen?
    op _#_ : Account MsgConf -> State [ctor] .
    op [_,_,_] : Truth State State -> State [frozen] .  *** if-then-else
    --------------------------------------------------------------------
    eq [tt,s,s'] = s  [variant] .
    eq [ff,s,s'] = s' [variant] .

    op [_,_] : State State -> StatePair [ctor] .  *** will be used to compute variants of rules
    -------------------------------------------------------------------------------------------

    vars n n' m x y x' y' : Nat . vars b b' : Truth .
    var msgs : MsgConf .

    rl < bal: n + m + x pend: x     overdraft: ff > #               msgs
    => < bal: n + m + x pend: x + m overdraft: ff > # withdraw(m) , msgs [narrowing] .

     *** actual withdrawing of money from account (done with ctor variants of original rule)

    rl < bal: N1:Nat + N3:Nat + N4:Nat pend: N3:Nat overdraft: ff > # M2:MsgConf , withdraw(N3:Nat + N4:Nat)
    => < bal: N1:Nat                   pend: 0      overdraft: ff > # M2:MsgConf [narrowing] .

    rl < bal: N1:Nat + N3:Nat pend: N3:Nat + N4:Nat overdraft: ff > # M2:MsgConf , withdraw(N3:Nat)
    => < bal: N1:Nat          pend: N4:Nat          overdraft: ff > # M2:MsgConf [narrowing] .

    rl < bal: N1:Nat pend: N2:Nat overdraft: ff > # M3:MsgConf , withdraw(1 + N1:Nat + N4:Nat)
    => < bal: N1:Nat pend: N2:Nat overdraft: tt > # M3:MsgConf [narrowing] .

    *** more money can at any time be deposited in the account if it is not in overdraft

    rl < bal: n     pend: x overdraft: ff > # msgs
    => < bal: n + m pend: x overdraft: ff > # msgs [narrowing] .
endm
```

Defined Operations
------------------

The following operations help to specify claims about the bank account system.

```maude
mod BANK-ACCOUNT-DEFINEDOPS is
   protecting BANK-ACCOUNT-CTOR .

    var N : Nat . var MSGS : MsgConf .

    op debts : MsgConf -> Nat .
    ---------------------------
    eq debts(mt)                 = 0 .
    eq debts(withdraw(N) , MSGS) = N + debts(MSGS) .
endm
```