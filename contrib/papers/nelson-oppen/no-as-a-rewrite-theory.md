Order Sorted Nelson Oppen as a rewrite theory
=============================================

Given decision procedures for the quantifier free formulae in a set of theories the Order-Sorted
Nelson-Oppen combination method gives us a decision procedure for the quantifier free fragment in
the combination of these theories, provided that the theories are *disjoint*, *stably infinite* for their shared
sorts and *optimally intersecting*. In theories stably infinite in a set of sorts, we can find models for each theory
such that the cardinalities of the carrier sets of those sorts match.

Stably Infinite

:   Let $T$ be an order-sorted first-order theory with signature $\Sigma = ((S, \le), F, P)$ and
    $s_1, s_2,\ldots s_n \in S$. Let $\F \subset \FO(\Sigma)$, be the set of first order formulae in
    $\Sigma$

    $T$ is stably infinite in sorts $s_1, s_2,\ldots s_n$ for $\F-$satisfiability iff every
    $T-$satisfiable formula $\phi \in \F$, is also satisfiable in a model
    $\mathcal B = (B, \__B) \in \model(T)$ such that $|B_{s_i}| \ge \chi_0, 1 \le i \le n$.

    For Nelson-Oppen combinations, requiring that both theories $T_1$ and $T_2$ are stably infinite
    intuitively means that we can always find models of both theories where the cardinalities
    of sorts $s_1, \ldots, s_n$ agree.

Notation: For sort $s$ and signature $\Sigma_i$, let $[s]_i$ denote it's
connected component of sorts in $\Sigma_i$

Optimally intersectable [@cs576]

:   The order-sorted signatures $\Sigma_1$ and $\Sigma_2$ are optimally
    intersectable iff:

    1.  **Functions and predicates sorts agree:** For each
        $f \in \fun(\Sigma_1) \intersect \fun(\Sigma_2)$ (resp,
        $p \in \pred(\Sigma_1) \intersect \pred(\Sigma_2)$),
        $\exists \{i, j\} \in \onetwo$ such that:

        -   $F_i(f) = F_j(f) \intersect ([s_1]_i\times\cdots\times [s_m]_i) \times [s_i]$
            (resp
            $P_i(p) = P_j(p) \intersect ([s_1]_i\times\cdots\times [s_m]_i)$

        -   $[s_l] \subset [s_l]_j, 1 \le l \le n$, and
            $[s]_i \subset [s]_j$ (resp.
            $[s_l]_i \subset [s_l]_j, 1 \le l \le n$).

    2.  **Intersection is a single component:** For every sort
        $s \in \SharedSorts$, we have
        $[s]_1 \intersect S_2 = [s]_2 \intersect [s]_1 = [s]_1\intersect [s]_2$

    3.  and, for any two sorts $s_i \in S_i$ and $s_j \in S_j$ any one
        of:

        i.  **Intersection is empty**:
            $[s_i]_i \intersect [s_j]_j = \emptyset$

        ii. **Intersection is the top sort of one component:**
            $[s_i]_1 \intersect [s_j]_2 = \{s_0\}$, where $s_0$ is the
            top-sort of at least one of the connected components.

        iii. **Once component is subsumed in the other:**

             a.  $\exists k \in \onetwo$ and $[s_k]_k$ has a top sort,
                 $[s_k]_k \subset [s_l]_l$ $\{k, l\} = \onetwo$.
             b.  $\le_k \intersect [s_k] = \le_l \intersect [s_k]_2^2$
             c.  (downward closure):
                 $\forall s \in [s_l]_l, \forall s' \in [s_k]_k, s\le_l s' \implies s\in [s_k]_k$


Given two order-sorted, optimally intersecting, stably-infinite theories $T_1$ and $T_2$ with
disjoint signatures $\Sigma_1$ and $\Sigma_2$ each with decision procedures for quantifier free
$T_i$-satisfiability we want to derive a decision procedure for quantifier free $T_1 \union T_2$
satisfiability. We can transform any formula $\phi$ into an *equisatisfiable* formula in disjunctive
normal form. Further, for each atom in such a formula we can apply "purification" to obtain a
formula where each atom is in the signature of one of the two theories.

Now, our task has become to find a $T_1 \union T_2$-model $M_0$ and an assignment
$a: \vars(\phi) \to M_0$ such that such that $M,a \models \varphi$. How can we decompose this
satisfiability problem into similar subproblems for the theories $T_{1}$ and $T_{2}$? What follows
summarizes more detailed arguments in [@cs576] about the order-sorted Nelson-Oppen combination.
Because of the stably infinite assumptions on the theories $T_{1},T_{2}$, as well as the assumption
that the corresponding signatures are optimally intersectable and disjoint, if $\varphi$ is purified
into an equisatisfiable conjunction $\varphi_{1} \wedge \varphi_{2}$ of formulas $\varphi_{1}$ and
$\varphi_{2}$ in the theories $T_{1}$ and $T_{2}$, we can always choose $M$ so that the shared sorts
in $M$ have infinite cardinalities, so that $M$ is the \emph{amalgamation} of models $M_1$ and $M_2$
of theories $T_{1}$ and $T_{2}$ that satisfy $\varphi_{1}$ and $\varphi_{2}$, and that have the same
cardinality for the shared sorts. The interesting question is the converse one: under what
conditions, given models $M_1$ and $M_2$ with same cardinality in shared sorts, and assignments
$a_{1}$ and $a_{2}$ such that $M_{i},a_{i} \models \varphi_{i}$, $1 \leq i \leq 2$ can we amalgamate
$M_1$ and $M_2$ into a single $T_{1} \cup T_{2}$-model $M$ with an assignment $a$ such that
$M,a \models \varphi$? The answer is that such an amalgamation $M$ and an assignment $a$ extending
both $a_{1}$ and $a_{2}$ after a suitable bijective identification of the sets for the shared sorts
will exist if and only if $a_{1}$ and $a_{2}$ generate the same equivalence relation among the
variables of $\varphi_{1}$ and $\varphi_{2}$ that have shared sorts. Therefore, since the
satisfaction of any such equivalence relation can be characterized by a conjuction $\psi$ of
equalities and disequalities among shared variables (called an "arrangement"), a "naive"
order-sorted Nelson-Oppen algorithm amounts to finding such a $\psi$ among all possible equivalence
relations such that $\varphi_{i}\wedge \psi$ is $T_{i}$-satisfiable, $1 \leq i \leq 2$.

The question now becomes how do we find such a arrangement of variables? Checking each equivalence
class for satisfiability is infeasable as the number of equivalence classes grows exponentially with
the number of variables, even in the order sorted case where we can restrict ourselves to
equivalences capatable with the sort structure of the signatures (e.g. we cannot have an equality
between a boolean and an integer variable). Instead of checking each of the possible partitions on
the shared variables, we choose a Darwinian approach, pruning classes of equivalences from the
search space if an identification of a single pair of variables implied by one theory is not
satisfiable in another (equality propagation). In the case of non-convex theories, we may have
$\phi \limplies (x_1 = y \lor x_2 = y)$ without either $\phi \limplies x_1 = y$ or
$\phi \limplies x_2 = y$ individually holding. Thus if any theory implies the disjunction of all
remaining identifications we branch our search, checking if at least one of the remaining
identifications is satisfiable (split). We can think of each equality propagation step of the
algorithm as pruning the search space (of arrangements) of unsatisfiable ones, and the split step
dividing the search space into a smaller groups where the split step can apply. The inference rules
for the Equality Propagation and Split rules are given in \ref{fig:proofrules} where $\phi_{E}$
denotes the equalities between variables with shared sorts obtained so far by previous inference
steps, and $\CandidateEqualities$ denotes the still uncommitted equalities between such shared
sorts. These rules are similar to rules presented in [@mannaCombining] for the unsorted case and
have a similar proof of correctness.

\begin{figure}
\footnotesize
$$\infer[\text{Equality Propagation}]
{ \begin{matrix*}[l]
        & \CheckSat(\phi_j \land \phi_E \land x_m = x_n) \\
  \land & \NelsonOppenSat(\phi_1 \land \phi_2 \land \phi_E \land x_m = x_n, \CandidateEqualities \setminus \{x_m = x_n\})
  \end{matrix*}
}
{ x_m = x_n \in \CandidateEqualities
& T_i \models (\phi_i \land \phi_E) \limplies x_m = x_n
& \NelsonOppenSat(\phi_1 \land \phi_2 \land \phi_E, \CandidateEqualities)
}
$$
$$\infer[\text{Split}]
{\Or_{x_m = x_n \in \CandidateEqualities}
 \left(\begin{matrix*}[l]
      &      &\CheckSat(\phi_1 \land \phi_E \land x_m = x_n) \\
      &\land & \CheckSat(\phi_2 \land \phi_E \land x_m = x_n) \\
      &\land & \NelsonOppenSat(\phi_1 \land \phi_2 \land \phi_E, \CandidateEqualities \setminus \{x_m = x_n\})
 \end{matrix*}\right)
}
{
& T_i \models (\phi_i \land \phi_E) \limplies \Or \CandidateEqualities
& \NelsonOppenSat(\phi_1 \land \phi_2 \land \phi_E, \CandidateEqualities)
}
$$
\caption{Inference rules for the Nelson-Oppen algorithm}
\label{fig:proofrules}
\end{figure}

