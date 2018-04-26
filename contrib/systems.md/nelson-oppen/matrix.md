Here, we demonstrate Nelson Oppen combination method on $2\times 2$ real matrices.
We define matrices using the following functional module:

```{ .maude .njr-thesis }
fmod MATRIX-X is
    sort X Matrix .
    op matrix : X X X X -> Matrix [ctor] .

    vars A B C D : X .
    op m11 : Matrix -> X .
    op m12 : Matrix -> X .
    op m21 : Matrix -> X .
    op m22 : Matrix -> X .

    eq m11(matrix(A, B, C, D)) = A .
    eq m12(matrix(A, B, C, D)) = B .
    eq m21(matrix(A, B, C, D)) = C .
    eq m22(matrix(A, B, C, D)) = D .
endfm
```

```{ .maude .njr-thesis }
fmod MATRIX-REAL is
    including MATRIX-X .
    sort Real .
    subsorts Real < X .
    --- Convince var-sat that Real is an infinite sort.
    op fake-zero :      -> Real [ctor] .
    op fake-succ : Real -> Real [ctor] .
endfm
```

```{ .maude .njr-thesis }
fmod MATRIX-INTEGER is
    including MATRIX-X .
    sort Integer .
    subsorts Integer < X .
    --- Convince var-sat that Integer is an infinite sort.
    op fake-zero :         -> Integer [ctor] .
    op fake-succ : Integer -> Integer [ctor] .
endfm

```

```{ .maude .njr-thesis }
load ../../contrib/tools/meta/nelson-oppen-combination.maude

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
    eq multiply(A, B) = 'matrix[ mulSum( 'm11[A], 'm11[B], 'm12[A], 'm21[B] )
                                      , mulSum( 'm11[A], 'm12[B], 'm12[A], 'm22[B] )
                                      , mulSum( 'm21[A], 'm11[B], 'm22[A], 'm21[B] )
                                      , mulSum( 'm21[A], 'm12[B], 'm12[A], 'm22[B] )
                                      ] .
    op determinant : Term -> Term .
    eq determinant(A) = '_-_ [ '_*_ [ 'm11[A], 'm22[A] ]
                             , '_*_ [ 'm12[A], 'm21[A] ]
                             ] .

    op identity : Term Term -> Term .
    eq identity(ZERO, ONE) = 'matrix[ ONE, ZERO, ZERO, ONE] .
endfm

```

```maude


--- reduce wellFormed(upModule('XXX, true), mulSum('m11['A:Matrix],'m11['A:Matrix],'m11['A:Matrix],'m11['A:Matrix])) .
--- reduce wellFormed(upModule('XXX, true), multiply('A:Matrix, 'B:Matrix)) .
--- reduce wellFormed(upModule('XXX, true), identity('0:Integer, '1:Integer)) .
--- reduce wellFormed(upModule('XXX, true), determinant('A:Matrix)) .
--- eof .
--- reduce purify(upModule('INTEGER, true), upModule('MATRIX-INTEGER, true)
---              , 'matrix['1:Integer,'0:Integer,'0:Integer,'1:Integer]) .
--- eof .
--- set break on .
--- break select wellFormed .
set print conceal on .
print conceal fmod_is_endfm .
```

``` { .maude .njr-thesis}
---    ceq purify(M, M', Q[TL]) = FV | ((FV ?= T) /\ QFF) if (not Q inO asTemplate(M)) /\ Q inO asTemplate(M')
---                                                       /\ T | QFF := purify(M', M, Q[TL])
---                                                       /\ FV      := joint-variable(M', M, T) .
---
--- reduce (not 'matrix inO asTemplate(upModule('INTEGER, true)))
---    and 'matrix inO asTemplate(upModule('MATRIX-INTEGER, true))
---     .
--- reduce wellFormed(upModule('MATRIX-INTEGER, true),
--- 'matrix['#makeVariable`(_+_`[_*_`[#makeVariable`(m11`[A:Matrix`]`):Integer`,#makeVariable`(m11`[B:Matrix`]`):Integer`]`,_*_`[#makeVariable`(m12`[A:Matrix`]`):Integer`,#makeVariable`(m21`[B:Matrix`]`):Integer`]`]`):Integer,'#makeVariable`(_+_`[_*_`[#makeVariable`(m11`[A:Matrix`]`):Integer`,#makeVariable`(m12`[B:Matrix`]`):Integer`]`,_*_`[#makeVariable`(m12`[A:Matrix`]`):Integer`,#makeVariable`(m22`[B:Matrix`]`):Integer`]`]`):Integer,'#makeVariable`(_+_`[_*_`[#makeVariable`(m21`[A:Matrix`]`):Integer`,#makeVariable`(m11`[B:Matrix`]`):Integer`]`,_*_`[#makeVariable`(m22`[A:Matrix`]`):Integer`,#makeVariable`(m21`[B:Matrix`]`):Integer`]`]`):Integer,'#makeVariable`(_+_`[_*_`[#makeVariable`(m21`[A:Matrix`]`):Integer`,#makeVariable`(m12`[B:Matrix`]`):Integer`]`,_*_`[#makeVariable`(m12`[A:Matrix`]`):Integer`,#makeVariable`(m22`[B:Matrix`]`):Integer`]`]`):Integer]
--- ) .
---
--- reduce purify(upModule('INTEGER, true), upModule('MATRIX-INTEGER, true),
--- 'matrix['_+_['_*_['m11['A:Matrix],'m11['B:Matrix]],'_*_['m12['A:Matrix],'m21['B:Matrix]]],'_+_['_*_['m11['A:Matrix],'m12['B:Matrix]],'_*_['m12['A:Matrix],'m22['B:Matrix]]],'_+_['_*_['m21[
---     'A:Matrix],'m11['B:Matrix]],'_*_['m22['A:Matrix],'m21['B:Matrix]]],'_+_['_*_['m21['A:Matrix],'m12['B:Matrix]],'_*_['m12['A:Matrix],'m22['B:Matrix]]]])
--- .
--- eof
reduce nelson-oppen-sat(( tagged(tt, (('mod > 'MATRIX-INTEGER); 'check-sat > 'var-sat))
                        , tagged(tt, (('mod > 'INTEGER       ); 'check-sat > 'smt-sat))),
          multiply('A:Matrix, 'B:Matrix) ?= identity('0:Integer, '1:Integer)
       /\ determinant('A:Matrix) ?= '2.Integer
       ) .
```
