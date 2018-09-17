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

   crl [on]    : < TIME,TMP,MODE ,BND,MIN,MAX > => { TIME    ,TMP  ,true ,BND,MIN,MAX } if TMP   < MIN + BND = true .
   crl [off]   : < TIME,TMP,MODE ,BND,MIN,MAX > => { TIME    ,TMP  ,false,BND,MIN,MAX } if MAX   < TMP + BND = true .
   crl [tick1] : { TIME,TMP,true ,BND,MIN,MAX } => < TIME + 1,TMP@0,true ,BND,MIN,MAX > if TMP   < TMP@0     = true /\ TMP@0 < TMP   + BND = true  [nonexec] .
   crl [tick2] : { TIME,TMP,false,BND,MIN,MAX } => < TIME + 1,TMP@0,false,BND,MIN,MAX > if TMP@0 < TMP       = true /\ TMP   < TMP@0 + BND = true  [nonexec] .
    rl [stop1] : < TIME,TMP,MODE ,BND,MIN,MAX > => [ TIME,TMP,MODE ,BND  ,MIN,MAX ] .
    rl [stop2] : { TIME,TMP,MODE ,BND,MIN,MAX } => [ TIME,TMP,MODE ,BND  ,MIN,MAX ] .
endm
```
