Hereditarily Finite Sets with Reals
-----------------------------------

In this example, we demonstrate the combination algorithm with an non-convex theory, non-linear real
arithmetic. The other theory we use, hereditarily finite sets, is convex and is an example of a
theory that cannot be implemented in CVC4 or Yices2 because of its use of recursive algebraic types
with axioms on its constructors. Hereditarily finite sets (HFS) are a model of set theory without
the axiom of infinity. Although hereditarily finite sets are expressive enough to encode constructs
like integers and real numbers, it is often more convenient (and efficienct) to parameterize them
with a more abstract representation of these constructs.

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

The elements of a hereditarily finite set are constructed inductively from three constructors.
First, `empty` is a `Set`:

``` {.test .njr-thesis}
    op empty :             -> Set                                       [ctor] .
```

Second, via the union operator, an associative, commutative and idemopotent operator:

``` {.test .njr-thesis}
    op _ , _ : Magma Magma -> Magma                          [ctor assoc comm] .
    ----------------------------------------------------------------------------
    eq M , M , M' = M , M'                                           [variant] .
    eq M , M      = M                                                [variant] .
```

Finally, any `Magma` can be enclosed in braces to form a `Set`.

``` {.test .njr-thesis}
    op { _ } : Magma       -> Set                                       [ctor] .
```

We also have a subset operator and the various equations defining it:

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

Finally, check the satisfiability the formula
$\{ x^2 , y^2, z^2 \} \subseteq \{ a \}$.
i.e. "is it possible for the set of squares of three distinct elements to be a subset
of a set with a single element." Proving that this is not satisfiable requires
understanding that sets are idemopotent and that at most two distinct reals can have
the same square in the theory of reals.

```test
set print attribute on .
```

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
                         ) .
```


This purifies to:

```njr-thesis
'tt.MyBool ?= '_C=_['`{_`}['_`,_['z2:Real,'x2:Real,'y2:Real]],'`{_`}['A:Real]]
``` 

in the theory of the hereditarily finite sets, and to:

```njr-thesis
    'x2:Real ?= '_*_['X:Real,'X:Real]
 /\ 'y2:Real ?= '_*_['Y:Real,'Y:Real]
 /\ 'z2:Real ?= '_*_['Z:Real,'Z:Real]
```

in the theory of the reals.

Initially, a few equalities are propagated from the theory of hereditarily finite sets:

```njr-thesis
'HFS-REAL: => 'x2:Real ?= 'y2:Real
'HFS-REAL: => 'y2:Real ?= 'z2:Real
'HFS-REAL: => 'z2:Real ?= 'A:Real
```

Then, since the reals are not convex, the split rule must be applied. The algorithm searches through
each of the remaining candidate equalities, and finds that adding both `'A:Real ?= 'X:Real` and
`'A:Real ?= 'Y:Real` is still satisfiable in both theories, and further propagates the equality:
`'X:Real ?= 'Y:Real`. At this point no further disjunctions of equalities are implied and the
algorithm terminates returning *satisfiable*.

```
Split? 'A:Real ?= 'X:Real \/ 'A:Real ?= 'Y:Real \/ 'A:Real ?= 'Z:Real \/ 'X:Real ?= 'Y:Real \/ 'X:Real ?= 'Z:Real \/ 'Y:Real ?= 'Z:Real
Split: 'HFS-REAL : 'A:Real ?= 'X:Real
Split? 'A:Real ?= 'Y:Real \/ 'A:Real ?= 'Z:Real \/ 'X:Real ?= 'Y:Real \/ 'X:Real ?= 'Z:Real \/ 'Y:Real ?= 'Z:Real
Split: 'HFS-REAL : 'A:Real ?= 'Y:Real
EqualityProp: 'HFS-REAL: => 'X:Real ?= 'Y:Real
Split? 'A:Real ?= 'Z:Real \/ 'Y:Real ?= 'Z:Real
rewrites: 28717 in 4549ms cpu (4626ms real) (6311 rewrites/second)
result Bool: (true).Bool
```
