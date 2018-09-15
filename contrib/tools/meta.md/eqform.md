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
6.  `EqLit`    - positive (`_?=_`) or (`_!=_`) negative equality literals
7.  `EqForm`   - the sort of formulas in case [3] above
8.  `NormForm` - the sort containing all formulas in cases [1-3] above
9.  `Form`     - the sort of all quantifier-free equational formulas

This library also provides other formula sorts that are needed for programming purposes:

10. `NoTrueForm`  - Formulas over ...
11. `NoFalseForm` - Formulas over ...

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

set include BOOL off .

fmod EQFORM is
  pr META-TERM .
  pr SUBSTITUTION-SET .

  sort TrueLit  FalseLit        .
  sort PosEqLit NegEqLit EqLit  .
  sort PosConj  NegConj  EqConj .
  sort PosDisj  NegDisj  EqDisj .
  sort PosForm  NegForm  EqForm .

  subsort PosEqLit NegEqLit < EqLit .
  ---
  subsort PosEqLit < PosConj PosDisj .
  subsort NegEqLit < NegConj NegDisj .
  subsort EqLit    < EqConj  EqDisj  .
  ---
  subsort PosConj NegConj < EqConj .
  subsort PosDisj NegDisj < EqDisj .
  subsort PosForm NegForm < EqForm .
  ---
  subsort PosConj PosDisj < PosForm .
  subsort NegConj NegDisj < NegForm .
  subsort EqConj  EqDisj  < EqForm  .

  sort TrivTrueForm TrivFalseForm NonTrivForm .
  subsort TrueLit  < TrivTrueForm  .
  subsort FalseLit < TrivFalseForm .
  subsort EqForm   < NonTrivForm   .

  sort NoTrueForm NoFalseForm .
  subsort NonTrivForm TrivFalseForm < NoTrueForm  .
  subsort NonTrivForm TrivTrueForm  < NoFalseForm .

  sort NormLit NormForm .
  subsort EqLit TrueLit FalseLit < NormLit < NormForm  .
  subsort EqForm TrivTrueForm TrivFalseForm < NormForm .

  sort Form .
  subsort NoTrueForm NoFalseForm NormForm < Form .

  op tt   :           -> TrueLit     [ctor] .
  op ff   :           -> FalseLit    [ctor] .
  op _?=_ : Term Term -> PosEqLit    [ctor prec 50] .
  op _!=_ : Term Term -> NegEqLit    [ctor prec 50] .
  op ~_   : Form      -> NonTrivForm [ctor prec 51] .

  op _/\_ : TrivTrueForm   TrivTrueForm   -> TrivTrueForm  [ctor assoc comm id: tt prec 52] .
  ---
  op _/\_ : TrivFalseForm  TrivFalseForm  -> TrivFalseForm [ditto] .
  op _/\_ : TrivTrueForm   TrivFalseForm  -> TrivFalseForm [ditto] .
  ---
  op _/\_ : TrivTrueForm   NonTrivForm    -> NonTrivForm   [ditto] .
  op _/\_ : TrivFalseForm  NonTrivForm    -> NonTrivForm   [ditto] .
  ---
  op _/\_ : PosConj        PosConj        -> PosConj       [ditto] .
  op _/\_ : NegConj        NegConj        -> NegConj       [ditto] .
  op _/\_ : EqConj         EqConj         -> EqConj        [ditto] .
  op _/\_ : PosForm        PosForm        -> PosForm       [ditto] .
  op _/\_ : NegForm        NegForm        -> NegForm       [ditto] .
  op _/\_ : EqForm         EqForm         -> EqForm        [ditto] .
  op _/\_ : NonTrivForm    NonTrivForm    -> NonTrivForm   [ditto] .
  op _/\_ : Form           Form           -> Form          [ditto] .

  op _\/_ : TrivFalseForm  TrivFalseForm  -> TrivFalseForm [ctor assoc comm id: ff prec 52] .
  op _\/_ : TrivTrueForm   TrivTrueForm   -> TrivTrueForm  [ditto] .
  op _\/_ : TrivFalseForm  TrivTrueForm   -> TrivTrueForm  [ditto] .
  ---
  op _\/_ : TrivFalseForm  NonTrivForm    -> NonTrivForm   [ditto] .
  op _\/_ : TrivTrueForm   NonTrivForm    -> NonTrivForm   [ditto] .
  ---
  op _\/_ : PosDisj        PosDisj        -> PosDisj       [ditto] .
  op _\/_ : NegDisj        NegDisj        -> NegDisj       [ditto] .
  op _\/_ : EqDisj         EqDisj         -> EqDisj        [ditto] .
  op _\/_ : PosForm        PosForm        -> PosForm       [ditto] .
  op _\/_ : NegForm        NegForm        -> NegForm       [ditto] .
  op _\/_ : EqForm         EqForm         -> EqForm        [ditto] .
  op _\/_ : NonTrivForm    NonTrivForm    -> NonTrivForm   [ditto] .
  op _\/_ : Form           Form           -> Form          [ditto] .

  --- We want: Form = NonTrivForm | TrivTrueForm | TrivFalseForm
  --- Thus, these are not needed
  ---
  --- op _/\_ : TrivTrueForm  Form -> Form [ditto] .
  --- op _/\_ : TrivFalseForm Form -> Form [ditto] .
  --- op _\/_ : TrivFalseForm Form -> Form [ditto] .
  --- op _\/_ : TrivTrueForm  Form -> Form [ditto] .

  var F : Form .
  vars CF CG : NoTrueForm .
  vars DF DG : NoFalseForm .
  vars T T' : Term .
  var SUB : Substitution .

  op _<<_ : Form Substitution -> Form .
  -------------------------------------
  eq tt << SUB = tt .
  eq ff << SUB = ff .

  eq (T ?= T') << SUB = (T << SUB) ?= (T' << SUB) .
  eq (T != T') << SUB = (T << SUB) != (T' << SUB) .

  eq (~ F) << SUB = ~ (F << SUB) .

  eq (CF /\ CG) << SUB = (CF << SUB) /\ (CG << SUB) .
  eq (DF \/ DG) << SUB = (DF << SUB) \/ (DG << SUB) .
endfm

fmod EQFORM-NNF is
  pr EQFORM .
  var F : Form .
  var CF CG : NoTrueForm .
  var DF DG : NoFalseForm .
  var NL : NormLit .
  var T1 T2 : Term .

  op nnf : Form -> [Form] .

  eq nnf(CF /\ CG) = nnf(CF) /\ nnf(CG) .
  eq nnf(DF \/ DG) = nnf(DF) \/ nnf(DG) .
  eq nnf(~ (CF /\ CG)) = nnf(~ CF) \/ nnf(~ CG) .
  eq nnf(~ (DF \/ DG)) = nnf(~ DF) /\ nnf(~ DG) .
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
  var F  : Form .
  var NF : NormForm .
  var VF : NonTrivForm .
  var CF : NoTrueForm .
  var DF : NoFalseForm .

  op nef : Form -> [NormForm] .
  op nef1 : Form -> [NormForm] .

  eq nef(F) = nef1(nnf(F)) .
  eq nef1(NF) = NF .
  eq nef1(ff /\ CF) = ff .
  eq nef1(tt \/ DF) = tt .
  eq nef1(VF /\ CF) = nef1(nef1(VF) /\ nef1(CF)) .
  eq nef1(VF \/ DF) = nef1(nef1(VF) \/ nef1(DF)) .
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
  var EF EG EH : EqForm .
  var NL : NormLit .

  op cnf-impl1 : NormForm -> [NormForm] .

  eq cnf-impl1(NL) = NL .
  eq cnf-impl1((EF /\ EG) \/ EH) = cnf-impl1((EF \/ EH) /\ (EG \/ EH)) .
  eq cnf-impl1(EF \/ EG) = cnf-impl1(EF) \/ cnf-impl1(EG) [owise] .
  eq cnf-impl1(EF /\ EG) = cnf-impl1(EF) /\ cnf-impl1(EG) .
endfm

fmod EQFORM-DNF-IMPL is
  pr EQFORM-NEF .
  var EF EG EH : EqForm .
  var NL : NormLit .

  op dnf-impl1 : NormForm -> [NormForm] .

 eq dnf-impl1(NL) = NL .
 eq dnf-impl1((EF \/ EG) /\ EH) = dnf-impl1((EF /\ EH) \/ (EG /\ EH)) .
 eq dnf-impl1(EF /\ EG) = dnf-impl1(EF) /\ dnf-impl1(EG) [owise] .
 eq dnf-impl1(EF \/ EG) = dnf-impl1(EF) \/ dnf-impl1(EG) .
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
  sort PosEqLitList PosConjList PosDisjList PosFormList .
  sort NegEqLitList NegConjList NegDisjList NegFormList .
  sort EqLitList    EqConjList    EqDisjList    EqFormList    .
  sort NormFormList FormList .

  sort PosEqLitNeList PosConjNeList PosDisjNeList PosFormNeList .
  sort NegEqLitNeList NegConjNeList NegDisjNeList NegFormNeList .
  sort EqLitNeList    EqConjNeList    EqDisjNeList    EqFormNeList    .
  sort NormFormNeList FormNeList .

  subsort PosEqLit  < PosEqLitNeList  < PosEqLitList  .
  subsort PosConj < PosConjNeList < PosConjList .
  subsort PosDisj < PosDisjNeList < PosDisjList .
  subsort PosForm < PosFormNeList < PosFormList .
  subsort NegEqLit  < NegEqLitNeList  < NegEqLitList  .
  subsort NegConj < NegConjNeList < NegConjList .
  subsort NegDisj < NegDisjNeList < NegDisjList .
  subsort NegForm < NegFormNeList < NegFormList .
  subsort EqLit     < EqLitNeList     < EqLitList     .
  subsort EqConj    < EqConjNeList    < EqConjList    .
  subsort EqDisj    < EqDisjNeList    < EqDisjList    .
  subsort EqForm    < EqFormNeList    < EqFormList    .
  subsort NormForm  < NormFormNeList  < NormFormList  .
  subsort Form      < FormNeList      < FormList      .

  --- List Functor
  --- Apply Pos/Neg Functor To Sorts
  subsort PosEqLitList  NegEqLitList  < EqLitList  .
  subsort PosConjList NegConjList < EqConjList .
  subsort PosDisjList NegDisjList < EqDisjList .
  subsort PosFormList NegFormList < EqFormList .
  --- Apply Pos/Neg Functor To Subsort Arrows
  subsort PosEqLitList < PosConjList PosDisjList < PosFormList .
  subsort NegEqLitList < NegConjList NegDisjList < NegFormList .
  subsort EqLitList    < EqConjList    EqDisjList    < EqFormList    .
  --- Other Cases
  subsort EmptyFormList < PosEqLitList NegEqLitList FormList .
  subsort EqFormList < NormFormList < FormList .

  --- NeList Functor
  --- Apply Pos/Neg Functor To Sorts
  subsort PosEqLitNeList  NegEqLitNeList  < EqLitNeList  .
  subsort PosConjNeList NegConjNeList < EqConjNeList .
  subsort PosDisjNeList NegDisjNeList < EqDisjNeList .
  subsort PosFormNeList NegFormNeList < EqFormNeList .
  --- Apply Pos/Neg Functor To Subsort Arrows
  subsort PosEqLitNeList < PosConjNeList PosDisjNeList < PosFormNeList .
  subsort NegEqLitNeList < NegConjNeList NegDisjNeList < NegFormNeList .
  subsort EqLitNeList    < EqConjNeList    EqDisjNeList    < EqFormNeList    .
  --- Other Cases
  subsort EqFormNeList < NormFormNeList < FormNeList .

  op mtFormList :                            -> EmptyFormList   [ctor] .
  op (_;_) : EmptyFormList   EmptyFormList   -> EmptyFormList   [ctor assoc id: mtFormList] .

  op (_;_) : PosEqLitList    PosEqLitList    -> PosEqLitList    [ctor ditto] .
  op (_;_) : PosConjList   PosConjList   -> PosConjList   [ctor ditto] .
  op (_;_) : PosDisjList   PosDisjList   -> PosDisjList   [ctor ditto] .
  op (_;_) : PosFormList   PosFormList   -> PosFormList   [ctor ditto] .
  op (_;_) : NegEqLitList    NegEqLitList    -> NegEqLitList    [ctor ditto] .
  op (_;_) : NegConjList   NegConjList   -> NegConjList   [ctor ditto] .
  op (_;_) : NegDisjList   NegDisjList   -> NegDisjList   [ctor ditto] .
  op (_;_) : NegFormList   NegFormList   -> NegFormList   [ctor ditto] .
  op (_;_) : EqLitList       EqLitList       -> EqLitList       [ctor ditto] .
  op (_;_) : EqConjList      EqConjList      -> EqConjList      [ctor ditto] .
  op (_;_) : EqDisjList      EqDisjList      -> EqDisjList      [ctor ditto] .
  op (_;_) : EqFormList      EqFormList      -> EqFormList      [ctor ditto] .
  op (_;_) : NormFormList    NormFormList    -> NormFormList    [ctor ditto] .
  op (_;_) : FormList        FormList        -> FormList        [ctor ditto] .

  op (_;_) : PosEqLitNeList  PosEqLitList    -> PosEqLitNeList  [ctor ditto] .
  op (_;_) : PosConjNeList PosConjList   -> PosConjNeList [ctor ditto] .
  op (_;_) : PosDisjNeList PosDisjList   -> PosDisjNeList [ctor ditto] .
  op (_;_) : PosFormNeList PosFormList   -> PosFormNeList [ctor ditto] .
  op (_;_) : NegEqLitNeList  NegEqLitList    -> NegEqLitNeList  [ctor ditto] .
  op (_;_) : NegConjNeList NegConjList   -> NegConjNeList [ctor ditto] .
  op (_;_) : NegDisjNeList NegDisjList   -> NegDisjNeList [ctor ditto] .
  op (_;_) : NegFormNeList NegFormList   -> NegFormNeList [ctor ditto] .
  op (_;_) : EqLitNeList     EqLitList       -> EqLitNeList     [ctor ditto] .
  op (_;_) : EqConjNeList    EqConjList      -> EqConjNeList    [ctor ditto] .
  op (_;_) : EqDisjNeList    EqDisjList      -> EqDisjNeList    [ctor ditto] .
  op (_;_) : EqFormNeList    EqFormList      -> EqFormNeList    [ctor ditto] .
  op (_;_) : NormFormNeList  NormFormList    -> NormFormNeList  [ctor ditto] .
  op (_;_) : FormNeList      FormList        -> FormNeList      [ctor ditto] .

  op (_;_) : PosEqLitList    PosEqLitNeList  -> PosEqLitNeList  [ctor ditto] .
  op (_;_) : PosConjList   PosConjNeList -> PosConjNeList [ctor ditto] .
  op (_;_) : PosDisjList   PosDisjNeList -> PosDisjNeList [ctor ditto] .
  op (_;_) : PosFormList   PosFormNeList -> PosFormNeList [ctor ditto] .
  op (_;_) : NegEqLitList    NegEqLitNeList  -> NegEqLitNeList  [ctor ditto] .
  op (_;_) : NegConjList   NegConjNeList -> NegConjNeList [ctor ditto] .
  op (_;_) : NegDisjList   NegDisjNeList -> NegDisjNeList [ctor ditto] .
  op (_;_) : NegFormList   NegFormNeList -> NegFormNeList [ctor ditto] .
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
  sort PosEqLitSet PosConjSet PosDisjSet PosFormSet .
  sort NegEqLitSet NegConjSet NegDisjSet NegFormSet .
  sort EqLitSet    EqConjSet    EqDisjSet    EqFormSet    .
  sort NormFormSet FormSet .

  sort PosEqLitNeSet PosConjNeSet PosDisjNeSet PosFormNeSet .
  sort NegEqLitNeSet NegConjNeSet NegDisjNeSet NegFormNeSet .
  sort EqLitNeSet    EqConjNeSet    EqDisjNeSet    EqFormNeSet    .
  sort NormFormNeSet FormNeSet .

  subsort PosEqLit  < PosEqLitNeSet  < PosEqLitSet  .
  subsort PosConj < PosConjNeSet < PosConjSet .
  subsort PosDisj < PosDisjNeSet < PosDisjSet .
  subsort PosForm < PosFormNeSet < PosFormSet .
  subsort NegEqLit  < NegEqLitNeSet  < NegEqLitSet  .
  subsort NegConj < NegConjNeSet < NegConjSet .
  subsort NegDisj < NegDisjNeSet < NegDisjSet .
  subsort NegForm < NegFormNeSet < NegFormSet .
  subsort EqLit     < EqLitNeSet     < EqLitSet     .
  subsort EqConj    < EqConjNeSet    < EqConjSet    .
  subsort EqDisj    < EqDisjNeSet    < EqDisjSet    .
  subsort EqForm    < EqFormNeSet    < EqFormSet    .
  subsort NormForm  < NormFormNeSet  < NormFormSet  .
  subsort Form      < FormNeSet      < FormSet      .

  --- Set Functor
  --- Apply Pos/Neg Functor To Sorts
  subsort PosEqLitSet  NegEqLitSet  < EqLitSet  .
  subsort PosConjSet NegConjSet < EqConjSet .
  subsort PosDisjSet NegDisjSet < EqDisjSet .
  subsort PosFormSet NegFormSet < EqFormSet .
  --- Apply Pos/Neg Functor To Subsort Arrows
  subsort PosEqLitSet < PosConjSet PosDisjSet < PosFormSet .
  subsort NegEqLitSet < NegConjSet NegDisjSet < NegFormSet .
  subsort EqLitSet    < EqConjSet    EqDisjSet    < EqFormSet    .
  --- Other Cases
  subsort EmptyFormSet < PosEqLitSet NegEqLitSet FormSet .
  subsort EqFormSet < NormFormSet < FormSet .

  --- NeSet Functor
  --- Apply Pos/Neg Functor To Sorts
  subsort PosEqLitNeSet  NegEqLitNeSet  < EqLitNeSet  .
  subsort PosConjNeSet NegConjNeSet < EqConjNeSet .
  subsort PosDisjNeSet NegDisjNeSet < EqDisjNeSet .
  subsort PosFormNeSet NegFormNeSet < EqFormNeSet .
  --- Apply Pos/Neg Functor To Subsort Arrows
  subsort PosEqLitNeSet < PosConjNeSet PosDisjNeSet < PosFormNeSet .
  subsort NegEqLitNeSet < NegConjNeSet NegDisjNeSet < NegFormNeSet .
  subsort EqLitNeSet    < EqConjNeSet    EqDisjNeSet    < EqFormNeSet    .
  --- Other Cases
  subsort EqFormNeSet < NormFormNeSet < FormNeSet .

  op mtFormSet :                           -> EmptyFormSet   [ctor] .
  op (_|_) : EmptyFormSet   EmptyFormSet   -> EmptyFormSet   [ctor assoc comm id: mtFormSet] .

  op (_|_) : PosEqLitSet    PosEqLitSet    -> PosEqLitSet    [ctor ditto] .
  op (_|_) : PosConjSet   PosConjSet   -> PosConjSet   [ctor ditto] .
  op (_|_) : PosDisjSet   PosDisjSet   -> PosDisjSet   [ctor ditto] .
  op (_|_) : PosFormSet   PosFormSet   -> PosFormSet   [ctor ditto] .
  op (_|_) : NegEqLitSet    NegEqLitSet    -> NegEqLitSet    [ctor ditto] .
  op (_|_) : NegConjSet   NegConjSet   -> NegConjSet   [ctor ditto] .
  op (_|_) : NegDisjSet   NegDisjSet   -> NegDisjSet   [ctor ditto] .
  op (_|_) : NegFormSet   NegFormSet   -> NegFormSet   [ctor ditto] .
  op (_|_) : EqLitSet       EqLitSet       -> EqLitSet       [ctor ditto] .
  op (_|_) : EqConjSet      EqConjSet      -> EqConjSet      [ctor ditto] .
  op (_|_) : EqDisjSet      EqDisjSet      -> EqDisjSet      [ctor ditto] .
  op (_|_) : EqFormSet      EqFormSet      -> EqFormSet      [ctor ditto] .
  op (_|_) : NormFormSet    NormFormSet    -> NormFormSet    [ctor ditto] .
  op (_|_) : FormSet        FormSet        -> FormSet        [ctor ditto] .

  op (_|_) : PosEqLitNeSet  PosEqLitSet    -> PosEqLitNeSet  [ctor ditto] .
  op (_|_) : PosConjNeSet PosConjSet   -> PosConjNeSet [ctor ditto] .
  op (_|_) : PosDisjNeSet PosDisjSet   -> PosDisjNeSet [ctor ditto] .
  op (_|_) : PosFormNeSet PosFormSet   -> PosFormNeSet [ctor ditto] .
  op (_|_) : NegEqLitNeSet  NegEqLitSet    -> NegEqLitNeSet  [ctor ditto] .
  op (_|_) : NegConjNeSet NegConjSet   -> NegConjNeSet [ctor ditto] .
  op (_|_) : NegDisjNeSet NegDisjSet   -> NegDisjNeSet [ctor ditto] .
  op (_|_) : NegFormNeSet NegFormSet   -> NegFormNeSet [ctor ditto] .
  op (_|_) : EqLitNeSet     EqLitSet       -> EqLitNeSet     [ctor ditto] .
  op (_|_) : EqConjNeSet    EqConjSet      -> EqConjNeSet    [ctor ditto] .
  op (_|_) : EqDisjNeSet    EqDisjSet      -> EqDisjNeSet    [ctor ditto] .
  op (_|_) : EqFormNeSet    EqFormSet      -> EqFormNeSet    [ctor ditto] .
  op (_|_) : NormFormNeSet  NormFormSet    -> NormFormNeSet  [ctor ditto] .
  op (_|_) : FormNeSet      FormSet        -> FormNeSet      [ctor ditto] .
endfm
```
