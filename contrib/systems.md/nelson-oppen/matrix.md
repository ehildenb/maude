## Matrices with real and integer entries

\newcommand \R {\mathbb{R}}

In this example, we define the theory of $2\times 2$ over $\R$ and prove that any invertible matrix
must have a non-zero determinant. Unfortunately, CVC4 is not able to solve the non-linear arithmetic
needed to prove this. We must instead use the Yices2, the other SMT solver available in Maude. Even
so, the default configuration for Yices2 does not enable the solver for non-linear arithmetic
(MCSAT), and running this example involved modifying the Maude source to enable that configuration.

We define Matrices in terms of an unspecified sort `X`, which we later instantiate with the Reals.
This module only defines accessors for each of the matrix's entries and does not define functions
such as multiplication, determinant and identity. This is because these functions must be defined in
terms of the underlying field's operators operator and Nelson-Oppen combination does not allow
sharing of function symbols.

```test
set include BOOL off .
load ../../../contrib/tools/meta/nelson-oppen-combination.maude
```

```{.test .njr-thesis}
fmod MATRIX-X is
    sort X Matrix .
    op matrix : X X X X -> Matrix [ctor] .

    vars A B C D : X .
    op m11 : Matrix -> X .
    op m12 : Matrix -> X .
    op m21 : Matrix -> X .
    op m22 : Matrix -> X .

    eq m11(matrix(A, B, C, D)) = A [variant] .
    eq m12(matrix(A, B, C, D)) = B [variant] .
    eq m21(matrix(A, B, C, D)) = C [variant] .
    eq m22(matrix(A, B, C, D)) = D [variant] .
endfm
```

Next, we define multiplication, determinant and identify as meta-functions --
functions over terms at the meta-level.

```test
fmod MATRIX-TEST is
    protecting NELSON-OPPEN-COMBINATION .

    vars A B A1 B1 A2 B2 ZERO ONE : Term .
```

```{.test .njr-thesis}
    op mulSum : Term Term Term Term -> Term .
    eq mulSum(A1, B1, A2, B2) = '_+_ [ '_*_ [ A1 , B1 ]
                                     , '_*_ [ A2 , B2 ]
                                     ] .

    op multiply : Term Term -> Term .
    eq multiply(A, B) = 'matrix[ mulSum('m11[A], 'm11[B], 'm12[A], 'm21[B])
                               , mulSum('m11[A], 'm12[B], 'm12[A], 'm22[B])
                               , mulSum('m21[A], 'm11[B], 'm22[A], 'm21[B])
                               , mulSum('m21[A], 'm12[B], 'm22[A], 'm22[B])
                               ] .
    op determinant : Term -> Term .
    eq determinant(A) = '_-_ [ '_*_ [ 'm11[A], 'm22[A] ]
                             , '_*_ [ 'm12[A], 'm21[A] ]
                             ] .

    op identity : Term Term -> Term .
    eq identity(ZERO, ONE) = 'matrix[ONE, ZERO, ZERO, ONE] .
```

```test
endfm
```

Finally, we the parameterise this theory over the reals:

``` {.test .njr-thesis}
fmod MATRIX-REAL is
    including MATRIX-X .
    sort Real .
    subsorts Real < X .
    --- Convince var-sat that Real is an infinite sort.
    op fake-zero :      -> Real [ctor] .
    op fake-succ : Real -> Real [ctor] .
endfm
```

Reducing this in via Nelson-Oppen yeilds:

``` {.test .njr-thesis}
reduce in MATRIX-TEST : nelson-oppen-valid(
    ( tagged(tt, (('mod > 'MATRIX-REAL); ('check-sat > 'var-sat)))
    , tagged(tt, (('mod > 'REAL);        ('check-sat > 'smt-sat)))
    ),
         (multiply('A:Matrix, 'B:Matrix) ?= identity('0/1.Real, '1/1.Real))
      => (determinant('A:Matrix) != '0/1.Real)
    ) .
```

TODO: Explain output

```test
fmod MATRIX-INTEGER is
    including MATRIX-X .
    sort Integer .
    subsorts Integer < X .
    --- Convince var-sat that Integer is an infinite sort.
    op fake-zero :         -> Integer [ctor] .
    op fake-succ : Integer -> Integer [ctor] .
endfm

```

It turns out that if we combine this module with the Integers instead of the Reals, we can prove
something stronger: that any invertible matrix must have determinant $\pm 1$.

``` {.test .njr-thesis}
reduce in MATRIX-TEST : nelson-oppen-valid(
       ( tagged(tt, (('mod > 'MATRIX-INTEGER);  ('check-sat > 'var-sat); ('convex > 'true)))
       , tagged(tt, (('mod > 'INTEGER       );  ('check-sat > 'smt-sat); ('convex > 'false)))
       ),
           (    multiply('A:Matrix, 'B:Matrix) ?= identity('0.Integer, '1.Integer)
             /\ 'm21['A:Matrix] ?= '0.Integer
             /\ 'm21['B:Matrix] ?= '0.Integer
           )
        => (    determinant('A:Matrix) ?= '1.Integer
             \/ determinant('A:Matrix) ?= '-_['1.Integer]
           )
     ) .
```
