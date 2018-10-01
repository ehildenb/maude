Hereditarily Finite Sets with Reals
-----------------------------------

In this example, we demonstrate the combination algorithm with non-convex theories -- non-linear
real arithmetic and hereditarily finite sets. Hereditarily finite sets is an example of a theory not
currently definable in CVC4 or Yices2 because of its use of algebraic data types modulo axioms like
associativity-commutativity and having FVP equations. Hereditarily finite sets (HFS) are a model of
set theory without the axiom of infinity. Although hereditarily finite sets are expressive enough to
encode constructs like the integers and the natural numbers, its initial model is a countable model
and so cannot encode the real numbers.

```test
set include BOOL off .

fmod HEREDITARILY-FINITE-SET is
    sort MyBool .
    op tt : -> MyBool [ctor] .
    op ff : -> MyBool [ctor] .
```

We have three sorts, `X`, the parametric sort, `Set`s and `Magma`s.
Both `X`s and `Set`s are `Magma`s.

``` {.test .njr-thesis}
    sorts X Set Magma .
    subsorts X Set < Magma .
```

```test
    vars M M' M'' : Magma .
    vars S        : Set   .
```

The elements of a hereditarily finite set can be elements of the parameter sort `X` of "atomic
elements", or can be other hereditarily constructed inductively from the following three
constructors. First, `empty` is a `Set`:

``` {.test .njr-thesis}
    op empty :             -> Set                                       [ctor] .
```

Second, the union operator is an associative, commutative and idemopotent operator:

``` {.test .njr-thesis}
    op _ , _ : Magma Magma -> Magma                          [ctor assoc comm] .
    ----------------------------------------------------------------------------
    eq M , M , M' = M , M'                                           [variant] .
    eq M , M      = M                                                [variant] .
```

Finally, a `Set` may be constructed from any `Magma` by enclosing it in braces.

``` {.test .njr-thesis}
    op { _ } : Magma       -> Set                                       [ctor] .
```

We also have a subset operator and the various equations (not detailed here) defining it:

``` {.test .njr-thesis}
    op _ C= _ : Magma Magma -> MyBool                                          .
```

```test
    ----------------------------------------------------------------------------
    eq empty C= M         = tt                                       [variant] .
    eq { M } C= { M, M' } = tt                                       [variant] .

    eq { M } C= { M     } = tt                                       [variant] .
    eq { M } C= empty     = ff                                       [variant] .

    eq { M, M' } C= { M, M'' }
     = {    M' } C= { M, M'' }                                       [variant] .
    eq { M, M' } C= { M      }
     = {    M' } C= { M      }                                       [variant] .

--- Since `var-sat` does not support `[owise]`, we do not implement the equation
--- for handling the negative case. Since the theory is OS-Compact, we can just let
--- the predicate get stuck, partially evaluated.

    op _ U _ : Set   Set   -> Set                                              .
    ----------------------------------------------------------------------------
    eq empty U S          = S                                        [variant] .
    eq { M } U { M' }     = { M, M' }                                [variant] .
```

```test
endfm

```

```test
fmod TEST-HEREDITARILY-FINITE-SET-SANITY is
    protecting HEREDITARILY-FINITE-SET .
    protecting BOOL .
    vars M M' : Magma .
endfm

reduce M, M', M == M, M' .
reduce { { empty }, { { empty } }, empty }
    == { { empty }, { { empty } }, empty }
     .

reduce empty     C= { empty  }                   == tt  .
reduce { empty } C= empty                               .
reduce { empty } C= { { empty } }                       .
reduce { empty } C= { empty, { empty } }         == tt  .
reduce { empty } C= { { empty }, { { empty } } }        .
reduce { M, M' } C= { M, M' }                           .

reduce { empty, empty } C= { empty } .

--- Nelson Oppen
--- ------------
---
--- We must trick `var-sat` into thinking that the `X` sort is countable.
```

We instantiate this module  with `Real`s as a subsort of `X`:

``` {.test .njr-thesis}
fmod HFS-REAL is
    including HEREDITARILY-FINITE-SET .
    sorts Real .
    subsorts Real < X .

    op fake-0 :      -> Real  [ctor] .
    op fake-s : Real -> Real  [ctor] .
endfm
```

```test
load ../../../contrib/tools/meta/nelson-oppen-combination

fmod HEREDITARILY-FINITE-SET-TEST-VARSAT is
    protecting BOOL .
    protecting VAR-SAT .
    protecting HFS-REAL .
    vars M M' : Magma .
endfm

--- TODO: This does not reduce as I expect it to
reduce upTerm({ X:Real, Y:Real, Z:Real } C= { X:Real }) .

reduce upTerm({ X:Real, Y:Real, Z:Real } C= { A:Real }) .
reduce upTerm({ X:Real, Y:Real, Z:Real } ) .

reduce var-sat( upModule('HFS-REAL, true)
              , upTerm({ empty , M } C= { empty }) ?= 'tt.MyBool
              ) .
reduce var-sat( upModule('HFS-REAL, true)
              ,   upTerm({ empty , M } C= { empty }) ?= 'tt.MyBool
               /\ upTerm(M) != 'empty.Set
              ) .
reduce var-sat( upModule('HFS-REAL, true)
              , upTerm({ empty , M } C= { empty , M' }) ?= 'tt.MyBool
              ) .

--- get variants { X:Magma, Y:Magma, Z:Magma } .
--- --- Lots and lots of variants? or variant computation is slow?
--- reduce var-sat( upModule('HFS-REAL, true)
---               , upTerm({ X:Magma, Y:Magma, Z:Magma } C= { X:Magma }) ?= 'tt.MyBool
---               ) == true .

reduce var-sat( upModule('HFS-REAL, true)
              , upTerm({ X:Real, Y:Real, Z:Real }) ?= upTerm({ X:Real })
              ) == true .

reduce var-sat( upModule('HFS-REAL, true)
              , upTerm({ X:Real, Y:Real, Z:Real } C= { X:Real }) ?= 'tt.MyBool
              ) == true .
```

Finally, we check the satisfiability of the formula $\{ x^2 , y^2, z^2 \} \subseteq \{ a \} \land x \ne y$. i.e. "is
it possible for the set of squares of three numbers, two of which must be distinct, to be a
subset of a set with a single element." This is indeed possible, since every positive real number
has two distinct square roots. Since set union is idempotent, if the two distinct numbers are
additive inverses of each other and the third is equal to either, then the proposition would indeed
be satisfied.

```test
set print attribute on .
```

Our query is:

``` {.test .njr-thesis}
reduce in NELSON-OPPEN-COMBINATION :
       nelson-oppen-sat( ( tagged(tt, ('mod > 'REAL)    ; ('check-sat > 'smt-sat))
                         , tagged(tt, ('mod > 'HFS-REAL); ('check-sat > 'var-sat))
                         )
                         , (  '_C=_[ '`{_`}['_`,_[ '_*_ [ 'Z:Real, 'Z:Real ]
                                                 , '_*_ [ 'X:Real, 'X:Real ]
                                                 , '_*_ [ 'Y:Real, 'Y:Real ]
                                                 ]]
                                   , '`{_`}['A:Real]]
                              ?= 'tt.MyBool
                           )
                           /\ 'X:Real != 'Y:Real
                         ) .
```

This purifies to:

```njr-thesis
    'x2:Real ?= '_*_['X:Real,'X:Real] 
 /\ 'y2:Real ?= '_*_['Y:Real,'Y:Real] 
 /\ 'z2:Real ?= '_*_['Z:Real,'Z:Real] 
 /\ 'X:Real != 'Y:Real,
``` 

in the theory of the hereditarily finite sets, and to:

```njr-thesis
    'tt.MyBool ?= '_C=_['`{_`}['_`,_['z2:Real,'x2:Real,'y2:Real]],'`{_`}['A:Real]] 
 /\ 'X:Real != 'Y:Real

```

in the theory of the reals.

Initially, a few equalities are propagated from the theory of hereditarily finite sets:

```njr-thesis
'HFS-REAL: => 'x2:Real ?= 'y2:Real
'HFS-REAL: => 'y2:Real ?= 'z2:Real
'HFS-REAL: => 'z2:Real ?= 'A:Real
```

Since no more identifications of variables are implied on their own and the theories are not convex,
the algorithm must check whether a disjunction of identifications is implied by either of the
theories, and indeed $x = z \lor y = z$ is implied. The algorithm splits the search space on the
remaining candidate equalities ($a = x$, $a = y$, $a = z$, $x = y$, $z = z$ and $y = z$). It first
tries the case where $a = x$ and finds that there are satisfiabile arrangements (this can happen
when $a = x = 1$). It then splits the search space again, but finds that there are no arrangements
$a = y$ possible (since that implies that $x = y$). However the case where $a = z$ is satisfiable.
This causes the the equality $x = z$ to be propagated. Now, since no further equalities or
disjunctions thereof hold, the algorithm concludes that the formula is satisfiable.

```
Split? 'A:Real ?= 'X:Real \/ 'A:Real ?= 'Y:Real \/ 'A:Real ?= 'Z:Real \/ 'X:Real ?= 'Y:Real \/ 'X:Real ?= 'Z:Real \/ 'Y:Real ?= 'Z:Real
Split: 'HFS-REAL : 'A:Real ?= 'X:Real
Split? 'A:Real ?= 'Y:Real \/ 'A:Real ?= 'Z:Real \/ 'X:Real ?= 'Y:Real \/ 'X:Real ?= 'Z:Real \/ 'Y:Real ?= 'Z:Real
Split: 'HFS-REAL : 'A:Real ?= 'Y:Real
Split: 'HFS-REAL : 'A:Real ?= 'Z:Real
EqualityProp: 'HFS-REAL: => 'X:Real ?= 'Z:Real
Split? 'A:Real ?= 'Y:Real \/ 'Y:Real ?= 'Z:Real
rewrites: 36007 in 4943ms cpu (4951ms real) (7284 rewrites/second)
result Bool: (true).Bool
```
