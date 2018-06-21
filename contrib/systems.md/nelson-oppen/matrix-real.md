\newcommand \R {\mathbb{R}}

$2 \times 2$ matrices parameterised over $\R$
=============================================

In this example, we define the theory of $2\times 2$ over $\R$ and prove that any invertible matrix must have a non-zero
determinant. Unfortunately, CVC4 is not able to solve the non-linear arithmetic needed to prove this. We must instead
use the Yices2, the other SMT solver available in Maude. Even so, the default configuration for Yices2 does not enable
the solver for non-linear arithmetic (MCSAT), and running this example involved modifying the Maude source to enable
that configuration.

We define matrices as a four tuple of reals with accessor functions for each entry.

```test
set include BOOL off .

fmod MATRIX-REAL is
    sort Real Matrix .
    op matrix : Real Real Real Real -> Matrix [ctor] .
    
    op m11 : Matrix -> Real .
    op m12 : Matrix -> Real .
    op m21 : Matrix -> Real .
    op m22 : Matrix -> Real .

    --- Convince var-sat that Real is an infinite sort.
    op fake-zero :      -> Real [ctor] .
    op fake-succ : Real -> Real [ctor] .

    vars A B C D : Real .

    eq m11(matrix(A, B, C, D)) = A [variant] .
    eq m12(matrix(A, B, C, D)) = B [variant] .
    eq m21(matrix(A, B, C, D)) = C [variant] .
    eq m22(matrix(A, B, C, D)) = D [variant] .
endfm
```

Since Nelson-Oppen combination does not allow theories to share functions or predicates, and multiplication and other
matrix operations are defined in terms of the operations in the underlying field, we do not define it in the previous
module and instead define it syntactically as functions over terms.

```test
load ../../../contrib/tools/meta/nelson-oppen-combination.maude

fmod TEST is
    protecting NELSON-OPPEN-COMBINATION .

    vars A B A1 B1 A2 B2 ZERO ONE : Term .

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

Finally, we check the validity of the hypothesis that $A \times B = I \implies \det(a) \ne 0$
where $A$, $B$ and $I$ are matrices and $I$ is the identity.

```test
set print attribute on .
reduce nelson-oppen-valid(
    ( tagged(tt, (('mod > 'MATRIX-REAL); ('check-sat > 'var-sat)))
    , tagged(tt, (('mod > 'REAL);        ('check-sat > 'smt-sat)))
    ),
       (multiply('A:Matrix, 'B:Matrix) ?= identity('0/1.Real, '1/1.Real))
    => (determinant('A:Matrix) != '0/1.Real)) .
```

This purifies to the formulae:

```
/\ '0/1.Real ?= '_-_['_*_['A11:Real, 'A22:Real],'_*_[ 'A12:Real, 'A21:Real]]
/\ '0Real:Real ?= '0/1.Real
/\ '1Real:Real ?= '1/1.Real 
/\ 'V14:Real ?= '_+_['_*_['A21:Real, 'B12:Real],'_*_[ 'A22:Real, 'B22:Real]] 
/\ 'V5:Real ?= '_+_['_*_['A11:Real, 'B11:Real],'_*_[ 'A12:Real, 'B21:Real]] 
/\ 'V6:Real ?= '_+_['_*_['A11:Real, 'B12:Real],'_*_[ 'A12:Real, 'B22:Real]] 
/\ 'V7:Real ?= '_+_['_*_['A21:Real, 'B11:Real],'_*_[ 'A22:Real, 'B21:Real]] 
```

```
/\ 'A11:Real ?= 'm11['A:Matrix] 
/\ 'A12:Real ?= 'm12['A:Matrix] 
/\ 'A21:Real ?= 'm21['A:Matrix] 
/\ 'A22:Real ?= 'm22['A:Matrix] 
/\ 'B11:Real ?= 'm11['B:Matrix] 
/\ 'B12:Real ?= 'm12['B:Matrix] 
/\ 'B21:Real ?= 'm21['B:Matrix] 
/\ 'B22:Real ?= 'm22['B:Matrix] 
/\     'matrix['1Real:Real,'0Real:Real, '0Real:Real,'1Real:Real]
    ?= 'matrix['V5:Real,'V6:Real,'V7:Real,'V14:Real]
```

Each theory propogates equalities:

```
EqualityProp: 'MATRIX-REAL: => '0Real:Real ?= 'V6:Real
EqualityProp: 'MATRIX-REAL: => '1Real:Real ?= 'V5:Real
EqualityProp: 'REAL:        => 'V6:Real    ?= 'V14:Real
EqualityProp: 'MATRIX-REAL: => 'V5:Real    ?= 'V7:Real
EqualityProp: 'REAL:        => 'A11:Real   ?= 'A21:Real
EqualityProp: 'REAL:        => 'A12:Real   ?= 'A22:Real
EqualityProp: 'MATRIX-REAL: => 'V7:Real    ?= 'V14:Real
```
