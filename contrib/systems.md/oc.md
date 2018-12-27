Ordered Communication
=====================

This is a very simple fault-tolerant communication protocol.
There are two size-one buffers used as uni-directional channels by the sender and the receiver.
Either of them can test if the channels are empty and each can read the contents of the channel directed to them.
The state is quite simple:

```
    [sender-counter ::  sender-list | channel-to-sender | channel-to-receiver | receiver-list :: receiver-counter]
```

The key problem is that values in channels can be lost.
This is modeled by the `[drop]` rules.
For this reason, both the sender and the receiver need to keep resending data and acks when they test that the channel where these should be placed is empty.
There is no way to know whether the channel is empty because something has been read on the other end; or becuase it has been lost.

The main property one would like to prove is "successful in-order-communication," which is the reachability formula:

```
    [1 :: l1 | null | nullp | nil :: 0] | k = | l1 |  ->* [k :: nil | null | nullp | l1 :: k]
```

Data Structures
---------------

Here we extend FVP numbers with pairs and lists.

```maude
sload ../tools/fvp/numbers.maude

fmod MAYBE-FVP-NAT is
   protecting FVP-NAT-PRED + TRUTH .

    sort MaybeNat .
    ---------------
    subsort Nat < MaybeNat .

    op null : -> MaybeNat [ctor] .
    ------------------------------
endfm

fmod ASSOC-LIST { X :: TRIV } is

    sort List{X} NeList{X} .
    ------------------------
    subsort X$Elt < NeList{X} < List{X} .

    var N : X$Elt . var L : NeList{X} .

    op nil :                     ->   List{X} [ctor] .
    op __  : NeList{X} NeList{X} -> NeList{X} [ctor assoc] .
    --------------------------------------------------------

    op head : NeList{X} -> X$Elt .
    ------------------------------
    eq head(N)   = N [variant] .
    eq head(N L) = N [variant] .
endfm

fmod ASSOC-LIST-FVP-NAT is
   protecting FVP-NAT .
   protecting ASSOC-LIST{Nat} * ( sort NeList{Nat} to NeListNat
                                , sort   List{Nat} to   ListNat
                                ) .
endfm

fmod ASSOC-LIST-FVP-BOOL is
   protecting FVP-BOOL-CTOR .
   protecting ASSOC-LIST{Bool} * ( sort NeList{Bool} to NeBoolList
                                 , sort   List{Bool} to   BoolList
                                 ) .
endfm

fmod PAIR-FVP-NAT is
   protecting FVP-NAT .

    sorts PairNat MaybePairNat .
    ----------------------------
    subsorts PairNat < MaybePairNat .

    op nullp :           -> MaybePairNat [ctor] .
    op [_,_] : Nat NzNat ->      PairNat [ctor] .
    ---------------------------------------------
endfm

fmod ASSOC-LIST-FVP-NAT-LEN is
   protecting ASSOC-LIST-FVP-NAT .

    vars L L' : ListNat .  var N : Nat .

    op |_| : ListNat -> Nat .
    -------------------------
    eq | nil  | = 0 [variant] .
    eq | N    | = 1 [variant] .
    eq | L L' | = | L | + | L' | .
endfm

fmod FT-COMM-DATA is
   protecting MAYBE-FVP-NAT .
   protecting PAIR-FVP-NAT .
   protecting ASSOC-LIST-FVP-NAT .
endfm
```

Fault Tolerant Protocol
-----------------------

```maude
mod FT-COMM is
  pr FT-COMM-DATA .
  sort State .
  op [_::_|_|_|_::_] : Nat ListNat MaybeNat MaybePairNat ListNat Nat -> State [ctor] .

  vars l l' l'' : NeListNat .  vars l1 l2 : ListNat . var n m k : Nat . vars p q : NzNat .
  var n? : MaybeNat .  var pa : PairNat . var pa? : MaybePairNat .
  rl [rec-1] :      [p :: l      | n?   | [n,m + 1] | nil :: m] => [p     :: l    | m + 1 | nullp     | n    :: m + 1] .
  rl [rec-+] :      [p :: l      | n?   | [n,m + 1] | l'  :: m] => [p     :: l    | m + 1 | nullp     | l' n :: m + 1] .
  rl [rec-ack-1] :  [p :: n      | p    | pa?       | l   :: q] => [p     :: nil  | null  | nullp     | l    :: q    ] .
  rl [rec-ack-2] :  [p :: n m    | p    | pa?       | l   :: q] => [p + 1 :: m    | null  | [m,p + 1] | l    :: q    ] .
  rl [rec-ack-+] :  [p :: n m l' | p    | pa?       | l   :: q] => [p + 1 :: m l' | null  | [m,p + 1] | l    :: q    ] .
 crl [resend-1] :   [p :: n      | n?   | nullp     | l1  :: m] => [p     :: n    | null  | [n,p]     | l1   :: m    ] if n? =/= p .
 crl [resend-+] :   [p :: n l    | n?   | nullp     | l1  :: m] => [p     :: n l  | null  | [n,p]     | l1   :: m    ] if n? =/= p .
  rl [resend-ack] : [p :: l      | n?   | [n,q]     | l'  :: q] => [p     :: l    | q     | nullp     | l'   :: q    ] .
  rl [drop-snd] :   [n :: l1     | n?   | pa        | l2  :: m] => [n     :: l1   | n?    | nullp     | l2   :: m    ] .
  rl [drop-ack] :   [n :: l1     | k    | pa?       | l2  :: m] => [n     :: l1   | null  | pa?       | l2   :: m    ] .
endm
```
