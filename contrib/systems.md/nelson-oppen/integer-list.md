Combining Integers with Lists
-----------------------------

\newcommand \head {\text{head}}

This example demonstrates the behavior of Nelson-Oppen when one of the theories
involved is not convex. We apply the algorithm on lists parameterized over
the natural numbers. While lists are convex, the natural numbers are not.

```test
load ../../../contrib/tools/meta/nelson-oppen-combination.maude
```

```{.test .njr-thesis}
fmod INTEGER-LIST is
    protecting BOOLEAN .
    sort Integer .

    --- Convince var-sat we are infinite
    op fake-0 :         -> Integer [ctor] .
    op fake-s : Integer -> Integer [ctor] .

    sort IntegerList NeIntegerList .
    subsort Integer < NeIntegerList < IntegerList .
    op _ _ :    NeIntegerList NeIntegerList -> NeIntegerList [ctor assoc] .
    op nil : -> IntegerList [ctor] .

    op head : NeIntegerList -> Integer .
    var N : Integer . var L : NeIntegerList .
    eq head(N)   = N [variant] .
    eq head(N L) = N [variant] .
endfm
```

```test
fmod TEST-NO-SMT-LIST is
    protecting REAL-INTEGER .
    protecting INTEGER-LIST .
    protecting NELSON-OPPEN-COMBINATION .
    protecting META-LEVEL .
endfm

reduce nelson-oppen-sat(( tagged(tt, ('mod > 'INTEGER-LIST); ('check-sat > 'var-sat))
                        , tagged(tt, ('mod > 'INTEGER     ); ('check-sat > 'smt-sat))),
           '_-_ [ '_*_ [ '2.Integer , 'head[ 'L:NeIntegerList ] ]
                , '_*_ [ '2.Integer , 'head[ 'M:NeIntegerList ] ] ]
        ?= '1.Integer)
     == false
     .
reduce nelson-oppen-sat(( tagged(tt, ('mod > 'INTEGER-LIST); 'check-sat > 'var-sat)
                        , tagged(tt, ('mod > 'INTEGER     ); 'check-sat > 'smt-sat)),
           '_-_ [ '_*_ [ '2.Integer , 'head[ 'L:NeIntegerList ] ]
                , '_*_ [ '2.Integer , 'head[ 'M:NeIntegerList ] ] ]
        ?= '0.Integer)
     == true
     .

reduce nelson-oppen-sat(( tagged(tt, ('mod > 'INTEGER-LIST); 'check-sat > 'var-sat)
                        , tagged(tt, ('mod > 'INTEGER     ); 'check-sat > 'smt-sat)),
           '_-_ [ 'head[ 'L:NeIntegerList ]
                , '_*_ [ '2.Integer , 'head[ 'M:NeIntegerList ] ] ]
        ?= '0.Integer)
     == true
     .
```

We demonstrate the algorithm with the formula:
$1 \le \head(L) \le 2
\land x = 1 \land x \ne \head(L)
\land y = 2 \land y \ne \head(L)$ .

``` {.test .njr-thesis}
reduce nelson-oppen-sat(( tagged(tt, (('mod > 'INTEGER-LIST); 'check-sat > 'var-sat))
                        , tagged(tt, (('mod > 'INTEGER     ); 'check-sat > 'smt-sat))),
          '_<=_ [ '1.Integer , 'head[ 'L:NeIntegerList ] ] ?= 'true.Boolean
       /\ '_<=_ [ 'head[ 'L:NeIntegerList ] , '2.Integer ] ?= 'true.Boolean
       /\ 'X:Integer ?= '1.Integer /\ 'X:Integer != 'head[ 'L:NeIntegerList ]
       /\ 'Y:Integer ?= '2.Integer /\ 'Y:Integer != 'head[ 'L:NeIntegerList ])
    == false
    .
```

