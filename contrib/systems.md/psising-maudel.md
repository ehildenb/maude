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

Modeling in Maude
-----------------

The Ising model is a 1D probabilistic cellular automata, so the Maude libraries for cellular automata and the PSI solver are imported here.

-   Modules `PSI-INTERNAL-SIMPLIFICATION` and `PSI-PROBABILITY-VECTORS` give us the probabalistic state needed for the Ising model.
-   Module `PERIODIC-GRID` is a 2D periodic cellular automata with parameters for setting the height, width, and local update rule.

```maude
load caut.maude
load ../tools/solvers/psi.maude

mod PSISING-MAUDEL is
   protecting PSI-INTERNAL-SIMPLIFICATION + PSI-PROBABILITY-VECTORS .
    extending PERIODIC-GRID * ( op true  to tt
                              , op false to ff
                              , op _<_   to _P<_
                              , op _>_   to _P>_
                              , op <_`,_> to [_`,_]
                              ) .

    vars DE DE' HD : DExp . vars S SL SR : State . var SS : States .
```

### Parameters

We'll restrict to 1D by setting `height` to 1, but leave the number of sites open (the `width`).

```maude
    eq height = 1 .
    ---------------
```

Parameters `J` and `T` are the site-coupling coefficient and temperature, respectively.
These must be provided by the user.

```maude
   ops J T : -> Rat .
   ------------------
```

### Cellular Automata State

The cellular automata state is a probability vector of length 2 (probability of spin up, probability of spin down).
Sort `2PVect` is subsorted into `State` to instantiate the cellular automata to hold a spin on each site.

```maude
    subsort 2PVect < State .
    ------------------------
```

The operators `spinUp` and `spinDown` are common states, provided here:

```maude
   ops spinUp spinDown : -> State .
   --------------------------------
    eq spinUp   = < 1 , 0 > .
    eq spinDown = < 0 , 1 > .
```

Operator `flip` will switch the probability distributions of sites.
We have that `flip(spinUp) == spinDown` and `flip(spinDown) == spinUp`.

```maude
    op flip : State -> State .
    --------------------------
    eq flip(< DE , DE' >) = < DE' , DE > .
```

### Local Update Rule

The local update rule for the Ising model depends on the change of energy associated with the state change.
`Hlocal` is the local *Hamiltonial*, which calculates the energy of a sites correlation with its neighbors (using correlation operator `_**_`).

```maude
    op Hlocal : State State State -> DExp .
    ---------------------------------------
    eq Hlocal(S, SL, SR) = (- J) * ((S ** SL) + (S ** SR)) .
```

Here the update rule is specified assuming the `CELLULAR-AUTOMATA` code has already loaded the surrounding system states.
The variable `HD` is calculated to be the change in energy associated with flipping the current site.

**TODO**: Is there a better way to format this for readability?

```maude
   ceq S -> left[SL] right[SR] SS = S -> [ HD < 0 ] ? flip(S)
                                                    : (exp(- HD / T) ? flip(S) : S)
                                 if HD := Hlocal(flip(S), SL, SR) - Hlocal(S, SL, SR) .
endm
```

The left-hand side of the rule matches on a state `S` with left neighbor `SL` and right neighbor `SR`.
The right-hand says, "if it's energetically favorable (`HD < 0`), then flip; otherwise only flip with probability (`exp(- HD / T)`)".
