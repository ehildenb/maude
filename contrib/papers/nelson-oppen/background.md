Satisfiability Modulo Theories (SMT)
------------------------------------

SMT problems are decision problems for checking whether a first-order logic formula $\phi(\vec x)$
is satisfiable in a theory $T$, i.e. whether there is a model $M$ of $T$ such that
$M \models \exists \vec x \phi(\vec x)$. Similarly, a formula is said to be valid if its negation
is unsatisfiable.

Checking satisfiabilty and its dual validity have a wide range of applications, including logistics,
optimization, software verification and program synthesis. In fact validity forms the core of
automated theorem proving. It importance has led to the standardization of a language, SMT-LIB for
describing SMT problems, and the SMT-COMP competition where the foremost solvers compete against
each other for fame. This has created a virtuous cycle where difficult real world SMT problems posed
by industry and academia and added to the benchmarks. The solvers compete at efficiently solving
these problems.

SMT has come a long way since Hilbert posed his problem of "mechanising mathematics". In 1929,
Persberger proved that linear integer arithmetic is indeed decidable, and although it was shown
later by Fischer and Rabin that algorithm must be worst case doubly exponential in the length of
formulae, the Simplex Algorithm and its variations has proven to be an effective method of solving
SMT for both real and integer quantifier free linear arithmetic efficiently.

SMT problems in automated theorem proving and programming verification commonly involve combinations
combined algorithm, and proving that it worked as promised. In 1979, Nelson and Oppen proposed a
combined theories of lists and of total orders. Prior to 1979, this involved manually looking for a
general algorithm for combining SMT solvers into one for the quantifier free fragment of the larger
of standard theories. For example, verifying a sorting algorithm may involve solving queries in the
theory.

<!--
* (1929) Persberger: linear arithmetic is decidable
    * (although the general algorithm was later shown to be worst case doubly exponential on the length of the formula)
      [@Fischer-and-rabin]
    * (Simplex method)
*   define
    *   FOL formula describing constraints over a set of variables, and a theory T deciding if there
        is an assignment of variables
    *   Validity

*   Examples
    *   Linear and non-linear Programming
    *   Boolean satisfiability

*   Motivation
    *   Automated theorem proving
    *   Formal program verification
    *   Optimization problems

*   The importance of SMT to these applications has
    *   led to the industry standardizing on an interface to solvers (the SMT2 format)
    *   There is an annual competition SMT*COMP where implementations compete
*   Quantifier free vs Quantified

-   Historically
    -   Methods for individual theories
    -   Prior to 1979, when wanted combination, had to manually work it out
    -   Nelson-Oppen combination changed that.
        -   Although initially published as a general combination method for any QF FOL theories
            later discovered that there were some basic criteria
        -   Further work generalized the algorithm to work for "Shiney" and "Polite" theories.
        -   Was also modified to work with Order-Sorted Logics
--!>

Maude
-----

The Maude System is a programming language and framework whos semantics are based in Rewriting
Logic. As such, there is little to no representational distance between a rewriting theory and its
implementation in Maude. It is often used for modeling and verification of systems. It has been used
to verify a wide spectrum of systems, from biological systems (Pathway Logic [@pathwaylogic]), to
Network Protocols (Maude NPA [@NPA]), to concensus algorithms, and programming languages (KFramework
[@kmaude]). The capabilities of many of these formal verification tools can be substantially
increased through leveraging the power of SMT solvers. Besides the SMT solvers mentioned previously,
Maude also offers access to the CVC4[@BCD+11] solver as well as Yices2, both industry standard
solvers. While both CVC4 and Yices2 themselves implement the Nelson-Oppen algorithm internally, it
does not allow cooperation between the algorithms implemented in Maude as rewrite theories, or other
solvers. Thus this implementation of the algorithm can be seen as a first step towards a rich,
robust and extensable ecosystem of cooperating SMT solvers.

### Logical Foundations of Maude

The semantics of execution in Maude is based on the initial models of order-sorted rewriting logic
theories. Rewriting logic contains as a sub-logic equational logic. Equational logic is the Horn
logic fragment of order-sorted first-order logic with equality for signatures involving only
function symbols. Rewriting logic defines over the elements of this model a transition system -- a
directed graph over the elements of the initial model of an equational theory. These allow capturing
non-deterministic behaviour in models, which the equational subset does not allow.

#### Unsorted vs Many-Sorted vs Order-Sorted Logics

Traditionally, first order logic has been used in an unsorted setting, i.e. there is a single set
elements in the model that can be quantified over. This can however make
representing some theories cumbersome. For example, in the theory of vector spaces there are two
types of objects that are of interest to us: *vectors* and *scalars*. If we approach this by
defining a signature whose terms can represent either vectors or scalars, along with predicates for
checking whether an element is a vector or a scalar, functions on vectors would become partial. We
could work around this by adding a third "type" of element to represent invalid results for these
functions, but this quickly becomes cumbersome.

[@cs476]

Many sorted logics offer a solution to this. A many sorted signature is a pair $\Sigma = (S, F)$
where $S$ is a set of sorts, and $F$ is a $S^{*}\times S$-indexed set of function symbols
$F = \{F_{a,r} : (a, r) \in S^{*}\times S\}$. If $f \in F_{s_1\times\ldots\times s_n, s}$, we write
$f: s_1\times\ldots\times s_n \to s$. For a many-sorted signature $\Sigma$, a many-sorted
$\Sigma$-algebra, is a pair $(A, \__{A})$, where $A = \{ A_s \}_{s\in S}$ is an $S$ indexed set, and
$\__{A}$ is the interpretation map, mapping each function symbol
$f: s_1\times\cdots\times s_n \to s$ to a function
$f_A : A_{s_1}\times\cdots\times A_{s_n} \to S_s$. Terms, formulae and sentences are defined as they
traditionally are in first order logic. Now, for the theory of vector spaces, we can define a
signature with two sorts: one for vectors and another for scalars and use it to axiomatize vector
spaces consicely.

However, we can do better than many-sorted logic. Take the theory of lists. The head function takes
a non-empty list and returns its first element. But, what happens when the list is empty? What does
the head function return in the case of an empty list? The head function must be partial.
[@stacs] Order-sorted signatures allow formallizing such partiality. An order sorted signature
$\Sigma = ((S, {\le_s}), F)$, where $(S, F)$ is a many-sorted signature, and ${\le_s}$ is a partial
order on the set $S$.  Models of order-sorted theories are order-sorted $\Sigma-$algebras.
For an order-sorted signature $\Sigma$ a order-sorted $\Sigma-$algebra is a pair $(A, \__A)$ where
$A$ is an $S$-indexed set of elements, and

1.  if $s {\le_s} s'$, then $A_s \subset A_{s'}$,
2.  if $a : \emptyset \to s$ and $a : \emptyset \to s'$ where $s$ and $s'$ are in the same connected
    component of sorts under ${\le_s}$,
3.  if $f : s_1 \times \cdots \times s_n \to s$ and $f : s'_1 \times \cdots \times s'_n \to s'$ and
    each $s_i$ and $s'_i$ are in the same connected component (i.e. $f$ is *subsort overloaded*)
    then for each
    $\vec a \in A_{s_1}\times\cdots\times S_{s_n} \intersect A_{s'_1}\times\cdots\times S_{s'_n}$
    $f_{A, s_1 \times \cdots \times s_n} \to s = f_{A, s'_1 \times \cdots \times s'_n \to s'}$
    (i.e. subsort-overloaded functions agree on common elements).
    
In an order-sorted setting, we can define lists with distinct sorted for the empty list and non-empty
lists. The head function can then be defined as a total function with domain non-empty lists.

#### Equational Logic

A *signature* $\Sigma$ is a set of function symbols and their arities. An *equational theory* is a
pair $(\Sigma, E)$, where $E$ is a set of algebraic identities on the terms $\terms$ constructed
from the signature $\Sigma$. For example, the group $\Z_5$ could be described as an equational
theory as follows: $$\Sigma = \{ 0, 1, \_+\_, -\_ \}$$

Note that underscores in the signature indicate holes for subterms, and
thus indicate the arity of the symbol.

$$\begin{aligned}
x + 0                     &= x                     &\quad\quad& \text{Additive Identity}   \\
x + (y + z)               &= (x + y) + z           &\quad\quad& \text{Associativity}       \\
x + y                     &= y + x                 &\quad\quad& \text{Commutativity}       \\
1 +  1 +  1 +  1 + 1      &= 0                     &\quad\quad& \text{Characteristic 5}    \\
x + (-x)                  &= 0                     &\quad\quad& \text{Inverses}            \\
\end{aligned}$$

\colorbox{red}{ XXX: The trivial group also models this. Do we need to some how state that 0 != 1 or that it is the
initial model?}

This equational theory can be implemented as a Maude *functional module* as follows:

```
fmod Z5 is
    sorts Z5 .
    op 0 : -> Z5                              [ctor] .
    op 1 : -> Z5                              [ctor] .
    op _ + _ : Z5 Z5 -> Z5   [assoc comm id: 0 ctor] .
    op   - _ : Z5    -> Z5                           .

    vars x y : Z5                        . --- x and y are variables of sort Z5
    eq 1 + 1 + 1 + 1 + 1 = 0             . --- Characteristic 5
    eq -(x + y)          = (-x) + (-y)   . --- Inverse distribute
    eq (-1)              = 1 + 1 + 1 + 1 . --- Inverse of 1
endfm
```

This program represents an equational theory $E = ((S, \le_S), \Sigma, E \union B)$.
Here, $S = \braces{\tt Z5}\, \le_s = \braces{{\tt NzZ5}, {\tt Z5}}$ and
$\Sigma = \braces{ {\tt 0}, {\tt 1}, {\tt \_ + \_}}$. The `fmod Z5 is ... endfm` construct defines a
*functional module* and describes an equational theory. The signature of this theory has a single
sort `Z5` The `op` declaration defines the terms and functions in the signature of that theory.
These are of the form `op NAME : ARGUMENTS -> RESULT [ATTRIBUTES]`. For example, `_ + _` takes two
terms of sort `Z5` and returns another of the same sort, while `0` and `1` are constants of sort
`Z5`. The `ctor` attribute marks a term as part of the constructor signature of the theory. The
`assoc`, `comm` and `id: 0` attributes mark the plus operator as being associative, commutative and
having `0` as its identity. The `vars` declaration allows using the tokens `x` and `y` as variables
in equations. Each `eq` construct represents an axiom in the equational theory.

Although ordinarily equations in equational theories are symmetric -- in a proof we may replace
equals by equals if a term matches either the left hand side or the right hand side -- equations in
Maude are only applied from left to right. This is to allow defining a terminating execution.
Attributes like `assoc` and `comm` allow specifying common axioms that would otherwise be difficult
to define in a terminating manner (and also make implementation of Maude's matching and unification
algorithms easier and more efficient.) Because of this directionality, the theories must be
*confluent* for them to form a well-defined equational theory. i.e. the application of equations
must yield the same final result irrespective of the order in which they are applied. Although tools
such as the Church-Rosser Checker and the Maude Termination Tool are provided to help check these,
the burden of making sure that functional modules are confluent and terminating is ultimately on the
programmer defining them. This orientation on the equations means that we will sometimes have to
define equations that would otherwise be mathematically deducible. For example, if we had defined
the functional module with the same eqautions as the equational theory, Maude would not have been
able to deduce that $-3 = 2$. However, it is trivial that each set of equations can be derived from
the other. Inspite of this, it can be seen from the example above that the representational distance
between an equational theory and its implementation in Maude very small.

Besides the syntax demonstrated above, Maude also supports conditional equations, i.e. an equation
that holds when some predicate over the term holds, and also an "otherwise" clause -- an equation
that will fire when no other equation holds.

#### Rewriting Logic

A rewrite theory $\mathcal R$ is the triple $(\Sigma, E, R)$, where $(\Sigma, E)$ is an equational
theory and $R$ the set of *one step rewrites* on the terms of the signature.

The rewrite rules $R$ define a relation $\rewrite \subset \terms\times\terms$. This relation is
obtained from the closure of $R$ under *reflexivity*, *$E-$equality* (equality under the set of
axioms $E$), *congruence* (if a subterm rewrites, then the rewrite "lifts" to all terms containing
that subterm; $t \rewrite t' \implies f(\ldots, t, \ldots) \rewrite f(\ldots, t', \ldots)$),
*replacement* (for any substitution $\theta$, $t \rewrite t' \implies t\theta \rewrite t'\theta$)
and *transitivity*.

<!--
-   **Reflexitivity:** For each term $t \in \terms(X)$,
    $$\infer{t \rewrites t} {}$$
-   **$E$-Equality:**
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
--!>

If $x \rewrites y$, we say "$x$ rewrites to $y$".

This relation defines a Kripke structure -- a labeled transition graph over the possible set of
states of a system. Execution of a program in Maude -- reducing a concrete term via the rewrite
relation $\rewrite$ -- involves following the edges of this transition graph and terminates when the
term it arrives at has no outward edges. Maude can also perform symbolic execution, i.e. reduce a
term that has variables, as well as search the strucutre for terms matching a pattern or predicate.
Kripke Structures are commonly used in the implementation of model checking and are the structures
over which Linear and Branching Temporal Logics are defined. Again, this makes the representational
distance between the specification of the model and the data structures we use to reason over it
minimal, making verification of correctness of model checkers and other tools that reason over these
structures easy.

Rewrite theories are defined in Maude through *system modules*. Since we implement the Nelson-Oppen
combination algorithm purely as a functional module, we do not go into the details of the
syntax for system modules here.

#### Reflective logic

Rewriting logic is a *reflective logic* -- its meta theory can be represented at the object level in
a consistent way. i.e. there is a *universal theory* $U$ and a function
$\overline { ( \_ \proves \_ ) }$ such that for any theory $T$,
$T \proves \phi \iff U \proves \overline{ T \proves \phi }$.
This is particularly interesting because it allows us to reason it allows us to implement both the
models we work over and the model checking tools we use in the same langauge. In fact, the implementation
of variant-based satisfiability by Stephen Sherik and of the Nelson-Oppen Combination Algorithm take
advantage of this.

In Maude, the built-in module `META-LEVEL` is used to do this lifting. Terms are represented in the
sort `Term`, and modules in the sort `Module`. The function
`upModule : ModuleExpression Bool -> Module` takes a `ModuleExpression`, a quote followed by the
module name (e.g. `'Z5`) and returns a term representing the module. Similarly, the function
`upTerm : Universal -> Term` takes a term of any sort and returns a meta-term of sort `Term`. Terms
at the meta-level are represented using quoted identifiers. Arguments to terms are placed in a comma
separated list within square brackets. Constants and variables have their sorts annoted as part of
the identifier. For example the term `1 + 1` is represented at the meta level as
`'_+_[ '1.Z5, '1.Z5 ]`, while the variable `X` as `'X:Z5`. Meta-terms can be reduced using the
`metaReduce` function.

`META-LEVEL`'s `upModule` function allows us to lift a theory and perform rewrites with it like any
other term.

[Reflection in General Logics, Rewriting Logic and Maude]:
https://www.sciencedirect.com/science/article/pii/S1571066105825538

### Decision Procedures in Maude

There are a few satisfiability procedures available in Maude, either implemented as in Maude at
the meta level, or external tools made accessible through the C++ API. It these tools
that we shall use as the base solvers for the Nelson-Oppen combination problem.

#### Variant-based Satisfiability

Variant-based satisfiability is a theory-generic procedure that applies to a large set of
user-definable order-sorted signature. The equations of this theory must satisfy the *finite variant
property* and may include axioms such as commutativity, associativity and commutativity or identity.

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
algorithm [@varsat].

Furthermore, if $(\Sigma, E \union B) \supseteq (\Omega, E_{\Omega} \union B_\Omega)$ is a
subsignature of constructors and $(\Omega, E_{\Omega} \union B_\Omega)$ is OS-compact, then
satisfiability of quantifier free formulae in this theory are decidable by variant-based
satisfiablity. This has been implmented in Maude by Sherik and Meseguer[@metalevelvarsat] and will
be used for demonstrating the order-sorted Nelson-Oppen combination method.
Refer to [@varsat] for a more in-depth description.

#### CVC4

CVC4 is an industry-standard automatic theorem prover that supports many theories including rational
and integer linear arithmetic, array, bitvectors and a subset of non-linear arithmetic. Although
CVC4 allows defining algebraic data types it does not allow these user-defined types it does not
allow terms constructed from them to have additional axioms. Since variant based satisfiablilties
[@cvc4]

#### Yices 2

Yices 2 is another industry-standard SMT solver that excels in non-linear real and integer
arithmetic.[@yices2]