## Unintepreted inverse functions and Reals

In this example, we use Nelson-Oppen combination to prove that the function $f(x) = x^2$ is not invertable

``` {.test}
set include BOOL off .
set print attribute on .
```

```test
fmod UNINTERPRETED-BIJECTIONS is
    sort Real .
    --- Convince var-sat that Real is an infinite sort.
    op fake-zero :      -> Real [ctor] .
    op fake-succ : Real -> Real [ctor] .
    vars X : Real .

    op f : Real -> Real [ctor] .
    op g : Real -> Real [ctor] .
    eq f(g(X)) = X [variant] .
    eq g(f(X)) = X [variant] .
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
```
