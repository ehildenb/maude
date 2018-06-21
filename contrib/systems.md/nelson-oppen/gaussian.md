```test
fmod GAUSIAN-INT is
    sorts Integer Gausian .
    subsorts Integer < Gausian .
    
    vars G : Gausian .

    op -_ : Integer -> Integer .
    op -_ : Gausian -> Gausian .
    
    op i : Gausian -> Gausian [ctor] .
    eq i(i(G)) = -(G) .
endfm

load ../../../contrib/tools/meta/nelson-oppen-combination.maude


--- If two product of two integers are the same, then they are no-coprime 
reduce in NELSON-OPPEN-COMBINATION :
    nelson-oppen-valid(
            ( tagged(tt, (('mod > 'INTEGER);     ('check-sat > 'smt-sat)))
            , tagged(tt, (('mod > 'REAL);        ('check-sat > 'smt-sat)))
            ),
                (   (  '_divisible_[ 'P:Integer , 'A:Integer ] ?= 'true.Boolean
                    /\ 'A:Integer != '0.Integer
                    /\ 'P:Integer != '0.Integer
                    )
                 => (  ( 'A:Integer ?= '1.Integer )
                    \/ ( 'A:Integer ?= 'P.Integer )
                )   )
             => (   ( '_divisible_[ '_*_[ 'X:Integer
                                        , 'Y:Integer
                                        ]
                                  , 'P:Integer
                                  ] ?= 'true.Boolean
                    /\ 'X:Integer != '0.Integer
                    /\ 'Y:Integer != '0.Integer
                    )
                 => (  '_divisible_[ 'X:Integer , 'P:Integer ] ?= 'true.Boolean
                    \/ '_divisible_[ 'Y:Integer , 'P:Integer ] ?= 'true.Boolean
                    )
                )
           )
           .
             
                   
```


```
fmod COMPLEX-POLYNOMIAL is
    sorts Real Imaginary Complex .
    subsorts Real Imaginary < Complex .
    
    op -_ : Integer -> Integer .
    op -_ : Imaginary -> Imaginary .
    op -_ : Complex -> Complex .
    
    op i : Complex   -> Complex [ctor]
    op i : Real      -> Imaginary .
    op i : Imaginary -> Real .
    eq i(i(Z)) = -Z .

    op x : -> Complex .
endfm
```
