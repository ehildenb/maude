Order Sorted Nelson Oppen as a rewrite theory
=============================================

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

Conditions on the theories
--------------------------

For the Nelson Oppen method to be viable, the order-sorted theories must meet the following conditions:

1.  They must be stably infinite
2.  They must be optimally intersecting.

Stably Infinite

:   Let $T$ be an order-sorted first-order theory with signature $\Sigma = ((S, \le), F, P)$ and
    $s_1, s_2,\ldots s_n \in S$. Let $\F \subset \FO(\Sigma)$, be the set of first order formulae in
    $\Sigma$

    $T$ is stably infinite in sorts $s_1, s_2,\ldots s_n$ for $\F-$satisfiability iff every
    $T-$satisfiable formula $\phi \in \F$, is also satisfiable in a model
    $\mathcal B = (B, \__B) \in \model(T)$ such that $|B_{s_i}| \ge \chi_0, 1 \le i \le n$.

    Intuitively, it means that we can always find models of both theories where the cardinalities
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

Overview
--------

Given two order-sorted, optimally intersecting, stably-infinite theories $T_1$ and $T_2$ with
signatures $\Sigma_1$ and $\Sigma_2$ each with decision procedures for quantifier free
$T_i$-satisfiability we want to derive a decision procedure for quantifier free $T_1 \union T_2$
satisfiability.

We can transform any formula $\phi$ into an *equisatisfiable* formula in disjunctive normal form.
Further, for each atom in such a formula we can apply "purification" to obtain a formula where each
atom is in the signature of one of the two theories.

Now, our task has become to find a model $M_0$ and an assignment $a: \vars(\phi) \to M_0$ such that
$M_0, a \models \purified$, where $\Sigma_0$ is the intersection of the two signatures. In general,
this requires knowing the semantics of each of the theories, but in the case of stably infinite
theories, the task is easier. With stable infiniteness, since every satisfiable formula has an
infinite model, if we need a value distinct from a witness we have infinite choices for either the
value or the witness. i.e. $$T_1 \union T_2 \not\models \phi \land \vec t_k \ne \vec t'_k 
\iff T_i \models \phi \limplies \vec t_k = \vec t'_k$$ This means that we do not need to find a
specific arrangement, but just a satisfiable equivalence relation of the shared variables. For any
formula, $\purified$, we have an equisatisfiable formula
$\Or_{equiv\in \Equiv(\SharedVariables)}\{ \purified \land \phi_{\equiv}\}$, where
$\Equiv(\SharedVariables)$ is the set of partitions on the shared variables $\SharedVariables$ and
$\phi_\equiv$ is the formula defining this equivalence relation. Since for stably infinite theories
we only care about the equivalence class being satisfiable, we can project this formula onto each of
the theories and check satisfiability.

\begin{figure}
$$\begin{matrix*}[l]
                    &    &                        &     & T_{\union}  \models& \QF(\Sigma_1 \union \sigma_2)                                              \\
\text{DNF}          &\iff&                        &     & T_{\union}  \models& \And \Lit(\Sigma_1 \union \sigma_2)                                        \\
\text{Purification} &\iff&                        &     & T_{\union}  \models& \And \Lit(\Sigma_1) \land \And\Lit(\sigma_2)                                \\
\text{Arrangement}  &\iff&                        &     & T_{\union}  \models& \Or ( \And \Lit(\Sigma_1) \land \And\Lit(\sigma_2) \land \And \phi_{\equiv} )\\
\text{Projection}   &\iff& \exists \phi_{\equiv}, &     & T_1 \models        & \And \Lit(\Sigma_1) \land \And \phi_{\equiv}                                \\
                    &    &                        &\text{and} & T_2 \models        & \And \Lit(\Sigma_2) \land \And \phi_{\equiv}
\end{matrix*}$$
\caption{Equisatisfiable formulae transformations on stably infinite theories we use for NO}
\end{figure}

Inference rules for Order-Sorted Nelson-Oppen
---------------------------------------------

Note that up to this point, we have only found a mathematically sound way of finding satisfiability
but do not yet have a viable efficient algorithm. Checking each equivalence class for satisfiability
is infeasable as the number of equivalence classes grows exponentially with the number of variables,
even in the order sorted case where we can restrict ourselves to equivalences capatable with the
sort structure.

Instead we choose a Darwinian approach, pruning classes of equivalences from the search space if
an identification of a single pair of variables implied by one theory is not satisfiable in another.
For the non-convex case, if any theory implies the disjunction of all remaining identifications
we branch our search, checking if at least one of the remaining identifications is satisfiable.

We can think of each step of the algorithm as splitting the search space into subsets where
a single additional identification holds. If only a single identification is implied equality propagation
causes the algorithm to decend into it. Otherwise, the split rule checks the satisfiability of each of the sets
in the split subspaces, and decends into each satisfiable one.

