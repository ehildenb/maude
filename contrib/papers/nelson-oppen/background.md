Satisfiability Modulo Theories (SMT)
------------------------------------

SMT problems are decision problems for checking whether a first-order logic formula $\phi(\vec x)$
is satisfiable in a theory $T$, i.e. that there is a model $M$ of $T$ such that
$M \models \exists \vec x \phi(\vec x)$.

Given a theory and a first order logic formula in it's signature, the Satisfiability Modulo Theories
decision problem is of deciding whether there is an assigment of variables such that the
interpretation of that forumla holds in some model of that theory. In this case, we say that the
forumla is "satisfiable". Otherwise we say that the formula is "unsatisfiable". Validity, an
important related concept, is the dual of satisfiability. A formula is "valid" in a theory, if in
every model of the theory, there is no assignment of variables that satisfies the formula. For
example, the statement "every natural number factorizers uniquely into a set of prime numbers" is
valid, whereas the first order logic statement "Peano arithmetic is consistent", in the theory of
Peano arithmetic, is not (by Godel's Second incompleteness theorem).

https://excape.cis.upenn.edu/documents/ClarkBarrettSlides.pdf

The concept of "validity" is central to automated reasoning, and although Church and Turing have
showed that the search for a general algorithm for checking validity (i.e. "automating mathematics")
is futile,

- effective quantifier elemenation
- (1929) Persberger: linear arithmetic is decidable
    - (although the general algorithm was later shown to be worst case doubly exponential on the length of the formula)
      [@Fischer-and-rabin]
    - (Simplex method)
- 

.   define
    .   FOL formula describing constraints over a set of variables, and a theory T deciding if there
        is an assignment of variables
    .   Validity

-   Examples
    -   Linear and non-linear Programming
    -   Boolean satisfiability

-   Motivation
    -   Automated theorem proving
    -   Formal program verification
    -   Optimization problems
-   The importance of SMT to these applications has
    -   led to the industry standardizing on an interface to solvers (the SMT2 format)
    -   There is an annual competition SMT-COMP where implementations compete
-   Quantifier free vs Quantified

-   Historically
    -   Methods for individual theories
    -   Prior to 1979, when wanted combination, had to manually work it out
    -   Nelson-Oppen combination changed that.
        -   Although initially published as a general combination method for any QF FOL theories
            later discovered that there were some basic criteria
        -   Further work generalized the algorithm to work for "Shiney" and "Polite" theories.
        -   Was also modified to work with Order-Sorted Logics

Maude
-----

The Maude System is a programming and framework commonly used for modelling and reasoning about
these models. Systems modelled using Maude include Biological Systems (Pathway Logic), Network
Protocols (Maude NPA), Concensus Algorithms, and Programming Languages (K Framework).

Maude's 

-   Applications
    -   as a theorem prover
    -   model checker
-   Modelling and verification
    -   Biological (pathway logic)
    -   Network Protocols (NPA)
    -   Concensus algorithms
    -   Programming languanges (KFramework)
-   Why does Maude take well to this
    -   Based in rewriting
    -   Rewriting can be modeled as Kripke structs same as what model checkers use
-   These tools can leverage SMT
    -   So having a strong SMT architecture makes it Awesome Sauce
-   reflective

-   Maude's semantics based on
    -  Initial model
    -  Unsorted / Many Sorted / Order sorted logics
    -  Equational Logic
    -  Rewriting logic

Logical Foundations of Maude
----------------------------


Maude is based on two logics, one contained in the other. 

The first, equational logic, is the Horn logic fragment of first-order logic with equality for
signatures involving only function symbols. The second, rewriting logic, is defined by transitions
between equivalence classes of terms defined by an equational theory.

### Equational Logic

A *signature* $\Sigma$ is a set of function symbols and their arities.
An *equational theory* is a pair $(\Sigma, E)$, where $E$ is a set of
algebraic identities on the terms $\terms$ constructed from the
signature $\Sigma$. For example, the group $\Z_5$ could be described as
an equational theory as follows: $$\Sigma = \{ 0, 1, \_+\_, -\_ \}$$

Note that underscores in the signature indicate holes for subterms, and
thus the arity of the symbol.

$$
\begin{aligned}
x + (y + z)               &= (x + y) + z &\quad\quad& \text{Associativity}       \\
x + y                     &= y + x       &\quad\quad& \text{Commutativity}       \\
x + (-x)                  &= 0           &\quad\quad& \text{Inverses}            \\
-(x + y)                  &= (-x) + (-y) &\quad\quad& \text{Inverse distributes} \\
(1 + (1 + (1 + (1 + 1)))) &= 0           &\quad\quad& \text{Modulo 5}            \\
\end{aligned}$$

The expressiveness of Equational Logic can be tuned by allowing
signatures to carry more or less information. In *many-sorted*
equational logic, each function symbol is also attached to a list of
_argument sorts_ $s_1, s_2, \ldots, s_n$, and a _result sort_ $s$.
Thus, in a many sorted equational theory, the signature is the $(S, \Sigma)$ 
where $S$ is a set of sorts, and $\Sigma$ is a set of function symbols with
argument and result sorts in $S$. For example, a vector space would have
sorts `Vector` and `Scalar`.

Further, by adding a partial order $<$ on the sort symbols, we get the more
expressive order-sorted equational logic. If $s_i < s_j$ we say that $s_i$ is
a subsort of $s_j$. For example, we could have `Integer < Rational < Real`.

### Rewriting Logic

A rewrite theory $\mathcal R$ is the triple $(\Sigma, E, R)$, where
$(\Sigma, E)$ is an equational theory and $R$ the set of *one step
rewrites* on the terms of the signature.
The rewrite rules describe a relation $\rewrite \subset \terms\times \terms$.
The sentences that $\mathcal R$ proves are obtained from the finite application of the following
rules:

-   **Reflexitivity:** For each term $t \in \terms(X)$,
    $$\infer{t \rewrites t} {}$$
-   **Equality:**
    $$\infer{ u' \rewrites v'}{ u \rewrites v & E \proves u = u' & E \proves v = v'}$$
-   **Congruence:** For each $f : k_1 \cdots k_n \to k$ in $\Sigma$, and
    $t_i, t_i' \in \terms(X), 1 \le i \le n$,
    $$\infer{f(t_1,\ldots, t_n) \rewrites f(t'_1, \ldots, t'_n)}{t_1 \rewrites t'_1 & \cdots & t_n \rewrites t'_n}$$
-   **Replacement:** For each rule $r : t \rewrites t'$ in $R$, with
    $\vars(t) \union \vars(t') = \{x_1, \ldots x_n\}$, and for each
    substitution,
    $\theta : \{x_1, \ldots x_n\} \longrightarrow \terms(X)$ with
    $\theta(x_1) = p_l$ with $\theta(x_l) = p_l, 1 \le l \le n$, then:
    $$\infer{\theta(t) \rewrites \theta'(t')}
            { p_1 \rewrites p'_1 & \cdots & p_n \rewrites p'_n}$$
-   **Transitivity:**
    $$\infer{ t_1 \rewrites t_3 } {t_1 \rewrites t_2 & t_2 \rewrites t_3 }$$

If $x \rewrites y$, we say "$x$ rewrites to $y$". 

For a more in-depth look at rewriting logic, see [@twentyears].

Maude
-----

The programming language, Maude, is based on rewriting logic.
A program in Maude is a rewrite theory, and a computation is a deduction based
on the inference rules above. A basic introduction to Maude follows. For a more
thorough guide see [@maudemanual].

An equational theory is specified as a _functional modules_:

```
fmod Z5 is
    protecting BOOL .

    sorts Z5 .
    op 0 : -> Z5                              [ctor] .
    op 1 : -> Z5                              [ctor] .
    op _ + _ : Z5 Z5 -> Z5   [assoc comm id: 0 ctor] .
    op _ * _ : Z5 Z5 -> Z5   [assoc comm id: 1     ] .

    vars x y : Z5 .

    --- Characteristic 5
    eq 1 + 1 + 1 + 1 + 1 = 0     .

    --- Define multiplication in terms of repeated multiplication
    eq x * 0 = 0                 .
    eq x * (1 + y) = x + (x * y) .
endfm
```

This program represents an equational theory $E = ((S, \le_S), \Sigma, E \union B)$. Here,
$S = \braces{\tt Z5}\, \le_s = \braces{{\tt NzZ5}, {\tt Z5}}$ and
$\Sigma = \braces{ {\tt 0}, {\tt 1}, {\tt \_ + \_}}$.
The `fmod Z5 is ... endfm` construct defines a *functional module* and describes an equational
theory. The signature of this theory has a single sort `Z5` The `op` declaration defines the
terms and functions in the signature of that theory. These are of the form
`op NAME : ARGUMENTS -> RESULT [ATTRIBUTES]`. For example, `_ + _` takes two terms of sort `Z5` and
returns another of the same sort, while `0` and `1` are constants of sort `Z5`. The `ctor` attribute
marks a term as part of the constructor signature of the theory. The `assoc`, `comm` and `id: 0`
attributes mark the plus operator as being associative, commutative and having `0` as its identity.
The `vars` declaration allows using the tokens `x` and `y` as variables in equations. Each `eq`
construct represents an axiom in the equational theory.

The `protecting BOOL` declaration includes the `BOOL` equational theory as a subtheory. Programmers
must be careful not to alter the semantics of protected theories, e.g. by adding an equation
`eq true = false .`

Although ordinarily equations in equational theories are symmetric -- in a proof we may replace
equals by equals if a term matches either the left hand side or the right hand side -- equations in
Maude are only applied from left to right. This is to allow defining a terminating execution.
Attributes like `assoc` and `comm` allow specifying common axioms that would be difficult to define
in a non-terminating way (and also make implementation of Maude's matching and unification
algorithms easier and more efficient.) Because of this directionality, the theories must be
*confluent* for them to form a well-defined equational theory. i.e. the application of equations must yield
the same final result irrespective of the order in which they are applied. Although tools such as
the Church-Rosser Checker and the Maude Termination Tool are provided, the burden of defining
confluent and terminating functional modules is ultimately on the programmer defining them. The
orientation on the equations means that we will sometimes have to define equations that would
otherwise be mathematically deducible.

Defining a program as above means that there is an extremely small representational distance between
the programs and the mathematical logic we use to reason about them.

### Maude Meta Level

Rewriting logic is a *reflective logic* -- its meta theory can be represented at the object level in
a consistent way. i.e. there is a *universal theory* $U$ and a function
$\overline { ( \_ \proves \_ ) }$ such that for any theory $T$,
$T \proves \phi \iff U \proves \overline{ T \proves \phi }$.

In Maude, the built-in module `META-LEVEL` is used to do this lifting. Terms are represented in the
sort `Term`, and modules in the sort `Module`. The function
`upModule : ModuleExpression Bool -> Module` takes a `ModuleExpression`, a quote follows by the
module name (e.g. `'Z5`) and returns a term representing the module. Similarly, the function
`upTerm : Universal -> Term` takes a term of any sort and returns a meta-term of sort `Term`.
Terms at the meta-level are represented using quoted identifiers. Arguments to terms are
placed in a comma separated list within square brackets. Constants and variables have
their sorts annoted as part of the identifier. For example the term `1 + 1` is represented
at the meta level as `'_+_[ '1.Z5, '1.Z5 ]`, while the variable `X` as `'X:Z5`.
Meta-terms can be reduced using the `metaReduce` function.

`META-LEVEL`'s
`upModule` function allows us to lift a theory and perform rewrites with
it like any other term.

[Reflection in General Logics, Rewriting Logic and Maude]:
https://www.sciencedirect.com/science/article/pii/S1571066105825538

### Decision Procedures in Maude

Some satisfiability procedures have been been implemented in Maude using the `META-LEVEL`. We will
use these solvers as the subsolvers for Nelson-Oppen.

#### Variant-based Satisfiability

Variant-based satisfiability is a theory-generic procedure that applies to
a large set of user-definable order-sorted signature. The equations of this
theory must satisfy the *finite variant property* and may include axioms such as
commutativity, associativity-commutativity or identity.
Refer to [@varsat] for a more in-depth description.

Let $T = (\Sigma, E \union B)$ where the equations $E$ are confluent, terminating and $B$-coherent
modulo axioms. A $E,B-$variant of a term $t$ is a pair $(u, \theta)$ such that
$u =_B (t\theta)!_{\vec E,B}$, where for any term $u$, $u!_{\vec E, B}$ denotes the fully simplified
term obtained by exhaustive simplification with the oriented equations $\vec E$ modulo $B$. Given
variants $(u, \theta)$ and $(v, \gamma)$ of $t$, $(u, \theta)$ is more general than $(v, \gamma)$
iff there is a substitution $\rho$ such that:

1. $\theta\rho =_B \gamma$ and
2. $u\rho =_B v$

A theory $T$ has the finite variant property (FVP) iff for each term $t$ there is a finite most
general complete set of variants. If a theory $(\Sigma, E\union B)$ is FVP and $B$ has a finitary
$B-$unification algorithm, then folding variant narrowing gives a finitary $E\union B$-unification
algorithm [@XXX].

Furthermore, if $(\Sigma, E \union B) \supseteq (\Omega, E_{\Omega} \union B_\Omega)$ is a subsignature of constructors and
$(\Omega, E_{\Omega} \union B_\Omega)$ is OS-compact,
then satisfiability of quantifier free formulae in this theory are decidable by variant-based
satisfiablity. This has been implmented in Maude by Sherik and Meseguer[@metalevelvarsat]
and will be used for demonstrating the order-sorted Nelson-Oppen combination method.

#### Congruence Closure

#### CVC4

CVC4 is an industry-standard automatic theorem prover for SMT problems that supports many theories
including rational and integer linear arithmetic, array, bitvectors and a subset of non-linear
arithmetic. Although CVC4 allows defining algebraic data types it does not allow these user-defined
types to have equations over them. Thus its power can clearly be augmented by combination with
variant-based satisfiability.

#### Yices 2
