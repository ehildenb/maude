Introduction
============

In 1928, David Hilbert posed the "Entscheidungsproblem" ("the decision problem") to the mathematical
community: a challenge to mechanize mathematics; to find an algorithm that takes as input any
first-order logic statement and return whether it is a true statement or not. Even though, in 1936,
Alan Turing and Alonzo Church independently showed that such an algorithm is impossible, great
progress has been made towards solving significant and profitable subsets of first-order logic
formulae.

Given a theory and a first order logic formula in it's signature, the Satisfiability Modulo Theories
problem is that of deciding whether there is an assigment of variables such that the interpretation
of that forumla holds in some model of that theory. In this case, we say that the forumla is
"satisfiable". Otherwise we say that the formula is "unsatisfiable". Validity, an important related
concept, is the dual of satisfiability. A formula is "valid" in a theory, if in every model of the
theory and for every possible assigment of variables, the formula holds. For example, the statement
"every natural number factorizers uniquely into a set of prime numbers" is valid in Peano
arithmetic, whereas any first order logic formulation of "Peano arithmetic is consistent" in the
theory of Peano arithmetic is not, due to Gödel's Incompleteness Theorems. An algorithmic check
for validity of arbitary formulae was Hilbert's dream and forms the core of SMT solving.

Although, alas, Hilbert's dream is impossible, and in general satisfiability is undecidable
(e.g. for non-linear integer arithmetic), there are subsets of theories that are decidable and
immensely useful for a variety of applications including solving optimization problems, program
verification and automated theorem proving.

Over the years, efficient algorithms were devised for linear real and integer arithmetic, non-linear
arithmetic, arrays (partial functions from the naturals) amongst others, as well as theory-generic
algorithms [@varsat]. Program verification and other applications, however, often involve working
with a combination of two or more theories (e.g. verification of a sorting algorithm may involve
using the combined theory of arrays and of total linear orders). Initially, solving satisfiability
problems in a combination of theories involved manually working out the combined procedure and
proving their correctness [@arith-with-func-symbols][@presburger-array]. In 1979, Greg Nelson and
Derek Oppen published a generic method for composing SMT solvers for two theories into one for the
quantifier free fragment of their union [@nelsonoppen]. In [@shostakcombination], Shostak introduced
a procedure for deciding combinations of "canonizable" and "solvable" theories, called Shostak
theories. Today, most SMT solvers use the Nelson-Oppen algorithm, with refinements for handling
Shostak, shiny, and polite theories, at their core.

In this thesis, we implement in rewriting logic an order-sorted Nelson-Oppen algorithm for composing
satisfiability modulo theory (SMT) solvers for first order theories into an SMT solver for the
quantifier-free fragment of their union. We build on the Tinelli and Zarba's work of extending the
Nelson-Oppen algorithm to order-sorted logics [@tinelliordersorted] and refer to the notes of
Meseguer [@cs576], to implement this algorithm as an order-sorted rewrite theory using the Maude
System.

Implementing this as a rewrite theory is particularly attractive for several reasons. Firstly, the
inference rules translate almost directly into axioms of an equational theory (used as rewriting
rules), making the algorithm much clearer than it would be in, e.g. C++. Secondly, many first order
logic theories can be defined as equational theories with an initial algebra semantics (a subset of
rewrite theories). This, in combination with rewriting logic being a reflective logic allows
implementing theory generic SMT solvers such as Variant-Based Satisfiability [@varsat] and congruence
closure possible. In particular, these solvers have been implemented in Maude [@metalevelvarsat]
taking advantage of reflection through Maude's `META-LEVEL`.

The Maude System is a programming language often used for modeling and verification of systems. It
has been used to verify a wide spectrum of systems, from biological systems (Pathway Logic
[@pathwaylogic]), to Cryptographic Protocols (Maude NPA [@NPA]), to concensus algorithms, to
programming languages (KFramework [@kmaude]), and so on (see [@twentyears] for a comprehensive
survey of such applications). The capabilities of many of these formal verification tools can be
substantially increased through leveraging the power of SMT solvers. Besides the SMT solvers
mentioned previously, Maude also offers access to CVC4 [@cvc4] as well as Yices [@yices2], both state of the
art solvers. While both CVC4 and Yices themselves implement the Nelson-Oppen algorithm internally,
those implementations do not allow cooperation between the algorithms implemented in Maude as rewrite theories, or
other solvers. Thus this implementation of the algorithm can be seen as a first step towards a rich,
robust and extensable ecosystem of cooperating SMT solvers.

