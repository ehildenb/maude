Thermostat
==========

A simple thermostat in maude, which switches between on/off based on some threshold temperatures.
The state components of the thermostate are the current time, the temperature, and the mode (on, off, turning on, turning off).

```maude
load ../tools/fvp/numbers.maude

mod THERMOSTAT is
   protecting FVP-NUMBERS .
   protecting FVP-NAT-PRED .

    sorts DelayMode InMode Mode .
    -----------------------------
    subsorts DelayMode InMode < Mode .

    sort Conf .
    -----------
    vars TIME TIME' TMP TMP' : Nat .
    var MODE : Mode . var IM : InMode . var DM : DelayMode .

   ops on off :        -> InMode    [ctor] .
    op delay  : InMode -> DelayMode [ctor] .
    ----------------------------------------

    op <_,_,_> : Nat Nat Mode -> Conf [ctor] .
    op {_,_,_} : Nat Nat Mode -> Conf [ctor] .
    ------------------------------------------
    rl [tick] : < TIME , TMP                  , MODE >
             => { TIME , heat-rate(MODE, TMP) , MODE } .

   crl [turning-off] : { TIME            , TMP , on         }
                    => < time-until(off) , TMP , delay(off) >
                    if max < TMP + bound = true .

   crl [inmode-on] : { TIME , TMP , on }
                  => < TIME , TMP , on >
                  if max < TMP + bound = false .

   crl [turning-on] : { TIME           , TMP , off       }
                   => < time-until(on) , TMP , delay(on) >
                   if TMP < min + bound = true .

   crl [inmode-off] : { TIME , TMP , off }
                   => < TIME , TMP , off >
                   if TMP < min + bound = false .

    rl [delaying] : { TIME + 1 , TMP , DM }
                 => < TIME     , TMP , DM > .

    rl [delay-over] : { 0 , TMP , delay(IM) }
                   => < 0 , TMP , IM        > .
```

The following are parameters which must be filled in for your particular thermostat.

```maude
   ops min max bound : -> Nat .
   ----------------------------

   ops time-until : InMode -> Nat .
   --------------------------------

    op heat-rate : Mode Nat -> Nat .
    --------------------------------
endm
```
