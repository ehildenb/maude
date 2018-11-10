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

   crl [delaying] : { TIME         , TMP , DM }
                 => < TIME monus 1 , TMP , DM >
                 if 0 < TIME = true .

    rl [delay-over] : { 0 , TMP , delay(IM) }
                   => < 0 , TMP , IM        > .
```

The thermostat `heat-rate` is determined as a function of the thermostat parameters.

```maude
    op heat-rate : Mode Nat -> Nat .
    --------------------------------
    eq heat-rate(MODE, TMP) = (TMP + source(MODE)) monus drain(TMP) .
```

The following are parameters which must be filled in for your particular thermostat.

```maude
   ops min max bound : -> Nat .
   ----------------------------

    op time-until : InMode -> Nat .
    -------------------------------

    op source : Mode -> Nat .
    -------------------------

    op drain : Nat -> Nat .
    -----------------------
endm
```

Thermostat Instantiation
------------------------

The following is an example thermostat over the naturals.

```maude
mod THERMOSTAT-COMFORTABLE is
    extending THERMOSTAT + FVP-NUMBERS .
    var TMP : Nat .
```

Here we setup a thermostat which tries to stay between `18` and `26`, and will switch states when `3` away from a boundary temperature.

```maude
    eq min   = 10 + 8      .
    eq max   = 10 + 10 + 6 .
    eq bound = 3           .
```

The thermostat takes `4` seconds to turn on, and `2` seconds to turn off.

```maude
    eq time-until(on)  = 4 .
    eq time-until(off) = 2 .
```

When turning on/off, the heater is effective for `1` unit temperature per second.
When on, the heater produces `5` units temperature per second, and when off produces nothing.
Every second, `3` units of temperature are drained to the environment.

```maude
    eq source(on)         = 5 [variant] .
    eq source(off)        = 0 [variant] .
    eq source(delay(on))  = 2 [variant] .
    eq source(delay(off)) = 2 [variant] .

    eq drain(TMP) = 3 [variant] .
endm
```
