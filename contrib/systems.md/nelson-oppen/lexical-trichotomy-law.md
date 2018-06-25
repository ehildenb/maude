Regarding other examples, here is one that might be interesting but it is
unclear whether we can handle it. One could define in var-sat a parametric
module:

```test
set include Bool off .
load ../../../contrib/tools/meta/nelson-oppen-combination
load smt

fmod PAIR is
  protecting BOOLEAN .
  sort X .      *** parameter sort
  sort Pair .   *** pairs

  op [_,_] : X X -> Pair [ctor] .
  
  op first  : Pair -> X .
  op second : Pair -> X .

  vars x y : X .

  eq first ([x,y]) = x [variant] .
  eq second([x,y]) = y [variant] .

  vars b b' : Boolean .
  
  op if : Boolean Boolean Boolean -> Boolean .

  eq if(true, b,b') = b  [variant] .
  eq if(false,b,b') = b' [variant] .
endfm
```

Then one could instantiate the parameter X with the rational numbers and define
a lexicographic order on pairs of rationals as follows:

```test
fmod PAIR-REAL is
  including PAIR .
  sort Real . 
  subsorts Real < X .
  
  --- Convince var-sat that Real is an infinite sort.
  op fake-zero :      -> Real [ctor] .
  op fake-succ : Real -> Real [ctor] .
endfm
```

```test
fmod PAIR-REAL-INSTANTIATED is
  including REAL .
  including PAIR-REAL .

  vars P Q : Pair .

  op _>lex_ : Pair Pair -> Boolean .
  eq P >lex Q = if( (first(P) > first(Q))
                  , true
                  , if( (first(Q) > first(P))
                      , false
                      , (second(P) > second(Q))
                      )
                  ) .
endfm
```

    op _>lex_ : Pair Pair -> Bool .  *** lexicographic order on pairs

    eq [x,y] >lex [x',y'] = if((x > y),tt,if((y > x),ff,(y > y'))) .

where x \> y is the order on the rationals.

Then one would like to prove the trichotomy law:

```test
set print attribute on .
reduce in NELSON-OPPEN-COMBINATION : nelson-oppen-valid(
     ( tagged(tt, (('mod > 'PAIR-REAL) ; 'check-sat > 'var-sat))
     , tagged(tt, (('mod > 'REAL     ) ; 'check-sat > 'smt-sat))
     ),
         (getTerm(metaReduce(upModule('PAIR-REAL-INSTANTIATED, false), '_>lex_[ 'P:Pair , 'Q:Pair]))) ?= 'true.Boolean
      \/ (getTerm(metaReduce(upModule('PAIR-REAL-INSTANTIATED, false), '_>lex_[ 'Q:Pair , 'P:Pair]))) ?= 'true.Boolean
      \/ 'P:Pair ?= 'Q:Pair
     ) 
     .
     
```

The only part where I am unsure about how we can define this is the function
symbol \>lex since it is unclear to which signature this function symbol
belongs. It does not belong to the parameterized module as such, and it does not
belong to RAT as such. In some tricky sense, the equation:

    eq [x,y] >lex [x',y'] = if((x > y),tt,if((y > x),ff,(y > y'))) .

has the finite variant property, since it can only evaluate to tt, ff, or x \> y

But the module needs to be instantiated before the order operator \> can be
defined.

What seems to be happening is that \>lex is defined in a third signature,
besides those of PAIR and of RAT and we may not have a way to deal with this.

Of course, there would be no problem verifying this for \>lex in var-sat for
pairs of naturals with the order on the naturals, but then Nelson-Oppen would
not be needed.

An alternative approach, used in CS 576 would be to make PAIR parametric on the
theory of total order, and then prove the above trichotomy result for any total
order by instantiating the parameter of total order to the natural numbers,
since I proved that a formula holds for total order if and only if it holds for
the natural numbers order. But again, we would not need to use Nelson-Oppen: all
could be done within var-sat.
