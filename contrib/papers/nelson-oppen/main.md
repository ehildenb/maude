---
title: Nelson Oppen Combination as a Rewrite Theory
author: Nishant Rodrigues
department: Mathematics
advisor: Prof. Jose Meseguer
commiteename: Advisor
committee: Prof. Jose Meseguer
degreeyear: 2018
---

\begin{abstract}

Solving Satisfiability Modulo Theories (SMT) problems in a key piece in automating tedious
mathematical proofs. It involves algorithmically solving systems of equalities and disequalities in
a variety of theories, such as linear and non-linear real and integer arithmetic, arrays,
uninterpreted and Boolean algebra. While solvers exist for many such theories or their subsets, SMT
solvers typically use the Nelson-Oppen combination method, an algorithm for producing a solver for
the quantifier free fragment of the combination of a number of such theories via cooperation between
solvers of those theories. Here, we present the Nelson-Oppen algorithm adapted for an order-sorted
setting as a rewriting logic theory. We implement this algorithm in the Maude System and instantiate
it with the theories of real and integer matrices to demonstrate its use in automated theory
proving, and with hereditarily finite sets with reals to show its use with non-convex theories. This
is done using both SMT solvers written in Maude itself via reflection (Variant-based satisfiability)
and using external solvers (CVC4 and Yices2). This work can be considered a first step towards
building a rich ecosystem of cooperating SMT solvers in Maude, that modeling and automated theory
proving tools typically written using the Maude System can leverage.

\end{abstract}

```include
introduction.md
```

```include
background.md
```

```include
no-as-a-rewrite-theory.md
```

# Implementation in Maude

```include
../../tools/meta.md/nelson-oppen-combination.md
```

# Examples

```include
../../systems.md/nelson-oppen/matrix.md
```

```include
../../systems.md/nelson-oppen/hereditarily-finite-set.md
```

```include
conclusion.md
```

# References
