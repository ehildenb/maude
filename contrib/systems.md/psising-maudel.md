PSIsing Maudel
==============

Ising Model
-----------

The Ising model is a simple model from statistical mechanics which demonstrates the phenomenon of *phase transitions*[@gallovati-phase-transitions-ising-model].
A $d$-dimensional lattice of particles with spin are set up to have pairwise interactions and interactions with the environment.
As the number of dimensions goes beyond 4, the Ising model is studied using *mean field theory* in physics[@kadanoff-phase-transitions-mean-field-theory].

For a configuration space $\Sigma : [I \to \{+1,-1\}]$ (assignment to spin up/down for each lattice site $i \in I$), the *energy* $E$ of the system in any configuration can be calculated exactly.
This is done using the *Hamiltonian* $H : \Sigma \to E$ where:

-   $\sigma \in \Sigma$ is the global spin configuration ($\sigma_j$ being the spin at site $j$);
-   $J_{i,j}$ is an interaction constant between sites $i$ and $j$;
-   $\mu$ is the magnetic moment of the material (how responsive it is to external magnetic fields); and
-   $h_j$ is the external magnetic field at site $j$.

$$H(\sigma) = - \sum_{\langle i , j \rangle} J_{i,j} \sigma_i \sigma_j - \mu \sum_j h_j \sigma_j$$

The probability of finding the system in a given configuration is given as a function of only the energy of the configuration $P_\beta : \Sigma \to [0,1]$, where:

-   $\beta = 1 / (k_B T)$ is inverse temperature (scaled by the the Boltzmann constant $k_B$); and
-   $Z_\beta$ is a normalization constant $\sum_{\sigma} e^{-\beta H(\sigma)}$.

$$P_\beta(\sigma) = \frac{e^{-\beta H(\sigma)}}{Z_\beta}$$

### Simplifying Assumptions

For the sake of this model, we:

-   set interaction between sites to be constant ($J_{i, j} = J$),
-   only consider nearest neighbor interactions (sum over $\langle i , j \rangle$), and
-   assume no external magnetic field ($\mu = 0$).

$$H(\sigma) = - J \sum_{\langle i , j \rangle} \sigma_i \sigma_j$$

Modelling in Maude
------------------

The Ising model is a 1D probabilistic cellular automata, so the Maude libraries for cellular automata and the PSI solver are imported here.

-   Modules `PSI-INTERNAL-SIMPLIFICATION` and `PSI-PROBABILITY-VECTORS` give us the probabalistic state needed for the Ising model.
-   Module `PERIODIC-GRID` is a 2D periodic cellular automata with parameters for setting the height, width, and local update rule.

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
The variable `HD` is calculated to be the change in energy associated with flipping the current site.
This says, "if it's energetically favorable (`HD < 0`), then flip; otherwise only flip with probability (`exp(- HD / T)`)".

**TODO**: Is there a better way to format this for readability?

```maude
   ceq S -> left[SL] right[SR] SS = S -> [ HD < 0 ] ? flip(S)
                                                    : (exp(- HD / T) ? flip(S) : S)
                                 if HD := Hlocal(flip(S), SL, SR) - Hlocal(S, SL, SR) .
endm
```
