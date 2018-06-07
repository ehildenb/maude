Conclusion & Future work
========================

The examples above have demonstrated the usefulness of Nelson-Oppen combination in Maude. Even so,
the tool is still a prototype. There are several avenues that need to be explored before fully
exploiting its potential. An obvious generalization is to handle combinations of more than two
theories. One obvious and simple generalization to make is to have the module take more than two
theories. Another is to accept a wider variaty of theories. 

Stable infiniteness requires that the theory has infinite models. However, there are several
important theories that are not stably infinite. For example, the theory of bit vectors
($\Z / 2^n\Z$) can be used to model "machine integers" widely used in many programming languages. In
[shiny], Tinelli and Zarba showed that this requirement can be reduced to the case where all but one
of the theories is "shiny". Further work by Ranise, Ringeissen and Zarba[@polite], and by Jovanovi
and Barrett[@politerevisited] provided an easier to compute alternative called strongly "polite"
theories. Extending this implementation to handle these cases would greatly expand the usefulness of
these theories.

Being a prototype, little effort has been spent on optimization. For example, when working with the
`var-sat` solver, the list of most general unifiers is computed repeatedly at every at every query
to the solver. Computing this list can be expensive depending on the term and theory under
consideration. For example, in an extreme case the term
`{ X:Magma, Y:Magma, Z:Magma } C= { X:Magma }` took tens of minutes to compute.

Another possible optimization is to take advantage of the propositional structure of the formula
through combination with a boolean SAT solver. The DPLL is an algorithm for deciding the
satisfiablilty of propositional logic formulae and forms the basis of most modern efficient solver
[@krstic2007architecting].

In general, one can envision incrementally building up towards a flexible, efficient and powerful
SMT infrastructure in Maude delegating both to external solvers as well as tools leveraging the
power and expressiveness of rewriting logic.
