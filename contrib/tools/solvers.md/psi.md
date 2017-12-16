PSI - Exact Probabalistic Inference
===================================

The [PSI Solver](http://psisolver.org) is an exact symbolic solver for probabilistic systems which can be expressed in its internal language[@gehr-misailovic-vechev-psi-solver].
The internal language supports arithmetic, logarithms/exponentials, Iverson brackets, Dirac delta functions, infinite sums, and some primitive distributions/integrals/anti-derivatives.
A high level imperative language is supplied which exports arithmetic, sampling from builtin distributions (both discrete and continuous), distribution marginalization, choice, and bounded looping.
The high level language is compiled into the internal language, where PSI performs many algebraic, guard, and integral simplifications to generate an exact symbolic closed-form solution distribution.
PSI also supports an approximation backend, which would allow for simulations of larger systems where exact symbolic solutions are not feasible.

Here we build the syntax and some semantics of the PSI solver, allowing us to use probability distributions directly in Maude.

### Utilities

`RENAMED-RATS` allows us to re-use syntax from the Prelude without operator names clashing.

```maude
set include BOOL off .

fmod RENAMED-RAT is
   protecting RAT * ( op true  to tt
                    , op false to ff
                    , op _<_   to _P<_
                    , op _>_   to _P>_
                    ) .
endfm
```

PSI Internal Language
---------------------

### Internal Language Syntax

The internal language is that of arithmetic expressions over discrete and continuous probability distributions.
Sort `DExp` is the grammar of this language:

-   Subsort `DVar` is for variables.
-   Subsort `DConst` is for rational numbers and other special constants.

```maude
fmod PSI-INTERNAL is
   protecting RENAMED-RAT .

    sorts DVar DConst DExp .
    ------------------------
    subsort Rat < DConst .
    subsorts DVar DConst < DExp .

    var DC : DConst . vars DE DE' : DExp .
```

Constants `pi` and `e` are real-valued constants added to sort `DConst`.
Distribution arithmetic over sort `DExp` is supported.

```maude
   ops pi e : -> DConst .
   ----------------------

    op -_ : DExp -> DExp [ditto] .
    ------------------------------

    op _+_ : DExp DExp -> DExp [ditto] .
    op _-_ : DExp DExp -> DExp [ditto] .
    op _*_ : DExp DExp -> DExp [ditto] .
    op _/_ : DExp DExp -> DExp [ditto] .
    op _^_ : DExp DExp -> DExp [ditto] .
    ------------------------------------
```

[Iverson Brackets](https://en.wikipedia.org/wiki/Iverson_bracket) are used to represent various conditionals.
The distribution $\left[ p \otimes q \right]$ for $\otimes \in \{ < , > , ≤ , ≥ , = , ≠ \}$ represents the probability of $\otimes$ evaluating to true on samples from distribution $p$ and $q$.

**TODO**: Stop using non-ascii syntax variants.

**TODO**: Add subsort `BDExp` and operator `[_]` to turn any boolean into a condition?

```maude
    op [_<_] : DExp DExp -> DExp .
    op [_>_] : DExp DExp -> DExp .
    op [_≤_] : DExp DExp -> DExp .
    op [_≥_] : DExp DExp -> DExp .
    op [_=_] : DExp DExp -> DExp [comm] .
    op [_≠_] : DExp DExp -> DExp [comm] .
    -------------------------------------
```

Support for the named primitives of PSI is provided here.

```maude
    op ln      : DExp -> DExp .
    op exp     : DExp -> DExp .
    op sin     : DExp -> DExp .
    op cos     : DExp -> DExp .
    op floor   : DExp -> DExp .
    op ceil    : DExp -> DExp .
    op dirac   : DExp -> DExp .
    op δ(0)[_] : DExp -> DExp .
    ---------------------------
    eq dirac(DE) = δ(0)[DE] .
    eq exp(DE)   = e ^ DE .
```

Operators in sort `Binder` bind a variable in context, which must be taken into account when doing substitution.
Four binders are provided for:

-   infinite sums over the integers (`sum_._`),
-   integrals over the real line (`int_._`),
-   the Error function, which is the antiderivative of the Gaussian distribution (`err_._`), and
-   limits at a point on the real line (`lim[N]_._`).

```maude
    sort Binder .
    -------------
    vars X Y : DVar . var S : Subst . var BIND : Binder .

    op __._ : Binder DVar DExp -> DExp [prec 20] .
    ----------------------------------------------

    op lim[_]      : Rat -> Binder .
   ops sum int err :     -> Binder .
   ---------------------------------
```

Substitutions are useful to rename variables in sub-expression or instantiate an expression with variables.
Here we provide an explicit substitution operator which is binder-aware (to avoid $\alpha$-capture).

```maude
    sort Subst .
    ------------

    op [_/_] : DExp DVar  -> Subst .
    op __    : DExp Subst -> DExp [prec 55] .
    -----------------------------------------
    eq X [ DE / Y ] = if X == Y then DE else X fi .

    eq DC         S = DC .
    eq (- DE)     S = - (DE S) .
    eq (DE + DE') S = (DE S) + (DE' S) .
    eq (DE - DE') S = (DE S) + (DE' S) .
    eq (DE * DE') S = (DE S) * (DE' S) .
    eq (DE / DE') S = (DE S) * (DE' S) .
    eq (DE ^ DE') S = (DE S) ^ (DE' S) .

    eq [ DE < DE' ] S = [ (DE S) < (DE' S) ] .
    eq [ DE > DE' ] S = [ (DE S) > (DE' S) ] .
    eq [ DE ≤ DE' ] S = [ (DE S) ≤ (DE' S) ] .
    eq [ DE ≥ DE' ] S = [ (DE S) ≥ (DE' S) ] .
    eq [ DE = DE' ] S = [ (DE S) = (DE' S) ] .
    eq [ DE ≠ DE' ] S = [ (DE S) ≠ (DE' S) ] .

    eq ln(DE)    S = ln(DE S) .
    eq exp(DE)   S = exp(DE S) .
    eq sin(DE)   S = sin(DE S) .
    eq cos(DE)   S = cos(DE S) .
    eq floor(DE) S = floor(DE S) .
    eq ceil(DE)  S = ceil(DE S) .
    eq δ(0)[DE]  S = δ(0)[DE S] .

    eq BIND X . DE [ DE' / Y ] = if X == Y then BIND X . DE else BIND X . (DE [ DE' / Y ]) fi .
endfm
```

### PSI Internal Language Simplifications

Many simple algebraic simplifications can be expressed directly in Maude and greatly reduce the size of the generated terms.
Here they are provided in a separate module so that in the future we can benchmark using Maude for these simplifications versus leaving all simplification to PSI.

```maude
fmod PSI-INTERNAL-SIMPLIFICATION is
    including PSI-INTERNAL .

    vars R R' : Rat . var NzR : NzRat .
    vars DE DE' DE'' : DExp . var DC : DConst . vars X Y : DVar .

    --- Arithmetic
    eq    - (- DE)  = DE .
    eq DE - (- DE') = DE + DE' .
    eq 0  + DE'     = DE' .
    eq 0  * DE'     = 0 .
    eq 1  * DE'     = DE' .

    --- Exponentiation
    eq DE ^ 0 = 1 .
    eq DE ^ 1 = DE .
    eq (DE ^ DE') * (DE ^ DE'') = DE ^ (DE + DE') .

    --- Iverson Brackets
    eq [ R < R' ] = if R P<  R' then 1 else 0 fi .
    eq [ R > R' ] = if R P>  R' then 1 else 0 fi .
    eq [ R ≤ R' ] = if R <=  R' then 1 else 0 fi .
    eq [ R ≥ R' ] = if R >=  R' then 1 else 0 fi .
    eq [ R = R' ] = if R ==  R' then 1 else 0 fi .
    eq [ R ≠ R' ] = if R =/= R' then 1 else 0 fi .

    --- Dirac Delta Arithmetic
    eq δ(0)[NzR] = 0 .
endfm
```

PSI Language Extensions
-----------------------

Here are several of the PSI language extensions provided incrementally as Maude modules.
The goal is to make these language extensions modular so that each individually has clear semantics.
Each Maude tool or system can import whichever extensions make expressing their model or system most elegant; over time we'll have a better idea about which ones are used frequently.

### Primitive Distributions

Many primitive distributions are useful for building up complicated probability distributions.
PSI has support for several by translating them into the internal language, along with assertions about the passed parameters.

**TODO**: Finish translation of primitive distributions.

**TODO**: Decide on how to handle error terms at the PSI internal level.

```maude
fmod PSI-PRIMITIVE-DISTRIBUTIONS is
   protecting PSI-INTERNAL .

    var X : DVar . var DE : DExp .

    op bernoulli  : DVar DExp      -> DExp .
    op poisson    : DVar DExp      -> DExp .
    op uniformInt : DVar DExp DExp -> DExp .
    op uniform    : DVar DExp DExp -> DExp .
    op gauss      : DVar DExp DExp -> DExp .
    op pareto     : DVar DExp DExp -> DExp .
    op beta       : DVar DExp DExp -> DExp .
    op gamma      : DVar DExp DExp -> DExp .
    ----------------------------------------
    eq bernoulli(X, DE) = DE * dirac(1 - X) + (1 - DE) * dirac(X) .
endfm
```

### Probabalistic Choice

Probabalistic choice (operator `_?_:_`) is a conditional where the condition is a sample from a probability distribution.
This operator is used by many higher-level language features.

**TODO**: Should the translation be `[ C ≠ 0 ] * B1 + [ C = 0 ] * B2`?
          Or perhaps something using the measure-theoretic version of conditional probabilities?
          I think the given translation only works for statistically independent branches, which seems rather unlikely.

```maude
fmod PSI-CHOICE is
   protecting PSI-INTERNAL .

    vars C B1 B2 : DExp .

    op _?_:_ : DExp DExp DExp -> DExp .
    -----------------------------------
    eq C ? B1 : B2 = (C * B1) + ((1 - C) * B2) .
endfm
```

### Probability Vectors

Probability vectors are vectors with entries between 0 and 1 which sum to 1.
They represent the probability of finding a system in any given discrete state (each state is an entry in the vector).

**TODO**: Procedurally generate this module for any `N` (length of probability vectors).

```maude
fmod PSI-PROBABILITY-VECTORS is
   protecting PSI-INTERNAL + PSI-CHOICE .

    sorts 2PVect 3PVect .
    ---------------------

    vars DE DE1 DE1' DE1'' DE2 DE2' DE2'' : DExp .
    vars 2P 2P' : 2PVect . vars 3P 3P' : 3PVect .

    op <_,_>   : DExp DExp      -> 2PVect .
    op <_,_,_> : DExp DExp DExp -> 3PVect .
    ---------------------------------------
```
Operators `_+_` and `_*_` provide vector addition and scalar-vector multiplication, respectively.
These can be used to scale/sum probability vectors.

**TODO**: Should we be doing some normalization on vector addition?
          As long as only operator `_?_:_` is used, we know the vectors will stay length 1.

```maude
    op _+_ : 2PVect 2PVect -> 2PVect .
    op _+_ : 3PVect 3PVect -> 3PVect .
    ----------------------------------
    eq < DE1 , DE1' >         +  < DE2 , DE2' >         = < DE1 + DE2 , DE1' + DE2' > .
    eq < DE1 , DE1' , DE1'' > +  < DE2 , DE2' , DE2'' > = < DE1 + DE2 , DE1' + DE2' , DE1'' + DE2'' > .

    op _*>_ : DExp 2PVect -> 2PVect .
    op _*>_ : DExp 3PVect -> 3PVect .
    ---------------------------------
    eq DE *> < DE1 , DE1' >         = < DE * DE1 , DE * DE1' > .
    eq DE *> < DE1 , DE1' , DE1'' > = < DE * DE1 , DE * DE1' , DE * DE1'' > .
```

Here we piecewise lift probabilistic choice to `2PVect` and `3PVect`.

```maude
    op _?_:_ : DExp 2PVect 2PVect -> 2PVect .
    op _?_:_ : DExp 3PVect 3PVect -> 3PVect .
    -----------------------------------------
    eq DE ? < DE1 , DE1' >         : < DE2 , DE2' >         = < DE ? DE1 : DE2 , DE ? DE1' : DE2' > .
    eq DE ? < DE1 , DE1' , DE1'' > : < DE2 , DE2' , DE2'' > = < DE ? DE1 : DE2 , DE ? DE1' : DE2' , DE ? DE1'' : DE2'' > .
```

The correlation (`_**_`) between two probability vectors measures roughly how much they agree on the individual components.
This quantity has useful interpretations in many systems.

**TODO**: Is `_**_` assoc/comm?

**TODO**: Define `_**_` over `3PVect`.

```maude
    op _**_ : 2PVect 2PVect -> DExp .
    ---------------------------------
    eq < DE1 , DE1' > ** < DE2 , DE2' > = (DE1 * (DE2 - DE2')) + (DE1' * (DE2' - DE2)) .
endfm
```

PSI User Language
-----------------

### PSI Imperative User Language

```maude
fmod PSI-USER is
   protecting PSI-PRIMITIVE-DISTRIBUTIONS .
   protecting RENAMED-RAT .

    sort Exp .
    ----------
    subsort DExp < Exp .

    op _+_ : Exp Exp -> Exp [ditto] .
    op _-_ : Exp Exp -> Exp [ditto] .
    op _*_ : Exp Exp -> Exp [ditto] .
    op _/_ : Exp Exp -> Exp [ditto] .
    op _^_ : Exp Exp -> Exp [ditto] .
    ---------------------------------

   ops true false : -> Exp .
    op _&&_ : Exp Exp -> Exp [assoc comm] .
    op _||_ : Exp Exp -> Exp [assoc comm] .
    ---------------------------------------

    op _<_   : Exp Exp -> Exp .
    op _>_   : Exp Exp -> Exp .
    op _≤_   : Exp Exp -> Exp .
    op _≥_   : Exp Exp -> Exp .
    op _===_ : Exp Exp -> Exp [comm] .
    op _≠_   : Exp Exp -> Exp [comm] .
    ----------------------------------

    op _?_:_ : Exp Exp Exp -> Exp [prec 50] .
    -----------------------------------------

    sorts NeDVarList DVarList NeExpList ExpList .
    ---------------------------------------------
    subsorts DVar < NeDVarList < DVarList .
    subsorts Exp  < NeExpList  < ExpList .
    subsort NeDVarList < NeExpList .
    subsort DVarList   < ExpList .

    op .DVarList : -> DVarList .
    op _,_     : DVarList   DVarList ->   DVarList [assoc id: .DVarList] .
    op _,_     : DVarList NeDVarList -> NeDVarList [ditto] .
    op _,_     : ExpList     ExpList ->   ExpList  [ditto] .
    op _,_     : ExpList   NeExpList -> NeExpList  [ditto] .
    ------------------------------------------------------

    op _(_) : DVar ExpList -> Exp .
    -------------------------------

    sort Function .
    ---------------

    op fun_(_) =_ : DVar DVarList Exp  -> Function [prec 50] .
    ----------------------------------------------------------

    sort Program .
    --------------
    subsort Function < Program .

    op __ : Program Program -> Program [assoc prec 70] .
    ----------------------------------------------------

    --- No imperative support yet
--- ;     sort Stmt .
--- ;     -----------
--- ;
--- ;     op skip :           -> Stmt .
--- ;     op _;_  : Stmt Stmt -> Stmt [assoc id: skip prec 55] .
--- ;     ------------------------------------------------------
--- ;
--- ;     op _:=_     : DVar  Exp -> Stmt [prec 50] .
--- ;     op _=_      : DVar  Exp -> Stmt [prec 50] .
--- ;     op observe_ : Exp     -> Stmt [prec 50] .
--- ;     op assert_  : Exp     -> Stmt [prec 50] .
--- ;     op return_  : Exp     -> Stmt [prec 50] .
--- ;     -----------------------------------------
--- ;
--- ;     op if_{_} else {_} : Exp Stmt Stmt -> Stmt .
--- ;     op for_in [_.._] {_} : DVar Exp Exp Stmt -> Stmt .
--- ;     ------------------------------------------------
--- ;
--- ;     sort Procedure .
--- ;     ----------------
--- ;     subsort Procedure < Program .
--- ;
--- ;     op def _(_) {_} : DVar DVarList Stmt -> Procedure .
--- ;     -----------------------------------------------

    --- No array support yet
--- ;     op _[_] : DVar  Exp -> Exp [prec 35] .
--- ;     op _:= array(_,_) : DVar  Exp Exp -> Stmt [prec 50] .
--- ;     op _[_] =_        : DVar  Exp Exp -> Stmt [prec 50] .

    --- No mariginalization yet
--- ;    ops Expectation FromMarginal SampleFrom : -> Sample .
--- ;     op _(_) : Sample ExpList -> Exp .   --- TODO: should be Exp or Stmt?
endfm
```

### PSI User to Internal Translation

```maude
fmod PSI-TRANSLATION is
   protecting PSI-USER .

    sorts Query Program? .
    ----------------------
    subsorts Query Program < Program? .

    op __ : Program? Program? -> Program? [ditto] .
    -----------------------------------------------

    op dist: _ : Exp -> Query [prec 50] .
    -------------------------------------

    op fv : DExp -> DVar .
    ----------------------

    sort TranslateEnv .
    -------------------

    var X : DVar . vars DE DE' DE'' : DExp . var DC : DConst .
    vars E E' E'' : Exp . vars NeEL NeEL' : NeExpList . var EL : ExpList .
    vars TE TE' TE'' : TranslateEnv .

    op .TranslateEnv :                           -> TranslateEnv .
    op _;_           : TranslateEnv TranslateEnv -> TranslateEnv [assoc id: .TranslateEnv prec 55] .
    ------------------------------------------------------------------------------------------------

    op Assert : DExp -> TranslateEnv .
    ----------------------------------

    op {_} :         TranslateEnv -> TranslateEnv .
    op _|_ : ExpList TranslateEnv -> TranslateEnv [prec 60] .
    ---------------------------------------------------------
    eq { { TE } }          = { TE } .
    eq { .DVarList  | TE } = .DVarList   | TE .
   ceq { E , NeEL | TE }   = DE' , NeEL' | TE''
                          if DE'   | TE'  := { E    | TE  }
                          /\ NeEL' | TE'' := { NeEL | TE' } .

    eq { X  | TE } = X  | TE .
    eq { DC | TE } = DC | TE .

   ceq {   -   E' | TE } =    - DE'                   | TE'                     if      DE' | TE' := {     E' | TE } .
   ceq { E +   E' | TE } = DE + DE'                   | TE'                     if DE , DE' | TE' := { E , E' | TE } .
   ceq { E -   E' | TE } = DE - DE'                   | TE'                     if DE , DE' | TE' := { E , E' | TE } .
   ceq { E *   E' | TE } = DE * DE'                   | TE'                     if DE , DE' | TE' := { E , E' | TE } .
   ceq { E /   E' | TE } = DE / DE'                   | TE' ; Assert([DE' ≠ 0]) if DE , DE' | TE' := { E , E' | TE } .
   ceq { E ^   E' | TE } = DE ^ DE'                   | TE'                     if DE , DE' | TE' := { E , E' | TE } .
   ceq { E &&  E' | TE } = [DE ≠ 0] * [DE' ≠ 0]       | TE'                     if DE , DE' | TE' := { E , E' | TE } .
   ceq { E ||  E' | TE } = [[DE ≠ 0] + [DE' ≠ 0] ≠ 0] | TE'                     if DE , DE' | TE' := { E , E' | TE } .
   ceq { E <   E' | TE } = [ DE < DE' ]               | TE'                     if DE , DE' | TE' := { E , E' | TE } .
   ceq { E >   E' | TE } = [ DE > DE' ]               | TE'                     if DE , DE' | TE' := { E , E' | TE } .
   ceq { E ≤   E' | TE } = [ DE ≤ DE' ]               | TE'                     if DE , DE' | TE' := { E , E' | TE } .
   ceq { E ≥   E' | TE } = [ DE ≥ DE' ]               | TE'                     if DE , DE' | TE' := { E , E' | TE } .
   ceq { E === E' | TE } = [ DE = DE' ]               | TE'                     if DE , DE' | TE' := { E , E' | TE } .
   ceq { E ≠   E' | TE } = [ DE ≠ DE' ]               | TE'                     if DE , DE' | TE' := { E , E' | TE } .

   ceq { E ? E' : E'' | TE } = [ DE ≠ 0 ] * DE' + [ DE = 0 ] * DE'' | TE' if DE , DE' , DE'' | TE' := { E , E', E'' | TE } .

   ceq { [ E < E' ] | TE } = [ DE < DE' ] | TE' if DE , DE' | TE' := { E , E' | TE } .
   ceq { [ E > E' ] | TE } = [ DE > DE' ] | TE' if DE , DE' | TE' := { E , E' | TE } .
   ceq { [ E ≤ E' ] | TE } = [ DE ≤ DE' ] | TE' if DE , DE' | TE' := { E , E' | TE } .
   ceq { [ E ≥ E' ] | TE } = [ DE ≥ DE' ] | TE' if DE , DE' | TE' := { E , E' | TE } .
   ceq { [ E = E' ] | TE } = [ DE = DE' ] | TE' if DE , DE' | TE' := { E , E' | TE } .
   ceq { [ E ≠ E' ] | TE } = [ DE ≠ DE' ] | TE' if DE , DE' | TE' := { E , E' | TE } .

   ceq { ln(E)    | TE } = ln(DE)    | TE' ; Assert([DE > 0]) if DE | TE' := { E | TE } .
   ceq { sin(E)   | TE } = sin(DE)   | TE'                    if DE | TE' := { E | TE } .
   ceq { cos(E)   | TE } = cos(DE)   | TE'                    if DE | TE' := { E | TE } .
   ceq { floor(E) | TE } = floor(DE) | TE'                    if DE | TE' := { E | TE } .
   ceq { ceil(E)  | TE } = ceil(DE)  | TE'                    if DE | TE' := { E | TE } .
   ceq { δ(0)[E]  | TE } = δ(0)[DE]  | TE'                    if DE | TE' := { E | TE } .

    op translate : ExpList -> TranslateEnv .
    ----------------------------------------
    eq translate(EL) = { EL | .TranslateEnv } .

    op translateExp : Exp -> [Exp] .
    --------------------------------
   ceq translateExp(E) = DE if DE | .TranslateEnv := translate(E) .
endfm
```
