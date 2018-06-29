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

```test
set print attribute on .
```

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

```njr-thesis
Purified:
tagged('0:Real ?= '0/1.Real 
 /\ '1:Real ?= '1/1.Real 
 /\ 'p11:Real ?= '_+_['_*_['a11:Real, 'b11:Real],'_*_[ 'a12:Real, 'b21:Real]] 
 /\ 'p12:Real ?= '_+_['_*_['a11:Real, 'b12:Real],'_*_[ 'a12:Real, 'b22:Real]] 
 /\ 'p21:Real ?= '_+_['_*_['a21:Real, 'b11:Real],'_*_[ 'a22:Real, 'b21:Real]] 
 /\ 'p22:Real ?= '_+_['_*_['a21:Real, 'b12:Real],'_*_[ 'a22:Real, 'b22:Real]] 
 /\ '0/1.Real ?= '_-_['_*_[ 'a11:Real, 'a22:Real],'_*_[ 'a12:Real, 'a21:Real]], ('check-sat > 'smt-sat) ; ( 'convex > 'false) ; 'mod > 'REAL)
tagged('a11:Real ?= 'm11['A:Matrix] 
 /\ 'b11:Real ?= 'm11['B:Matrix] 
 /\ 'a12:Real ?= 'm12['A:Matrix] 
 /\ 'b12:Real ?= 'm12['B:Matrix] 
 /\ 'a21:Real ?= 'm21['A:Matrix] 
 /\ 'b21:Real ?= 'm21['B:Matrix] 
 /\ 'a22:Real ?= 'm22['A:Matrix] 
 /\ 'b22:Real ?= 'm22['B:Matrix] 
 /\ 'matrix[ '1:Real,'0:Real, '0:Real,'1:Real] ?= 'matrix['p11:Real,'p12:Real,'p21:Real,'p22:Real], ('check-sat > 'var-sat) ; ('convex > 'false) ; 'mod > 'MATRIX-REAL)
EqualityProp: 'MATRIX-REAL: => '0:Real ?= 'p12:Real
EqualityProp: 'MATRIX-REAL: => '1:Real ?= 'p11:Real
EqualityProp: 'REAL: => 'p12:Real ?= 'p22:Real
EqualityProp: 'MATRIX-REAL: => 'p11:Real ?= 'p21:Real
EqualityProp: 'REAL: => 'a11:Real ?= 'a21:Real
EqualityProp: 'REAL: => 'a12:Real ?= 'a22:Real
EqualityProp: 'MATRIX-REAL: => 'p21:Real ?= 'p22:Real
result Bool: (true).Bool
```

It turns out that if we combine this module with the Integers instead of the Reals, we can prove
something stronger: that any invertible matrix must have determinant $\pm 1$.

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

TODO: Explain/Format output

```njr-thesis
Purified:
tagged('0:Integer ?= 'a21:Integer 
 /\ '0:Integer ?= 'b21:Integer 
 /\ '0:Integer ?= '0.Integer 
 /\ '1:Integer ?= '1.Integer 
 /\ 'p11:Integer ?= '_+_['_*_[ 'a11:Integer, 'b11:Integer],'_*_[ 'a12:Integer, 'b21:Integer]] 
 /\ 'p12:Integer ?= '_+_['_*_[ 'a11:Integer, 'b12:Integer],'_*_[ 'a12:Integer, 'b22:Integer]] 
 /\ 'p21:Integer ?= '_+_['_*_[ 'a21:Integer, 'b11:Integer],'_*_[ 'a22:Integer, 'b21:Integer]] 
 /\ 'p22:Integer ?= '_+_['_*_[ 'a21:Integer, 'b12:Integer],'_*_[ 'a22:Integer, 'b22:Integer]] 
 /\ '1.Integer != '_-_['_*_[ 'a11:Integer, 'a22:Integer],'_*_[ 'a12:Integer, 'a21:Integer]] 
 /\ '-_['1.Integer] != '_-_[ '_*_['a11:Integer, 'a22:Integer],'_*_[ 'a12:Integer, 'a21:Integer]], ('check-sat > 'smt-sat) ; ( 'convex > 'false) ; 'mod > 'INTEGER) tagged('0:Integer ?= 'a21:Integer 
 /\ '0:Integer ?= 'b21:Integer 
 /\ 'a11:Integer ?= 'm11['A:Matrix] 
 /\ 'b11:Integer ?= 'm11['B:Matrix] 
 /\ 'a12:Integer ?= 'm12['A:Matrix] 
 /\ 'b12:Integer ?= 'm12['B:Matrix] 
 /\ 'a21:Integer ?= 'm21['A:Matrix] 
 /\ 'b21:Integer ?= 'm21['B:Matrix] 
 /\ 'a22:Integer ?= 'm22['A:Matrix] 
 /\ 'b22:Integer ?= 'm22['B:Matrix] 
 /\ 'matrix[ '1:Integer,'0:Integer, '0:Integer,'1:Integer] ?= 'matrix['p11:Integer,'p12:Integer,'p21:Integer,'p22:Integer], ( 'check-sat > 'var-sat) ; ('convex > 'true) ; 'mod > 'MATRIX-INTEGER)
EqualityProp: 'INTEGER: => '0:Integer ?= 'p21:Integer
EqualityProp: 'INTEGER: => 'p21:Integer ?= 'a21:Integer
EqualityProp: 'INTEGER: => 'a21:Integer ?= 'b21:Integer
EqualityProp: 'MATRIX-INTEGER: => '1:Integer ?= 'p11:Integer
EqualityProp: 'INTEGER: => 'a11:Integer ?= 'b11:Integer
EqualityProp: 'MATRIX-INTEGER: => 'p11:Integer ?= 'p22:Integer
result Bool: (true).Bool
```

