Equational Formulas
===================

For theorem proving purposes with equational logic, there are essentially three
kinds of quantifier free formulas that we care about:

-   true
-   false
-   any formula without true, false, or negation [1]

This follows because:

-   any formula with negation is logically equivalent to one without negation
-   any formula without negation is logically equivalent to a formula in one of
    the three cases above by evaluating away true and false [1]

This library provides formula sorts to cover these core cases and more:

-   `TrueLit`  - the formula `tt`, i.e. the true literal
-   `FalseLit` - the formula `ff`, i.e. the false literal
-   `EqLit`    - positive `_?=_` or `_!=_` negative equality literals
-   `EqForm`   - recursive applications of `/\` and `\/` over `EqLit`formulas
-   `NormForm` - `TrueLit`, `FalseLit`, or `EqForm`
-   `Form`     - the sort of all quantifier-free equational formulas

This library also provides other formula sorts that are needed for programming
purposes:

-   `NonTrivForm` - syntactically non-trivial formulas, i.e. formulas that
                    do not evaluate to `tt` or `ff` by identity equations alone
-   `NoTrueForm`  - `NonTrivForm` or `TrueLit`
-   `NoFalseForm` - `NonTrivForm` or `FalseLit`

Q: Why are these seemingly weird sorts needed?

A: Because `tt` and `ff` are actually _identities_ for the operators `/\` and
`\/` respectively. Sorts `NoTrueForm` and `NoFalseForm` allow us to recurse
properly inside a `/\` or `\/` operator respectively because they forbid their
respective identities from appearing. If we forget to use them and use variables
of sort Form instead, with very high probability, the code will not terminate,
e.g. if the equation (a) `g(F /\ G) = g(F) /\ g(G)` has variables `F` and `G` of
sort `Form`, when we write `g(S ?= T)`, then equation (a) will apply because
`S ?= T` equals `S ?= T /\ tt` by the identity laws, so that, `g(S ?= T)` equals
`g(S ?= T) /\ g(tt)`.

On the other hand, the fact that `/\` and `\/` are also sets allows us to
extract certain subformulas directly using pattern matching alone. When we are
performing operations over these operators as sets, we use `tt` and `ff` as the
set identity operators respectively.

See the NNF module below to see how to traverse over formulas as TREES. See the
NEF, DNF, and CNF modules below for more SET-like examples. Note NEF stands for
Normal Equational Form. It converts formulas of type Form into type NormForm.

[1] More precisely, these claims depend on the underlying formula structure. If
it exposes positive and negative equality literals, as we do, then alll claims
hold. If the negative equality literal is represented by negating a positive
equality literal, then negations cannot be totally evaluated away; in that case,
one would consider a formula to be normalized if it (a) true and false literals
did not appear anywhere (b) negation only appeared directly above a positive
equality literal.

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

Note that, due to preregularity requirements, the number of sorts grows in a
multiplicative fashion every time one applies a functor to the formula data
strcuture. For this reason, to avoid generating many unneeded sorts, we only
generate functor liftings for:

-   Subsorts of `EqForm` (12 sorts in total)
-   `NormForm`
-   `Form`

In practice, these are the primary cases that we care about, due to the
considerations mentioned above.

Note: many data functors (such as list and set) generate a unique sort for
non-empty and possibly empty data structures, so that we actually generate
2 * N + 1 additional sorts each time, where the extra sort comes from a base
sort which helps ensure preregularity in complex cases (this gives 29 sorts
generated for both the list and set functor assuming the 14 above).

```maude
load terms.maude
load ../meta/meta-aux.maude

set include BOOL off .

fmod EQFORM-IMPL{X :: TRIV} is

  sort TrueLit{X}   FalseLit{X}         .
  sort PosEqLit{X}  NegEqLit{X}  EqLit{X}  .
  sort PosEqConj{X} NegEqConj{X} EqConj{X} .
  sort PosEqDisj{X} NegEqDisj{X} EqDisj{X} .
  sort PosEqForm{X} NegEqForm{X} EqForm{X} .

  subsort PosEqLit{X} NegEqLit{X} < EqLit{X} .
  ---
  subsort PosEqLit{X} < PosEqConj{X} PosEqDisj{X} .
  subsort NegEqLit{X} < NegEqConj{X} NegEqDisj{X} .
  subsort EqLit{X}    < EqConj{X}    EqDisj{X}    .
  ---
  subsort PosEqConj{X} NegEqConj{X} < EqConj{X} .
  subsort PosEqDisj{X} NegEqDisj{X} < EqDisj{X} .
  subsort PosEqForm{X} NegEqForm{X} < EqForm{X} .
  ---
  subsort PosEqConj{X} PosEqDisj{X} < PosEqForm{X} .
  subsort NegEqConj{X} NegEqDisj{X} < NegEqForm{X} .
  subsort EqConj{X}    EqDisj{X}    < EqForm{X}    .

  sort TrivTrueForm{X} TrivFalseForm{X} NonTrivForm{X} .
  subsort TrueLit{X}  < TrivTrueForm{X}  .
  subsort FalseLit{X} < TrivFalseForm{X} .
  subsort EqForm{X}   < NonTrivForm{X}   .

  sort NoTrueForm{X} NoFalseForm{X} .
  subsort NonTrivForm{X} TrivFalseForm{X} < NoTrueForm{X}  .
  subsort NonTrivForm{X} TrivTrueForm{X}  < NoFalseForm{X} .

  sort NormLit{X} NormForm{X} .
  subsort EqLit{X} TrueLit{X} FalseLit{X} < NormLit{X} < NormForm{X}  .
  subsort EqForm{X} TrivTrueForm{X} TrivFalseForm{X} < NormForm{X} .

  sort Form{X} .
  subsort NoTrueForm{X} NoFalseForm{X} NormForm{X} < Form{X} .

  --- needed for preregularity with if_then_else_fi
  sort Eq+TrueLit{X} Eq+FalseLit{X} Eq+TrivTrueForm{X} Eq+TrivFalseForm{X} .
  subsort EqLit{X}  TrueLit{X}       < Eq+TrueLit{X}       < Eq+TrivTrueForm{X}  NormLit{X}  .
  subsort EqLit{X}  FalseLit{X}      < Eq+FalseLit{X}      < Eq+TrivFalseForm{X} NormLit{X}  .
  subsort EqForm{X} TrivTrueForm{X}  < Eq+TrivTrueForm{X}  < NoFalseForm{X}      NormForm{X} .
  subsort EqForm{X} TrivFalseForm{X} < Eq+TrivFalseForm{X} < NoTrueForm{X}       NormForm{X} .

  --- add these subsorts and (op ~_ : Lit -> NonTrivLit) to get proper literals
  sort NonTrivLit{X} NoFalseLit{X} NoTrueLit{X} Lit{X} .
  subsort EqLit{X} < NonTrivLit{X} < NonTrivForm{X} .
  subsort NormLit{X} NoFalseLit{X} NoTrueLit{X} < Lit{X} < Form{X} .
  subsort NonTrivLit{X} Eq+TrueLit{X}  < NoFalseLit{X} < NoFalseForm{X} .
  subsort NonTrivLit{X} Eq+FalseLit{X} < NoTrueLit{X}  < NoTrueForm{X}  .

  op tt   :             -> TrueLit{X}     [ctor] .
  op ff   :             -> FalseLit{X}    [ctor] .
  op _?=_ : X$Elt X$Elt -> PosEqLit{X}    [ctor comm prec 50] .
  op _!=_ : X$Elt X$Elt -> NegEqLit{X}    [ctor comm prec 50] .
  op ~_   : Lit{X}      -> NonTrivLit{X}  [ctor prec 51] .
  op ~_   : Form{X}     -> NonTrivForm{X} [ctor prec 51] .

  op _/\_ : TrivTrueForm{X}   TrivTrueForm{X}   -> TrivTrueForm{X}  [ctor assoc comm id: tt prec 52] .
  ---
  op _/\_ : TrivFalseForm{X}  TrivFalseForm{X}  -> TrivFalseForm{X} [ditto] .
  op _/\_ : TrivTrueForm{X}   TrivFalseForm{X}  -> TrivFalseForm{X} [ditto] .
  ---
  op _/\_ : TrivTrueForm{X}   NonTrivForm{X}    -> NonTrivForm{X}   [ditto] .
  op _/\_ : TrivFalseForm{X}  NonTrivForm{X}    -> NonTrivForm{X}   [ditto] .
  ---
  op _/\_ : PosEqConj{X}      PosEqConj{X}      -> PosEqConj{X}     [ditto] .
  op _/\_ : NegEqConj{X}      NegEqConj{X}      -> NegEqConj{X}     [ditto] .
  op _/\_ : EqConj{X}         EqConj{X}         -> EqConj{X}        [ditto] .
  op _/\_ : PosEqForm{X}      PosEqForm{X}      -> PosEqForm{X}     [ditto] .
  op _/\_ : NegEqForm{X}      NegEqForm{X}      -> NegEqForm{X}     [ditto] .
  op _/\_ : EqForm{X}         EqForm{X}         -> EqForm{X}        [ditto] .
  op _/\_ : NonTrivForm{X}    NonTrivForm{X}    -> NonTrivForm{X}   [ditto] .
  op _/\_ : Form{X}           Form{X}           -> Form{X}          [ditto] .

  op _\/_ : TrivFalseForm{X}  TrivFalseForm{X}  -> TrivFalseForm{X} [ctor assoc comm id: ff prec 52] .
  op _\/_ : TrivTrueForm{X}   TrivTrueForm{X}   -> TrivTrueForm{X}  [ditto] .
  op _\/_ : TrivFalseForm{X}  TrivTrueForm{X}   -> TrivTrueForm{X}  [ditto] .
  ---
  op _\/_ : TrivFalseForm{X}  NonTrivForm{X}    -> NonTrivForm{X}   [ditto] .
  op _\/_ : TrivTrueForm{X}   NonTrivForm{X}    -> NonTrivForm{X}   [ditto] .
  ---
  op _\/_ : PosEqDisj{X}      PosEqDisj{X}      -> PosEqDisj{X}     [ditto] .
  op _\/_ : NegEqDisj{X}      NegEqDisj{X}      -> NegEqDisj{X}     [ditto] .
  op _\/_ : EqDisj{X}         EqDisj{X}         -> EqDisj{X}        [ditto] .
  op _\/_ : PosEqForm{X}      PosEqForm{X}      -> PosEqForm{X}     [ditto] .
  op _\/_ : NegEqForm{X}      NegEqForm{X}      -> NegEqForm{X}     [ditto] .
  op _\/_ : EqForm{X}         EqForm{X}         -> EqForm{X}        [ditto] .
  op _\/_ : NonTrivForm{X}    NonTrivForm{X}    -> NonTrivForm{X}   [ditto] .
  op _\/_ : Form{X}           Form{X}           -> Form{X}          [ditto] .
  ---------------------------------------------------------------------------

  var EqL : EqLit{X} .
  vars F F' : Form{X} .

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
    sort TrueLit{Term}          to TrueLit,
    sort FalseLit{Term}         to FalseLit,
    sort PosEqLit{Term}         to PosEqLit,
    sort NegEqLit{Term}         to NegEqLit,
    sort EqLit{Term}            to EqLit,
    sort PosEqConj{Term}        to PosEqConj,
    sort NegEqConj{Term}        to NegEqConj,
    sort EqConj{Term}           to EqConj,
    sort PosEqDisj{Term}        to PosEqDisj,
    sort NegEqDisj{Term}        to NegEqDisj,
    sort EqDisj{Term}           to EqDisj,
    sort PosEqForm{Term}        to PosEqForm,
    sort NegEqForm{Term}        to NegEqForm,
    sort EqForm{Term}           to EqForm,
    sort TrivTrueForm{Term}     to TrivTrueForm,
    sort TrivFalseForm{Term}    to TrivFalseForm,
    sort NonTrivForm{Term}      to NonTrivForm,
    sort NoTrueForm{Term}       to NoTrueForm,
    sort NoFalseForm{Term}      to NoFalseForm,
    sort NormLit{Term}          to NormLit,
    sort NormForm{Term}         to NormForm,
    sort Form{Term}             to Form,
    sort Eq+TrueLit{Term}       to Eq+TrueLit,
    sort Eq+FalseLit{Term}      to Eq+FalseLit,
    sort Eq+TrivTrueForm{Term}  to Eq+TrivTrueForm,
    sort Eq+TrivFalseForm{Term} to Eq+TrivFalseForm,
    sort NonTrivLit{Term}       to NonTrivLit,
    sort NoFalseLit{Term}       to NoFalseLit,
    sort NoTrueLit{Term}        to NoTrueLit,
    sort Lit{Term}              to Lit) .

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

  op mtFormList :                              -> EmptyFormList   [ctor] .
  op (_;_) : EmptyFormList    EmptyFormList    -> EmptyFormList   [ctor assoc id: mtFormList] .

  op (_;_) : PosEqLitList     PosEqLitList     -> PosEqLitList    [ctor ditto] .
  op (_;_) : PosEqConjList    PosEqConjList    -> PosEqConjList   [ctor ditto] .
  op (_;_) : PosEqDisjList    PosEqDisjList    -> PosEqDisjList   [ctor ditto] .
  op (_;_) : PosEqFormList    PosEqFormList    -> PosEqFormList   [ctor ditto] .
  op (_;_) : NegEqLitList     NegEqLitList     -> NegEqLitList    [ctor ditto] .
  op (_;_) : NegEqConjList    NegEqConjList    -> NegEqConjList   [ctor ditto] .
  op (_;_) : NegEqDisjList    NegEqDisjList    -> NegEqDisjList   [ctor ditto] .
  op (_;_) : NegEqFormList    NegEqFormList    -> NegEqFormList   [ctor ditto] .
  op (_;_) : EqLitList        EqLitList        -> EqLitList       [ctor ditto] .
  op (_;_) : EqConjList       EqConjList       -> EqConjList      [ctor ditto] .
  op (_;_) : EqDisjList       EqDisjList       -> EqDisjList      [ctor ditto] .
  op (_;_) : EqFormList       EqFormList       -> EqFormList      [ctor ditto] .
  op (_;_) : NormFormList     NormFormList     -> NormFormList    [ctor ditto] .
  op (_;_) : FormList         FormList         -> FormList        [ctor ditto] .

  op (_;_) : PosEqLitNeList   PosEqLitList     -> PosEqLitNeList  [ctor ditto] .
  op (_;_) : PosEqConjNeList  PosEqConjList    -> PosEqConjNeList [ctor ditto] .
  op (_;_) : PosEqDisjNeList  PosEqDisjList    -> PosEqDisjNeList [ctor ditto] .
  op (_;_) : PosEqFormNeList  PosEqFormList    -> PosEqFormNeList [ctor ditto] .
  op (_;_) : NegEqLitNeList   NegEqLitList     -> NegEqLitNeList  [ctor ditto] .
  op (_;_) : NegEqConjNeList  NegEqConjList    -> NegEqConjNeList [ctor ditto] .
  op (_;_) : NegEqDisjNeList  NegEqDisjList    -> NegEqDisjNeList [ctor ditto] .
  op (_;_) : NegEqFormNeList  NegEqFormList    -> NegEqFormNeList [ctor ditto] .
  op (_;_) : EqLitNeList      EqLitList        -> EqLitNeList     [ctor ditto] .
  op (_;_) : EqConjNeList     EqConjList       -> EqConjNeList    [ctor ditto] .
  op (_;_) : EqDisjNeList     EqDisjList       -> EqDisjNeList    [ctor ditto] .
  op (_;_) : EqFormNeList     EqFormList       -> EqFormNeList    [ctor ditto] .
  op (_;_) : NormFormNeList   NormFormList     -> NormFormNeList  [ctor ditto] .
  op (_;_) : FormNeList       FormList         -> FormNeList      [ctor ditto] .

  op (_;_) : PosEqLitList     PosEqLitNeList   -> PosEqLitNeList  [ctor ditto] .
  op (_;_) : PosEqConjList    PosEqConjNeList  -> PosEqConjNeList [ctor ditto] .
  op (_;_) : PosEqDisjList    PosEqDisjNeList  -> PosEqDisjNeList [ctor ditto] .
  op (_;_) : PosEqFormList    PosEqFormNeList  -> PosEqFormNeList [ctor ditto] .
  op (_;_) : NegEqLitList     NegEqLitNeList   -> NegEqLitNeList  [ctor ditto] .
  op (_;_) : NegEqConjList    NegEqConjNeList  -> NegEqConjNeList [ctor ditto] .
  op (_;_) : NegEqDisjList    NegEqDisjNeList  -> NegEqDisjNeList [ctor ditto] .
  op (_;_) : NegEqFormList    NegEqFormNeList  -> NegEqFormNeList [ctor ditto] .
  op (_;_) : EqLitList        EqLitNeList      -> EqLitNeList     [ctor ditto] .
  op (_;_) : EqConjList       EqConjNeList     -> EqConjNeList    [ctor ditto] .
  op (_;_) : EqDisjList       EqDisjNeList     -> EqDisjNeList    [ctor ditto] .
  op (_;_) : EqFormList       EqFormNeList     -> EqFormNeList    [ctor ditto] .
  op (_;_) : NormFormList     NormFormNeList   -> NormFormNeList  [ctor ditto] .
  op (_;_) : FormList         FormNeList       -> FormNeList      [ctor ditto] .
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
  op (_|_) : EmptyFormSet    EmptyFormSet  -> EmptyFormSet   [ctor assoc comm id: mtFormSet] .

  op (_|_) : PosEqLitSet     PosEqLitSet   -> PosEqLitSet    [ctor ditto] .
  op (_|_) : PosEqConjSet    PosEqConjSet  -> PosEqConjSet   [ctor ditto] .
  op (_|_) : PosEqDisjSet    PosEqDisjSet  -> PosEqDisjSet   [ctor ditto] .
  op (_|_) : PosEqFormSet    PosEqFormSet  -> PosEqFormSet   [ctor ditto] .
  op (_|_) : NegEqLitSet     NegEqLitSet   -> NegEqLitSet    [ctor ditto] .
  op (_|_) : NegEqConjSet    NegEqConjSet  -> NegEqConjSet   [ctor ditto] .
  op (_|_) : NegEqDisjSet    NegEqDisjSet  -> NegEqDisjSet   [ctor ditto] .
  op (_|_) : NegEqFormSet    NegEqFormSet  -> NegEqFormSet   [ctor ditto] .
  op (_|_) : EqLitSet        EqLitSet      -> EqLitSet       [ctor ditto] .
  op (_|_) : EqConjSet       EqConjSet     -> EqConjSet      [ctor ditto] .
  op (_|_) : EqDisjSet       EqDisjSet     -> EqDisjSet      [ctor ditto] .
  op (_|_) : EqFormSet       EqFormSet     -> EqFormSet      [ctor ditto] .
  op (_|_) : NormFormSet     NormFormSet   -> NormFormSet    [ctor ditto] .
  op (_|_) : FormSet         FormSet       -> FormSet        [ctor ditto] .

  op (_|_) : PosEqLitNeSet   PosEqLitSet   -> PosEqLitNeSet  [ctor ditto] .
  op (_|_) : PosEqConjNeSet  PosEqConjSet  -> PosEqConjNeSet [ctor ditto] .
  op (_|_) : PosEqDisjNeSet  PosEqDisjSet  -> PosEqDisjNeSet [ctor ditto] .
  op (_|_) : PosEqFormNeSet  PosEqFormSet  -> PosEqFormNeSet [ctor ditto] .
  op (_|_) : NegEqLitNeSet   NegEqLitSet   -> NegEqLitNeSet  [ctor ditto] .
  op (_|_) : NegEqConjNeSet  NegEqConjSet  -> NegEqConjNeSet [ctor ditto] .
  op (_|_) : NegEqDisjNeSet  NegEqDisjSet  -> NegEqDisjNeSet [ctor ditto] .
  op (_|_) : NegEqFormNeSet  NegEqFormSet  -> NegEqFormNeSet [ctor ditto] .
  op (_|_) : EqLitNeSet      EqLitSet      -> EqLitNeSet     [ctor ditto] .
  op (_|_) : EqConjNeSet     EqConjSet     -> EqConjNeSet    [ctor ditto] .
  op (_|_) : EqDisjNeSet     EqDisjSet     -> EqDisjNeSet    [ctor ditto] .
  op (_|_) : EqFormNeSet     EqFormSet     -> EqFormNeSet    [ctor ditto] .
  op (_|_) : NormFormNeSet   NormFormSet   -> NormFormNeSet  [ctor ditto] .
  op (_|_) : FormNeSet       FormSet       -> FormNeSet      [ctor ditto] .

  var S : Substitution . var NeSS : NeSubstitutionSet . var SS : SubstitutionSet .
  var F : Form         . var FNeS : FormNeSet         . var FS : FormSet .

  op _<<_ : FormSet SubstitutionSet -> [FormSet] .
  ------------------------------------------------
  eq mtFormSet  << SS = mtFormSet .
  eq (F | FNeS) << SS = (F << SS) | (FNeS << SS) .

  eq FS << .SubstitutionSet = mtFormSet .
  eq FS << (S | NeSS)       = (FS << S) | (FS << NeSS) .
endfm
```

```maude
fmod EQFORM-OPERATIONS is
  pr EQFORM .
  pr TERM-EXTRA . --- defines vars() : Term -> QidSet

  vars F F1 F2 : Form . vars PEC1 : PosEqConj .
  vars M : Module . vars T T' : Term .

  op  wellFormed : Module Form -> [Bool] .
  op $wellFormed : Module Form -> [Bool] .
  ----------------------------------------
 ceq  wellFormed(M,F) = $wellFormed(M,F) if wellFormed(M) .

  eq $wellFormed(M,tt)       = true .
  eq $wellFormed(M,ff)       = true .
  eq $wellFormed(M,~ F)      = $wellFormed(M,F) .
 ceq $wellFormed(M,F1 /\ F2) = $wellFormed(M,F1) and-then $wellFormed(M,F2) if F1 =/= tt /\ F2 =/= tt .
 ceq $wellFormed(M,F1 \/ F2) = $wellFormed(M,F1) and-then $wellFormed(M,F2) if F1 =/= ff /\ F2 =/= ff .

  eq $wellFormed(M,T ?= T') = wellFormed(M,T) and-then wellFormed(M,T') and-then sameKind(M,leastSort(M,T),leastSort(M,T')) .
  eq $wellFormed(M,T != T') = wellFormed(M,T) and-then wellFormed(M,T') and-then sameKind(M,leastSort(M,T),leastSort(M,T')) .

  op normalize : Module Form -> [Form] .
  --------------------------------------
  eq normalize(M,tt)       = tt .
  eq normalize(M,ff)       = ff .
  eq normalize(M,~ F)      = ~ normalize(M,F) .
 ceq normalize(M,F1 /\ F2) = normalize(M,F1) /\ normalize(M,F2) if F1 =/= tt /\ F2 =/= tt .
 ceq normalize(M,F1 \/ F2) = normalize(M,F1) \/ normalize(M,F2) if F1 =/= ff /\ F2 =/= ff .

  eq normalize(M,T ?= T') = getTerm(metaNormalize(M,T)) ?= getTerm(metaNormalize(M,T')) .
  eq normalize(M,T != T') = getTerm(metaNormalize(M,T)) != getTerm(metaNormalize(M,T')) .

  op reduce : Module Form -> [Form] .
  -----------------------------------
  eq reduce(M,tt)       = tt .
  eq reduce(M,ff)       = ff .
  eq reduce(M,~ F)      = ~ reduce(M,F) .
 ceq reduce(M,F1 /\ F2) = reduce(M,F1) /\ reduce(M,F2) if F1 =/= tt /\ F2 =/= tt .
 ceq reduce(M,F1 \/ F2) = reduce(M,F1) \/ reduce(M,F2) if F1 =/= ff /\ F2 =/= ff .

  eq reduce(M,T ?= T') = getTerm(metaReduce(M,T)) ?= getTerm(metaReduce(M,T')) .
  eq reduce(M,T != T') = getTerm(metaReduce(M,T)) != getTerm(metaReduce(M,T')) .

  op vars : Form -> QidSet .
  --------------------------
  eq vars(tt)       = none .
  eq vars(ff)       = none .
  eq vars(~ F)      = vars(F) .
 ceq vars(F1 /\ F2) = vars(F1) ; vars(F2) if F1 =/= tt /\ F2 =/= tt .
 ceq vars(F1 \/ F2) = vars(F1) ; vars(F2) if F1 =/= ff /\ F2 =/= ff .

  eq vars(T ?= T') = vars(T) ; vars(T') .
  eq vars(T != T') = vars(T) ; vars(T') .

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

  var C : EqConj . var D : EqDisj .
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
  eq toDisjSet'(tt)     = mtFormSet .
  eq toDisjSet'(ff)     = mtFormSet .
  eq toDisjSet'(D /\ F) = D | toDisjSet'(F) .

  op toConjSet  : Form -> [EqConjSet] .
  op toConjSet' : Form -> [EqConjSet] .
  -------------------------------------
  eq toConjSet (F)      = toConjSet'(dnf(F)) .
  eq toConjSet'(tt)     = mtFormSet .
  eq toConjSet'(ff)     = mtFormSet .
  eq toConjSet'(C \/ F) = C | toConjSet'(F) .

  op toEqSet : PosEqLitSet -> EquationSet .
  -----------------------------------------
  eq toEqSet((T ?= T') | PELS) = (eq T = T' [none] .) toEqSet(PELS) .
  eq toEqSet(mtFormSet)        = none .

  op toPosEqLits : PosEqForm          -> PosEqLitSet .
  op toPosEqLits : UnificationProblem -> PosEqLitSet .
  ----------------------------------------------------
  eq toPosEqLits(tt)      = mtFormSet .
  eq toPosEqLits(ff)      = mtFormSet .
  eq toPosEqLits(T ?= T') = T ?= T' .
  eq toPosEqLits(~ F)     = toPosEqLits(F) .
 ceq toPosEqLits(F /\ F') = toPosEqLits(F) | toPosEqLits(F') if F =/= tt /\ F' =/= tt .
 ceq toPosEqLits(F \/ F') = toPosEqLits(F) | toPosEqLits(F') if F =/= ff /\ F' =/= ff .

  eq toPosEqLits(T =? T' /\ UP) = (T ?= T') | toPosEqLits(UP) .
  eq toPosEqLits(T =? T')       = T ?= T' .
endfm
```
