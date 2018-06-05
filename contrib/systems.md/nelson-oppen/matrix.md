We define Matrices in terms of an undefined sort `X`, without any functions
such as multiplication. This is because multiplication must be defined in
terms of the underlying field's multiplication operator and Nelson-Oppen combination
does not allow sharing of function symbols.

```test
set include BOOL off .

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

We the define parameterise this theory over the reals and the integers:

```test
fmod MATRIX-REAL is
    including MATRIX-X .
    sort Real .
    subsorts Real < X .
    --- Convince var-sat that Real is an infinite sort.
    op fake-zero :      -> Real [ctor] .
    op fake-succ : Real -> Real [ctor] .
endfm
```

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

We define multiplciation and the calculation of the determinant as
meta-functions defining them syntactically, in terms of the fields
multiplication and addition operators.

```test
load ../../../contrib/tools/meta/nelson-oppen-combination.maude

fmod TEST is
    protecting NELSON-OPPEN-COMBINATION .

    vars A B A1 B1 A2 B2 ZERO ONE : Term .

    --- Meta level function for generating term representing multiplciation of
    --- matrices
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
endfm

```

```test
reduce wellFormed(upModule('INTEGER, true), '1.Integer) .
reduce wellFormed(upModule('REAL-INTEGER, true), '1.Integer) .
reduce wellFormed(upModule('REAL, true), '1/1.Real) .
reduce wellFormed(upModule('REAL, true), '_*_[ 'x:Real, 'x:Real ]) .
reduce wellFormed(upModule('REAL, true), '0/1.Real) .

reduce smt-sat('INTEGER, '1.Integer ?= '0.Integer) .
reduce smt-sat('REAL,    '0/5.Real ?= '0/1.Real) .
reduce smt-sat('REAL-INTEGER, '_*_[ 'x:Real, 'x:Real ] ?= '0/1.Real) .
reduce smt-sat('REAL,
                  '_*_[ 'X:Real, 'X:Real ] ?= '0/1.Real
               /\ '_*_[ 'Y:Real, 'Y:Real ] ?= '0/1.Real
               /\ 'X:Real != 'Y:Real
              ) .
reduce smt-sat('REAL,
                  '_*_[ 'X:Real, 'X:Real ] ?= '1/1.Real
               /\ '_*_[ 'Y:Real, 'Y:Real ] ?= '1/1.Real
               /\ 'X:Real != 'Y:Real
              ) .

reduce smt-sat('INTEGER,
                  ('_*_[ 'X:Integer, 'Y:Integer ] ?= '0.Integer)
               => (  ('X:Integer ?= '0.Integer)
                  \/ ('Y:Integer ?= '0.Integer))
              ) .
reduce purify( upModule('MATRIX-INTEGER, true), upModule('INTEGER, true)
             , identity('0.Integer, '1.Integer)) .
```

```test
set print attribute on .

---
--- --- Are there invertible (real) matrices whos determinants are two?
---
--- reduce nelson-oppen-sat(( tagged(tt, (('mod > 'MATRIX-REAL);  ('check-sat > 'var-sat); ('convex > 'true)))
---                         , tagged(tt, (('mod > 'REAL);         ('check-sat > 'smt-sat); ('convex > 'false)))),
---           multiply('A:Matrix, 'B:Matrix) ?= identity('0/1.Real, '1/1.Real)
---        /\ determinant('A:Matrix) ?= '2/1.Real
---        ) .

--- Are there invertible (integer) matrices whos determinants are two?

reduce smt-sat('INTEGER,
           'V4:Integer ?= '1.Integer
		/\ 'V4:Integer ?= '_+_['_*_['A11:Integer,'B11:Integer],'_*_['A12:Integer,'B21:Integer]]
		/\ 'V3:Integer ?= '0.Integer
		/\ 'V3:Integer ?= '_+_['_*_['A11:Integer,'B12:Integer],'_*_['A12:Integer,'B22:Integer]]
		/\ 'V2:Integer ?= '_+_['_*_['A21:Integer,'B11:Integer],'_*_['A22:Integer,'B21:Integer]]
		/\ 'V13:Integer ?= '_+_['_*_['A21:Integer,'B12:Integer],'_*_['A22:Integer,'B22:Integer]]
		/\ 'V13:Integer != 'A22:Integer) .

reduce nelson-oppen-sat(( tagged(tt, (('mod > 'MATRIX-INTEGER);  ('check-sat > 'var-sat); ('convex > 'true)))
                        , tagged(tt, (('mod > 'INTEGER       );  ('check-sat > 'smt-sat); ('convex > 'false)))),
          multiply('A:Matrix, 'B:Matrix) ?= identity('0.Integer, '1.Integer)
       /\ 'A:Matrix ?= identity('0.Integer, '1.Integer)
       /\ 'B:Matrix ?= identity('0.Integer, '1.Integer)
   ) .
```
