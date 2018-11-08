Thermostat
==========

A simple thermostat in maude, which switches between on/off based on some threshold temperatures.
The state components of the thermostate are the current time, the temperature, and the mode (on, off, turning on, turning off).

Parametric Thermostat
---------------------

### Unital Ordered Rings

The thermostat is parametric in an ordered ring with unit (which will be the data for the temperature).

```maude
fth UNITAL-ORDERED-RING is

    sorts RingBool Ring .
    ---------------------
    vars R R' : Ring .

    ops 0 1 : -> Ring [ctor] .
    --------------------------

    op _+_ : Ring Ring -> Ring [ctor assoc comm id: 0] .
    ----------------------------------------------------

    op -_ : Ring -> Ring [ctor] .
    -----------------------------
    eq - 0       = 0 [nonexec] .
    eq R + (- R) = 0 [nonexec] .

    op _-_ : Ring Ring -> Ring .
    ----------------------------
    eq R - R' = R + (- R') [nonexec] .

    ops true false : -> RingBool [ctor] .
    -------------------------------------

    op _<_ : Ring Ring -> RingBool .
    --------------------------------
endfth
```

### Thermostat Definition

```maude
mod THERMOSTAT{TIME :: UNITAL-ORDERED-RING, TMP :: UNITAL-ORDERED-RING} is

    sorts DelayMode InMode Mode .
    -----------------------------
    subsorts DelayMode InMode < Mode .

    sort Conf .
    -----------
    vars TIME TIME' : TIME$Ring . vars TMP TMP' : TMP$Ring .
    var MODE : Mode . var IM : InMode . var DM : DelayMode .

   ops on off :        -> InMode    [ctor] .
    op delay  : InMode -> DelayMode [ctor] .
    ----------------------------------------

    op <_,_,_> : TIME$Ring TMP$Ring Mode -> Conf [ctor] .
    op {_,_,_} : TIME$Ring TMP$Ring Mode -> Conf [ctor] .
    -----------------------------------------------------
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

   crl [delaying] : { TIME     , TMP , DM }
                 => < TIME - 1 , TMP , DM >
                 if 0 < TIME = true .

    rl [delay-over] : { 0 , TMP , delay(IM) }
                   => < 0 , TMP , IM        > .
```

The thermostat `heat-rate` is determined as a function of the thermostat parameters.

```maude
    op heat-rate : Mode TMP$Ring -> TMP$Ring .
    ------------------------------------------
    eq heat-rate(MODE, TMP) = (TMP + source(MODE)) - drain(TMP) .
```

The following are parameters which must be filled in for your particular thermostat.

```maude
    ops min max bound : -> TMP$Ring .
    ---------------------------------

    op time-until : InMode -> TIME$Ring .
    -------------------------------------

    op source : Mode -> TMP$Ring .
    ------------------------------

    op drain : TMP$Ring -> TMP$Ring .
    ---------------------------------
endm
```

Thermostat Instantiation
------------------------

### Instantiation to FVP-INT

 ```maude
load ../tools/fvp/numbers.maude

view RingInt from UNITAL-ORDERED-RING to FVP-INT-PRED is
    sort RingBool to Bool .
    sort Ring     to Int  .
endv

mod THERMOSTAT-INT is protecting THERMOSTAT{RingInt, RingInt} . endm
```

The following is an example thermostat over the integers.

```maude
mod THERMOSTAT-INT-COMFORTABLE is
    extending THERMOSTAT-INT + FVP-NUMBERS .
    var TMP : Int .
```

Here we setup a thermostat which tries to stay between `18` and `26`, and will switch states when `3` away from a boundary temperature.

```maude
    eq min   = 10 + 8      [variant] .
    eq max   = 10 + 10 + 6 [variant] .
    eq bound = 3           [variant] .
```

The thermostat takes `4` seconds to turn on, and `2` seconds to turn off.

```maude
    eq time-until(on)  = 4 [variant] .
    eq time-until(off) = 2 [variant] .
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

### Instantiation to REAL

```maude
load ../tools/base/smt.maude

view RingReal from UNITAL-ORDERED-RING to REAL is
    sort RingBool to Boolean .
    sort Ring     to Real    .

    op 0 to term 0/1 .
    op 1 to term 1/1 .
endv

mod THERMOSTAT-REAL is protecting THERMOSTAT{RingReal, RingReal} . endm
```

The following is an example thermostat over the reals.
The bound and timing parameters are the same as for the integers, just interpereted over the reals instead.

```maude
mod THERMOSTAT-REAL-COMFORTABLE is
    extending THERMOSTAT-REAL .
    extending FVP-NUMBERS .

    vars TIME TMP TMP' : Real .
    var IM : InMode . var MD : DelayMode . var MODE : Mode .

    eq min   = 18/1 [variant] .
    eq max   = 26/1 [variant] .
    eq bound = 3/1  [variant] .

    eq time-until(on)  = 4/1 [variant] .
    eq time-until(off) = 2/1 [variant] .

    eq source(on)         = 5/1 [variant] .
    eq source(off)        = 0/1 [variant] .
    eq source(delay(on))  = 2/1 [variant] .
    eq source(delay(off)) = 2/1 [variant] .
```

However, the drain to the outside is now inverse linear with the current temperature.

```maude
    eq drain(TMP) = 1/10 * (2/1 - TMP) [variant] .
endm
```
