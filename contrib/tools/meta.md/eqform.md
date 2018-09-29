Equational Formulas
===================

For theorem proving purposes with equational logic, there are essentially three kinds of quantifier free formulas that we care about:

1.  true
2.  false
3.  any formula without true, false, or negation

This follows because:

-   any formula with negation is equivalent to one without negation
-   any formula without negation is equivalent to a formula in one of the three cases above by evaluating away true and false

This library provides formula sorts to cover these core cases and more:

4.  `True`     - the sort of the formula in case [1] above
5.  `False`    - the sort of the formula in case [2] above
6.  `TruthLit` - the sort of formulas in cases [1-2] above
7.  `EqLit`    - positive (`_?=_`) or (`_!=_`) negative equality literals
8.  `EqForm`   - the sort of formulas in case [3] above
9.  `NormForm` - the sort containing all formulas in cases [1-3] above
10. `Form`     - the sort of all quantifier-free equational formulas

This library also provides other formula sorts that are needed for programming purposes:

11. `NegTruthLit` - (~ ... ~ true) or (~ ... ~ false)
12. `NoTrueForm`  - Formulas over False + EqLit + NegTruthLit
13. `NoFalseForm` - Formulas over True + EqLit + NegTruthLit

Q:  Why are these seemingly weird sorts needed?

A:  Because true and false are actually _identities_ for the operators
    `/\` and `\/` respectively. Sorts [12] and [13] allow us to recurse properly
    inside a `/\` or `\/` operator respectively because they forbid their
    respective identities from appearing. If we forget to use them and use
    variables of sort Form instead, with very high probability, the code
    will fail to terminate.

    On the other hand, the fact that `/\` and `\/` are also sets allows us to
    extract certain subformulas directly using pattern matching alone. When
    we are performing operations over these operators as sets, we use true
    and false as the set identity operators respectively.

    See the NNF module below to see how to traverse over formulas as TREES.
    See the NEF, DNF, and CNF modules below for more SET-like examples.
    Note that NEF stands for Normal Equational Form. It converts formulas
    of type Form into type NormForm.

Style Notes
-----------

-   Name variables of different formula sorts consistently for readability,
    e.g. 'C'X for NoTrueForm variables safe for conjunctions,
         'D'X for NoFalseForm variables safe for disjunctions, or
         'N'X for NormForm variables
-   Think about if using TREE-like traversal or SET-like traversal makes more
    sense for your application as your write code
-   Be sure to use a variable appropriate for the kind of iteration you will
    doing
-   Prefer working with normalized formualas (of sort NormForm) unless you
    have an explicit reason not to do so
-   Prefer the functions nef, cnf, or dnf defined respectively in the modules
    NEF, CNF, and DNF below for converting formulas to type NormForm unless
    you have a specific application in mind

Additional Data Structures
--------------------------

This library provides additional data structures beyond just formulas.
Currently, this list includes:

-   Lists of formulas
-   Sets of formulas

Note that, due to preregularity requirements, the number of sorts grows
in a multiplicative fashion every time one applies a functor to the
formula data strcuture. For this reason, to avoid generating many unneeded
sorts, we only generate functor liftings for:

A.  Subsorts of EqForm (12 sorts in total)
B.  NormForm
C.  Form

In practice, these are the primary cases that we care about, due to the
considerations mentioned above.

Note: many data functors (such as list and set) generate a unique sort for
non-empty and possibly empty data structures, so that we actually generate
2 * N + 1 additional sorts each time, where the extra sort comes from a
base sort which helps ensure preregularity in complex cases (this gives
29 sorts generated for both the list and set functor assuming the 14 above).

```maude
load terms.maude
load ../meta/meta-aux.maude

set include BOOL off .

fmod EQFORM-IMPL{X :: TRIV} is
  pr BOOL .
  --- Sorts For Formulas Over Equalities
  sort PosEqLit{X} PosEqConj{X} PosEqDisj{X} PosEqForm{X} .
  sort NegEqLit{X} NegEqConj{X} NegEqDisj{X} NegEqForm{X} .
  sort EqLit{X}    EqConj{X}    EqDisj{X}    EqForm{X}    .

  --- Sorts For Formulas Over Everything
  sort TrueLit{X} FalseLit{X} TruthLit{X} .
  ---
  sort NegTruthLit{X}  NoTrueLit{X}  NoFalseLit{X}  NormLit{X}  Lit{X}  .
  sort NegTruthConj{X} NoTrueConj{X} NoFalseConj{X} NormConj{X} Conj{X} .
  sort NegTruthDisj{X} NoTrueDisj{X} NoFalseDisj{X} NormDisj{X} Disj{X} .
  sort NegTruthForm{X} NoTrueForm{X} NoFalseForm{X} NormForm{X} Form{X} .

  --- Subsorts For Formulas Over Equalities
  --- Apply PosEq/NegEq Functor To Sorts
  subsort PosEqLit{X}  NegEqLit{X}  < EqLit{X}  .
  subsort PosEqConj{X} NegEqConj{X} < EqConj{X} .
  subsort PosEqDisj{X} NegEqDisj{X} < EqDisj{X} .
  subsort PosEqForm{X} NegEqForm{X} < EqForm{X} .

  --- Apply PosEq/NegEq Functor To Subsort Arrows
  subsort PosEqLit{X} < PosEqConj{X} PosEqDisj{X} < PosEqForm{X} .
  subsort NegEqLit{X} < NegEqConj{X} NegEqDisj{X} < NegEqForm{X} .
  subsort EqLit{X}    < EqConj{X}    EqDisj{X}    < EqForm{X}    .

  --- Subsorts For Formulas Over Everything
  --- Define Non-Equality Literals
  subsort TrueLit{X}  FalseLit{X}    < TruthLit{X}   .
  subsort TrueLit{X}  NegTruthLit{X} < NoFalseLit{X} .
  subsort FalseLit{X} NegTruthLit{X} < NoTrueLit{X}  .
  subsort TruthLit{X} NoFalseLit{X}  NoTrueLit{X} < NormLit{X} .

  --- Apply Each Functor To Sorts
  subsort NegTruthLit{X} < NegTruthConj{X} NegTruthDisj{X} < NegTruthForm{X} .
  subsort NoTrueLit{X}   < NoTrueConj{X}   NoTrueDisj{X}   < NoTrueForm{X} .
  subsort NoFalseLit{X}  < NoFalseConj{X}  NoFalseDisj{X}  < NoFalseForm{X} .
  subsort NormLit{X}     < NormConj{X}     NormDisj{X}     < NormForm{X} .
  subsort Lit{X}         < Conj{X}         Disj{X}         < Form{X} .

  --- Apply Each Functor To Subsort Arrows
  subsort EqLit{X}  < NegTruthLit{X}  < NoTrueLit{X}  NoFalseLit{X}  < NormLit{X}  < Lit{X}  .
  subsort EqConj{X} < NegTruthConj{X} < NoTrueConj{X} NoFalseConj{X} < NormConj{X} < Conj{X} .
  subsort EqDisj{X} < NegTruthDisj{X} < NoTrueDisj{X} NoFalseDisj{X} < NormDisj{X} < Disj{X} .
  subsort EqForm{X} < NegTruthForm{X} < NoTrueForm{X} NoFalseForm{X} < NormForm{X} < Form{X} .

  vars F F' : Form{X} . var EqL : EqLit{X} .

  --- Define Literals
  op tt : -> TrueLit{X} [ctor] .
  op ff : -> FalseLit{X} [ctor] .
  op _?=_ : X$Elt X$Elt -> PosEqLit{X} [ctor comm prec 50] .
  op _!=_ : X$Elt X$Elt -> NegEqLit{X} [ctor comm prec 50] .
  op ~_ : TruthLit{X} -> NegTruthLit{X} [ctor prec 51] .
  op ~_ : NegTruthLit{X} -> NegTruthLit{X} [ctor ditto] .
  op ~_ : Form{X} -> Form{X} [ctor ditto] .

  --- Conjunctions/Disjunctions Over Equalities
  op _/\_ : PosEqConj{X} PosEqConj{X} -> PosEqConj{X} [ctor assoc comm id: tt prec 52] .
  op _/\_ : NegEqConj{X} NegEqConj{X} -> NegEqConj{X} [ctor ditto] .
  op _/\_ : EqConj{X} EqConj{X} -> EqConj{X} [ctor ditto] .
  ---
  op _/\_ : PosEqForm{X} PosEqForm{X} -> PosEqForm{X} [ctor ditto] .
  op _/\_ : NegEqForm{X} NegEqForm{X} -> NegEqForm{X} [ctor ditto] .
  op _/\_ : EqForm{X} EqForm{X} -> EqForm{X} [ctor ditto] .
  ---
  op _\/_ : PosEqDisj{X} PosEqDisj{X} -> PosEqDisj{X} [ctor assoc comm id: ff prec 52] .
  op _\/_ : NegEqDisj{X} NegEqDisj{X} -> NegEqDisj{X} [ctor ditto] .
  op _\/_ : EqDisj{X} EqDisj{X} -> EqDisj{X} [ctor ditto] .
  ---
  op _\/_ : PosEqForm{X} PosEqForm{X} -> PosEqForm{X} [ctor ditto] .
  op _\/_ : NegEqForm{X} NegEqForm{X} -> NegEqForm{X} [ctor ditto] .
  op _\/_ : EqForm{X} EqForm{X} -> EqForm{X} [ctor ditto] .

  --- Conjunctions/Disjunctions Over Everything
  op _/\_ : NegTruthConj{X} NegTruthConj{X} -> NegTruthConj{X} [ctor ditto] .
  op _/\_ : NoTrueConj{X} NoTrueConj{X} -> NoTrueConj{X} [ctor ditto] .
  op _/\_ : NoFalseConj{X} NoFalseConj{X} -> NoFalseConj{X} [ctor ditto] .
  op _/\_ : Conj{X} Conj{X} -> Conj{X} [ctor ditto] .
  ---
  op _/\_ : NegTruthForm{X} NegTruthForm{X} -> NegTruthForm{X} [ctor ditto] .
  op _/\_ : NoTrueForm{X} NoTrueForm{X} -> NoTrueForm{X} [ctor ditto] .
  op _/\_ : NoFalseForm{X} NoFalseForm{X} -> NoFalseForm{X} [ctor ditto] .
  op _/\_ : Form{X} Form{X} -> Form{X} [ctor ditto] .
  ---
  op _\/_ : NegTruthDisj{X} NegTruthDisj{X} -> NegTruthDisj{X} [ctor ditto] .
  op _\/_ : NoTrueDisj{X} NoTrueDisj{X} -> NoTrueDisj{X} [ctor ditto] .
  op _\/_ : NoFalseDisj{X} NoFalseDisj{X} -> NoFalseDisj{X} [ctor ditto] .
  op _\/_ : Disj{X} Disj{X} -> Disj{X} [ctor ditto] .
  ---
  op _\/_ : NegTruthForm{X} NegTruthForm{X} -> NegTruthForm{X} [ctor ditto] .
  op _\/_ : NoTrueForm{X} NoTrueForm{X} -> NoTrueForm{X} [ctor ditto] .
  op _\/_ : NoFalseForm{X} NoFalseForm{X} -> NoFalseForm{X} [ctor ditto] .
  op _\/_ : Form{X} Form{X} -> Form{X} [ctor ditto] .
  ---------------------------------------------------
  eq ff /\ EqL = ff .
  eq tt \/ EqL = tt .

  eq EqL /\ EqL = EqL .
  eq EqL \/ EqL = EqL .

  --- Implication
  op _=>_  : Form{X} Form{X} -> Form{X} .
  op _<=>_ : Form{X} Form{X} -> Form{X} .
  ---------------------------------------
  eq F  => F' = (~ F) \/ F' .
  eq F <=> F' = (F => F') /\ (F' => F) .
endfm

view Term from TRIV to META-TERM is sort Elt to Term . endv

fmod EQFORM is
   pr SUBSTITUTION-SET .
   pr EQFORM-IMPL{Term} * (
      --- Map Formulas Over Equalities
      sort PosEqLit{Term} to PosEqLit,
      sort NegEqLit{Term} to NegEqLit,
      sort EqLit{Term} to EqLit,
      ---
      sort PosEqConj{Term} to PosEqConj,
      sort NegEqConj{Term} to NegEqConj,
      sort EqConj{Term} to EqConj,
      ---
      sort PosEqDisj{Term} to PosEqDisj,
      sort NegEqDisj{Term} to NegEqDisj,
      sort EqDisj{Term} to EqDisj,
      ---
      sort PosEqForm{Term} to PosEqForm,
      sort NegEqForm{Term} to NegEqForm,
      sort EqForm{Term} to EqForm,

      --- Map Formulas Over Everything
      sort TrueLit{Term} to TrueLit,
      sort FalseLit{Term} to FalseLit,
      sort TruthLit{Term} to TruthLit,
      ---
      sort NegTruthLit{Term} to NegTruthLit,
      sort NoTrueLit{Term} to NoTrueLit,
      sort NoFalseLit{Term} to NoFalseLit,
      sort NormLit{Term} to NormLit,
      sort Lit{Term} to Lit,
      ---
      sort NegTruthConj{Term} to NegTruthConj,
      sort NoTrueConj{Term} to NoTrueConj,
      sort NoFalseConj{Term} to NoFalseConj,
      sort NormConj{Term} to NormConj,
      sort Conj{Term} to Conj,
      ---
      sort NegTruthDisj{Term} to NegTruthDisj,
      sort NoTrueDisj{Term} to NoTrueDisj,
      sort NoFalseDisj{Term} to NoFalseDisj,
      sort NormDisj{Term} to NormDisj,
      sort Disj{Term} to Disj,
      ---
      sort NegTruthForm{Term} to NegTruthForm,
      sort NoTrueForm{Term} to NoTrueForm,
      sort NoFalseForm{Term} to NoFalseForm,
      sort NormForm{Term} to NormForm,
      sort Form{Term} to Form) .

  vars T T' : Term . vars F F' : Form . var SUB : Substitution .

  op _<<_ : Form Substitution -> [Form] .
  ---------------------------------------
  eq tt << SUB = tt .
  eq ff << SUB = ff .

  eq (T ?= T') << SUB = (T << SUB) ?= (T' << SUB) .
  eq (T != T') << SUB = (T << SUB) != (T' << SUB) .

  eq (~ F) << SUB = ~ (F << SUB) .

 ceq (F /\ F') << SUB = (F << SUB) /\ (F' << SUB) if F =/= tt /\ F' =/= tt .
 ceq (F \/ F') << SUB = (F << SUB) \/ (F' << SUB) if F =/= ff /\ F' =/= ff .
endfm

fmod EQFORM-NNF is
  pr EQFORM .
  var F G : Form .
  var CF CG : NoTrueForm .
  var DF DG : NoFalseForm .
  var NL : NormLit .
  var T1 T2 : Term .

  op nnf : Form -> [Form] .

 ceq nnf(F /\ G) = nnf(F) /\ nnf(G) if F =/= tt /\ G =/= tt .
 ceq nnf(F \/ G) = nnf(F) \/ nnf(G) if F =/= ff /\ G =/= ff .
 ceq nnf(~ (F /\ G)) = nnf(~ F) \/ nnf(~ G) if F =/= tt /\ G =/= tt .
 ceq nnf(~ (F \/ G)) = nnf(~ F) /\ nnf(~ G) if F =/= ff /\ G =/= ff .
  eq nnf(~ ~ F) = nnf(F) .
  eq nnf(~ tt) = ff .
  eq nnf(~ ff) = tt .
  eq nnf(~ T1 ?= T2) = T1 != T2 .
  eq nnf(~ T1 != T2) = T1 ?= T2 .
  eq nnf(NL) = NL .
endfm

--- NOTE: NEF is Normal Equational Form
---       Removes all negation and unneeded true/false
fmod EQFORM-NEF is
  pr EQFORM-NNF .
  var F G : Form .
  var NF : NormForm .
  var EF : EqForm .
  var CF : NoTrueForm .
  var DF : NoFalseForm .

  op nef : Form -> [NormForm] .
  op nef1 : Form -> [NormForm] .

  eq nef(F) = nef1(nnf(F)) .
  eq nef1(NF) = NF .
 ceq nef1(ff /\ F) = ff if F =/= tt .
 ceq nef1(tt \/ F) = tt if F =/= ff .
 ceq nef1(F /\ G) = nef1(F /\ nef1(G)) if F =/= tt /\ G =/= tt [owise] .
 ceq nef1(F \/ G) = nef1(F \/ nef1(G)) if F =/= ff /\ G =/= ff [owise] .
endfm

fth FUN is inc TRIV .
  op F : Elt -> Elt .
endfth

fmod FIXF{X :: FUN} is
  pr TRUTH .
  var A A' : X$Elt .
  op FixF : X$Elt -> [X$Elt] .
  op FixF : X$Elt X$Elt -> [X$Elt] .
  eq FixF(A) = FixF(A,F(A)) .
  eq FixF(A,A') = if A == A' then A' else FixF(A') fi .
endfm

fmod EQFORM-CNF-IMPL is
  pr EQFORM-NEF .
  var F G H : Form .
  var EF EG EH : EqForm .
  var NL : NormLit .

  op cnf-impl1 : NormForm -> [NormForm] .

  eq cnf-impl1(NL) = NL .
 ceq cnf-impl1((F /\ G) \/ H) = cnf-impl1((F \/ H) /\ (G \/ H)) if F =/= tt /\ G =/= tt /\ H =/= ff .
 ceq cnf-impl1(F \/ G) = cnf-impl1(F) \/ cnf-impl1(G) if F =/= ff /\ G =/= ff [owise] .
 ceq cnf-impl1(F /\ G) = cnf-impl1(F) /\ cnf-impl1(G) if F =/= tt /\ G =/= tt .
endfm

fmod EQFORM-DNF-IMPL is
  pr EQFORM-NEF .
  var F G H : Form .
  var EF EG EH : EqForm .
  var NL : NormLit .

  op dnf-impl1 : NormForm -> [NormForm] .

  eq dnf-impl1(NL) = NL .
 ceq dnf-impl1((F \/ G) /\ H) = dnf-impl1((F /\ H) \/ (G /\ H)) if F =/= ff /\ G =/= ff /\ H =/= tt .
 ceq dnf-impl1(F /\ G) = dnf-impl1(F) /\ dnf-impl1(G) if F =/= tt /\ G =/= tt [owise] .
 ceq dnf-impl1(F \/ G) = dnf-impl1(F) \/ dnf-impl1(G) if F =/= ff /\ G =/= ff .
endfm

view cnf from FUN to EQFORM-CNF-IMPL is sort Elt to NormForm . op F to cnf-impl1 . endv
view dnf from FUN to EQFORM-DNF-IMPL is sort Elt to NormForm . op F to dnf-impl1 . endv

fmod EQFORM-CNF is
  pr FIXF{cnf} * (op FixF to cnf-impl) .
  op cnf : Form -> EqForm .
  eq cnf(F:Form) = cnf-impl(nef(F:Form)) .
endfm

fmod EQFORM-DNF is
  pr FIXF{dnf} * (op FixF to dnf-impl) .
  op dnf : Form -> EqForm .
  eq dnf(F:Form) = dnf-impl(nef(F:Form)) .
endfm

fmod EQFORM-LIST is
  pr EQFORM .

  sort EmptyFormList .
  sort PosEqLitList PosEqConjList PosEqDisjList PosEqFormList .
  sort NegEqLitList NegEqConjList NegEqDisjList NegEqFormList .
  sort EqLitList    EqConjList    EqDisjList    EqFormList    .
  sort NormFormList FormList .

  sort PosEqLitNeList PosEqConjNeList PosEqDisjNeList PosEqFormNeList .
  sort NegEqLitNeList NegEqConjNeList NegEqDisjNeList NegEqFormNeList .
  sort EqLitNeList    EqConjNeList    EqDisjNeList    EqFormNeList    .
  sort NormFormNeList FormNeList .

  subsort PosEqLit  < PosEqLitNeList  < PosEqLitList  .
  subsort PosEqConj < PosEqConjNeList < PosEqConjList .
  subsort PosEqDisj < PosEqDisjNeList < PosEqDisjList .
  subsort PosEqForm < PosEqFormNeList < PosEqFormList .
  subsort NegEqLit  < NegEqLitNeList  < NegEqLitList  .
  subsort NegEqConj < NegEqConjNeList < NegEqConjList .
  subsort NegEqDisj < NegEqDisjNeList < NegEqDisjList .
  subsort NegEqForm < NegEqFormNeList < NegEqFormList .
  subsort EqLit     < EqLitNeList     < EqLitList     .
  subsort EqConj    < EqConjNeList    < EqConjList    .
  subsort EqDisj    < EqDisjNeList    < EqDisjList    .
  subsort EqForm    < EqFormNeList    < EqFormList    .
  subsort NormForm  < NormFormNeList  < NormFormList  .
  subsort Form      < FormNeList      < FormList      .

  --- List Functor
  --- Apply PosEq/NegEq Functor To Sorts
  subsort PosEqLitList  NegEqLitList  < EqLitList  .
  subsort PosEqConjList NegEqConjList < EqConjList .
  subsort PosEqDisjList NegEqDisjList < EqDisjList .
  subsort PosEqFormList NegEqFormList < EqFormList .
  --- Apply PosEq/NegEq Functor To Subsort Arrows
  subsort PosEqLitList < PosEqConjList PosEqDisjList < PosEqFormList .
  subsort NegEqLitList < NegEqConjList NegEqDisjList < NegEqFormList .
  subsort EqLitList    < EqConjList    EqDisjList    < EqFormList    .
  --- Other Cases
  subsort EmptyFormList < PosEqLitList NegEqLitList FormList .
  subsort EqFormList < NormFormList < FormList .

  --- NeList Functor
  --- Apply PosEq/NegEq Functor To Sorts
  subsort PosEqLitNeList  NegEqLitNeList  < EqLitNeList  .
  subsort PosEqConjNeList NegEqConjNeList < EqConjNeList .
  subsort PosEqDisjNeList NegEqDisjNeList < EqDisjNeList .
  subsort PosEqFormNeList NegEqFormNeList < EqFormNeList .
  --- Apply PosEq/NegEq Functor To Subsort Arrows
  subsort PosEqLitNeList < PosEqConjNeList PosEqDisjNeList < PosEqFormNeList .
  subsort NegEqLitNeList < NegEqConjNeList NegEqDisjNeList < NegEqFormNeList .
  subsort EqLitNeList    < EqConjNeList    EqDisjNeList    < EqFormNeList    .
  --- Other Cases
  subsort EqFormNeList < NormFormNeList < FormNeList .

  op mtFormList :                            -> EmptyFormList   [ctor] .
  op (_;_) : EmptyFormList   EmptyFormList   -> EmptyFormList   [ctor assoc id: mtFormList] .

  op (_;_) : PosEqLitList    PosEqLitList    -> PosEqLitList    [ctor ditto] .
  op (_;_) : PosEqConjList   PosEqConjList   -> PosEqConjList   [ctor ditto] .
  op (_;_) : PosEqDisjList   PosEqDisjList   -> PosEqDisjList   [ctor ditto] .
  op (_;_) : PosEqFormList   PosEqFormList   -> PosEqFormList   [ctor ditto] .
  op (_;_) : NegEqLitList    NegEqLitList    -> NegEqLitList    [ctor ditto] .
  op (_;_) : NegEqConjList   NegEqConjList   -> NegEqConjList   [ctor ditto] .
  op (_;_) : NegEqDisjList   NegEqDisjList   -> NegEqDisjList   [ctor ditto] .
  op (_;_) : NegEqFormList   NegEqFormList   -> NegEqFormList   [ctor ditto] .
  op (_;_) : EqLitList       EqLitList       -> EqLitList       [ctor ditto] .
  op (_;_) : EqConjList      EqConjList      -> EqConjList      [ctor ditto] .
  op (_;_) : EqDisjList      EqDisjList      -> EqDisjList      [ctor ditto] .
  op (_;_) : EqFormList      EqFormList      -> EqFormList      [ctor ditto] .
  op (_;_) : NormFormList    NormFormList    -> NormFormList    [ctor ditto] .
  op (_;_) : FormList        FormList        -> FormList        [ctor ditto] .

  op (_;_) : PosEqLitNeList  PosEqLitList    -> PosEqLitNeList  [ctor ditto] .
  op (_;_) : PosEqConjNeList PosEqConjList   -> PosEqConjNeList [ctor ditto] .
  op (_;_) : PosEqDisjNeList PosEqDisjList   -> PosEqDisjNeList [ctor ditto] .
  op (_;_) : PosEqFormNeList PosEqFormList   -> PosEqFormNeList [ctor ditto] .
  op (_;_) : NegEqLitNeList  NegEqLitList    -> NegEqLitNeList  [ctor ditto] .
  op (_;_) : NegEqConjNeList NegEqConjList   -> NegEqConjNeList [ctor ditto] .
  op (_;_) : NegEqDisjNeList NegEqDisjList   -> NegEqDisjNeList [ctor ditto] .
  op (_;_) : NegEqFormNeList NegEqFormList   -> NegEqFormNeList [ctor ditto] .
  op (_;_) : EqLitNeList     EqLitList       -> EqLitNeList     [ctor ditto] .
  op (_;_) : EqConjNeList    EqConjList      -> EqConjNeList    [ctor ditto] .
  op (_;_) : EqDisjNeList    EqDisjList      -> EqDisjNeList    [ctor ditto] .
  op (_;_) : EqFormNeList    EqFormList      -> EqFormNeList    [ctor ditto] .
  op (_;_) : NormFormNeList  NormFormList    -> NormFormNeList  [ctor ditto] .
  op (_;_) : FormNeList      FormList        -> FormNeList      [ctor ditto] .

  op (_;_) : PosEqLitList    PosEqLitNeList  -> PosEqLitNeList  [ctor ditto] .
  op (_;_) : PosEqConjList   PosEqConjNeList -> PosEqConjNeList [ctor ditto] .
  op (_;_) : PosEqDisjList   PosEqDisjNeList -> PosEqDisjNeList [ctor ditto] .
  op (_;_) : PosEqFormList   PosEqFormNeList -> PosEqFormNeList [ctor ditto] .
  op (_;_) : NegEqLitList    NegEqLitNeList  -> NegEqLitNeList  [ctor ditto] .
  op (_;_) : NegEqConjList   NegEqConjNeList -> NegEqConjNeList [ctor ditto] .
  op (_;_) : NegEqDisjList   NegEqDisjNeList -> NegEqDisjNeList [ctor ditto] .
  op (_;_) : NegEqFormList   NegEqFormNeList -> NegEqFormNeList [ctor ditto] .
  op (_;_) : EqLitList       EqLitNeList     -> EqLitNeList     [ctor ditto] .
  op (_;_) : EqConjList      EqConjNeList    -> EqConjNeList    [ctor ditto] .
  op (_;_) : EqDisjList      EqDisjNeList    -> EqDisjNeList    [ctor ditto] .
  op (_;_) : EqFormList      EqFormNeList    -> EqFormNeList    [ctor ditto] .
  op (_;_) : NormFormList    NormFormNeList  -> NormFormNeList  [ctor ditto] .
  op (_;_) : FormList        FormNeList      -> FormNeList      [ctor ditto] .
endfm

fmod EQFORM-SET is
  pr EQFORM .

  sort EmptyFormSet .
  sort PosEqLitSet PosEqConjSet PosEqDisjSet PosEqFormSet .
  sort NegEqLitSet NegEqConjSet NegEqDisjSet NegEqFormSet .
  sort EqLitSet    EqConjSet    EqDisjSet    EqFormSet    .
  sort NormFormSet FormSet .

  sort PosEqLitNeSet PosEqConjNeSet PosEqDisjNeSet PosEqFormNeSet .
  sort NegEqLitNeSet NegEqConjNeSet NegEqDisjNeSet NegEqFormNeSet .
  sort EqLitNeSet    EqConjNeSet    EqDisjNeSet    EqFormNeSet    .
  sort NormFormNeSet FormNeSet .

  subsort PosEqLit  < PosEqLitNeSet  < PosEqLitSet  .
  subsort PosEqConj < PosEqConjNeSet < PosEqConjSet .
  subsort PosEqDisj < PosEqDisjNeSet < PosEqDisjSet .
  subsort PosEqForm < PosEqFormNeSet < PosEqFormSet .
  subsort NegEqLit  < NegEqLitNeSet  < NegEqLitSet  .
  subsort NegEqConj < NegEqConjNeSet < NegEqConjSet .
  subsort NegEqDisj < NegEqDisjNeSet < NegEqDisjSet .
  subsort NegEqForm < NegEqFormNeSet < NegEqFormSet .
  subsort EqLit     < EqLitNeSet     < EqLitSet     .
  subsort EqConj    < EqConjNeSet    < EqConjSet    .
  subsort EqDisj    < EqDisjNeSet    < EqDisjSet    .
  subsort EqForm    < EqFormNeSet    < EqFormSet    .
  subsort NormForm  < NormFormNeSet  < NormFormSet  .
  subsort Form      < FormNeSet      < FormSet      .

  --- Set Functor
  --- Apply PosEq/NegEq Functor To Sorts
  subsort PosEqLitSet  NegEqLitSet  < EqLitSet  .
  subsort PosEqConjSet NegEqConjSet < EqConjSet .
  subsort PosEqDisjSet NegEqDisjSet < EqDisjSet .
  subsort PosEqFormSet NegEqFormSet < EqFormSet .
  --- Apply PosEq/NegEq Functor To Subsort Arrows
  subsort PosEqLitSet < PosEqConjSet PosEqDisjSet < PosEqFormSet .
  subsort NegEqLitSet < NegEqConjSet NegEqDisjSet < NegEqFormSet .
  subsort EqLitSet    < EqConjSet    EqDisjSet    < EqFormSet    .
  --- Other Cases
  subsort EmptyFormSet < PosEqLitSet NegEqLitSet FormSet .
  subsort EqFormSet < NormFormSet < FormSet .

  --- NeSet Functor
  --- Apply PosEq/NegEq Functor To Sorts
  subsort PosEqLitNeSet  NegEqLitNeSet  < EqLitNeSet  .
  subsort PosEqConjNeSet NegEqConjNeSet < EqConjNeSet .
  subsort PosEqDisjNeSet NegEqDisjNeSet < EqDisjNeSet .
  subsort PosEqFormNeSet NegEqFormNeSet < EqFormNeSet .
  --- Apply PosEq/NegEq Functor To Subsort Arrows
  subsort PosEqLitNeSet < PosEqConjNeSet PosEqDisjNeSet < PosEqFormNeSet .
  subsort NegEqLitNeSet < NegEqConjNeSet NegEqDisjNeSet < NegEqFormNeSet .
  subsort EqLitNeSet    < EqConjNeSet    EqDisjNeSet    < EqFormNeSet    .
  --- Other Cases
  subsort EqFormNeSet < NormFormNeSet < FormNeSet .

  op mtFormSet :                           -> EmptyFormSet   [ctor] .
  op (_|_) : EmptyFormSet   EmptyFormSet   -> EmptyFormSet   [ctor assoc comm id: mtFormSet] .

  op (_|_) : PosEqLitSet    PosEqLitSet    -> PosEqLitSet    [ctor ditto] .
  op (_|_) : PosEqConjSet   PosEqConjSet   -> PosEqConjSet   [ctor ditto] .
  op (_|_) : PosEqDisjSet   PosEqDisjSet   -> PosEqDisjSet   [ctor ditto] .
  op (_|_) : PosEqFormSet   PosEqFormSet   -> PosEqFormSet   [ctor ditto] .
  op (_|_) : NegEqLitSet    NegEqLitSet    -> NegEqLitSet    [ctor ditto] .
  op (_|_) : NegEqConjSet   NegEqConjSet   -> NegEqConjSet   [ctor ditto] .
  op (_|_) : NegEqDisjSet   NegEqDisjSet   -> NegEqDisjSet   [ctor ditto] .
  op (_|_) : NegEqFormSet   NegEqFormSet   -> NegEqFormSet   [ctor ditto] .
  op (_|_) : EqLitSet       EqLitSet       -> EqLitSet       [ctor ditto] .
  op (_|_) : EqConjSet      EqConjSet      -> EqConjSet      [ctor ditto] .
  op (_|_) : EqDisjSet      EqDisjSet      -> EqDisjSet      [ctor ditto] .
  op (_|_) : EqFormSet      EqFormSet      -> EqFormSet      [ctor ditto] .
  op (_|_) : NormFormSet    NormFormSet    -> NormFormSet    [ctor ditto] .
  op (_|_) : FormSet        FormSet        -> FormSet        [ctor ditto] .

  op (_|_) : PosEqLitNeSet  PosEqLitSet    -> PosEqLitNeSet  [ctor ditto] .
  op (_|_) : PosEqConjNeSet PosEqConjSet   -> PosEqConjNeSet [ctor ditto] .
  op (_|_) : PosEqDisjNeSet PosEqDisjSet   -> PosEqDisjNeSet [ctor ditto] .
  op (_|_) : PosEqFormNeSet PosEqFormSet   -> PosEqFormNeSet [ctor ditto] .
  op (_|_) : NegEqLitNeSet  NegEqLitSet    -> NegEqLitNeSet  [ctor ditto] .
  op (_|_) : NegEqConjNeSet NegEqConjSet   -> NegEqConjNeSet [ctor ditto] .
  op (_|_) : NegEqDisjNeSet NegEqDisjSet   -> NegEqDisjNeSet [ctor ditto] .
  op (_|_) : NegEqFormNeSet NegEqFormSet   -> NegEqFormNeSet [ctor ditto] .
  op (_|_) : EqLitNeSet     EqLitSet       -> EqLitNeSet     [ctor ditto] .
  op (_|_) : EqConjNeSet    EqConjSet      -> EqConjNeSet    [ctor ditto] .
  op (_|_) : EqDisjNeSet    EqDisjSet      -> EqDisjNeSet    [ctor ditto] .
  op (_|_) : EqFormNeSet    EqFormSet      -> EqFormNeSet    [ctor ditto] .
  op (_|_) : NormFormNeSet  NormFormSet    -> NormFormNeSet  [ctor ditto] .
  op (_|_) : FormNeSet      FormSet        -> FormNeSet      [ctor ditto] .
endfm
```

```maude
fmod EQFORM-OPERATIONS is
  pr EQFORM .
  pr TERM-EXTRA . --- defines vars() : Term -> QidSet

  vars F F1 F2 : Form . vars TL : TruthLit . vars PEC1 : PosEqConj .
  vars M : Module . vars T T' : Term .

  op  wellFormed : Module Form -> Bool .
  op $wellFormed : Module Form -> Bool .
  --------------------------------------
 ceq  wellFormed(M,F)        = $wellFormed(M,F) if wellFormed(M) .
 ceq $wellFormed(M,F1 /\ F2) = $wellFormed(M,F1) and-then $wellFormed(M,F2) if F1 =/= tt /\ F2 =/= tt .
 ceq $wellFormed(M,F1 \/ F2) = $wellFormed(M,F1) and-then $wellFormed(M,F2) if F1 =/= ff /\ F2 =/= ff .
  eq $wellFormed(M,~ F)      = $wellFormed(M,F) .
  --- eq lit
  eq $wellFormed(M,T ?= T')  = wellFormed(M,T) and-then wellFormed(M,T') and-then sameKind(M,leastSort(M,T),leastSort(M,T')) .
  eq $wellFormed(M,T != T')  = wellFormed(M,T) and-then wellFormed(M,T') and-then sameKind(M,leastSort(M,T),leastSort(M,T')) .
  --- true/false lit or mtForm
  eq $wellFormed(M,TL)       = true .

  op normalize : Module Form -> Form .
  ------------------------------------
 ceq normalize(M,F1 /\ F2) = normalize(M,F1) /\ normalize(M,F2) if F1 =/= tt /\ F2 =/= tt .
 ceq normalize(M,F1 \/ F2) = normalize(M,F1) \/ normalize(M,F2) if F1 =/= ff /\ F2 =/= ff .
  eq normalize(M,~ F)      = ~ normalize(M,F) .
  eq normalize(M,T ?= T')  = getTerm(metaNormalize(M,T)) ?= getTerm(metaNormalize(M,T')) .
  eq normalize(M,T != T')  = getTerm(metaNormalize(M,T)) != getTerm(metaNormalize(M,T')) .
  eq normalize(M,TL)       = TL .

  op reduce : Module Form -> Form .
  ---------------------------------
 ceq reduce(M,F1 /\ F2) = reduce(M,F1) /\ reduce(M,F2) if F1 =/= tt /\ F2 =/= tt .
 ceq reduce(M,F1 \/ F2) = reduce(M,F1) \/ reduce(M,F2) if F1 =/= ff /\ F2 =/= ff .
  eq reduce(M,~ F)      = ~ reduce(M,F) .
  eq reduce(M,T ?= T')  = getTerm(metaReduce(M,T)) ?= getTerm(metaReduce(M,T')) .
  eq reduce(M,T != T')  = getTerm(metaReduce(M,T)) != getTerm(metaReduce(M,T')) .
  eq reduce(M,TL)       = TL .

  op vars : Form -> QidSet .
  --------------------------
 ceq vars(F1 /\ F2) = vars(F1) ; vars(F2) if F1 =/= tt /\ F2 =/= tt .
 ceq vars(F1 \/ F2) = vars(F1) ; vars(F2) if F1 =/= ff /\ F2 =/= ff .
  eq vars(~ F)      = vars(F) .
  eq vars(T ?= T')  = vars(T) ; vars(T') .
  eq vars(T != T')  = vars(T) ; vars(T') .
  eq vars(TL)       = none .

  --- INP: PosConj
  --- PRE: PosConj has no ff literals
  --- OUT: UnificationProblem
  op toUnifProb : PosEqConj -> UnificationProblem .
  ------------------------------------------------
  eq toUnifProb((T ?= T') /\ PEC1) = T =? T' /\ toUnifProb(PEC1) .
  eq toUnifProb(T ?= T')           = T =? T' .
endfm
```

```maude
fmod EQFORM-SET-OPERATIONS is
  pr EQFORM-OPERATIONS .
  pr EQFORM-SET .
  pr EQFORM-CNF .
  pr EQFORM-DNF .

  var TL : TruthLit . var C : EqConj . var D : EqDisj .
  var FS : FormSet . var F F' : Form . var UP : UnificationProblem .
  var PEA : PosEqLit . var PELS : PosEqLitSet . var T T' : Term . var M : Module .

  op wellFormed : Module FormSet -> Bool .
  ----------------------------------------
  eq wellFormed(M , F | F' | FS) = wellFormed(M,F) and-then wellFormed(M,F' | FS) .
  eq wellFormed(M , mtFormSet)   = true .

  op disj-join : FormSet -> [Form] .
  op disj-join : FormSet -> [Form] .
  ----------------------------------
  eq disj-join(F | FS)    = F \/ disj-join(FS) .
  eq disj-join(mtFormSet) = ff .

  op conj-join : FormSet -> [Form] .
  op conj-join : FormSet -> [Form] .
  ----------------------------------
  eq conj-join(F | FS)    = F /\ conj-join(FS) .
  eq conj-join(mtFormSet) = tt .

  op toDisjSet  : Form -> [EqDisjSet] .
  op toDisjSet' : Form -> [EqDisjSet] .
  -------------------------------------
  eq toDisjSet (F)      = toDisjSet'(cnf(F)) .
  eq toDisjSet'(TL)     = mtFormSet .
  eq toDisjSet'(D /\ F) = D | toDisjSet'(F) .

  op toConjSet  : Form -> [EqConjSet] .
  op toConjSet' : Form -> [EqConjSet] .
  -------------------------------------
  eq toConjSet (F)      = toConjSet'(dnf(F)) .
  eq toConjSet'(TL)     = mtFormSet .
  eq toConjSet'(C \/ F) = C | toConjSet'(F) .

  op toEqSet : PosEqLitSet -> EquationSet .
  -----------------------------------------
  eq toEqSet((T ?= T') | PELS) = (eq T = T' [none] .) toEqSet(PELS) .
  eq toEqSet(mtFormSet)        = none .

  op toPosEqLits : PosEqForm          -> PosEqLitSet .
  op toPosEqLits : UnificationProblem -> PosEqLitSet .
  ----------------------------------------------------
  eq toPosEqLits(TL)      = mtFormSet .
  eq toPosEqLits(T ?= T') = T ?= T' .
  eq toPosEqLits(~ F)     = toPosEqLits(F) .
 ceq toPosEqLits(F /\ F') = toPosEqLits(F) | toPosEqLits(F') if F =/= tt /\ F' =/= tt .
 ceq toPosEqLits(F \/ F') = toPosEqLits(F) | toPosEqLits(F') if F =/= ff /\ F' =/= ff .

  eq toPosEqLits(T =? T' /\ UP) = (T ?= T') | toPosEqLits(UP) .
  eq toPosEqLits(T =? T')       = T ?= T' .
endfm
```
