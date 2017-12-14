PSIsing Maudel
==============

```maude
load caut.maude
load ../tools/solvers/psi.maude

mod PSISING-MAUDEL is
   protecting PSI-INTERNAL-SIMPLIFICATION .
    extending PERIODIC-GRID * ( op true  to tt
                              , op false to ff
                              , op _<_   to _P<_
                              , op _>_   to _P>_
                              ) .

    vars DE DE' DE1 DE1' DE2 DE2' HD : DExp .
    vars S S' SL SR : State . var SS : States .
```

System Specialization
---------------------

To instantiate the `PERIODIC-GRID` module, we need to supply several things:

-   Definitions for the `height` and `width` of the grid.
-   The datatype of system state.
-   The local system update rule.

### Parameters

We'll restrict to 1D situations by setting `height` to 1, but leave the number of sites open (the `width`).

```maude
    eq height = 1 .
```

Parameters `J` and `T` are the coupling coefficient and temperature, respectively.

```maude
   ops J T : -> Rat .
   ------------------
```

### System State

The state of a system at a given time is a tuple of the probability of finding it spin up/down respectively.

-   `[_,_]` is a system state vector.
-   `spinUp` and `spinDown` are example (useful) states.
-   `flip` switches the probabilities of being spin up/down around.

```maude
    op [_,_] : DExp DExp -> State .
    -------------------------------

   ops spinUp spinDown : -> State .
   --------------------------------
    eq spinUp   = [ 1 , 0 ] .
    eq spinDown = [ 0 , 1 ] .

    op flip : State -> State .
    --------------------------
    eq flip([ DE , DE' ]) = [ DE' , DE ] .
```

State tuples `[_,_]` are elements of a *vector field* with the sort `DExp` for scalars.

-   `_+_` provides vector addition,
-   `_*>_` provides scalar-vector multiplication, and
-   `_?_:_` provides probabalistic choice.

**TODO**: Should we be doing some normalization on vector addition?
**TODO**: Vector field or Module? Do we really have division?

```maude
    op _+_  : State State -> State .
    op _*>_ : DExp  State -> State .
    --------------------------------
    eq [ DE1 , DE1' ] +  [ DE2 , DE2' ] = [ DE1 + DE2 , DE1' + DE2' ] .
    eq             DE *> [ DE1 , DE1' ] = [ DE  * DE1 , DE   * DE1' ] .

    op _?_:_ : DExp State State -> State .
    --------------------------------------
    eq DE ? S : S' = (DE *> S) + ((1 - DE) *> S') .
```

-   `_**_` calculates the correlation between two states.
-   `Hlocal` is the local *Hamiltonial*, which calculates the energy of a sites correlation with its neighbors.

```maude
    op _**_ : State State -> DExp .
    -------------------------------
    eq [ DE1 , DE1' ] ** [ DE2 , DE2' ] = DE1 * (DE2 - DE2') + DE1' * (DE2' - DE2) .

    op Hlocal : State State State -> DExp .
    ---------------------------------------
    eq Hlocal(S, SL, SR) = (- J) * ((S ** SL) + (S ** SR)) .
```

### Local Update Rule

After specifying the state vector field data-structure, it's relatively simple to define the update rule.
The variable `HD` is the change in energy associated with flipping the current site.
This says, "if it's energetically favorable (`H < 0)`), then flip; otherwise only flip with probability (`exp(- HD / T)`)".

**TODO**: Is there a better way to format this for readability?

```maude
   ceq S -> left[SL] right[SR] SS = S -> [ HD < 0 ] ? flip(S)
                                                    : (exp(- HD / T) ? flip(S) : S)
                                 if HD := Hlocal(flip(S), SL, SR) - Hlocal(S, SL, SR) .
endm
```