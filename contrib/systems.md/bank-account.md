Bank Account
============

The Bank Account models a single account in a bank.
The account can receive deposits and withdrawals, and should never go into overdraft under proper usage.

```maude
load ../tools/fvp/numbers.maude
```

Bank Account
------------

Here is the high-level specification of the bank account system.

```maude
mod BANK-ACCOUNT is
   protecting FVP-NAT-PRED .

    sorts Msg MsgConf .
    -------------------
    subsort Msg < MsgConf .

    op withdraw : Nat -> Msg [ctor] .
    op mt       : -> MsgConf [ctor] .
    op _,_ : MsgConf MsgConf -> MsgConf [ctor assoc comm id: mt] .

    sort Account .
    ------------

    op < bal:_pend:_overdraft:_> : Nat Nat Bool -> Account [ctor] .
    ---------------------------------------------------------------

    sorts State StatePair .
    -----------------------
    vars s s' : State .

    --- TODO: Should [_,_,_] be frozen?
    op _#_ : Account MsgConf -> State [ctor] .
    op [_,_,_] : Bool State State -> State [frozen] .  *** if-then-else
    -------------------------------------------------------------------
    eq [true ,s,s'] = s  [variant] .
    eq [false,s,s'] = s' [variant] .

    op [_,_] : State State -> StatePair [ctor] .  *** will be used to compute variants of rules
    -------------------------------------------------------------------------------------------

    vars n n' m x y x' y' : Nat . vars b b' : Bool .
    var msgs : MsgConf .

    *** requesting to draw money having sufficient funds; the amount requested is
    *** added to the amount of pending withdraw requests

    rl < bal: n + m + x pend: x     overdraft: false > #               msgs
    => < bal: n + m + x pend: x + m overdraft: false > # withdraw(m) , msgs .

    *** actual withdrawing of money from account

    rl < bal: n pend: x overdraft: false > # withdraw(m) , msgs
    => [ n < m , < bal: n           pend: x           overdraft: true  > # msgs
               , < bal: (n monus m) pend: (x monus m) overdraft: false > # msgs
       ] .

    *** more money can at any time be deposited in the account if it is not in overdraft

    rl < bal: n     pend: x overdraft: false > # msgs
    => < bal: n + m pend: x overdraft: false > # msgs .
endm
```

After performing constructor decomposition, you should arrive at this specification of the bank account system.

```maude
mod BANK-ACCOUNT-CTOR is
   protecting FVP-NAT-PRED .

    sorts Msg MsgConf .
    -------------------
    subsort Msg < MsgConf .

    op withdraw : Nat -> Msg [ctor] .
    op mt       : -> MsgConf [ctor] .
    op _,_ : MsgConf MsgConf -> MsgConf [ctor assoc comm id: mt] .

    sort Account .
    --------------

    op < bal:_pend:_overdraft:_> : Nat Nat Bool -> Account [ctor] .
    ---------------------------------------------------------------

    sorts State StatePair .
    -----------------------
    vars s s' : State .

    --- TODO: Should [_,_,_] be frozen?
    op _#_ : Account MsgConf -> State [ctor] .
    op [_,_,_] : Bool State State -> State [frozen] .  *** if-then-else
    -------------------------------------------------------------------
    eq [true ,s,s'] = s  [variant] .
    eq [false,s,s'] = s' [variant] .

    op [_,_] : State State -> StatePair [ctor] .  *** will be used to compute variants of rules
    -------------------------------------------------------------------------------------------

    vars n n' m x y x' y' : Nat . vars b b' : Bool .
    var msgs : MsgConf .

    rl < bal: n + m + x pend: x     overdraft: false > #               msgs
    => < bal: n + m + x pend: x + m overdraft: false > # withdraw(m) , msgs
    [narrowing label initWithdrawal] .

     *** actual withdrawing of money from account (done with ctor variants of original rule)

    rl < bal: N1:Nat + N3:Nat + N4:Nat pend: N3:Nat overdraft: false > # M2:MsgConf , withdraw(N3:Nat + N4:Nat)
    => < bal: N1:Nat                   pend: 0      overdraft: false > # M2:MsgConf
    [narrowing label doWithdrawal1] .

    rl < bal: N1:Nat + N3:Nat pend: N3:Nat + N4:Nat overdraft: false > # M2:MsgConf , withdraw(N3:Nat)
    => < bal: N1:Nat          pend: N4:Nat          overdraft: false > # M2:MsgConf
    [narrowing label doWithdrawal2] .

    rl < bal: N1:Nat pend: N2:Nat overdraft: false > # M3:MsgConf , withdraw(1 + N1:Nat + N4:Nat)
    => < bal: N1:Nat pend: N2:Nat overdraft: true  > # M3:MsgConf
    [narrowing label overdraft] .

    *** more money can at any time be deposited in the account if it is not in overdraft

    rl < bal: n     pend: x overdraft: false > # msgs
    => < bal: n + m pend: x overdraft: false > # msgs
    [narrowing label deposit] .
endm
```

Defined Operations
------------------

The following operations help to specify claims about the bank account system.

```maude
mod BANK-ACCOUNT-DEFINEDOPS is
   protecting BANK-ACCOUNT-CTOR .
   protecting FVP-BOOL-EQFORM .

    var N : Nat . var MSGS : MsgConf .

    op debts : MsgConf -> Nat .
    ---------------------------
    eq debts(mt)                 = 0 .
    eq debts(withdraw(N) , MSGS) = N + debts(MSGS) .
endm
```
