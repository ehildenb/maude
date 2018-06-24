Conclusion & Future work
========================

The examples above have demonstrated the usefulness of Nelson-Oppen combination in Maude. Even so,
the tool is still a prototype. As mentioned previously, the Nelson-Oppen method forms the keystone
of general SMT solver. Other key pieces need to be implemented in Maude for the solver to be
efficient and viable.

For example, in this implementation, prior purification and to applying the
Nelson-Oppen algorithm, we conver the formula into its DNF form. This can lead to an exponential
blow up in the length of the formula. A more efficient solution would be to take advantage of a boolean
structure by using a SAT solver, for example the DPLL algorithm [@DPLL][@krstic2007architecting].

Being a prototype, little effort has been spent on optimization. For example, when working with the
`var-sat` solver, the list of most general unifiers is computed repeatedly at every at every query
to the solver. Computing this list can be expensive depending on the term and theory under
consideration. For example, in an extreme case the term
`{ X:Magma, Y:Magma, Z:Magma } C= { X:Magma }` took tens of minutes to compute.

Stable infiniteness requires that the theory has infinite models. However, there are several
important theories that are not stably infinite. For example, the theory of bit vectors
($\Z / 2^n\Z$) can be used to model "machine integers" widely used in many programming languages. In
[shiny], Tinelli and Zarba showed that this requirement can be reduced to the case where all but one
of the theories is "shiny". Further work by Ranise, Ringeissen and Zarba[@polite], and by Jovanovi
and Barrett[@politerevisited] provided an easier to compute alternative called strongly "polite"
theories. Extending this implementation to handle these cases would greatly expand the usefulness of
these theories.

Work also needs to be done to expand the the implementation to handle more than two theories, and
theories that share a sub-signature, though this work is mostly on the purification front.

In general, one can envision incrementally building up towards a flexible, efficient and powerful
SMT infrastructure in Maude delegating both to external solvers as well as to tools that leverage the
power and expressiveness of rewriting logic.
