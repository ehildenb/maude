Combining Integers with Lists
-----------------------------

\newcommand \head {\text{head}}

In this example we demonstrate the Nelson-Oppen combination where one of the theories involved is
not convex and the split rule needs to be applied to get the correct result. Here, lists are a
convex theory, but the integers with order are now.

```test
load ../../../contrib/tools/meta/nelson-oppen-combination.maude
```

We implement the lists as a theory that has the finite variant property and use variant based
satisfiability to decide its formulae. Here, the head function returns the first element of the
list.

```{.test .njr-thesis}
fmod INTEGER-LIST is
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

For the integers, we use one of the external SMT solvers, CVC4 for checking satisfiability.

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

We use Nelson-Oppen to mechanically prove that
$1 \le \head(L) \land
\head(L) \le 2
\limplies \head(L) = 1 \lor \head(L) = 2$

``` {.test .njr-thesis}
reduce nelson-oppen-valid(( tagged(tt, (('mod > 'INTEGER-LIST); 'check-sat > 'var-sat))
                          , tagged(tt, (('mod > 'INTEGER     ); 'check-sat > 'smt-sat))),
           (  '_<=_ [ '1.Integer , 'head[ 'L:NeIntegerList ] ] ?= 'true.Boolean
           /\ '_<=_ [ 'head[ 'L:NeIntegerList ] , '2.Integer ] ?= 'true.Boolean
           )
        => (  'head[ 'L:NeIntegerList ] ?= '1.Integer
           \/ 'head[ 'L:NeIntegerList ] ?= '2.Integer
           )
       ) .
```

