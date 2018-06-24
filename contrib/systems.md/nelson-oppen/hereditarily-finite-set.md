Hereditarily Finite Sets
------------------------

Hereditarily finite sets (HFS) are a model of set theory without the axiom of
infinity. Although hereditarily finite sets are expressive enough to encode
constructs like integers and real numbers, it is often more convenient (and
efficiency) to parameterize them with a more abstract representation of these
constructs.

In Maude, we implement HFS as the following functional module:

```test
set include BOOL off .

fmod HEREDITARILY-FINITE-SET is
    sort MyBool .
    op tt : -> MyBool [ctor] .
    op ff : -> MyBool [ctor] .
```

We have three sorts, `X`, the parametric sort, `Set`s and `Magma`s

``` {.test .njr-thesis}
    sorts X Set Magma .
    subsorts X Set < Magma .
```

The empty set is a `Set`, as well as a `Magma` enclosed in braces.

``` {.test .njr-thesis}
    op empty :             -> Set                                       [ctor] .
    op { _ } : Magma       -> Set                                       [ctor] .
```

```test
    vars M M' M'' : Magma .
    vars S        : Set   .
```

The `Magma` union operator (different from the set union operator) is idemopotent.

``` {.test .njr-thesis}
    op _ , _ : Magma Magma -> Magma                          [ctor assoc comm] .
    ----------------------------------------------------------------------------
    eq M , M , M' = M , M'                                           [variant] .
    eq M , M      = M                                                [variant] .
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

Finally, we call `nelson-oppen-sat` on the formula $\{ x^2 , y^2, z^2 \} \subseteq \{ a \} \land x \ne y \land y \ne z \land x \ne z$.

``` {.test .njr-thesis}
reduce in NELSON-OPPEN-COMBINATION :
       nelson-oppen-sat(( tagged(tt, ('mod > 'REAL)    ; ('check-sat > 'smt-sat))
                        , tagged(tt, ('mod > 'HFS-REAL); ('check-sat > 'var-sat))
                        )
                       ,    '_C=_[ '`{_`}['_`,_[ '_*_ [ 'Z:Real, 'Z:Real ]
                                               , '_*_ [ 'X:Real, 'X:Real ]
                                               , '_*_ [ 'Y:Real, 'Y:Real ]
                                               ]]
                                 , '`{_`}['A:Real]]
                            ?= 'tt.MyBool
                         /\ 'X:Real != 'Y:Real
                         /\ 'Y:Real != 'Z:Real
                         /\ 'X:Real != 'Z:Real
                       ) .
```

As a control, we check that dropping the requirement that $x \ne z$ results in the formula being
reported satisfiable.

``` {.test .njr-thesis}
reduce in NELSON-OPPEN-COMBINATION :
       nelson-oppen-sat(( tagged(tt, ('mod > 'REAL)                        ; ('check-sat > 'smt-sat))
                        , tagged(tt, ('mod > 'HFS-REAL); ('check-sat > 'var-sat))
                        )
                       ,    '_C=_[ '`{_`}['_`,_[ '_*_ [ 'Z:Real, 'Z:Real ]
                                               , '_*_ [ 'X:Real, 'X:Real ]
                                               , '_*_ [ 'Y:Real, 'Y:Real ]
                                               ]]
                                 , '`{_`}['A:Real]]
                            ?= 'tt.MyBool
                         /\ 'X:Real != 'Y:Real
                         /\ 'Y:Real != 'Z:Real
                       ) .
```

