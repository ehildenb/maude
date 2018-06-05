Besides the names of the theories, `nelson-oppen-sat` requires additional information about theories
such as which procedure to use for checking satisfiability. We use "tagged formulae" to represent
this information. For example, the term
`tagged('1.Nat ?= '2.Nat, (('mod > 'NAT), ('check-sat > 'var-sat)))` represents the formula
"$1 = 2$" in the module of `NAT`, and that we should use the `var-sat` procedure to check its
satisfiability. In the implementation in Maude, these tagged formula are represented by the sort
`TaggedFormula` and sets of tagged formulae by the sort `TaggedFormulaSet`. For rewriting logic
variables (not to be confused with variables part of the formula we are rewriting over) of the sort
`TaggedFormula` we use the variables `TF1` and `TF2`, while for `TaggedFormulaSet` we use `TFS`.

```maude
load foform.maude
load cterms.maude
load ../varsat/var-sat.maude
load smt.maude

fmod TAGGED-FOFORM is
    protecting FOFORM .
    sort TaggedFormula TaggedFormulaSet Tags Tag .
    subsorts Tag < Tags .
    subsorts TaggedFormula < TaggedFormulaSet .

    op tagged : FOForm? Tags -> TaggedFormula .
    op empty : -> Tags [ctor] .
    op _ ; _ : Tags Tags     -> Tags [ctor assoc comm id: empty] .
    op _ > _ : Qid  Qid      -> Tag . --- TODO Genneralize to QidSet?

    op empty : -> TaggedFormulaSet [ctor] .
    op _,_ : TaggedFormulaSet TaggedFormulaSet -> TaggedFormulaSet [ctor comm assoc id: empty] .
endfm
```

```maude
fmod FOFORM-VARIABLES is
    protecting FOFORM .
    protecting VARIABLESET .

    vars PHI PHI' : FOForm .
    vars T1 T2 : Term .
    vars TL : TermList .
    vars X : Variable .

    op vars : FOForm   -> VariableSet .
    op vars : Term     -> VariableSet .
    op vars : TermList -> VariableSet .
    ----------------------------------
    eq vars(mtForm) = none .
    eq vars(tt)     = none .
    eq vars(ff)     = none .
    eq vars(PHI /\ PHI') = vars(PHI) ; vars(PHI') .
    eq vars(PHI \/ PHI') = vars(PHI) ; vars(PHI') .
    eq vars(T1 ?= T2) = vars(T1) ; vars(T2) .
    eq vars(T1 != T2) = vars(T1) ; vars(T2) .
    eq vars((T1, NETL:NeTermList)) = vars(T1) ; vars(NETL:NeTermList) .
    eq vars(Q:Qid[TL]) = vars(TL) .
    eq vars(X) = X .
    eq vars(T1) = none [owise] . --- TODO: Doable without owise?
endfm
```

```maude
fmod FOFORM-TO-SMT is
    protecting META-TERM .
    protecting FOFORM .

    vars FO1 FO2 : FOForm .
    vars T1  T2  : Term   .

    op foform-to-smt : FOForm -> Term .
    op foform-to-smt : Term   -> Term .
    ---------------------------------------------------------------------------------
    eq foform-to-smt(tt)         = 'true.Boolean .
    eq foform-to-smt(FO1 /\ FO2) = '_and_  [foform-to-smt(FO1), foform-to-smt(FO2)] .
    eq foform-to-smt(FO1 \/ FO2) = '_or_   [foform-to-smt(FO1), foform-to-smt(FO2)] .
    eq foform-to-smt(T1  ?=  T2) = '_===_  [foform-to-smt(T1),  foform-to-smt(T2)]  .
    eq foform-to-smt(T1  !=  T2) = '_=/==_ [foform-to-smt(T1),  foform-to-smt(T2)]  .
    eq foform-to-smt('tt.Bool*)  = 'true.Boolean       .
    eq foform-to-smt('ff.Bool*)  = 'false.Boolean      .
    eq foform-to-smt(T1)         = T1          [owise] .
endfm
```

```maude
fmod NO-CHECK-HELPER is
    protecting FOFORM .
    protecting TAGGED-FOFORM .
    protecting FOFORM-TO-SMT .
    protecting FOFORMSIMPLIFY .

    protecting VAR-SAT .

    op check-valid           : TaggedFormula -> Bool .
    op check-sat             : TaggedFormula -> Bool .
    op $check-sat.dnf        : TaggedFormula -> Bool .
    op $check-sat.print      : ModuleExpression FOForm Bool -> Bool .

    var ME   : ModuleExpression .
    var PHI  : FOForm           .
    var SAT? : Bool             .
    vars TS  : Tags             .

    --- strictNot gets stuck when it cannot evaluate further. This prevents
    --- `valid` from returning true if `sat` gets stuck
    op strictNot        : Bool -> Bool .
    op $strictNot.error : Bool -> Bool .
    ------------------------------
    eq strictNot(true)   = false .
    eq strictNot(false)  = true  .
    eq strictNot(B:Bool) = $strictNot.error(B:Bool) [owise print "----- what is? " B:Bool]  .

    op smt-sat : ModuleExpression FOForm -> Bool .
    eq smt-sat(ME, PHI)
     = metaCheck([ME], foform-to-smt(PHI))
     .

    eq check-valid(tagged(PHI, TS)) = strictNot(check-sat(tagged(~ PHI, TS))) .
    eq check-sat  (tagged(PHI, TS)) = $check-sat.dnf       (tagged(simplify(PHI), TS)) .

    eq $check-sat.dnf       (tagged(PHI, ('mod > ME); ('check-sat > 'var-sat); TS))
     = $check-sat.print(ME, PHI, var-sat(upModule(ME, true), PHI))
---    [print "var-sat? " ME ": " PHI]
     .

    eq $check-sat.dnf       (tagged(PHI, ('mod > ME); ('check-sat > 'smt-sat); TS))
     = $check-sat.print(ME, PHI, smt-sat(ME, PHI))
---    [print "smt-sat? " ME ": " PHI]
     .

    eq $check-sat.print(ME, PHI, SAT?) = SAT?
---    [print "check-sat: " ME ": " PHI " is " SAT? ]
     .

    eq smt-sat(ME, PHI) = metaCheck([ME], foform-to-smt(PHI))
     .
endfm
```

```maude
fmod NELSON-OPPEN-COMBINATION is
    protecting NO-CHECK-HELPER .
    protecting FOFORM-DEFINEDOPS .
    protecting FOFORMSET .
    protecting FOFORMSIMPLIFY . protecting DNF . protecting NNF .
    protecting PURIFICATION .
    protecting TAGGED-FOFORM .
    protecting FOFORM-VARIABLES .

    vars MCONJ1 MCONJ2 : Conj? .
    vars CONJ PHI1 PHI2 : Conj .
    vars PHI : QFForm .
    vars CANDEQ DISJ?1 DISJ?2 : Disj? .
    vars M1 M2 : Module .
    vars ME1 ME2 : Qid . --- TODO: Wierd, Qids are a subsort of ModuleExpr s not the other way around
    vars TFS : TaggedFormulaSet .
    vars TF1 TF2 : TaggedFormula .
    vars TS1 TS2 : Tags .
    vars X  X1  X2  : Variable .
    vars XS XS1 XS2 : VariableSet .
    vars T1 T2 : Term . var TL : TermList .

--- Tag a conjunction of wellFormed atoms into TaggedFormula. Atoms in the
--- intersection of multiple theories are copied into each tag.
--- TODO: Ill formed formulae are silently ignored.

    op tagWellFormed           : TaggedFormulaSet EqConj? -> TaggedFormulaSet .
    op tagWellFormed.hasConvex : TaggedFormulaSet EqConj? -> TaggedFormulaSet .
    op $tagWellFormed.filter   : ModuleExpression EqConj? -> EqConj? .
    -----------------------------------------------------------------------------
    eq tagWellFormed(TFS, CONJ) = tagWellFormed.hasConvex(addConvexTag(TFS), CONJ) .
    eq tagWellFormed.hasConvex(empty, CONJ) = empty .
    eq tagWellFormed.hasConvex((tagged(PHI1, ('mod > ME1); TS1 ), TFS), CONJ)
     = ( tagged(PHI1 /\ $tagWellFormed.filter(ME1, CONJ), ('mod > ME1) ; TS1)
       , tagWellFormed(TFS, CONJ)) .
    eq $tagWellFormed.filter(ME1, A:EqAtom /\ MCONJ2)
     = if A:EqAtom in upModule(ME1, true) then A:EqAtom /\ $tagWellFormed.filter(ME1, MCONJ2)
                                    else             $tagWellFormed.filter(ME1, MCONJ2)
                                    fi
     .
    eq $tagWellFormed.filter(ME1, mtForm) = mtForm .

    op addConvexTag : TaggedFormulaSet -> TaggedFormulaSet .
    --------------------------------------------------------
    eq addConvexTag((tagged(PHI1, ('convex > _:Qid ) ; TS1 ), TFS))
     = tagged(PHI1, ('convex > _:Qid ) ; TS1 ), addConvexTag(TFS)
     .
    eq addConvexTag((tagged(PHI1,                      TS1 ), TFS))
     = tagged(PHI1, ('convex > 'false) ; TS1 ), addConvexTag(TFS)
     [owise] .
    eq addConvexTag(empty) = empty .

    op in-module : Module VariableSet -> VariableSet .
    eq in-module(M1, X1 ; XS) = if wellFormed(M1, X1)
                                then X1
                                else none
                                fi ; in-module(M1, XS) .
    eq in-module(M1, none) = none .

    op var-intersect : VariableSet VariableSet -> VariableSet .
    -------------------------------------------------------
    eq var-intersect(X1 ; XS1, X1 ; XS2) = X1 ; var-intersect(XS1, XS2) .
    eq var-intersect(XS1, XS2)           = none [owise] .
```

The `nelson-oppen-sat` function that implements the algorithm, takes as input
a `TaggedFormulaSet` and a quantifier free formula (of sort `QFForm`)
and returns a `Bool`.

```{.maude .njr-thesis}
    op nelson-oppen-sat    : TaggedFormulaSet QFForm                 -> Bool .
```

```maude
    op $nosat.dnf          : TaggedFormulaSet QFForm                 -> Bool .
    op $nosat.purified     : TaggedFormulaSet EqConj                 -> Bool .
    op $nosat.tagged       : TaggedFormulaSet                        -> Bool .
    op $nosat.basicSat     : TaggedFormulaSet                        -> Bool .
    op $nosat.ep           : TaggedFormulaSet PosEqDisj              -> Bool .
    op $nosat.split        : TaggedFormulaSet PosEqDisj              -> Bool .
    op $nosat.split.genEqs : TaggedFormulaSet PosEqDisj PosEqDisj    -> Bool .
    --------------------------------------------------------------------------
```

Given a quantifier free formula `PHI` in the set of theories `TFS` (each tagged with information
regarding covexitivity, and information about which procedure to use for checking sat), we first
convert it to the disjunctive normal form (DNF) and simplify it (e.g. $\bot \land \phi$ becomes
$\bot$).

```{ .maude .njr-thesis }
    eq nelson-oppen-sat(TFS, PHI)
     = $nosat.dnf(TFS, simplify(toDNF(toNNF(simplify(PHI))))) .
```

The algorithm then considers each disjunction separately.

```{ .maude .njr-thesis }
    eq $nosat.dnf(TFS, CONJ \/ PHI)
     =  $nosat.dnf(TFS, CONJ) or-else $nosat.dnf(TFS, PHI) .
```

We then purify each disjunction into a disjunction of "pure" atoms each wellformed in the signature
of one of the theories, and tagged with the appropriate information.

```{ .maude .njr-thesis }
   ceq $nosat.dnf(TFS , CONJ)
     = $nosat.purified(TFS, purify(ME1, ME2, CONJ))
    if    ( tagged(tt, ('mod > ME1); TS1)
          , tagged(tt, ('mod > ME2); TS2))
       := TFS
     .
    eq $nosat.purified(TFS, CONJ)
     = $nosat.tagged(tagWellFormed(TFS, CONJ)) .
```

Next, we make sure each of the tagged formulae (`TF1`, `TF2`) are satisfiable on their own.

```{ .maude .njr-thesis }
    eq $nosat.tagged((TF1, TF2))
     = check-sat(TF1) and-then check-sat(TF2) and-then $nosat.basicSat(TF1, TF2)
       [print "Purified:\n\t" TF1 "\n\t" TF2]
     .
```

From the set of shared variables $\SharedVariables := \vars(\phi_1) \intersect \vars(\phi_2)$ we
define a set of candidate equalities.

$$\CandidateEqualities := \{ x_i = y_i | x_i, y_i \in \SharedVariables_{s_i}, x_i \not\equiv y_i \}$$

where $\SharedVariables_{s_i}$ is the subset of shared variables in the connected component of sort $s_i$.

```{ .maude .njr-thesis }
   ceq $nosat.basicSat(TFS)
     = $nosat.ep( TFS
                , candidate-equalities(in-module(moduleIntersect(ME1, ME2), vars(PHI1) ; vars(PHI2)))
                )
    if ( tagged(PHI1, ('mod > ME1); _1:Tags)
       , tagged(PHI2, ('mod > ME2); _2:Tags))
       :=  TFS
     .
```

```maude
    op candidate-equalities : VariableSet -> PosEqDisj .
    op candidate-equalities : Variable VariableSet VariableSet -> PosEqDisj .
    ---------------------------------------------------------------------
    eq candidate-equalities(X ; XS1) = candidate-equalities(X, XS1, XS1) .
   ceq candidate-equalities(X, X1 ; XS1, XS2)   = X ?= X1 \/ candidate-equalities(X, XS1, XS2) if     getType(X) == getType(X1) .
   ceq candidate-equalities(X, X1 ; XS1, XS2)   =            candidate-equalities(X, XS1, XS2) if not getType(X) == getType(X1) .
    eq candidate-equalities(X, none, X2 ; XS2)  = candidate-equalities(X2, XS2, XS2) .
    eq candidate-equalities(X, none, none)      = ff .
```

Next, we apply the equality propagation inference rule. If any identification of variables is
implied by a theory, we propagate that identification to the other theories.

$$\infer[\text{Equality Propagation}]
  { \begin{matrix*}[l]
          & \CheckSat(\phi_j \land \phi_E \land x_m = x_n) \\
    \land & \NelsonOppenSat(\phi_1 \land \phi_2 \land \phi_E \land x_m = x_n, \CandidateEqualities \setminus \{x_m = x_n\})
    \end{matrix*}
  }
  { x_m = x_n \in \CandidateEqualities
  & T_i \models \phi_i \and \phi_E \implies x_m = x_n
  & \NelsonOppenSat(\phi_1 \land \phi_2 \land \phi_E, \CandidateEqualities)
  }
$$

```{ .maude .njr-thesis }
   ceq $nosat.ep(( tagged(PHI1, ('mod > ME1); TS1)
                 , tagged(PHI2, ('mod > ME2); TS2)), X1 ?= X2 \/ CANDEQ)
     =          check-sat(tagged(PHI2 /\ X1 ?= X2, ('mod > ME2); TS2))
       and-then $nosat.ep(( tagged(PHI1 /\ X1 ?= X2, ('mod > ME1); TS1)
                          , tagged(PHI2 /\ X1 ?= X2, ('mod > ME2); TS2))
                         , CANDEQ)
    if check-valid(tagged(PHI1 => (X1 ?= X2), ('mod > ME1); TS1)) [print "EqualityProp: " ME1 ": => " X1 " ?= " X2 ] .
```

If, after checking each identification individually, there are none that are implied we apply the split
rule.

```{ .maude .njr-thesis }
    eq $nosat.ep(TFS, CANDEQ) = $nosat.split(TFS, CANDEQ) [owise print "=== Split? " TFS CANDEQ ] .
```

If there are no variables left to identify, then we are satisfiable

```{ .maude .njr-thesis }
    eq $nosat.split(TFS, mtForm) = true .
```

However, if there some disjunction of identifications implied and we are in a non-convex theory, we
"split". i.e. we try each of the possible identification left in turn and see if atleast one of them
is satisfiable.

```{ .maude .njr-thesis }
   ceq $nosat.split(TFS, CANDEQ)
     = $nosat.split.genEqs(TFS, CANDEQ, CANDEQ)
    if    ( tagged(PHI1, ('mod > ME1) ; ('convex > 'false) ; TS1)
          , tagged(PHI2, ('mod > ME2) ;                      TS2))
       := TFS
    /\ check-valid(tagged((PHI1) => (CANDEQ), ('mod > ME1); ('convex > 'false) ; TS1))
                                            [print "===== Split : "  ME1 " : " PHI1 " => " CANDEQ ]
     .
```

Otherwise, since there are no implied identifications and the theories
are stably-infinite, the equation is satisfiable.

```{ .maude .njr-thesis }
    eq $nosat.split(TFS, CANDEQ) = true [owise] .
```

We use `$nosat.split.genEqs` to generate this disequality of sat problems.

$$\infer[\text{Split}]
  {\Or_{x_m = x_n \in \CandidateEqualities}
   \left(\begin{matrix*}[l]
        &      &\CheckSat(\phi_1 \land \phi_E \land x_m = x_n) \\
        &\land & \CheckSat(\phi_2 \land \phi_E \land x_m = x_n) \\
        &\land & \NelsonOppenSat(\phi_1 \land \phi_2 \land \phi_E, \CandidateEqualities \setminus \{x_m = x_n\})
   \end{matrix*}\right)
  }
  {
  & T_i \models \phi_i \and \phi_E \implies \And \CandidateEqualities
  & \NelsonOppenSat(\phi_1 \land \phi_2 \land \phi_E, \CandidateEqualities)
  }
$$

```{ .maude .njr-thesis }
    eq $nosat.split.genEqs((tagged(PHI1, ('mod > ME1); TS1), tagged(PHI2, ('mod > ME2); TS2))
                          , X1 ?= X2 \/ DISJ?1, X1 ?= X2 \/ DISJ?2)
     = (          check-sat(tagged(PHI1 /\ X1 ?= X2, ('mod > ME1); TS1))
         and-then check-sat(tagged(PHI2 /\ X1 ?= X2, ('mod > ME2); TS2))
         and-then $nosat.ep(( tagged(PHI1 /\ X1 ?= X2, ('mod > ME1); TS1)
                            , tagged(PHI2 /\ X1 ?= X2, ('mod > ME2); TS2))
                           , DISJ?2)
       )
       or-else $nosat.split.genEqs(( tagged(PHI1, ('mod > ME1); TS1)
                                   , tagged(PHI2, ('mod > ME2); TS2))
                               , DISJ?1, X1 ?= X2 \/ DISJ?2)
     .

    eq $nosat.split.genEqs(( tagged(PHI1, ('mod > ME1); TS1)
                           , tagged(PHI2, ('mod > ME2); TS2))
                       , mtForm, DISJ?2)
     = false
     .
endfm
```
