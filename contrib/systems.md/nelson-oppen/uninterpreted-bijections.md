## Unintepreted inverse functions and Reals

In this example, we use Nelson-Oppen combination to prove that the function $f(x) = x^2$ is not invertable

``` {.test}
set include BOOL off .
set print attribute on .
```

```test
fmod COMPLEX is
    --- Convince var-sat that Real is an infinite sort.
    op fake-zero :      -> Real [ctor] .
    op fake-succ : Real -> Real [ctor] .
    vars X : Real .

    op even : Real -> Real .
    op odd  : Real -> Real .
    eq even(-X) =  even(X)      [variant] .
    eq odd (-X) = -odd(X)       [variant] .
   
    op h : Real -> Real .
    eq h(X + Y) = h(X) + h(Y) .
    eq h(X * Y) = h(X) * h(Y) .
endfm

load ../../../contrib/tools/meta/nelson-oppen-combination.maude
set print attribute on .
reduce in VAR-SAT :
   var-valid(upModule('UNINTERPRETED-BIJECTIONS, true),
            ('f['X:Real] ?= 'Y:Real) => ('g['Y:Real] ?= 'X:Real)) .

reduce in VAR-SAT :
   var-valid(upModule('UNINTERPRETED-BIJECTIONS, true),
            ('f['X:Real] ?= 'Y:Real /\ 'f['Z:Real] ?= 'Y:Real)
         => ('X:Real ?= 'Z:Real)) .

reduce in NELSON-OPPEN-COMBINATION :
        simplify(toDNF(toNNF(simplify(~
                      ((   ('f['X:Real] ?= 'Y:Real)
                       => ('Y:Real ?= '_*_['X:Real,'X:Real])
                      )))))) .

reduce in NELSON-OPPEN-COMBINATION :
    nelson-oppen-valid(( tagged(tt, (('mod > 'UNINTERPRETED-BIJECTIONS);  ('check-sat > 'var-sat))) 
                     , tagged(tt, (('mod > 'REAL                    );  ('check-sat > 'smt-sat))))
                    , (   ('f['X:Real] ?= 'Y:Real)
                       => ('Y:Real ?= '_*_['X:Real,'X:Real])
                      ) => ('X:Real != 'X:Real) 
                    ) .
```
