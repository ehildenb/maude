Thermostat
==========

A simple thermostat in maude, which switches between on/off based on some threshold temperatures.

```maude
load ../tools/fvp/numbers.maude

mod THERMOSTAT is
   protecting FVP-NAT-PRED .

    sort Conf .
    -----------

    var TIME TMP TMP@0 BND MIN MAX : Nat .
    var MODE : Bool .

    op <_,_,_,_,_,_> : Nat Nat Bool Nat Nat Nat -> Conf [ctor] .
    op {_,_,_,_,_,_} : Nat Nat Bool Nat Nat Nat -> Conf [ctor] .
    op [_,_,_,_,_,_] : Nat Nat Bool Nat Nat Nat -> Conf [ctor] .
    ------------------------------------------------------------

   crl [on]    : < TIME,TMP,MODE,BND,MIN,MAX > => { TIME    ,TMP  ,tt,BND,MIN,MAX } if TMP   < MIN + BND = tt .
   crl [off]   : < TIME,TMP,MODE,BND,MIN,MAX > => { TIME    ,TMP  ,ff,BND,MIN,MAX } if MAX   < TMP + BND = tt .
   crl [tick1] : { TIME,TMP,tt,  BND,MIN,MAX } => < TIME + 1,TMP@0,tt,BND,MIN,MAX > if TMP   < TMP@0     = tt /\ TMP@0 < TMP   + BND = tt [nonexec] .
   crl [tick2] : { TIME,TMP,ff,  BND,MIN,MAX } => < TIME + 1,TMP@0,ff,BND,MIN,MAX > if TMP@0 < TMP       = tt /\ TMP   < TMP@0 + BND = tt [nonexec] .
    rl [stop1] : < TIME,TMP,MODE,BND,MIN,MAX > => [ TIME,TMP,MODE,BND,MIN,MAX ] .
    rl [stop2] : { TIME,TMP,MODE,BND,MIN,MAX } => [ TIME,TMP,MODE,BND,MIN,MAX ] .
endm
```
