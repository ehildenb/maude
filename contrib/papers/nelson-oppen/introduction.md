Introduction
============

In this thesis, we implement in rewriting logic the order-sorted Nelson-Oppen algorithm for
composing satisfiability modulo theory (SMT) solvers for first order theories into an SMT solver for
the quantifier-free fragment of their union.

SMT problems are decision problems for checking whether a first-order logic formula $\phi(\vec x)$
is satisfiable in a theory $T$, i.e. that there is a model $M$ of $T$ such that
$M \models \exists \vec x \phi(\vec x)$. Although in general SMT is undecidable (e.g. for
non-linear integer arithmetic), there are subsets of theories that are decidable and immensely
useful for a variety of applications including solving optimization problems, program verification
and automated theorem proving.

Over the years, efficient algorithms were devised for linear real and integer arithmetic, non-linear
arithmetic, arrays (partial functions from the naturals) amongst others, as well as theory-generic
algorithms[@sat-modulo-nothing][@sat-modulo-ac]. Program verification and other applications,
however, often involve working with a combination of two or more theories (e.g. verification of a
sorting algorithm may involve using the combined theory of arrays and of total linear orders).
Initially, solving satisfiability problems in a combination of theories involved manually working
out the combined procedure and proving their correctness[@XXX]. In 1979, Greg Nelson and Derek Oppen
published a generic method for composing SMT solvers for two theories into one for the quantifier
free fragment of their union [@nelson-oppen]. Today, most SMT solvers use the Nelson-Oppen algorithm
at their core.

In this work, building on the Tenelli's work on extending the algorithm to order-sorted logics
[@tinelli-order-sorted] and refering to the notes of Meseguer [@cs576], we implement this algorithm
as an order-sorted rewrite theory using the Maude System.

Implementing this as a rewrite theory is particularly attractive for several reasons. Firstly, the
inference rules translate almost directly into axioms of the equational theory making the algorithm
much clearer than it would be in, e.g. C++. Secondly, many first order logic theories can be defined
as equational theories (a subset of rewrite theories). This, in combination with, rewriting logic
being a reflective logic allows implementing theory generic SMT solvers such as Variant-Based
Satisfiability[@var-sat] and congruence closure possible. In particular, these solvers have been
implemented in Maude[@skerik-var-sat] taking advantage of reflection through Maude's `META-LEVEL`.

The Maude System is a programming language whos semantics are based in Rewriting Logic. As such,
there is little to no representational distance between a rewriting theory and its implementation in
Maude. It is often used for modeling and verification of systems. It has been used to verify a wide
spectrum of systems, from biological systems (Pathway Logic [@pathwaylogic]), to Network Protocols
(Maude NPA [@NPA]), to concensus algorithms, and programming languages (KFramework [@kmaude]). The
capabilities of many of these formal verification tools can be substantially increased through
leveraging the power of SMT solvers. Besides the SMT solvers mentioned previously, Maude also offers
access to the CVC4[@BCD+11] solver as well as Yices2, both industry standard solvers. While both
CVC4 and Yices2 themselves implement the Nelson-Oppen algorithm internally, it does not allow
cooperation between the algorithms implemented in Maude as rewrite theories, or other solvers. Thus
this implementation of the algorithm can be seen as a first step towards a rich, robust and
extensable ecosystem of cooperating SMT solvers.
