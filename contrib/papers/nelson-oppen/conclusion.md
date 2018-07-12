Conclusion & Future work
========================

The examples above have demonstrated the usefulness of Nelson-Oppen combination in Maude. Even so,
the tool is still a prototype. As mentioned previously, the Nelson-Oppen method forms the keystone
of general SMT solving. Other key pieces need to be implemented in Maude for the solver to be
efficient and viable.

For example, in this implementation, prior to purification and to applying the
Nelson-Oppen algorithm, we convert the formula into its DNF form. This can lead to an exponential
blow up in the length of the formula. A more efficient solution would be to take advantage of a boolean
structure by using a SAT solver by extension of the DPLL algorithm [@DPLL]
to the so-called DPLL(T) algorithm [@DPLLT][@krstic2007architecting].

Being a prototype, little effort has been spent on optimization. For example, when working with the
`var-sat` solver, the list of most general unifiers is computed repeatedly at every query to the
solver. Computing this list can be expensive depending on the term and theory under consideration.
For example, in an extreme case checking the satisfiability of the forumala
`{ X:Magma, Y:Magma, Z:Magma } C= { X:Magma } ?= true` using `var-sat` takes tens of minutes. Most
of this time is spend calculating this list of unifiers. If such a formula were to arise in the
course of the Nelson-Oppen algorithm, the list of unifiers may need to be computed several times at
prohibitive cost.

Stable infiniteness requires that the theory has infinite models. However, there are several
important theories that are not stably infinite. For example, the theory of bit vectors
($\Z / 2^n\Z$) can be used to model "machine integers" widely used by many programming languages. In
[@shiny], Tinelli and Zarba showed that this requirement can be reduced to the case where all but
one of the theories is "shiny". Further work by Ranise, Ringeissen and Zarba [@polite], and by
Jovanovi and Barrett [@politerevisited] provided an easier to compute alternative called strongly
"polite" theories. Extending this implementation to handle these cases would greatly expand the
usefulness of these theories.

Work also needs to be done to expand the the implementation to handle more than two theories at a
time, and theories that share a sub-signature, though this work is mostly on the purification front.

In general, one can envision incrementally building up towards a flexible, efficient and powerful
SMT infrastructure in Maude delegating subproblems both to external solvers as well as to tools that leverage the
power and expressiveness of rewriting logic.
