First-Order Formulae
--------------------

This file implements a first-order logic formula datatype. The main datatype is
defined in FOFORM. Various functionalities are provided to compute normal forms:
NNF, PNF, DNF, and CNF or simplify/rename formulas. These functions are stored
in separate modules from FOFORM and are called through META-LEVEL reflection.
This isolates the modules from one another and simplifies the algorithm design.

```maude
load meta-aux.maude       --- library of extensions to Maude's META-LEVEL module
load variables.maude      --- next-gen renaming library

fmod REFLECT is
  pr META-LEVEL .
  pr UNIT-FM .
  op modReduce   : Module Term   -> [Term] .
  op redReflect  : Qid Term      -> [Term] .
  op sortReflect : Qid Term Type -> [Bool] .
  var Mod : Module .
  var M : Qid .
  var T : Term .
  var TY : Type .
  eq modReduce(Mod,T)    = if Mod =/= noModule then getTerm(metaReduce(Mod,T)) else T fi .
  eq redReflect(M,T)     = getTerm(metaReduce(upModule(M,false),T)) .
  eq sortReflect(M,T,TY) = sortLeq(upModule(M,false),leastSort(upModule(M,false),T),TY) .
endfm

fmod FOFORM is
  pr META-LEVEL .
  --- NOTE: This sort structure is complicated. Edit at your own risk (unless you want to simplify it).
  --- Sort Declarations
  --- Non-Empty/Possibly Empty Forms
  sort TrueLit FalseLit TruthLit PosEqLit NegEqLit Truth+PosEqLit Truth+NegEqLit EqLit Lit .
  sort ConstConj PosEqConj NegEqConj EqConj PosConj NegConj Conj .
  sort ConstDisj PosEqDisj NegEqDisj EqDisj PosDisj NegDisj Disj .
  sort PosEqQFForm NegEqQFForm EqQFForm QFForm AEQForm FOForm .
  sort EmptyForm TruthLit? PosEqLit? NegEqLit? Truth+NegEqLit? Truth+PosEqLit? EqLit? Lit? .
  sort ConstConj? PosEqConj? NegEqConj? EqConj? PosConj? NegConj? Conj? .
  sort ConstDisj? PosEqDisj? NegEqDisj? EqDisj? PosDisj? NegDisj? Disj? .
  sort PosEqQFForm? NegEqQFForm? EqQFForm? QFForm? AEQForm? FOForm? .
  --- Subsorting
  --- Lits
  subsort TrueLit   FalseLit  < TruthLit .
  subsort PosEqLit NegEqLit < EqLit          < Lit .
  subsort TruthLit PosEqLit < Truth+PosEqLit < Lit .
  subsort TruthLit NegEqLit < Truth+NegEqLit < Lit .
  --- Non-Lits
  subsort PosEqConj PosEqDisj  < PosEqQFForm < EqQFForm .
  subsort NegEqConj NegEqDisj  < NegEqQFForm < EqQFForm .
  subsort EqConj EqDisj EqLit < EqQFForm    < QFForm .
  subsort Lit                 < Conj Disj   < QFForm < FOForm .
  subsort AEQForm              < FOForm .
  --- Conjunctions/Disjunctions
  subsort PosEqLit                  < PosEqConj       < PosConj .
  subsort NegEqLit                  < NegEqConj       < NegConj .
  subsort PosEqLit                  < PosEqDisj       < PosDisj .
  subsort NegEqLit                  < NegEqDisj       < NegDisj .
  subsort PosEqConj NegEqConj EqLit < EqConj          < Conj .
  subsort PosEqDisj NegEqDisj EqLit < EqDisj          < Disj .
  subsort TruthLit                  < ConstConj       < PosConj NegConj < Conj .
  subsort TruthLit                  < ConstDisj       < PosDisj NegDisj < Disj .
  subsort Truth+PosEqLit            < PosConj PosDisj .
  subsort Truth+NegEqLit            < NegConj NegDisj .
  --- Link Non-Empty/Possibly Empty Forms
  subsort TruthLit       < TruthLit?       . subsort PosEqLit   < PosEqLit?   .
  subsort Truth+PosEqLit < Truth+PosEqLit? . subsort NegEqLit   < NegEqLit?   .
  subsort EqLit          < EqLit?          . subsort Lit        < Lit?        .
  subsort Truth+NegEqLit < Truth+NegEqLit? . subsort PosEqConj   < PosEqConj?   .
  subsort ConstConj       < ConstConj?       . subsort NegEqConj   < NegEqConj?   .
  subsort EqConj          < EqConj?          . subsort Conj        < Conj?        .
  subsort PosConj         < PosConj?         . subsort PosEqDisj   < PosEqDisj?   .
  subsort NegConj         < NegConj?         . subsort NegEqDisj   < NegEqDisj?   .
  subsort ConstDisj       < ConstDisj?       . subsort PosDisj     < PosDisj?     .
  subsort NegDisj         < NegDisj?         . subsort EqDisj      < EqDisj?      .
  subsort QFForm          < QFForm?          . subsort Disj        < Disj?        .
  subsort FOForm          < FOForm?          . subsort AEQForm     < AEQForm?     .
  subsort EqQFForm        < EqQFForm?        . subsort PosEqQFForm < PosEqQFForm? .
  subsort NegEqQFForm     < NegEqQFForm?     .
  --- Possibly Empty Lits
  subsort EmptyForm < TruthLit?            < Truth+PosEqLit? Truth+NegEqLit? < Lit? .
  subsort EmptyForm < PosEqLit? NegEqLit? < EqLit?                           < Lit? .
  subsort EmptyForm < TruthLit? PosEqLit? < Truth+PosEqLit? .
  subsort EmptyForm < TruthLit? NegEqLit? < Truth+NegEqLit? .
  --- Possibly Empty Non-Lits
  subsort EmptyForm < PosEqConj? PosEqDisj?   < PosEqQFForm? < EqQFForm? .
  subsort EmptyForm < NegEqConj? NegEqDisj?   < NegEqQFForm? < EqQFForm? .
  subsort EmptyForm < EqConj? EqDisj? EqLit? < EqQFForm?    < QFForm?   .
  subsort EmptyForm < Lit?                   < Conj? Disj?  < QFForm?   < FOForm? .
  subsort EmptyForm < AEQForm?                < FOForm? .
  --- Possibly Empty Conjunctions/Disjunctions
  subsort EmptyForm < PosEqLit?                    < PosEqConj?        < PosConj? .
  subsort EmptyForm < NegEqLit?                    < NegEqConj?        < NegConj? .
  subsort EmptyForm < PosEqLit?                    < PosEqDisj?        < PosDisj? .
  subsort EmptyForm < NegEqLit?                    < NegEqDisj?        < NegDisj? .
  subsort EmptyForm < PosEqConj? NegEqConj? EqLit? < EqConj?           < Conj? .
  subsort EmptyForm < PosEqDisj? NegEqDisj? EqLit? < EqDisj?           < Disj? .
  subsort EmptyForm < TruthLit?                    < ConstConj?        < PosConj? NegConj? < Conj? .
  subsort EmptyForm < TruthLit?                    < ConstDisj?        < PosDisj? NegDisj? < Disj? .
  subsort EmptyForm < Truth+PosEqLit?              < PosConj? PosDisj? .
  subsort EmptyForm < Truth+NegEqLit?              < NegConj? NegDisj? .

  --- Litic Formulas
  op  mtForm :           -> EmptyForm [ctor] .
  op  tt     :           -> TrueLit   [ctor] .
  op  ff     :           -> FalseLit  [ctor] .
  op  _?=_   : Term Term -> PosEqLit [ctor comm prec 50] .
  op  _!=_   : Term Term -> NegEqLit [ctor comm prec 50] .
  --- Non-empty Conjunctions/Disjunctions (NeSets)
  op  _/\_   : ConstConj? ConstConj     -> ConstConj   [ctor assoc comm id: mtForm prec 51] .
  op  _/\_   : PosEqConj? PosEqConj     -> PosEqConj   [ctor ditto] .
  op  _/\_   : NegEqConj? NegEqConj     -> NegEqConj   [ctor ditto] .
  op  _/\_   : EqConj? EqConj           -> EqConj      [ctor ditto] .
  op  _/\_   : PosConj? PosConj         -> PosConj     [ctor ditto] .
  op  _/\_   : NegConj? NegConj         -> NegConj     [ctor ditto] .
  op  _/\_   : Conj? Conj               -> Conj        [ctor ditto] .
  op  _/\_   : PosEqQFForm? PosEqQFForm -> PosEqQFForm [ctor ditto] .
  op  _/\_   : NegEqQFForm? NegEqQFForm -> NegEqQFForm [ctor ditto] .
  op  _/\_   : EqQFForm? EqQFForm       -> EqQFForm    [ctor ditto] .
  op  _/\_   : QFForm? QFForm           -> QFForm      [ctor ditto] .
  op  _/\_   : AEQForm? AEQForm         -> AEQForm     [ctor ditto] .
  op  _/\_   : FOForm? FOForm           -> FOForm      [ctor ditto] .
  op  _\/_   : ConstDisj? ConstDisj     -> ConstDisj   [ctor assoc comm id: mtForm prec 51] .
  op  _\/_   : PosEqDisj? PosEqDisj     -> PosEqDisj   [ctor ditto] .
  op  _\/_   : NegEqDisj? NegEqDisj     -> NegEqDisj   [ctor ditto] .
  op  _\/_   : EqDisj? EqDisj           -> EqDisj      [ctor ditto] .
  op  _\/_   : PosDisj? PosDisj         -> PosDisj     [ctor ditto] .
  op  _\/_   : NegDisj? NegDisj         -> NegDisj     [ctor ditto] .
  op  _\/_   : Disj? Disj               -> Disj        [ctor ditto] .
  op  _\/_   : PosEqQFForm? PosEqQFForm -> PosEqQFForm [ctor ditto] .
  op  _\/_   : NegEqQFForm? NegEqQFForm -> NegEqQFForm [ctor ditto] .
  op  _\/_   : EqQFForm? EqQFForm       -> EqQFForm    [ctor ditto] .
  op  _\/_   : QFForm? QFForm           -> QFForm      [ctor ditto] .
  op  _\/_   : AEQForm? AEQForm         -> AEQForm     [ctor ditto] .
  op  _\/_   : FOForm? FOForm           -> FOForm      [ctor ditto] .
  --- Possibly Empty Conjunctions/Disjunctions (Sets)
  op  _/\_   : PosEqConj? PosEqConj?     -> PosEqConj?   [ctor ditto] .
  op  _/\_   : NegEqConj? NegEqConj?     -> NegEqConj?   [ctor ditto] .
  op  _/\_   : EqConj? EqConj?           -> EqConj?      [ctor ditto] .
  op  _/\_   : EmptyForm EmptyForm       -> EmptyForm    [ctor ditto] .
  op  _/\_   : ConstConj? ConstConj?     -> ConstConj?   [ctor ditto] .
  op  _/\_   : PosConj? PosConj?         -> PosConj?     [ctor ditto] .
  op  _/\_   : NegConj? NegConj?         -> NegConj?     [ctor ditto] .
  op  _/\_   : Conj? Conj?               -> Conj?        [ctor ditto] .
  op  _/\_   : PosEqQFForm? PosEqQFForm? -> PosEqQFForm? [ctor ditto] .
  op  _/\_   : NegEqQFForm? NegEqQFForm? -> NegEqQFForm? [ctor ditto] .
  op  _/\_   : EqQFForm? EqQFForm?       -> EqQFForm?    [ctor ditto] .
  op  _/\_   : QFForm? QFForm?           -> QFForm?      [ctor ditto] .
  op  _/\_   : AEQForm? AEQForm?         -> AEQForm?     [ctor ditto] .
  op  _/\_   : FOForm? FOForm?           -> FOForm?      [ctor ditto] .
  op  _\/_   : EmptyForm EmptyForm       -> EmptyForm    [ctor ditto] .
  op  _\/_   : ConstDisj? ConstDisj?     -> ConstDisj?   [ctor ditto] .
  op  _\/_   : PosEqDisj? PosEqDisj?     -> PosEqDisj?   [ctor ditto] .
  op  _\/_   : NegEqDisj? NegEqDisj?     -> NegEqDisj?   [ctor ditto] .
  op  _\/_   : EqDisj? EqDisj?           -> EqDisj?      [ctor ditto] .
  op  _\/_   : PosDisj? PosDisj?         -> PosDisj?     [ctor ditto] .
  op  _\/_   : NegDisj? NegDisj?         -> NegDisj?     [ctor ditto] .
  op  _\/_   : Disj? Disj?               -> Disj?        [ctor ditto] .
  op  _\/_   : PosEqQFForm? PosEqQFForm? -> PosEqQFForm? [ctor ditto] .
  op  _\/_   : NegEqQFForm? NegEqQFForm? -> NegEqQFForm? [ctor ditto] .
  op  _\/_   : EqQFForm? EqQFForm?       -> EqQFForm?    [ctor ditto] .
  op  _\/_   : QFForm? QFForm?           -> QFForm?      [ctor ditto] .
  op  _\/_   : AEQForm? AEQForm?         -> AEQForm?     [ctor ditto] .
  op  _\/_   : FOForm? FOForm?           -> FOForm?      [ctor ditto] .
  --- Negations and Quantifiers
  op  ~_     : QFForm           -> QFForm  [ctor prec 49] .
  op  ~_     : AEQForm          -> AEQForm [ctor ditto] .
  op  ~_     : FOForm           -> FOForm  [ctor ditto] .
  op  A[_]_  : NeQidSet QFForm  -> AEQForm [ctor prec 52] .
  op  E[_]_  : NeQidSet QFForm  -> AEQForm [ctor prec 52] .
  op  A[_]_  : NeQidSet AEQForm -> AEQForm [ctor ditto] .
  op  E[_]_  : NeQidSet AEQForm -> AEQForm [ctor ditto] .
  op  A[_]_  : NeQidSet FOForm  -> FOForm  [ctor ditto] .
  op  E[_]_  : NeQidSet FOForm  -> FOForm  [ctor ditto] .
  op  A[_]_  : QidSet QFForm    -> AEQForm [ditto] .
  op  E[_]_  : QidSet QFForm    -> AEQForm [ditto] .
  op  A[_]_  : QidSet AEQForm   -> AEQForm [ditto] .
  op  E[_]_  : QidSet AEQForm   -> AEQForm [ditto] .
  op  A[_]_  : QidSet FOForm    -> FOForm  [ditto] .
  op  E[_]_  : QidSet FOForm    -> FOForm  [ditto] .
  --- Error Terms
  op error   : String -> [FOForm] [ctor] .
  --- Remove useless quantifiers
  eq  A[none] F:AEQForm = F:AEQForm .
  eq  E[none] F:AEQForm = F:AEQForm .
endfm

fmod FOFORMSET is
  pr FOFORM .
  sort FormEmptySet .
  sort TruthLitSet PosEqLitSet NegEqLitSet Truth+PosEqLitSet Truth+NegEqLitSet EqLitSet LitSet .
  sort ConstConjSet PosEqConjSet NegEqConjSet EqConjSet PosConjSet NegConjSet ConjSet .
  sort ConstDisjSet PosEqDisjSet NegEqDisjSet EqDisjSet PosDisjSet NegDisjSet DisjSet .
  sort PosEqQFFormSet NegEqQFFormSet EqQFFormSet QFFormSet AEQFormSet FOFormSet .
  sort EmptyFormSet PosEqLit?Set NegEqLit?Set TruthLit?Set Truth+NegEqLit?Set Truth+PosEqLit?Set EqLit?Set Lit?Set .
  sort ConstConj?Set PosEqConj?Set NegEqConj?Set EqConj?Set PosConj?Set NegConj?Set Conj?Set .
  sort ConstDisj?Set PosEqDisj?Set NegEqDisj?Set EqDisj?Set PosDisj?Set NegDisj?Set Disj?Set .
  sort PosEqQFForm?Set NegEqQFForm?Set EqQFForm?Set QFForm?Set AEQForm?Set FOForm?Set .
  --- Subsorting
  subsort EmptyForm       < EmptyFormSet       .
  subsort TruthLit       < TruthLitSet       . subsort TruthLit?       < TruthLit?Set       .
  subsort PosEqLit       < PosEqLitSet       . subsort PosEqLit?       < PosEqLit?Set       .
  subsort NegEqLit       < NegEqLitSet       . subsort NegEqLit?       < NegEqLit?Set       .
  subsort Truth+PosEqLit < Truth+PosEqLitSet . subsort Truth+PosEqLit? < Truth+PosEqLit?Set .
  subsort Truth+NegEqLit < Truth+NegEqLitSet . subsort Truth+NegEqLit? < Truth+NegEqLit?Set .
  subsort EqLit          < EqLitSet          . subsort EqLit?          < EqLit?Set          .
  subsort Lit            < LitSet            . subsort Lit?            < Lit?Set            .
  subsort ConstConj       < ConstConjSet       . subsort ConstConj?       < ConstConj?Set       .
  subsort PosEqConj       < PosEqConjSet       . subsort PosEqConj?       < PosEqConj?Set       .
  subsort NegEqConj       < NegEqConjSet       . subsort NegEqConj?       < NegEqConj?Set       .
  subsort EqConj          < EqConjSet          . subsort EqConj?          < EqConj?Set          .
  subsort PosConj         < PosConjSet         . subsort PosConj?         < PosConj?Set         .
  subsort NegConj         < NegConjSet         . subsort NegConj?         < NegConj?Set         .
  subsort Conj            < ConjSet            . subsort Conj?            < Conj?Set            .
  subsort ConstDisj       < ConstDisjSet       . subsort ConstDisj?       < ConstDisj?Set       .
  subsort PosEqDisj       < PosEqDisjSet       . subsort PosEqDisj?       < PosEqDisj?Set       .
  subsort NegEqDisj       < NegEqDisjSet       . subsort NegEqDisj?       < NegEqDisj?Set       .
  subsort EqDisj          < EqDisjSet          . subsort EqDisj?          < EqDisj?Set          .
  subsort PosDisj         < PosDisjSet         . subsort PosDisj?         < PosDisj?Set         .
  subsort NegDisj         < NegDisjSet         . subsort NegDisj?         < NegDisj?Set         .
  subsort Disj            < DisjSet            . subsort Disj?            < Disj?Set            .
  subsort PosEqQFForm     < PosEqQFFormSet     . subsort PosEqQFForm?     < PosEqQFForm?Set     .
  subsort NegEqQFForm     < NegEqQFFormSet     . subsort NegEqQFForm?     < NegEqQFForm?Set     .
  subsort EqQFForm        < EqQFFormSet        . subsort EqQFForm?        < EqQFForm?Set        .
  subsort QFForm          < QFFormSet          . subsort QFForm?          < QFForm?Set          .
  subsort AEQForm         < AEQFormSet         . subsort AEQForm?         < AEQForm?Set         .
  subsort FOForm          < FOFormSet          . subsort FOForm?          < FOForm?Set          .
  --- Lits Sets
  subsort FormEmptySet < PosEqLitSet NegEqLitSet < EqLitSet          < LitSet .
  subsort FormEmptySet < TruthLitSet PosEqLitSet < Truth+PosEqLitSet < LitSet .
  subsort FormEmptySet < TruthLitSet NegEqLitSet < Truth+NegEqLitSet < LitSet .
  --- Non-Lit Sets
  subsort PosEqConjSet PosEqDisjSet    < PosEqQFFormSet     < EqQFFormSet .
  subsort NegEqConjSet NegEqDisjSet    < NegEqQFFormSet     < EqQFFormSet .
  subsort EqConjSet EqDisjSet EqLitSet < EqQFFormSet        < QFFormSet .
  subsort LitSet                       < ConjSet DisjSet    < QFFormSet   < FOFormSet .
  subsort FormEmptySet                 < AEQFormSet         < FOFormSet .
  --- Conjunctions/Disjunctions Sets
  subsort PosEqLitSet                        < PosEqConjSet          < PosConjSet .
  subsort NegEqLitSet                        < NegEqConjSet          < NegConjSet .
  subsort PosEqLitSet                        < PosEqDisjSet          < PosDisjSet .
  subsort NegEqLitSet                        < NegEqDisjSet          < NegDisjSet .
  subsort PosEqConjSet NegEqConjSet EqLitSet < EqConjSet             < ConjSet .
  subsort PosEqDisjSet NegEqDisjSet EqLitSet < EqDisjSet             < DisjSet .
  subsort TruthLitSet                        < ConstConjSet          < PosConjSet NegConjSet < ConjSet .
  subsort TruthLitSet                        < ConstDisjSet          < PosDisjSet NegDisjSet < DisjSet .
  subsort Truth+PosEqLitSet                  < PosConjSet PosDisjSet .
  subsort Truth+NegEqLitSet                  < NegConjSet NegDisjSet .
  --- Link Non-Empty/Possibly Empty Forms
  subsort TruthLitSet       < TruthLit?Set       . subsort PosEqLitSet       < PosEqLit?Set       .
  subsort NegEqLitSet       < NegEqLit?Set       . subsort EqLitSet          < EqLit?Set          .
  subsort Truth+PosEqLitSet < Truth+PosEqLit?Set . subsort Truth+NegEqLitSet < Truth+NegEqLit?Set .
  subsort LitSet            < Lit?Set            . subsort NegEqConjSet       < NegEqConj?Set       .
  subsort PosEqConjSet       < PosEqConj?Set       . subsort EqConjSet          < EqConj?Set          .
  subsort ConstConjSet       < ConstConj?Set       . subsort ConjSet            < Conj?Set            .
  subsort PosConjSet         < PosConj?Set         . subsort PosEqDisjSet       < PosEqDisj?Set       .
  subsort NegConjSet         < NegConj?Set         . subsort NegEqDisjSet       < NegEqDisj?Set       .
  subsort ConstDisjSet       < ConstDisj?Set       . subsort EqDisjSet          < EqDisj?Set          .
  subsort PosDisjSet         < PosDisj?Set         . subsort DisjSet            < Disj?Set            .
  subsort NegDisjSet         < NegDisj?Set         . subsort AEQFormSet         < AEQForm?Set         .
  subsort PosEqQFFormSet     < PosEqQFForm?Set     . subsort NegEqQFFormSet     < NegEqQFForm?Set     .
  subsort EqQFFormSet        < EqQFForm?Set        . subsort QFFormSet          < QFForm?Set          .
  subsort FOFormSet          < FOForm?Set          .
  --- Possibly Empty Lits Sets
  subsort FormEmptySet < EmptyFormSet .
  subsort EmptyFormSet < PosEqLit?Set NegEqLit?Set < EqLit?Set          < Lit?Set .
  subsort EmptyFormSet < TruthLit?Set PosEqLit?Set < Truth+PosEqLit?Set < Lit?Set .
  subsort EmptyFormSet < TruthLit?Set NegEqLit?Set < Truth+NegEqLit?Set < Lit?Set .
  --- Possibly Empty Non-Lit Sets
  subsort PosEqConj?Set PosEqDisj?Set     < PosEqQFForm?Set   < EqQFForm?Set .
  subsort NegEqConj?Set NegEqDisj?Set     < NegEqQFForm?Set   < EqQFForm?Set .
  subsort EqConj?Set EqDisj?Set EqLit?Set < EqQFForm?Set      < QFForm?Set .
  subsort Lit?Set                         < Conj?Set Disj?Set < QFForm?Set < FOForm?Set .
  subsort EmptyFormSet                    < AEQForm?Set       < FOForm?Set .
  --- Possibly Empty Conjunctions/Disjunctions
  subsort EmptyFormSet < PosEqLit?Set                          < PosEqConj?Set           < PosConj?Set .
  subsort EmptyFormSet < NegEqLit?Set                          < NegEqConj?Set           < NegConj?Set .
  subsort EmptyFormSet < PosEqLit?Set                          < PosEqDisj?Set           < PosDisj?Set .
  subsort EmptyFormSet < NegEqLit?Set                          < NegEqDisj?Set           < NegDisj?Set .
  subsort EmptyFormSet < PosEqConj?Set NegEqConj?Set EqLit?Set < EqConj?Set              < Conj?Set .
  subsort EmptyFormSet < PosEqDisj?Set NegEqDisj?Set EqLit?Set < EqDisj?Set              < Disj?Set .
  subsort EmptyFormSet < TruthLit?Set                          < ConstConj?Set           < PosConj?Set NegConj?Set < Conj?Set .
  subsort EmptyFormSet < TruthLit?Set                          < ConstDisj?Set           < PosDisj?Set NegDisj?Set < Disj?Set .
  subsort EmptyFormSet < Truth+PosEqLit?Set                    < PosConj?Set PosDisj?Set .
  subsort EmptyFormSet < Truth+NegEqLit?Set                    < NegConj?Set NegDisj?Set .
  --- Empty [Formula Sets]
  op mtFormSet :                                    -> FormEmptySet        [ctor] .
  op _|_ : FormEmptySet        FormEmptySet         -> FormEmptySet        [ctor assoc comm id: mtFormSet prec 53] .
  --- [Non-Empty Formula] Sets
  op _|_ : PosEqLitSet         PosEqLitSet          -> PosEqLitSet         [ctor ditto] .
  op _|_ : NegEqLitSet         NegEqLitSet          -> NegEqLitSet         [ctor ditto] .
  op _|_ : TruthLitSet         TruthLitSet          -> TruthLitSet         [ctor ditto] .
  op _|_ : Truth+PosEqLitSet   Truth+PosEqLitSet    -> Truth+PosEqLitSet   [ctor ditto] .
  op _|_ : Truth+NegEqLitSet   Truth+NegEqLitSet    -> Truth+NegEqLitSet   [ctor ditto] .
  op _|_ : EqLitSet            EqLitSet             -> EqLitSet            [ctor ditto] .
  op _|_ : LitSet              LitSet               -> LitSet              [ctor ditto] .
  op _|_ : ConstConjSet        ConstConjSet         -> ConstConjSet        [ctor ditto] .
  op _|_ : PosEqConjSet        PosEqConjSet         -> PosEqConjSet        [ctor ditto] .
  op _|_ : NegEqConjSet        NegEqConjSet         -> NegEqConjSet        [ctor ditto] .
  op _|_ : EqConjSet           EqConjSet            -> EqConjSet           [ctor ditto] .
  op _|_ : PosConjSet          PosConjSet           -> PosConjSet          [ctor ditto] .
  op _|_ : NegConjSet          NegConjSet           -> NegConjSet          [ctor ditto] .
  op _|_ : ConjSet             ConjSet              -> ConjSet             [ctor ditto] .
  op _|_ : ConstDisjSet        ConstDisjSet         -> ConstDisjSet        [ctor ditto] .
  op _|_ : PosEqDisjSet        PosEqDisjSet         -> PosEqDisjSet        [ctor ditto] .
  op _|_ : NegEqDisjSet        NegEqDisjSet         -> NegEqDisjSet        [ctor ditto] .
  op _|_ : EqDisjSet           EqDisjSet            -> EqDisjSet           [ctor ditto] .
  op _|_ : PosDisjSet          PosDisjSet           -> PosDisjSet          [ctor ditto] .
  op _|_ : NegDisjSet          NegDisjSet           -> NegDisjSet          [ctor ditto] .
  op _|_ : DisjSet             DisjSet              -> DisjSet             [ctor ditto] .
  op _|_ : NegEqQFFormSet      NegEqQFFormSet       -> NegEqQFFormSet      [ctor ditto] .
  op _|_ : PosEqQFFormSet      PosEqQFFormSet       -> PosEqQFFormSet      [ctor ditto] .
  op _|_ : EqQFFormSet         EqQFFormSet          -> EqQFFormSet         [ctor ditto] .
  op _|_ : QFFormSet           QFFormSet            -> QFFormSet           [ctor ditto] .
  op _|_ : AEQFormSet          AEQFormSet           -> AEQFormSet          [ctor ditto] .
  op _|_ : FOFormSet           FOFormSet            -> FOFormSet           [ctor ditto] .
  --- [Possibly Empty Formula] Sets
  op _|_ : EmptyFormSet         EmptyFormSet        -> EmptyFormSet        [ctor ditto] .
  op _|_ : PosEqLit?Set         PosEqLit?Set        -> PosEqLit?Set        [ctor ditto] .
  op _|_ : NegEqLit?Set         NegEqLit?Set        -> NegEqLit?Set        [ctor ditto] .
  op _|_ : TruthLit?Set         TruthLit?Set        -> TruthLit?Set        [ctor ditto] .
  op _|_ : Truth+PosEqLit?Set   Truth+PosEqLit?Set  -> Truth+PosEqLit?Set  [ctor ditto] .
  op _|_ : Truth+NegEqLit?Set   Truth+NegEqLit?Set  -> Truth+NegEqLit?Set  [ctor ditto] .
  op _|_ : EqLit?Set            EqLit?Set           -> EqLit?Set           [ctor ditto] .
  op _|_ : Lit?Set              Lit?Set             -> Lit?Set             [ctor ditto] .
  op _|_ : ConstConj?Set        ConstConj?Set       -> ConstConj?Set       [ctor ditto] .
  op _|_ : PosEqConj?Set        PosEqConj?Set       -> PosEqConj?Set       [ctor ditto] .
  op _|_ : NegEqConj?Set        NegEqConj?Set       -> NegEqConj?Set       [ctor ditto] .
  op _|_ : EqConj?Set           EqConj?Set          -> EqConj?Set          [ctor ditto] .
  op _|_ : PosConj?Set          PosConj?Set         -> PosConj?Set         [ctor ditto] .
  op _|_ : NegConj?Set          NegConj?Set         -> NegConj?Set         [ctor ditto] .
  op _|_ : Conj?Set             Conj?Set            -> Conj?Set            [ctor ditto] .
  op _|_ : ConstDisj?Set        ConstDisj?Set       -> ConstDisj?Set       [ctor ditto] .
  op _|_ : PosEqDisj?Set        PosEqDisj?Set       -> PosEqDisj?Set       [ctor ditto] .
  op _|_ : NegEqDisj?Set        NegEqDisj?Set       -> NegEqDisj?Set       [ctor ditto] .
  op _|_ : EqDisj?Set           EqDisj?Set          -> EqDisj?Set          [ctor ditto] .
  op _|_ : PosDisj?Set          PosDisj?Set         -> PosDisj?Set         [ctor ditto] .
  op _|_ : NegDisj?Set          NegDisj?Set         -> NegDisj?Set         [ctor ditto] .
  op _|_ : Disj?Set             Disj?Set            -> Disj?Set            [ctor ditto] .
  op _|_ : NegEqQFForm?Set      NegEqQFForm?Set     -> NegEqQFForm?Set     [ctor ditto] .
  op _|_ : PosEqQFForm?Set      PosEqQFForm?Set     -> PosEqQFForm?Set     [ctor ditto] .
  op _|_ : EqQFForm?Set         EqQFForm?Set        -> EqQFForm?Set        [ctor ditto] .
  op _|_ : QFForm?Set           QFForm?Set          -> QFForm?Set          [ctor ditto] .
  op _|_ : AEQForm?Set          AEQForm?Set         -> AEQForm?Set         [ctor ditto] .
  op _|_ : FOForm?Set           FOForm?Set          -> FOForm?Set          [ctor ditto] .
endfm

fmod FOFORMBASICLIST is
  pr FOFORM .
  sort FormEmptyList .
  sort QFForm?List FOForm?List .
  subsort FormEmptyList QFForm? < QFForm?List .
  subsort FormEmptyList FOForm? < FOForm?List .
  subsort QFForm?List < FOForm?List .
  op nilFormList :                     -> FormEmptyList [ctor] .
  op _;_ : FormEmptyList FormEmptyList -> FormEmptyList [ctor assoc id: nilFormList] .
  op _;_ : FOForm?List   FOForm?List   -> FOForm?List   [ctor ditto] .
  op _;_ : QFForm?List   QFForm?List   -> QFForm?List   [ctor ditto] .
endfm

fmod FOFORM-CONVERSION is
  pr FOFORMSET .
  pr FOFORMBASICLIST .
  op set2list : FOForm?Set  -> FOForm?List .
  op list2set : FOForm?List -> FOForm?Set  .
  var F : FOForm? . var FS : FOForm?Set . var FL : FOForm?List .
  eq set2list(F | FS)       = F ; set2list(FS) .
  eq set2list(mtFormSet)    = nilFormList .
  eq list2set(F ; FL)       = F | list2set(FL) .
  eq list2set(nilFormList)  = mtFormSet .
endfm

fmod FOFORM-DEFINEDOPS is
  pr FOFORM .
  op _=>_  : FOForm FOForm -> FOForm [ctor] .
  op _<=>_ : FOForm FOForm -> FOForm [ctor] .
  var F1 F2 : FOForm .
  eq F1  => F2 = (~ F1) \/ F2 .
  eq F1 <=> F2 = (F1 => F2) /\ (F2 => F1) .
endfm

fmod FOFORMSIMPLIFY-IMP-IMPL is
  pr FOFORM .
  var F G H K : FOForm . var K? : FOForm? .
  var C : Conj . var D : Disj . var T T' : Term .

  --- Repeated Subformula
  eq F /\ F = F .
  eq F \/ F = F .

  --- Implication
  eq (~ (F /\ G)) \/  F               = tt .
  eq (~  F      ) \/ ((F \/ K?) /\ H) = (~ F) \/ H .
  eq (~ (F /\ G)) \/ ((F \/ K?) /\ H) = (~ G) \/ H .

  --- Break up implication into clauses
  eq (~ (F /\ (G \/ H)) ) \/ K = ((~ (F /\ G)) \/ K) /\ ((~ (F /\ H)) \/ K) .
endfm

fmod FOFORMSIMPLIFY-IMPL is
  pr FOFORM .
  var F G H K : FOForm . var K? : FOForm? .
  var C : Conj . var D : Disj . var T T' : Term .

  --- Repeated subformula in Conj/Disj
  eq F /\ F = F .
  eq F \/ F = F .
  --- Negated TruthLit
  eq ~ tt = ff .
  eq ~ ff = tt .
  --- TruthLit in Conj/Disj
  eq ff /\ C = ff .
  eq tt /\ C = C  .
  eq tt \/ D = tt .
  eq ff \/ D = D  .
  --- Negated Formula
  eq F  \/ ~ F = tt .
  eq F  /\ ~ F = ff .

  --- eq T ?= T' /\ T != T' /\ C = ff .
  --- eq T ?= T' \/ T != T' \/ D = tt .
  eq (T ?= T' /\ T != T') = ff .
  eq (T ?= T' \/ T != T') = tt .

  --- Trivial Equality/Disequality
  eq T ?= T = tt .
  eq T != T = ff .
endfm

fmod FOFORMSIMPLIFY is
  pr FOFORM .
  pr REFLECT .
  op simplify : FOForm -> FOForm .
  var F : FOForm .
  eq simplify(F) = downTerm(redReflect('FOFORMSIMPLIFY-IMPL,upTerm(F)),error("FOForm Simplify Failed")) .
endfm

fmod FOFORMREDUCE-IMPL is
  pr FOFORM .   ---
  pr TERM-EXTRA . --- vars() function
  op red  : Module Bool FOForm? ~> FOForm? .
  op red  : Module Bool FOForm  ~> FOForm  .
  op red  : Module Bool EqLit  ~> Lit  .
  op redL : Module Lit Lit Term Term ~> Lit .
  op noteq   : Module Term Term -> Bool  .
  var F F' : FOForm . var T T' : Term . var M : Module . var E : EqLit .
  var Q  : NeQidSet . var AT AF : TruthLit . var B : Bool .
  eq red(M,B,F /\ F')   = red(M,B,F) /\ red(M,B,F') .
  eq red(M,B,F \/ F')   = red(M,B,F) \/ red(M,B,F') .
  eq red(M,B,mtForm)    = mtForm .
  eq red(M,B,A[Q] F)    = A[Q] red(M,B,F) .
  eq red(M,B,E[Q] F)    = E[Q] red(M,B,F) .
  eq red(M,B,~ F)       = ~ red(M,B,F) .
  eq red(M,B,AT)        = AT .
  eq red(M,B,T ?= T')   = if B
                             then redL(M,tt,ff,getTerm(metaReduce(M,T)),getTerm(metaReduce(M,T')))
                             else getTerm(metaReduce(M,T)) ?= getTerm(metaReduce(M,T'))
			  fi .
  eq red(M,B,T != T')   = if B
                             then redL(M,ff,tt,getTerm(metaReduce(M,T)),getTerm(metaReduce(M,T')))
			     else getTerm(metaReduce(M,T)) != getTerm(metaReduce(M,T'))
			  fi .
  --- if both sides are ground, check equality
 ceq redL(M,AT,AF,T,T') = if T == T' then AT else AF fi if vars('a[T,T']) == none .
  --- if one side is ground, check disequality via least sorts (if possible)
 ceq redL(M,AT,AF,T,T') = AF if noteq(M,T,T') or-else noteq(M,T',T) .
  --- otherwise, return simplified atom
  eq redL(M,AT,AF,T,T') = if AT == tt then T ?= T' else T != T' fi [owise] .
  --- INP: Module Term1 Term2
  --- PRE: Term1 and Term2 are well-defined in Module
  --- OUT: true iff Term1 is ground and its least sort is greater than Term2;
  ---      this implies Term1 is NOT equal to Term2
  eq noteq(M,T,T') = (vars(T) == none and-then
    sortLeq(M,leastSort(M,T'),leastSort(M,T)) and-then leastSort(M,T) =/= leastSort(M,T')) == true .
endfm

fmod FOFORMREDUCE is
  pr FOFORM .
  pr REFLECT .
  op  reduce : Module FOForm ~> FOForm .
  op  reduce : Module Bool FOForm ~> FOForm .
  op $reduce : [FOForm] -> FOForm .
  var M : Module . var F : FOForm . var S : String . var G : [FOForm] . var B : Bool .
  eq  reduce(M,F)      = reduce(M,true,F) .
  eq  reduce(M,B,F)    = downTerm(redReflect('FOFORMREDUCE-IMPL,'red[upTerm(M),upTerm(B),upTerm(F)]),error("FOForm Reduce Failed")) .
  eq $reduce(F)        = F .
  eq $reduce(error(S)) = error(S) .
  eq $reduce(G)        = error("Formula IllFormed") [owise] .
endfm

fmod FOFORM-OPERATIONS is
  pr FOFORM .
  pr TERM-EXTRA . --- defines vars() : Term -> QidSet
  op  size       : FOForm? -> Nat .
  op  depth      : FOForm? -> Nat .
  op  wellFormed : Module FOForm? -> Bool .
  op $wellFormed : Module FOForm? -> Bool .
  op  normalize  : Module FOForm? -> FOForm? .
  op  toUnifProb : PosConj -> UnificationProblem .
  op $toUnifProb : PosConj -> UnificationProblem .
  op  trueId     : FOForm? -> FOForm  .
  op  falseId    : FOForm? -> FOForm  .
  op  true2mt    : FOForm? -> FOForm? .
  op  false2mt   : FOForm? -> FOForm? .
  op  vars       : FOForm? -> QidSet  .
  var M : Module . var F? : FOForm? . var F1 F2 : FOForm . var QS : NeQidSet .
  var TA : TruthLit . var T T' : Term . var PC : PosConj . var C : Conj .
  --- get the size/depth of a formula
  eq size(A[QS] F1)  = s(size(F1)) .
  eq size(E[QS] F1)  = s(size(F1)) .
  eq size(F1 /\ F2)  = s(size(F1) + size(F2)) .
  eq size(F1 \/ F2)  = s(size(F1) + size(F2)) .
  eq size(~ F1)      = s(size(F1)) .
  eq size(A:Lit)    = 1 .
  eq size(mtForm)    = 0 .
  eq depth(A[QS] F1) = s(depth(F1)) .
  eq depth(E[QS] F1) = s(depth(F1)) .
  eq depth(F1 /\ F2) = s(max(depth(F1),depth(F2))) .
  eq depth(F1 \/ F2) = s(max(depth(F1),depth(F2))) .
  eq depth(~ F1)     = s(depth(F1)) .
  eq depth(A:Lit)   = 1 .
  eq depth(mtForm)   = 0 .
  --- INP: Module FOForm?
  --- PRE: N/A
  --- OUT: true iff FOForm? is a wellFormed formula over module M
  --- non-lits
 ceq  wellFormed(M,F?)       = $wellFormed(M,F?) if wellFormed(M) .
  eq $wellFormed(M,F1 /\ F2) = $wellFormed(M,F1) and-then $wellFormed(M,F2) .
  eq $wellFormed(M,F1 \/ F2) = $wellFormed(M,F1) and-then $wellFormed(M,F2) .
  eq $wellFormed(M,~ F1)     = $wellFormed(M,F1) .
  eq $wellFormed(M,A[QS] F1) = $wellFormed(M,F1) .
  eq $wellFormed(M,E[QS] F1) = $wellFormed(M,F1) .
  --- eq lit
  eq $wellFormed(M,T ?= T')  = wellFormed(M,T) and-then wellFormed(M,T') and-then sameKind(M,leastSort(M,T),leastSort(M,T')) .
  eq $wellFormed(M,T != T')  = wellFormed(M,T) and-then wellFormed(M,T') and-then sameKind(M,leastSort(M,T),leastSort(M,T')) .
  --- true/false lit or mtForm
  eq $wellFormed(M,TA)       = true .
  eq $wellFormed(M,mtForm)   = true .
  --- PRE: FOForm is well-formed in Module
  --- OUT: A metanormalized formula
  --- INP: FOForm?
  eq normalize(M,F1 /\ F2) = normalize(M,F1) /\ normalize(M,F2) .
  eq normalize(M,F1 \/ F2) = normalize(M,F1) \/ normalize(M,F2) .
  eq normalize(M,~ F1)     = ~ normalize(M,F1) .
  eq normalize(M,A[QS] F1) = A[QS] normalize(M,F1) .
  eq normalize(M,E[QS] F1) = E[QS] normalize(M,F1) .
  eq normalize(M,T ?= T')  = getTerm(metaNormalize(M,T)) ?= getTerm(metaNormalize(M,T')) .
  eq normalize(M,T != T')  = getTerm(metaNormalize(M,T)) != getTerm(metaNormalize(M,T')) .
  eq normalize(M,TA)       = TA .
  eq normalize(M,mtForm)   = mtForm .
  --- PRE: N/A
  --- OUT: QidSet of all MetaVariables in the FOForm?
  eq vars(F1 /\ F2) = vars(F1) ; vars(F2) .
  eq vars(F1 \/ F2) = vars(F1) ; vars(F2) .
  eq vars(A[QS] F1) = vars(F1) .
  eq vars(E[QS] F1) = vars(F1) .
  eq vars(~ F1)     = vars(F1) .
  eq vars(TA)       = none .
  eq vars(mtForm)   = none .
  eq vars(T ?= T')  = vars(T) ; vars(T') .
  eq vars(T != T')  = vars(T) ; vars(T') .
  --- INP: PosConj
  --- PRE: N/A
  --- OUT: UnificationProblem if PosConj has no ff literals;
  ---      Otherwise, fail to reduce
 ceq  toUnifProb(PC)              = $toUnifProb(PC) if not PC :: ConstConj .
  eq $toUnifProb(TA /\ PC)        = $toUnifProb(PC) .
  eq $toUnifProb((T ?= T') /\ PC) = T =? T' /\ $toUnifProb(PC) .
  eq $toUnifProb(T ?= T')         = T =? T' .
  --- INP: FOForm?
  --- PRE: N/A
  --- OUT: obvious from definition
  eq  trueId  (mtForm) = tt .
  eq  trueId  (F1)     = F1 [owise] .
  eq  falseId (mtForm) = ff .
  eq  falseId (F1)     = F1 [owise] .
  eq  true2mt (tt)     = mtForm .
  eq  true2mt (F1)     = F1 [owise] .
  eq  false2mt(ff)     = mtForm .
  eq  false2mt(F1)     = F1 [owise] .
endfm

fmod FOFORM-QUANTIFIERS is
  pr META-LEVEL .
  pr FOFORM .
  --- Sort Declarations
  sort Quantifier UniQuantifier ExiQuantifier .
  sort QuantifierList EmptyQuantifierList UniQuantifierList ExiQuantifierList QLFormPair .
  --- Subsorting Declarations
  subsort UniQuantifier ExiQuantifier < Quantifier < QuantifierList .
  subsort EmptyQuantifierList < UniQuantifierList ExiQuantifierList < QuantifierList .
  subsort UniQuantifier < UniQuantifierList .
  subsort ExiQuantifier < ExiQuantifierList .
  --- Quantifier Lists
  op  A[_]   : QidSet                                  -> UniQuantifier       [ctor] .
  op  E[_]   : QidSet                                  -> ExiQuantifier       [ctor] .
  op  none   :                                         -> EmptyQuantifierList [ctor] .
  op  __     : EmptyQuantifierList EmptyQuantifierList -> EmptyQuantifierList [ctor assoc id: none] .
  op  __     : UniQuantifierList UniQuantifierList     -> UniQuantifierList   [ctor ditto] .
  op  __     : ExiQuantifierList ExiQuantifierList     -> ExiQuantifierList   [ctor ditto] .
  op  __     : QuantifierList QuantifierList           -> QuantifierList      [ctor ditto] .
  op ((_,_)) : QuantifierList FOForm                   -> QLFormPair          [ctor] .
  --- Defined Symbols
  op getForm           : QLFormPair -> FOForm .
  op getQuantifierList : QLFormPair -> QuantifierList .
  op stripQuantifiers  : FOForm -> QLFormPair .
  op stripQuantifiers  : QuantifierList FOForm -> QLFormPair .
  op appendQuantifiers : QuantifierList FOForm -> FOForm .
  ---
  var F : FOForm . var QL : QuantifierList . var Q : QidSet .
  --- projections
  eq getForm((QL,F)) = F .
  eq getQuantifierList((QL,F)) = QL .
  --- strip/append quantifiers
  eq stripQuantifiers (F)         = stripQuantifiers(none,F) .
  eq stripQuantifiers (QL,A[Q] F) = stripQuantifiers(QL A[Q],F) .
  eq stripQuantifiers (QL,E[Q] F) = stripQuantifiers(QL E[Q],F) .
  eq stripQuantifiers (QL,F)      = (QL,F) [otherwise] .
  eq appendQuantifiers(QL A[Q],F) = appendQuantifiers(QL, A[Q] F) .
  eq appendQuantifiers(QL E[Q],F) = appendQuantifiers(QL, E[Q] F) .
  eq appendQuantifiers(none,F)    = F .
endfm

fmod FOFORM-TUPLES is
  pr FOFORM .
  pr MODULE-LIST .
  sort FOFormNatPair FOFormPair ModFOFormPair NeModListFOFormPair .
  subsort ModFOFormPair < NeModListFOFormPair .
  op ((_,_)) : NeModuleList FOForm? -> NeModListFOFormPair [ctor] .
  op ((_,_)) : Module FOForm?       -> ModFOFormPair [ctor] .
  op ((_,_)) : FOForm? Nat          -> FOFormNatPair [ctor] .
  op ((_,_)) : FOForm? FOForm?      -> FOFormPair    [ctor] .

  var ML : NeModuleList . var F : FOForm? .

  op getForm : NeModListFOFormPair ~> FOForm? .
  eq getForm((ML,F)) = F .
endfm

fmod FOFORM-SUBSTITUTION is
  pr META-LEVEL .
  pr SUBSTITUTION-HANDLING . --- from full-maude
  pr FOFORM .
  op _<<_    : FOForm? Substitution -> FOForm? .
  op toConj? : Substitution -> Conj? .
  ---
  var U V : Term   . var X Y  : Variable . var S : Substitution .
  var F G : FOForm . var Q Q' : QidSet   . var I : Qid . var N : Nat .
  --- base case
  eq  F << none = F .
  --- quantifiers
  eq (A[X ; Q] F) << (X <- Y ; S) = (A[X ; Q] F) << S .
  eq (E[X ; Q] F) << (X <- Y ; S) = (E[X ; Q] F) << S .
  eq (A[Q]     F) <<  S           = A[Q] (F << S) [owise] .
  eq (E[Q]     F) <<  S           = E[Q] (F << S) [owise] .
  --- other symbols
  eq (F \/ G) << S = (F << S) \/ (G << S) .
  eq (F /\ G) << S = (F << S) /\ (G << S) .
  eq (~ F)    << S = ~ (F << S)           .
  eq (U ?= V) << S = (U << S) ?= (V << S) .
  eq (U != V) << S = (U << S) != (V << S) .
  eq  tt      << S =  tt                  .
  eq  ff      << S =  ff                  .
  eq  mtForm  << S =  mtForm              .

  --- INP: Substitution
  --- PRE: None
  --- OUT: Conj?
  eq toConj?(X <- U ; S) = X ?= U /\ toConj?(S) .
  eq toConj?(none)       = mtForm .
endfm

fmod FOFORM-CONSTS-TO-VARS is
  pr VARIABLES-TO-CONSTANTS .
  pr FOFORM .
  pr FOFORM-SUBSTITUTION .
  op constsToVars  : Substitution QFForm? -> QFForm? .
  var S : Substitution . var F1 F2 : QFForm . var T T' : Term .
  eq constsToVars(S,mtForm)   = mtForm .
  eq constsToVars(S,F1 /\ F2) = constsToVars(S,F1) /\ constsToVars(S,F2) .
  eq constsToVars(S,F1 \/ F2) = constsToVars(S,F1) \/ constsToVars(S,F2) .
  eq constsToVars(S,~ F1)     = ~ constsToVars(S,F1) .
  eq constsToVars(S,T ?= T')  = constsToVars(S,T) ?= constsToVars(S,T') .
  eq constsToVars(S,T != T')  = constsToVars(S,T) != constsToVars(S,T') .
endfm

fmod FOFORMSET-CONSTS-TO-VARS is
  pr FOFORM-CONSTS-TO-VARS .
  pr FOFORMSET .
  op constsToVars : Substitution QFForm?Set -> QFForm?Set .
  var S : Substitution . var F F' : QFForm? . var FS : QFForm?Set .
  eq constsToVars(S,F | F' | FS) = constsToVars(S,F) | constsToVars(S,F' | FS) .
  eq constsToVars(S,mtFormSet)   = mtFormSet .
endfm

fmod FOFORMSUBSTITUTION-PAIR is
  pr FOFORM-SUBSTITUTION . --- substitution application
  pr SUBSTITUTION-AUX    . --- id
  pr FOFORM-OPERATIONS   . --- vars
  sort FOFormSubstPair .
  op ((_,_))  : FOForm? Substitution -> FOFormSubstPair   [ctor] .
  op errFOFormSubstPair : QidList    -> [FOFormSubstPair] [ctor] .
  op errFOFormSubstPairMsg : [FOFormSubstPair] -> QidList [ctor] .
  op idpair   : FOForm? -> FOFormSubstPair .
  op getForm  : FOFormSubstPair -> FOForm? .
  op getSub   : FOFormSubstPair -> Substitution .
  op _Pair<<_ : FOForm? Substitution -> FOFormSubstPair .
  op _<<_     : FOFormSubstPair Substitution -> FOFormSubstPair .
  ---
  var F : FOForm? . var S S' : Substitution .
  var QL : QidList . var FK : [FOFormSubstPair] .

  eq errFOFormSubstPairMsg(errFOFormSubstPair(QL)) = QL .
  eq errFOFormSubstPairMsg(FK) = nil [owise] .

  eq idpair(F) = (F,idsub(vars(F))) .
  ---
  eq getForm((F,S)) = F .
  eq getSub ((F,S)) = S .
  ---
  eq F Pair<< S     = (F << S,S) .
  eq (F,S) << S'    = (F << S,S << S') .
endfm

fmod FOFORM-SUBSTITUTIONSET is
  pr META-LEVEL .
  pr SUBSTITUTIONSET . --- from full-maude
  pr FOFORMSET .
  pr FOFORM-SUBSTITUTION .
  op _<<_  : FOForm? SubstitutionSet -> FOForm?Set .
  ---
  var S S' : Substitution . var SS : SubstitutionSet . var F : FOForm? .
  --- base case
  eq F << .SubstitutionSet = mtFormSet .
  eq F << (S | S' | SS)    = (F << S) | (F << S' | SS) .
endfm

fmod FOFORMSUBSTITUTION-PAIRSET is
  pr FOFORMSUBSTITUTION-PAIR .
  pr FOFORM-SUBSTITUTIONSET .
  sort FOFormSubstPairSet .
  subsort FOFormSubstPair < FOFormSubstPairSet .
  op _|_       : FOFormSubstPairSet FOFormSubstPairSet -> FOFormSubstPairSet [ctor assoc comm id: mtFSPS] .
  op mtFSPS    : -> FOFormSubstPairSet [ctor] .
  ---
  op idpair    : FOForm?Set -> FOFormSubstPairSet .
  op build     : FOForm? SubstitutionSet -> FOFormSubstPairSet .
  ---
  op getForm   : FOFormSubstPairSet -> FOForm?Set .
  op getSub    : FOFormSubstPairSet -> SubstitutionSet .
  ---
  op dnf-join  : FOFormSubstPairSet -> QFForm? .
  op _Pair<<_  : FOForm? SubstitutionSet -> FOFormSubstPairSet .
  op _<<_      : FOFormSubstPairSet SubstitutionSet -> FOFormSubstPairSet .
  op idem      : FOFormSubstPairSet -> FOFormSubstPairSet .
  ---
  var F F' : FOForm? . var FS : FOForm?Set . var C? : Conj? . var S S' : Substitution . var SS : SubstitutionSet .
  var FP FP' : FOFormSubstPair . var FPS : FOFormSubstPairSet .
  var QL : QidList . var FK : [FOFormSubstPair] . var SK : [Substitution] .

  eq errFOFormSubstPairMsg(errFOFormSubstPair(QL) | FK) = QL .

  eq idpair(F | F' | FS) = idpair(F) | idpair(F' | FS) .
  eq idpair(mtFormSet)   = mtFSPS .

  --- INP: FOForm? SubstitutionSet
  --- PRE: None
  --- OUT: An FOFormSubstPairSet built by pairing FOForm? with each Substitution
  eq build(F,S | SS)           = (F,S) | build(F,SS) .
  eq build(F,.SubstitutionSet) = mtFSPS .

  --- projections
  eq getForm((F,S) | FPS) = F | getForm(FPS) .
  eq getForm(mtFSPS)      = mtFormSet .
  eq getSub((F,S) | FPS)  = S | getSub(FPS) .
  eq getSub(mtFSPS)       = .SubstitutionSet .

  --- INP: FOFormSubstPairSet
  --- PRE: Each FOForm in the argument is also a Conj?
  --- OUT: A DNF?
  eq dnf-join((C?,S) | FPS) = (C? /\ toConj?(S)) \/ dnf-join(FPS) .
  eq dnf-join(mtFSPS)       = mtForm .

  --- Substitution Functions
  eq (FP | FP' | FPS) << SS               = (FP << SS) | ((FP' | FPS) << SS) .
  eq mtFSPS           << SS               = mtFSPS .
  eq FPS              << .SubstitutionSet = mtFSPS .
  eq FP               << (S | S' | SS)    = (FP << S) | (FP << (S' | SS)) .

  --- Construct pairs from one side
  eq F Pair<< (S | S' | SS)    = (F Pair<< S) | (F Pair<< (S' | SS)) .
  eq F Pair<< .SubstitutionSet = mtFSPS .
  eq F Pair<< errsub(QL) | SK  = errFOFormSubstPair(QL) [owise] .

  --- Apply the idempotency equation
  eq idem(FP | FP | FPS) = idem(FP | FPS) .
  eq idem(FPS)           = FPS [owise] .
  eq idem(errFOFormSubstPair(QL) | FK) = errFOFormSubstPair(QL) .
endfm

fmod FOFORM-RENAME is
  pr FOFORM .
  pr RENAME-METAVARS .
  op renameAllVar : Module FindResult FOForm? -> FOForm? .
  op renameTmpVar : Module FindResult FOForm? -> FOForm? .
  op unwrapFOForm : TermData                  -> FOForm? .
  var U : Module . var F : FOForm? . var N : FindResult . var T : Term .
  eq renameAllVar(U,N,F) = unwrapFOForm(#renameAllVar(U,N,upTerm(F))) .
  eq renameTmpVar(U,N,F) = unwrapFOForm(#renameTmpVar(U,N,upTerm(F))) .
  eq unwrapFOForm(termdata(T,N)) = downTerm(T,error("Rename failed")) .
endfm

fmod FOFORMSET-RENAME is
  pr FOFORMSET .
  pr FOFORM-RENAME .
  op renameAllVar    : Module FindResult FOForm?Set -> FOForm?Set .
  op renameTmpVar    : Module FindResult FOForm?Set -> FOForm?Set .
  op unwrapFOFormSet : TermData                     -> FOForm?Set .
  var U : Module . var F : FOForm?Set . var N : FindResult . var T : Term .
  eq renameAllVar(U,N,F) = unwrapFOFormSet(#renameAllVar(U,N,upTerm(F))) .
  eq renameTmpVar(U,N,F) = unwrapFOFormSet(#renameTmpVar(U,N,upTerm(F))) .
  eq unwrapFOFormSet(termdata(T,N)) = downTerm(T,error("Rename failed")) .
endfm

fmod FOFORMBASICLIST-RENAME is
  pr FOFORMBASICLIST .
  pr FOFORM-RENAME .
  op renameAllVar     : Module FindResult FOForm?List -> FOForm?List .
  op renameTmpVar     : Module FindResult FOForm?List -> FOForm?List .
  op unwrapFOFormList : TermData                      -> FOForm?List .
  var U : Module . var F : FOForm?List . var N : FindResult . var T : Term .
  eq renameAllVar(U,N,F) = unwrapFOFormList(#renameAllVar(U,N,upTerm(F))) .
  eq renameTmpVar(U,N,F) = unwrapFOFormList(#renameTmpVar(U,N,upTerm(F))) .
  eq unwrapFOFormList(termdata(T,N)) = downTerm(T,error("Rename failed")) .
endfm

fmod FQF-IMPL is
  pr FOFORM-SUBSTITUTION .
  pr FOFORM-TUPLES .
  pr FOFORM-RENAME .
  op renameQuantifiers : Module FOForm?   -> FOForm? .
  op $rq  : FOFormNatPair                 -> FOForm? .
  op $rq  : Nat FOForm?                   -> FOFormNatPair .
  op $rq2 : Qid FOFormNatPair FOForm?     -> FOFormNatPair .
  op $rq2 : Qid FOForm? FOFormNatPair     -> FOFormNatPair .
  op $rq1 : Qid FOFormNatPair             -> FOFormNatPair .
  op $rqQ : Qid FOFormNatPair QidSet      -> FOFormNatPair .
  op $rqQ : Qid Nat FOForm? QidSet QidSet -> FOFormNatPair .
  ---
  var Q : Qid . var F G  : FOForm . var X : Variable . var K : [FOForm] .
  var N : Nat . var S S' : QidSet . var P : QFForm?  . var M : Module .
  --- entry point
  eq renameQuantifiers(M,P)  = P .
  eq renameQuantifiers(M,F)  = $rq($rq(0,renameAllVar(M,notFound,F))) [owise] .
  --- dispatch handlers for different cases
  eq $rq((F,N))            = F .
  eq $rq(N,P:QFForm?)      = (P:QFForm?,N) .
  eq $rq(N,F /\ G)         = $rq2('/\,$rq(N,F),G) [owise] .
  eq $rq(N,F \/ G)         = $rq2('\/,$rq(N,F),G) [owise] .
  eq $rq(N,~ F)            = $rq1('~ ,$rq(N,F))   [owise] .
  eq $rq(N,A[Q] F)         = $rqQ('A ,$rq(N,F),Q) [owise] .
  eq $rq(N,E[Q] F)         = $rqQ('E ,$rq(N,F),Q) [owise] .
  eq $rq(N,K)              = (K,N)                [owise] .
  --- and/or
  eq $rq2(Q,(F,N),G)       = $rq2(Q,F,$rq(N,G)) .
  eq $rq2('/\,F,(G,N))     = (F /\ G,N) .
  eq $rq2('\/,F,(G,N))     = (F \/ G,N) .
  --- not
  eq $rq1('~,(F,N))        = (~ F,N) .
  --- quantifiers
  eq $rqQ(Q,(F,N),S)       = $rqQ(Q,N,F,S,none)   .
  eq $rqQ(Q, N,F,X ; S,S') = $rqQ(Q,s(N),F << (X <- tmpvar(N,X)),S,S' ; tmpvar(N,X)) .
  eq $rqQ('A,N,F,none, S') = (A[S'] F,N) .
  eq $rqQ('E,N,F,none, S') = (E[S'] F,N) .
endfm

fmod FQF is
  pr META-LEVEL .
  pr REFLECT .
  pr FOFORM .
  --- convert to fresh quantifier form
  op toFQF : Module FOForm -> FOForm .
  var M : Module . var F : FOForm .
  --- perform conversion and validation
  eq toFQF(M,F) = downTerm(redReflect('FQF-IMPL,'renameQuantifiers[upTerm(M),upTerm(F)]),error("FQF failed")) .
endfm

fmod NNF-IMPL is
  pr META-LEVEL .
  sort EmptyForm TrueLit FalseLit NNF Form .
  subsort EmptyForm TrueLit FalseLit < NNF < Form .
  --- Negation Normal Forms
  op  mtForm :             -> EmptyForm [ctor] .
  op  tt     :             -> TrueLit   [ctor] .
  op  ff     :             -> FalseLit  [ctor] .
  op  _?=_   : Term Term   -> NNF  [ctor comm] .
  op  _!=_   : Term Term   -> NNF  [ctor comm] .
  op  _/\_   : NNF NNF     -> NNF  [ctor assoc comm prec 51] .
  op  _\/_   : NNF NNF     -> NNF  [ctor assoc comm prec 51] .
  op  A[_]_  : QidSet NNF  -> NNF  [ctor prec 52] .
  op  E[_]_  : QidSet NNF  -> NNF  [ctor prec 52] .
  --- General Formulas
  op  ~_    : Form         -> Form [prec 50] .
  op  _/\_  : Form Form    -> Form [ctor ditto] .
  op  _\/_  : Form Form    -> Form [ctor ditto] .
  op  A[_]_ : QidSet Form  -> Form [ctor ditto] .
  op  E[_]_ : QidSet Form  -> Form [ctor ditto] .
  var T T' : Term . var F F' : Form . var Q : QidSet .
  --- Push negations down everywhere --- flip sign accordingly
  eq ~ tt        = ff .
  eq ~ ff        = tt .
  eq ~ T ?= T'   = T != T'  .
  eq ~ T != T'   = T ?= T'  .
  eq ~ (A[Q] F)  = E[Q] ~ F .
  eq ~ (E[Q] F)  = A[Q] ~ F .
  eq ~ (F /\ F') = ~ F \/ ~ F' .
  eq ~ (F \/ F') = ~ F /\ ~ F' .
endfm

fmod NNF is
  pr META-LEVEL .
  pr REFLECT .
  pr FOFORM .
  op toNNF : FOForm? -> FOForm? .
  op nnf?  : FOForm? -> Bool .
  var F : FOForm? .
  eq toNNF(F) = downTerm(redReflect('NNF-IMPL,upTerm(F)),error("NNF failed")) .
  eq nnf?(F)  = sortReflect('NNF-IMPL,upTerm(F),'NNF) .
endfm

fmod PNF-IMPL is
  pr META-LEVEL .
  sort EmptyForm TrueLit FalseLit QFF EPNF APNF PNF Form .
  subsort EmptyForm TrueLit FalseLit < QFF < PNF < Form .
  --- Quantifier Free Formulas
  op  mtForm :              -> EmptyForm [ctor] .
  op  tt     :              -> TrueLit   [ctor] .
  op  ff     :              -> FalseLit  [ctor] .
  op  _?=_   : Term Term    -> QFF  [ctor comm] .
  op  _!=_   : Term Term    -> QFF  [ctor comm] .
  op  _/\_   : QFF QFF      -> QFF  [ctor assoc comm prec 50] .
  op  _\/_   : QFF QFF      -> QFF  [ctor assoc comm prec 50] .
  op  ~_     : QFF          -> QFF  [ctor prec 51] .
  --- Prenex Normal Forms
  op  A[_]_  : QidSet PNF   -> PNF  [ctor prec 51] .
  op  E[_]_  : QidSet PNF   -> PNF  [ctor prec 51] .
  --- General Formulas
  op  _/\_   : Form Form    -> Form [ctor ditto] .
  op  _\/_   : Form Form    -> Form [ctor ditto] .
  op  ~_     : Form         -> Form [ctor ditto] .
  op  A[_]_  : QidSet Form  -> Form [ctor ditto] .
  op  E[_]_  : QidSet Form  -> Form [ctor ditto] .
  --- Bubble Quantifiers Up
  var F G : Form . var Q : QidSet .
  eq  ~ A[Q] F      = E[Q] ~ F      .
  eq  ~ E[Q] F      = A[Q] ~ F      .
  eq  (A[Q] F) /\ G = A[Q] (F /\ G) .
  eq  (E[Q] F) /\ G = E[Q] (F /\ G) .
endfm

fmod PNF is
  pr META-LEVEL .
  pr REFLECT .
  pr FOFORM .
  pr FQF .
  op toPNF  : Module FOForm -> FOForm .
  op toPNF1 : FOForm -> FOForm .
  op pnf?   : FOForm -> Bool   .
  var M : Module . var F : FOForm . var K : [FOForm] .
  eq toPNF (M,F) = if F :: QFForm? or-else pnf?(F) then F else toPNF1(toFQF(M,F)) fi .
  eq toPNF1(F)   = downTerm(redReflect('PNF-IMPL,upTerm(F)),error("PNF Failed")) .
  eq toPNF1(K)   = K [owise] .
  eq pnf?(F)     = sortReflect('PNF-IMPL,upTerm(F),'PNF) .
endfm

fmod DNF-IMPL is
  pr META-LEVEL .
  sort EmptyForm TrueLit FalseLit Conj DNF QDNF Form .
  subsort EmptyForm TrueLit FalseLit < Conj < DNF < QDNF < Form .
  --- Disjunctive Normal Forms
  op  mtForm :             -> EmptyForm [ctor] .
  op  tt     :             -> TrueLit   [ctor] .
  op  ff     :             -> FalseLit  [ctor] .
  op  _?=_   : Term Term   -> Conj [ctor comm] .
  op  _!=_   : Term Term   -> Conj [ctor comm] .
  op  _/\_   : Conj Conj   -> Conj [ctor assoc comm] .
  op  _\/_   : DNF DNF     -> DNF  [ctor assoc comm] .
  op  A[_]_  : QidSet QDNF -> QDNF [ctor] .
  op  E[_]_  : QidSet QDNF -> QDNF [ctor] .
  --- General Formulas
  op  _/\_   : Form Form   -> Form [ctor ditto] .
  op  _\/_   : Form Form   -> Form [ctor ditto] .
  op  ~_     : Form        -> Form [ctor] .
  op  A[_]_  : QidSet Form -> Form [ctor] .
  op  E[_]_  : QidSet Form -> Form [ctor] .
  ---
  var F F' F'' : Form .
  --- With NNF/PNF transformations, one equation is enough
  eq F /\ (F' \/ F'') = (F /\ F') \/ (F /\ F'') .
endfm

fmod DNF is
  pr META-LEVEL .
  pr REFLECT .
  pr FOFORM .
  pr PNF + NNF .
  op toDNF : Module FOForm? -> FOForm? .
  op toDNF : QFForm? -> QFForm? .
  op qdnf? : FOForm? -> Bool   .
  op dnf?  : FOForm? -> Bool   .
  var M : Module . var F : FOForm? . var Q : QFForm? .
  --- order: call NNF, PNF, and then CNF conversion function
  eq toDNF(M,F) = downTerm(redReflect('DNF-IMPL,upTerm(toPNF(M,toNNF(F)))),error("DNF failed")) .
  eq toDNF(Q)   = downTerm(redReflect('DNF-IMPL,upTerm(toNNF(Q))),error("DNF failed")) .
  eq qdnf?(F)   = sortReflect('DNF-IMPL,upTerm(F),'QDNF) .
  eq dnf?(F)    = sortReflect('DNF-IMPL,upTerm(F),'DNF) .
endfm

fmod CNF-IMPL is
  pr META-LEVEL .
  sort EmptyForm TrueLit FalseLit Disj CNF QCNF Form .
  subsort EmptyForm TrueLit FalseLit < Disj < CNF < QCNF < Form .
  --- Disjunctive Normal Forms
  op  mtForm :             -> EmptyForm [ctor] .
  op  tt     :             -> TrueLit   [ctor] .
  op  ff     :             -> FalseLit  [ctor] .
  op  _?=_   : Term Term   -> Disj [ctor comm] .
  op  _!=_   : Term Term   -> Disj [ctor comm] .
  op  _\/_   : Disj Disj   -> Disj [ctor assoc comm] .
  op  _/\_   : CNF CNF     -> CNF  [ctor assoc comm] .
  op  A[_]_  : QidSet QCNF -> QCNF [ctor] .
  op  E[_]_  : QidSet QCNF -> QCNF [ctor] .
  --- General Formulas
  op  _/\_   : Form Form   -> Form [ctor ditto] .
  op  _\/_   : Form Form   -> Form [ctor ditto] .
  op  ~_     : Form        -> Form [ctor] .
  op  A[_]_  : QidSet Form -> Form [ctor] .
  op  E[_]_  : QidSet Form -> Form [ctor] .
  ---
  var F F' F'' : Form .
  --- With NNF/PNF transformations, one equation is enough
  eq F \/ (F' /\ F'') = (F \/ F') /\ (F \/ F'') .
endfm

fmod CNF is
  pr META-LEVEL .
  pr REFLECT .
  pr FOFORM .
  pr PNF + NNF .
  op toCNF : Module FOForm? -> FOForm? .
  op toCNF : QFForm? -> QFForm? .
  op qcnf? : FOForm? -> Bool   .
  op cnf?  : FOForm? -> Bool   .
  var M : Module . var F : FOForm? . var Q : QFForm? .
  --- order: call NNF, PNF, and then CNF conversion function
  eq toCNF(M,F) = downTerm(redReflect('CNF-IMPL,upTerm(toPNF(M,toNNF(F)))),error("CNF failed")) .
  eq toCNF(Q)   = downTerm(redReflect('CNF-IMPL,upTerm(toNNF(Q))),error("CNF failed")) .
  eq qcnf?(F)   = sortReflect('CNF-IMPL,upTerm(F),'QCNF) .
  eq  cnf?(F)   = sortReflect('CNF-IMPL,upTerm(F),'CNF) .
endfm

fmod FOFORMSET-OPERATIONS is
  pr FOFORM-OPERATIONS .
  pr FOFORMSET .
  pr CNF .
  pr DNF .
  op disj-join    : FOForm?Set -> FOForm? .
  op disj-join    : QFForm?Set -> QFForm? .
  op conj-join    : FOForm?Set -> FOForm? .
  op conj-join    : QFForm?Set -> QFForm? .
  op toPosEqLits : PosEqQFForm -> PosEqLitSet .
  op toPosEqLits : UnificationProblem -> PosEqLitSet .
  op toEqSet      : PosEqLitSet -> EquationSet .
  op wellFormed   : Module FOForm?Set -> Bool .
  ---
  op toDisjSet  : QFForm? ~> DisjSet .
  op toDisjSet' : QFForm? ~> DisjSet .
  op toConjSet  : QFForm? ~> ConjSet .
  op toConjSet' : QFForm? ~> ConjSet .
  ---
  var FS : FOForm?Set . var FF FF' : FOForm? . var F : QFForm? . var D : Disj . var UP : UnificationProblem .
  var C : Conj . var PEA : PosEqLit . var PEAS : PosEqLitSet . var T T' : Term . var M : Module .
  ---
  eq toDisjSet (F)      = toDisjSet'(toCNF(F)) .
  eq toDisjSet'(D /\ F) = D | toDisjSet'(F) .
  eq toDisjSet'(mtForm) = mtFormSet .
  ---
  eq toConjSet (F)      = toConjSet'(toDNF(F)) .
  eq toConjSet'(C \/ F) = C | toConjSet'(F) .
  eq toConjSet (mtForm) = mtFormSet .
  ---
  eq disj-join(FF | FS)   = FF \/ disj-join(FS) .
  eq disj-join(mtFormSet) = ff .
  eq conj-join(FF | FS)   = FF /\ conj-join(FS) .
  eq conj-join(mtFormSet) = tt .
  ---
  eq toPosEqLits(PEA /\ F) = PEA | toPosEqLits(F) .
  eq toPosEqLits(PEA \/ F) = PEA | toPosEqLits(F) .
  eq toPosEqLits(mtForm)   = mtFormSet .
  ---
  eq toPosEqLits(T =? T' /\ UP) = T ?= T' | toPosEqLits(UP) .
  eq toPosEqLits(T =? T')       = T ?= T' .
  ---
  eq toEqSet(T ?= T' | PEAS) = (eq T = T' [none] .) toEqSet(PEAS) .
  eq toEqSet(mtFormSet)      = none .
  ---
  eq wellFormed(M,FF | FF' | FS) = wellFormed(M,FF) and-then wellFormed(M,FF' | FS) .
  eq wellFormed(M,mtFormSet)     = true .
endfm

fmod FOFORM-DESCENT-MAP is
  pr FOFORM .
  pr UNIT-FM .
  op descent-map : Module Module QFForm? -> QFForm? .
  op check-map?  : QFForm? ~> QFForm? .
  ---
  var DM RM : Module . var F : FOForm? .
  --- INP: Module:DM Module:RM QFForm?
  --- PRE: DM should have a function reduce : Form -> Form which returns ff in case of any error
  ---      Form is wellFormed w.r.t to RM
  --- OUT: Use DM to descent-map Form
  eq descent-map(noModule,RM,F) = F  .
  eq descent-map(DM,RM,F)       = if F == ff then ff else
    check-map?(downTerm(getTerm(metaReduce(DM,'reduce[upTerm(RM),upTerm(F)])),ff)) fi [owise] .
 ceq check-map?(F)              = F if F =/= ff .
endfm

fmod FOFORM-EASY-SIMPLIFY is
  pr FOFORMSIMPLIFY     .
  pr FOFORMREDUCE       .
  pr FOFORM-DESCENT-MAP .
  pr NNF                .
  op srs  : Module FOForm?        -> FOForm? .
  op srsd : Module Module FOForm? -> FOForm? .
  var DM RM : Module . var F : FOForm? .
  eq srs (RM,F)    = simplify(toNNF(reduce(RM,simplify(F)))) .
  eq srsd(DM,RM,F) = descent-map(DM,RM,srs(RM,F)) .
endfm

--- NOTE: this simplification of extracting a substitution and applying it is NOT deterministic...
--- this module extracts fragments out of QFForms that look like substitutions and applies them
fmod FOFORM-EXTRACT-SUBSTITUTION is
  pr FOFORMSUBSTITUTION-PAIRSET .
  pr DNF .
  pr SUBSTITUTION-HANDLING .
  pr FOFORM-OPERATIONS .
  pr FOFORMSET-OPERATIONS .

  op is-sub?         : Module QFForm? -> Bool .
  op #extract-subs   : Module QFForm? -> FOFormSubstPairSet .
  op #extract-subs   : Module QFForm? FOFormSubstPairSet -> FOFormSubstPairSet .
  op #extract-sub    : Module Conj? -> FOFormSubstPair .
  op #extract-sub    : Module Conj? Conj? Substitution -> FOFormSubstPair .
  op extract-sub     : Module Conj? -> Conj? .
  op extract-imp-sub : Module QFForm ~> QFForm .

  var Q1? Q2? : QFForm? . var Q Q' : QFForm . var C : Conj . var C1? C2? C3? : Conj? . var FS : FOFormSubstPairSet .
  var V : Variable . var T : Term . var S : Substitution . var A : Lit . var U : Module .

  --- INP: QFForm?
  --- PRE:
  --- OUT: Check if the QFForm? is equivalent to a disjunction of substitutions (which is always satisfiable)
  eq is-sub?(U,Q1?) = getForm(#extract-subs(U,Q1?)) == mtForm .

  --- INP: QFForm?
  --- PRE:
  --- OUT: [1] QFForm? is first converted into a DNF
  ---      [2] In each conjunct, substitution-like fragments are #extracted (but not applied) into a FOFormSubstPair
  ---      [3] We return a set of all such pairs (one for each conjunct)
  eq #extract-subs (U,Q1?)               = #extract-subs(U,toDNF(Q1?),mtFSPS) .
  eq #extract-subs (U,mtForm,FS)         = FS .
  eq #extract-subs (U,C \/ Q1?,FS)       = #extract-subs(U,Q1?,FS | #extract-sub(U,C)) .

  --- INP: Conj?
  --- PRE:
  --- OUT: An FOFormSubstPair (C'?,S?) such that C? = C'? /\ S?  and S? is the substitution-like fragment (may be empty)
  eq #extract-sub(U,C1?)                 = #extract-sub(U,C1?,mtForm,none) .
 ceq #extract-sub(U,V ?= T /\ C1?,C2?,S) = #extract-sub(U,(C1? /\ C2?) << (V <- T),mtForm,S .. (V <- T))
  if sortLeq(U,leastSort(U,T),getType(V)) /\ not V in vars(T) .
  eq #extract-sub(U,A /\ C1?,C2?,S)      = #extract-sub(U,C1?,C2? /\ A,S) [owise] .
  eq #extract-sub(U,mtForm,C2?,S)        = (C2?,S) .


  eq extract-sub(U,C1?) = getForm(#extract-sub(U,C1?)) .

  eq extract-imp-sub(U, Q \/ ~ C) = Q << getSub(#extract-sub(U,C)) \/ ~ trueId(getForm(#extract-sub(U,C))) .
  eq extract-imp-sub(U,(Q \/ ~ C) /\ Q') = extract-imp-sub(U,Q \/ ~ C) /\ extract-imp-sub(U,Q') .
  eq extract-imp-sub(U,Q) = Q [owise] .
endfm

---(
fmod FOFORM-AUX is
  pr MGCI . --- for ctor-term
  pr SUBSTITUTIONPAIRSET .
  pr FOFORMSUBSTITUTION-PAIR .
  pr STREAM{FOFormSubstPair} .

  --- Simplify constraints by apply substitutions whenever possible
  op litToNonRecBinding   : Module Lit -> Substitution .
  op bindingsToConj       : Substitution -> PosEqConj .
  op findBinding          : Module QFForm -> FOFormSubstPair .
  op findBindings         : Module QFForm -> FOFormSubstPair .
  op $findBindings        : Module QFForm QFForm Substitution -> Stream{FOFormSubstPair} .
  op $findBindings        : Module QFForm QFForm Substitution Lit Substitution -> Stream{FOFormSubstPair} .
  op recSimplifyForm      : Module QFForm Substitution -> FOFormSubstPair .
  op $recSimplifyForm     : Module FOFormSubstPair Substitution -> FOFormSubstPair .
  op simplifyForm         : Module QFForm -> QFForm .
  op simplifyBindings     : Module Substitution -> Substitution .
  op removeGround         : QFForm -> QFForm .
  op filterBindingsByCtor : Module Substitution -> SubstitutionPair .
  op removeRedundantLits  : QFForm -> QFForm .

  var F F' : QFForm . var V : Variable    .
  var T T' : Term . var VS  : VariableSet . var M : Module . var L : Lit .
  var S S' S1 S2 : Substitution . var B1 B2 : Bool . var F? F'? : QFForm? .

  --- INP: Substitution
  --- PRE: None
  --- OUT: A PosEqConj where each V <- T becomes V ?= T
  eq bindingsToConj(V <- T ; S) = V ?= T /\ bindingsToConj(S) .
  eq bindingsToConj(none)       = tt .

  --- INP: Module Lit
  --- PRE: Lit is well-formed
  --- OUT: If Lit is of the form V ?= T or T ?= V then
  ---      returns a substitution V <- T if leastSort(T) < leastSort(V) and not V in vars(T)
  ---      otherwise returns none
  eq litToNonRecBinding(M,V ?= T) =
    if sortLeq(M,leastSort(M,T),leastSort(M,V)) and not V in vars(T) then V <- T else none fi .
  eq litToNonRecBinding(M,L)      = none [owise] .

  --- INP: Module FOForm1
  --- PRE: FOForm1 is well-formed
  --- OUT: Given Form1 = F /\ B with B a substitution, returns (F,B),
  ---      otherwise returns (Form1,none)
  eq findBinding(M,F)                   = pick!(0,$findBindings(M,F,mtForm,none)) .
  eq findBindings(M,F)                  = last!($findBindings(M,F,mtForm,none))   .
  eq $findBindings(M,mtForm,F',S)       = (F',S) & end .
  eq $findBindings(M,L /\ F?,F'?,S)     = $findBindings(M,F?,F'?,S,L,litToNonRecBinding(M,L)) .
  eq $findBindings(M,F?,F'?,S,L,V <- T) = (F? /\ F'?,S ; V <- T) & $findBindings(M,F?,F'?,S ; V <- T) .
  eq $findBindings(M,F?,F'?,S,L,none)   = $findBindings(M,F?,F'? /\ L,S) .

  --- INP: Module FOForm Substitution
  --- PRE: None
  --- OUT: If FOForm = F /\ B with B a non-circular binding, then performs
  ---      such substitutions followed by a simplification until a fixpoint
  ---      is reached; returns the substituted formula plus the set of bindings
  ---      that were generated
  eq recSimplifyForm(M,F,S)           = $recSimplifyForm(M,findBinding(M,F),S) .
  eq $recSimplifyForm(M,(F,V <- T),S) =
    if S == none then
      recSimplifyForm(M,F << (V <- T),V <- T)
    else
      recSimplifyForm(M,F << (V <- T),(S << (V <- T)) ; V <- T)
    fi .
 ceq $recSimplifyForm(M,(F,none),S)   =
    if F == F' then (F,S') else recSimplifyForm(M,F',S') fi
  if F' := removeRedundantLits(simplifyForm(M,F))
  /\ S' := simplifyBindings(M,S) .

  --- INP: Module Substitution (SubstitutionPair)
  --- PRE: Substitution is well-formed with respect to module
  --- OUT: Split substitution S into pair (S1,S2) where S = S1 ; S2
  ---      and each V <- T in S1 has ctor-term?(M,T) holds
  op $filterBindingsByCtor : Module Substitution SubstitutionPair -> SubstitutionPair .
  eq filterBindingsByCtor(M,S)                   = $filterBindingsByCtor(M,S,(none,none)) .
  eq $filterBindingsByCtor(M,V <- T ; S,(S1,S2)) =
    if ctor-term?(M,T) then $filterBindingsByCtor(M,S,(S1 ; V <- T,S2))
      else $filterBindingsByCtor(M,S,(S1,S2 ; V <- T)) fi .
  eq $filterBindingsByCtor(M,none,(S1,S2)) = (S1,S2) .

  --- INP: Bool VariableSet Substitution
  --- PRE: None
  --- OUT: Split substitution S into pair (S1,S2) where S = S1 ; S2
  ---      and each V <- T in S1 has V in VariableSet holds
  op filterBindingsByVars : VariableSet Substitution -> SubstitutionPair .
  op $filterBindingsByVars : VariableSet Substitution SubstitutionPair -> SubstitutionPair .
  eq filterBindingsByVars(VS,S) = $filterBindingsByVars(VS,S,(none,none)) .
  eq $filterBindingsByVars(VS,V <- T ; S,(S1,S2)) =
    if V in VS then $filterBindingsByVars(VS,S,(S1 ; V <- T,S2))
      else $filterBindingsByVars(VS,S,(S1,S2 ; V <- T)) fi .
  eq $filterBindingsByVars(VS,none,(S1,S2)) = (S1,S2) .

  --- INP: Module Form
  --- PRE: FOForm should be well-formed, Module should satisfy executability requirments
  --- OUT: A form where all terms have been meta-reduced
  eq simplifyForm(M,F /\ F') = simplifyForm(M,F) /\ simplifyForm(M,F') .
  eq simplifyForm(M,T ?= T') = getTerm(metaReduce(M,T)) ?= getTerm(metaReduce(M,T')) .
  eq simplifyForm(M,T != T') = getTerm(metaReduce(M,T)) != getTerm(metaReduce(M,T')) .
  eq simplifyForm(M,L)       = L [owise] .

  --- INP: Module Substitution
  --- PRE: Substitution should be well-formed, Module should satisfy executability requirments
  --- OUT: A substitution where all terms have been meta-reduced
  eq simplifyBindings(M,V <- T ; S) = V <- getTerm(metaReduce(M,T)) ; simplifyBindings(M,S) .
  eq simplifyBindings(M,none)       = none .

  --- INP: FOForm
  --- PRE: None
  --- OUT: A form where redundant elements are deleted (by assoc-comm)
  eq removeRedundantLits(L /\ L /\ F?) = L /\ F?   .
  eq removeRedundantLits(L \/ L \/ F?) = L /\ F?   .
  eq removeRedundantLits(F)            = F [owise] .

  --- INP: FOForm
  --- PRE: None
  --- OUT: All literals L in FOForm that are ground are removed
  eq removeGround(F /\ F')  = removeGround(F) /\ removeGround(F') .
  eq removeGround(F \/ F')  = removeGround(F) \/ removeGround(F') .
  eq removeGround(L:Lit)   = if vars(L:Lit) =/= none then L:Lit else tt fi .
endfm
---)

fmod FOFORM-PRINTER is pr FOFORM . pr GENERIC-PRINTER .
  op  print    : Module QFForm? -> QidList .
  op  printImp : Module QFForm? -> QidList .
  var M : Module . var F F' : QFForm . var T T' : Term .
  --- print formulas
  eq print(M,F /\ F')          = '`( print(M,F) '/\ print(M,F') '`) .
  eq print(M,F \/ F')          = '`( print(M,F) '\/ print(M,F') '`) .
  eq print(M,T ?= T')          = print(M,T) '=   print(M,T') .
  eq print(M,T != T')          = print(M,T) '=/= print(M,T') .
  eq print(M,~ F)              = '~ print(M,F) .
  eq print(M,mtForm)           = 'true .
  eq print(M,tt)               = 'true .
  eq print(M,ff)               = 'false .
  eq print(M,F:[QFForm?])      = 'Error: 'Cannot 'Print 'Ill-formed 'Formula [owise] .
  --- print formulas as implication
  eq printImp(M,(~ F) \/ F')   = print(M,F) &sp '=> &sp print(M,F') .
endfm

fmod FOFORMSET-PRINTER is pr FOFORMSET . pr FOFORM-PRINTER .
  op  print : Module Qid QFForm?Set -> QidList .
  var M : Module . var F F' : QFForm? . var FS : QFForm?Set . var Q : Qid .
  --- print formula sets
  eq print(M,Q,F | F | FS) = print(M,F) Q print(M,F | FS) .
  eq print(M,Q,F)          = print(M,F) .
  eq print(M,Q,mtFormSet)  = 'None .
endfm

--- this module defines a generic structure to represent the success/failure
--- of a formula reduction --- which also includes an optional status code:
--- true/false/unknown/errb --- to represent the result of the reduction
fmod GENERIC-FORMULA-REDUCTION is
  pr MAYBE-BOOL .
  pr FOFORMSET .
  sort QFFormSetBoolPair .
  op ((_,_)) : QFForm?Set MaybeBool -> QFFormSetBoolPair [ctor] .
  ---
  op true? : QFFormSetBoolPair -> Bool  .
  op bool  : QFFormSetBoolPair -> MaybeBool .
  op form  : QFFormSetBoolPair -> QFForm?Set .
  --- projections
  var B : MaybeBool . var F : QFForm?Set .
  eq true?((F,B)) = B == true .
  eq form ((F,B)) = F .
  eq bool ((F,B)) = B .
endfm

--- NOTE: implementing this kind of module REALLY should be the first
---       step in building a theorem prover, i.e. the ideal theorem
---       prover should produce a proof witness as its result that can
---       be inspected and validated
--- NOTE: implementing this here makes NO sense because this file is
---       NOT a theorem prover---we keep this only as a reference
fmod PROOF-WITNESS is
  pr FOFORM .
  --- Kinds of Proofs
  sort SatWitness ValWitness Witness .
  subsort SatWitness ValWitness < Witness .
  subsort ValWitness < SatWitness .
  --- Proof witnesses have projections
  op pi       : Witness    -> FOForm . --- the formula proved
  op algebra  : Witness    -> Module . --- Maude module which corresponds to algebra class
  op verify   : Witness    -> Bool   . --- verify if the proof object is correct for algebra/formula
  op complete : Witness    -> Bool   . --- if this witness was generated by a complete method
  op ctor     : Witness    -> Bool   . --- if this witness was generated by a constructive proof method
  --- Default non-constructive proofs
  op sat2val  : SatWitness -> ValWitness [ctor] .
  op val2sat  : ValWitness -> SatWitness [ctor] .
  --- Default non-construcitive proof verifier
  var SW : SatWitness . var VW : ValWitness .
  eq verify(sat2val(SW))   = complete(SW) and-then verify(SW) .
  eq verify(val2sat(VW))   = complete(VW) and-then verify(VW) .
  eq pi(sat2val(SW))       = ~ pi(SW)     .
  eq pi(val2sat(VW))       = ~ pi(VW)     .
  eq algebra(sat2val(SW))  = algebra(SW)  .
  eq algebra(val2sat(VW))  = algebra(VW)  .
  eq complete(sat2val(SW)) = complete(SW) .
  eq complete(val2sat(VW)) = complete(VW) .
  eq ctor(sat2val(SW))     = false        .
  eq ctor(val2sat(VW))     = false        .
endfm

--- this module defines a "pretty" printer for core Maude, i.e.
--- defines new syntax that stands out better than the old syntax
--- NB: such pretty printing should always occur before display to
---     the user and not earlier because otherwise the user will
---     have to extend their functions to deal with the new
---     constructors that we invent here.
fmod FOFORM-CORE-PRETTYPRINT is
  pr FOFORM .

  --- syntax for (respectively):
  ---  positive predicates
  ---  negative predicates
  ---  implications
  op ##_   : Term          -> QFForm [ctor format(g o o)] .
  op !!_   : Term          -> QFForm [ctor format(g o o)] .
  op _=>_ : QFForm QFForm -> QFForm [ctor format(o rn on d)] .

  var F F' : QFForm . var Q : Qid . var T : Term .
  var TA : TruthLit . var E : EqLit .

  op prettyPrint : Qid QFForm -> QFForm .
  eq prettyPrint(Q,F /\ F')     = prettyPrint(Q,F) /\ prettyPrint(Q,F') .
  eq prettyPrint(Q,(~ F) \/ F') = prettyPrint(Q,F) => prettyPrint(Q,F') .
  eq prettyPrint(Q,F \/ F')     = prettyPrint(Q,F) \/ prettyPrint(Q,F') [owise] .
  eq prettyPrint(Q,~ F)         = ~ prettyPrint(Q,F) .
  eq prettyPrint(Q,TA)          = TA .
  eq prettyPrint(Q,Q != T)      = !! T .
  eq prettyPrint(Q,Q ?= T)      = ## T .
  eq prettyPrint(Q,E)           = E [owise] .
endfm

--- Views for some of our formula datatypes
--- NeSet
view FOForm          from TRIV to FOFORM                  is sort Elt to FOForm          . endv
view QFForm          from TRIV to FOFORM                  is sort Elt to QFForm          . endv
view AEQForm         from TRIV to FOFORM                  is sort Elt to AEQForm         . endv
view Conj            from TRIV to FOFORM                  is sort Elt to Conj            . endv
view Disj            from TRIV to FOFORM                  is sort Elt to Disj            . endv
view PosConj         from TRIV to FOFORM                  is sort Elt to PosConj         . endv
view PosDisj         from TRIV to FOFORM                  is sort Elt to PosDisj         . endv
view NegConj         from TRIV to FOFORM                  is sort Elt to NegConj         . endv
view NegDisj         from TRIV to FOFORM                  is sort Elt to NegDisj         . endv
--- Set
view FOForm?         from TRIV to FOFORM                  is sort Elt to FOForm?         . endv
view QFForm?         from TRIV to FOFORM                  is sort Elt to QFForm?         . endv
view AEQForm?        from TRIV to FOFORM                  is sort Elt to AEQForm?        . endv
view Conj?           from TRIV to FOFORM                  is sort Elt to Conj?           . endv
view Disj?           from TRIV to FOFORM                  is sort Elt to Disj?           . endv
view PosConj?        from TRIV to FOFORM                  is sort Elt to PosConj?        . endv
view PosDisj?        from TRIV to FOFORM                  is sort Elt to PosDisj?        . endv
view NegConj?        from TRIV to FOFORM                  is sort Elt to NegConj?        . endv
view NegDisj?        from TRIV to FOFORM                  is sort Elt to NegDisj?        . endv
--- FOFormSubstPair
view FOFormSubstPair from TRIV to FOFORMSUBSTITUTION-PAIR is sort Elt to FOFormSubstPair . endv
```
