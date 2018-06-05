The theory `UNINTERPRETED-FUNCTION` has a single free function symbol `f : Integer -> Integer`.

https://www.cs.upc.edu/~oliveras/TDV/NO.pdf

```maude
load ../../contrib/tools/meta/nelson-oppen-combination.maude

fmod UNINTERPRETED-FUNCTION is
    sort Integer .
    op fake-zero : -> Integer [ctor] .
    op f : Integer -> Integer [ctor] .
endfm

set print attribute on .
```

We run the Nelson Oppen Algorithm with the formula $f(f(x) - f(y)) = a \land f(x) = a + 1 \land x = y \land x = 0$.

```{.maude .njr-thesis}
reduce in NELSON-OPPEN-COMBINATION :
       nelson-oppen-sat(
                        ( tagged(tt, (('mod > 'INTEGER)               ; 'check-sat > 'smt-sat))
                        , tagged(tt, (('mod > 'UNINTERPRETED-FUNCTION); 'check-sat > 'var-sat))
                        ),
         'f['_-_['f['X:Integer], 'f['Y:Integer]]] ?= 'A:Integer
      /\ 'f['Z:Integer] ?= '_+_['A:Integer, '2.Integer]
      /\ 'X:Integer ?= 'Y:Integer
      /\ 'Z:Integer ?= '0.Integer
     )
    .

```

\newcommand \addition[2] {#1 + #2}
\newcommand \subtraction[2] {#1 - #2}
\newcommand \f[1] {f(#1)}

This purifies to:

1. $\phi_{INTEGER} :=  n_{1} = \addition{a}{2}
                 \land n_{2} = \subtraction{n_{3}}{n_{4}}
                 \land n_{5} = a
                 \land 0 = z
                 \land x = y$
2. $\phi_{UF} :=  n_1 = \f{z}
            \land n_{5} = a
            \land n_{5} = \f{n_{2}}
            \land n_{3} = \f{x}
            \land n_{4} = \f{y}
            \land x = y$

<!-- -->

1. The implication: $T_{UF} \models \phi_{UF} \implies N_{3} = N_{4}$
   propogating this equation $T_{INTEGER}$.
   $$N_{1} = \f{Z}  \land N_{5} = A  \land N_{5} = \f{N_{2}}  \land N_{3} = \f{X}  \land N_{4} = \f{Y}  \land X = Y => N_{3} = N_{4}$$
2. The implication $T_{INTEGER} \models \phi_{INTEGER} \implies N_{2} = Z$
   causes $N_{2} = Z$ to be propagated.
   $$\addition{A}{2}  \land N_{2} = \subtraction{N_{3}}{N_{4}}  \land N_{5} = A  \land N_{3} = N_{4}  \land 0 = Z  \land X = Y => N_{2} = Z$$

3. The implication $T_{UF} \models\phi_{UF} \implies N_{1} = N_{5}$ to be propagated.

   $$N_{1} = \f{Z}  \land N_{2} = Z  \land N_{5} = A  \land N_{5} = \f{N_{2}}  \land N_{3} = N_{4}  \land N_{3} = \f{X}  \land N_{4} = \f{Y}  \land X = Y => N_{1} = N_{5}$$

4. Since this is unsatisfiable in $T_{INTEGER}$ (since we have $N_{1} = N_{5}$ and $N_{1} = \addition{A}{2} \land \land N_{5} = A$)
