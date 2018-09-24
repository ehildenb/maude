First-Order Formulae
--------------------

This file implements a first-order logic formula datatype. The main datatype is
defined in FOFORM. Various functionalities are provided to compute normal forms:
NNF, PNF, DNF, and CNF or simplify/rename formulas. These functions are stored
in separate modules from FOFORM and are called through META-LEVEL reflection.
This isolates the modules from one another and simplifies the algorithm design.

```maude
load variables.maude

fmod BOOL-ERR is
   protecting MAYBE-BOOL * ( sort MaybeBool to Bool? ) .
endfm

fmod REFLECT is
  pr META-LEVEL .
  op redReflect  : Qid Term      -> [Term] .
  op sortReflect : Qid Term Type -> [Bool] .
  var M : Qid .
  var T : Term .
  var TY : Type .
  eq redReflect(M,T)     = getTerm(metaReduce(upModule(M,false),T)) .
  eq sortReflect(M,T,TY) = sortLeq(upModule(M,false),leastSort(upModule(M,false),T),TY) .
endfm

fmod FOFORM is
  pr META-LEVEL .
  --- NOTE: This sort structure is complicated. Edit at your own risk (unless you want to simplify it).
  --- Sort Declarations
  --- Non-Empty/Possibly Empty Forms
  sort TrueAtom FalseAtom TruthAtom PosEqAtom NegEqAtom Truth+PosEqAtom Truth+NegEqAtom EqAtom Atom .
  sort ConstConj PosEqConj NegEqConj EqConj PosConj NegConj Conj .
  sort ConstDisj PosEqDisj NegEqDisj EqDisj PosDisj NegDisj Disj .
  sort PosEqQFForm NegEqQFForm EqQFForm QFForm AEQForm FOForm .
  sort EmptyForm TruthAtom? PosEqAtom? NegEqAtom? Truth+NegEqAtom? Truth+PosEqAtom? EqAtom? Atom? .
  sort ConstConj? PosEqConj? NegEqConj? EqConj? PosConj? NegConj? Conj? .
  sort ConstDisj? PosEqDisj? NegEqDisj? EqDisj? PosDisj? NegDisj? Disj? .
  sort PosEqQFForm? NegEqQFForm? EqQFForm? QFForm? AEQForm? FOForm? .
  --- Subsorting
  --- Atoms
  subsort TrueAtom  FalseAtom < TruthAtom .
  subsort PosEqAtom NegEqAtom < EqAtom          < Atom .
  subsort TruthAtom PosEqAtom < Truth+PosEqAtom < Atom .
  subsort TruthAtom NegEqAtom < Truth+NegEqAtom < Atom .
  --- Non-Atoms
  subsort PosEqConj PosEqDisj  < PosEqQFForm < EqQFForm .
  subsort NegEqConj NegEqDisj  < NegEqQFForm < EqQFForm .
  subsort EqConj EqDisj EqAtom < EqQFForm    < QFForm .
  subsort Atom                 < Conj Disj   < QFForm < FOForm .
  subsort AEQForm              < FOForm .
  --- Conjunctions/Disjunctions
  subsort PosEqAtom                  < PosEqConj       < PosConj .
  subsort NegEqAtom                  < NegEqConj       < NegConj .
  subsort PosEqAtom                  < PosEqDisj       < PosDisj .
  subsort NegEqAtom                  < NegEqDisj       < NegDisj .
  subsort PosEqConj NegEqConj EqAtom < EqConj          < Conj .
  subsort PosEqDisj NegEqDisj EqAtom < EqDisj          < Disj .
  subsort TruthAtom                  < ConstConj       < PosConj NegConj < Conj .
  subsort TruthAtom                  < ConstDisj       < PosDisj NegDisj < Disj .
  subsort Truth+PosEqAtom            < PosConj PosDisj .
  subsort Truth+NegEqAtom            < NegConj NegDisj .
  --- Link Non-Empty/Possibly Empty Forms
  subsort TruthAtom       < TruthAtom?       . subsort PosEqAtom   < PosEqAtom?   .
  subsort Truth+PosEqAtom < Truth+PosEqAtom? . subsort NegEqAtom   < NegEqAtom?   .
  subsort EqAtom          < EqAtom?          . subsort Atom        < Atom?        .
  subsort Truth+NegEqAtom < Truth+NegEqAtom? . subsort PosEqConj   < PosEqConj?   .
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
  --- Possibly Empty Atoms
  subsort EmptyForm < TruthAtom?            < Truth+PosEqAtom? Truth+NegEqAtom? < Atom? .
  subsort EmptyForm < PosEqAtom? NegEqAtom? < EqAtom?                           < Atom? .
  subsort EmptyForm < TruthAtom? PosEqAtom? < Truth+PosEqAtom? .
  subsort EmptyForm < TruthAtom? NegEqAtom? < Truth+NegEqAtom? .
  --- Possibly Empty Non-Atoms
  subsort EmptyForm < PosEqConj? PosEqDisj?   < PosEqQFForm? < EqQFForm? .
  subsort EmptyForm < NegEqConj? NegEqDisj?   < NegEqQFForm? < EqQFForm? .
  subsort EmptyForm < EqConj? EqDisj? EqAtom? < EqQFForm?    < QFForm?   .
  subsort EmptyForm < Atom?                   < Conj? Disj?  < QFForm?   < FOForm? .
  subsort EmptyForm < AEQForm?                < FOForm? .
  --- Possibly Empty Conjunctions/Disjunctions
  subsort EmptyForm < PosEqAtom?                    < PosEqConj?        < PosConj? .
  subsort EmptyForm < NegEqAtom?                    < NegEqConj?        < NegConj? .
  subsort EmptyForm < PosEqAtom?                    < PosEqDisj?        < PosDisj? .
  subsort EmptyForm < NegEqAtom?                    < NegEqDisj?        < NegDisj? .
  subsort EmptyForm < PosEqConj? NegEqConj? EqAtom? < EqConj?           < Conj? .
  subsort EmptyForm < PosEqDisj? NegEqDisj? EqAtom? < EqDisj?           < Disj? .
  subsort EmptyForm < TruthAtom?                    < ConstConj?        < PosConj? NegConj? < Conj? .
  subsort EmptyForm < TruthAtom?                    < ConstDisj?        < PosDisj? NegDisj? < Disj? .
  subsort EmptyForm < Truth+PosEqAtom?              < PosConj? PosDisj? .
  subsort EmptyForm < Truth+NegEqAtom?              < NegConj? NegDisj? .

  --- Atomic Formulas
  op  mtForm :           -> EmptyForm [ctor] .
  op  tt     :           -> TrueAtom  [ctor] .
  op  ff     :           -> FalseAtom [ctor] .
  op  _?=_   : Term Term -> PosEqAtom [ctor comm prec 50] .
  op  _!=_   : Term Term -> NegEqAtom [ctor comm prec 50] .
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
  sort TruthAtomSet PosEqAtomSet NegEqAtomSet Truth+PosEqAtomSet Truth+NegEqAtomSet EqAtomSet AtomSet .
  sort ConstConjSet PosEqConjSet NegEqConjSet EqConjSet PosConjSet NegConjSet ConjSet .
  sort ConstDisjSet PosEqDisjSet NegEqDisjSet EqDisjSet PosDisjSet NegDisjSet DisjSet .
  sort PosEqQFFormSet NegEqQFFormSet EqQFFormSet QFFormSet AEQFormSet FOFormSet .
  sort EmptyFormSet PosEqAtom?Set NegEqAtom?Set TruthAtom?Set Truth+NegEqAtom?Set Truth+PosEqAtom?Set EqAtom?Set Atom?Set .
  sort ConstConj?Set PosEqConj?Set NegEqConj?Set EqConj?Set PosConj?Set NegConj?Set Conj?Set .
  sort ConstDisj?Set PosEqDisj?Set NegEqDisj?Set EqDisj?Set PosDisj?Set NegDisj?Set Disj?Set .
  sort PosEqQFForm?Set NegEqQFForm?Set EqQFForm?Set QFForm?Set AEQForm?Set FOForm?Set .
  --- Subsorting
  subsort EmptyForm       < EmptyFormSet       .
  subsort TruthAtom       < TruthAtomSet       . subsort TruthAtom?       < TruthAtom?Set       .
  subsort PosEqAtom       < PosEqAtomSet       . subsort PosEqAtom?       < PosEqAtom?Set       .
  subsort NegEqAtom       < NegEqAtomSet       . subsort NegEqAtom?       < NegEqAtom?Set       .
  subsort Truth+PosEqAtom < Truth+PosEqAtomSet . subsort Truth+PosEqAtom? < Truth+PosEqAtom?Set .
  subsort Truth+NegEqAtom < Truth+NegEqAtomSet . subsort Truth+NegEqAtom? < Truth+NegEqAtom?Set .
  subsort EqAtom          < EqAtomSet          . subsort EqAtom?          < EqAtom?Set          .
  subsort Atom            < AtomSet            . subsort Atom?            < Atom?Set            .
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
  --- Atoms Sets
  subsort FormEmptySet < PosEqAtomSet NegEqAtomSet < EqAtomSet          < AtomSet .
  subsort FormEmptySet < TruthAtomSet PosEqAtomSet < Truth+PosEqAtomSet < AtomSet .
  subsort FormEmptySet < TruthAtomSet NegEqAtomSet < Truth+NegEqAtomSet < AtomSet .
  --- Non-Atom Sets
  subsort PosEqConjSet PosEqDisjSet     < PosEqQFFormSet     < EqQFFormSet .
  subsort NegEqConjSet NegEqDisjSet     < NegEqQFFormSet     < EqQFFormSet .
  subsort EqConjSet EqDisjSet EqAtomSet < EqQFFormSet        < QFFormSet .
  subsort AtomSet                       < ConjSet DisjSet    < QFFormSet   < FOFormSet .
  subsort FormEmptySet                  < AEQFormSet         < FOFormSet .
  --- Conjunctions/Disjunctions Sets
  subsort PosEqAtomSet                        < PosEqConjSet          < PosConjSet .
  subsort NegEqAtomSet                        < NegEqConjSet          < NegConjSet .
  subsort PosEqAtomSet                        < PosEqDisjSet          < PosDisjSet .
  subsort NegEqAtomSet                        < NegEqDisjSet          < NegDisjSet .
  subsort PosEqConjSet NegEqConjSet EqAtomSet < EqConjSet             < ConjSet .
  subsort PosEqDisjSet NegEqDisjSet EqAtomSet < EqDisjSet             < DisjSet .
  subsort TruthAtomSet                        < ConstConjSet          < PosConjSet NegConjSet < ConjSet .
  subsort TruthAtomSet                        < ConstDisjSet          < PosDisjSet NegDisjSet < DisjSet .
  subsort Truth+PosEqAtomSet                  < PosConjSet PosDisjSet .
  subsort Truth+NegEqAtomSet                  < NegConjSet NegDisjSet .
  --- Link Non-Empty/Possibly Empty Forms
  subsort TruthAtomSet       < TruthAtom?Set       . subsort PosEqAtomSet       < PosEqAtom?Set       .
  subsort NegEqAtomSet       < NegEqAtom?Set       . subsort EqAtomSet          < EqAtom?Set          .
  subsort Truth+PosEqAtomSet < Truth+PosEqAtom?Set . subsort Truth+NegEqAtomSet < Truth+NegEqAtom?Set .
  subsort AtomSet            < Atom?Set            . subsort NegEqConjSet       < NegEqConj?Set       .
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
  --- Possibly Empty Atoms Sets
  subsort FormEmptySet < EmptyFormSet .
  subsort EmptyFormSet < PosEqAtom?Set NegEqAtom?Set < EqAtom?Set          < Atom?Set .
  subsort EmptyFormSet < TruthAtom?Set PosEqAtom?Set < Truth+PosEqAtom?Set < Atom?Set .
  subsort EmptyFormSet < TruthAtom?Set NegEqAtom?Set < Truth+NegEqAtom?Set < Atom?Set .
  --- Possibly Empty Non-Atom Sets
  subsort PosEqConj?Set PosEqDisj?Set      < PosEqQFForm?Set   < EqQFForm?Set .
  subsort NegEqConj?Set NegEqDisj?Set      < NegEqQFForm?Set   < EqQFForm?Set .
  subsort EqConj?Set EqDisj?Set EqAtom?Set < EqQFForm?Set      < QFForm?Set .
  subsort Atom?Set                         < Conj?Set Disj?Set < QFForm?Set < FOForm?Set .
  subsort EmptyFormSet                     < AEQForm?Set       < FOForm?Set .
  --- Possibly Empty Conjunctions/Disjunctions
  subsort EmptyFormSet < PosEqAtom?Set                          < PosEqConj?Set           < PosConj?Set .
  subsort EmptyFormSet < NegEqAtom?Set                          < NegEqConj?Set           < NegConj?Set .
  subsort EmptyFormSet < PosEqAtom?Set                          < PosEqDisj?Set           < PosDisj?Set .
  subsort EmptyFormSet < NegEqAtom?Set                          < NegEqDisj?Set           < NegDisj?Set .
  subsort EmptyFormSet < PosEqConj?Set NegEqConj?Set EqAtom?Set < EqConj?Set              < Conj?Set .
  subsort EmptyFormSet < PosEqDisj?Set NegEqDisj?Set EqAtom?Set < EqDisj?Set              < Disj?Set .
  subsort EmptyFormSet < TruthAtom?Set                          < ConstConj?Set           < PosConj?Set NegConj?Set < Conj?Set .
  subsort EmptyFormSet < TruthAtom?Set                          < ConstDisj?Set           < PosDisj?Set NegDisj?Set < Disj?Set .
  subsort EmptyFormSet < Truth+PosEqAtom?Set                    < PosConj?Set PosDisj?Set .
  subsort EmptyFormSet < Truth+NegEqAtom?Set                    < NegConj?Set NegDisj?Set .
  --- Empty [Formula Sets]
  op mtFormSet :                                    -> FormEmptySet        [ctor] .
  op _|_ : FormEmptySet        FormEmptySet         -> FormEmptySet        [ctor assoc comm id: mtFormSet prec 53] .
  --- [Non-Empty Formula] Sets
  op _|_ : PosEqAtomSet        PosEqAtomSet         -> PosEqAtomSet        [ctor ditto] .
  op _|_ : NegEqAtomSet        NegEqAtomSet         -> NegEqAtomSet        [ctor ditto] .
  op _|_ : TruthAtomSet        TruthAtomSet         -> TruthAtomSet        [ctor ditto] .
  op _|_ : Truth+PosEqAtomSet  Truth+PosEqAtomSet   -> Truth+PosEqAtomSet  [ctor ditto] .
  op _|_ : Truth+NegEqAtomSet  Truth+NegEqAtomSet   -> Truth+NegEqAtomSet  [ctor ditto] .
  op _|_ : EqAtomSet           EqAtomSet            -> EqAtomSet           [ctor ditto] .
  op _|_ : AtomSet             AtomSet              -> AtomSet             [ctor ditto] .
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
  op _|_ : PosEqAtom?Set        PosEqAtom?Set       -> PosEqAtom?Set       [ctor ditto] .
  op _|_ : NegEqAtom?Set        NegEqAtom?Set       -> NegEqAtom?Set       [ctor ditto] .
  op _|_ : TruthAtom?Set        TruthAtom?Set       -> TruthAtom?Set       [ctor ditto] .
  op _|_ : Truth+PosEqAtom?Set  Truth+PosEqAtom?Set -> Truth+PosEqAtom?Set [ctor ditto] .
  op _|_ : Truth+NegEqAtom?Set  Truth+NegEqAtom?Set -> Truth+NegEqAtom?Set [ctor ditto] .
  op _|_ : EqAtom?Set           EqAtom?Set          -> EqAtom?Set          [ctor ditto] .
  op _|_ : Atom?Set             Atom?Set            -> Atom?Set            [ctor ditto] .
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

fmod FOFORM-DEFINEDOPS is
  pr FOFORM .
  op _=>_  : FOForm FOForm -> FOForm .
  op _<=>_ : FOForm FOForm -> FOForm .
  var F1 F2 : FOForm .
  eq F1  => F2 = (~ F1) \/ F2 .
  eq F1 <=> F2 = (F1 => F2) /\ (F2 => F1) .
endfm

fmod FOFORMSIMPLIFY-IMPL is
  pr FOFORM .
  var C : Conj . var D : Disj . vars F F' : FOForm . vars T T' : Term .
  --- Repeated subformula in Conj/Disj
  eq F /\ F = F .
  eq F \/ F = F .
  --- Negated TruthAtom
  eq ~ tt = ff .
  eq ~ ff = tt .
  --- TruthAtom in Conj/Disj
  eq ff /\ C = ff .
  eq tt /\ C = C  .
  eq tt \/ D = tt .
  eq ff \/ D = D  .
  --- Negated Formula
  eq F  \/ ~ F = tt .
  eq F  /\ ~ F = ff .
  --- De Morgan's Laws
  eq ~(F /\ F') = ~ F \/ ~ F' .
  eq ~(F \/ F') = ~ F /\ ~ F' .
  --- Negated Equality/Disequality
  eq ~(T ?= T') = T != T' .
  eq ~(T != T') = T ?= T' .
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
  pr FOFORM .
  pr UNIT .
  op reduce  : Module FOForm? -> FOForm? .
  op reduce  : Module FOForm  -> FOForm  .
  op reduceL : Module EqAtom  ~> EqAtom  .
  op reduceL : Bool Term Term ~> EqAtom  .
  var F : FOForm . var F? : FOForm?  . var E : EqAtom . var T T' : Term . var M : Module .
  var L : Atom   . var Q  : NeQidSet . var B : Bool . var TK TK' : [Term] .
  eq reduce(M,E /\ F?)   = reduceL(M,E) /\ reduce(M,F?) .
  eq reduce(M,E \/ F?)   = reduceL(M,E) \/ reduce(M,F?) .
  eq reduce(M,L /\ F?)   = L /\ reduce(M,F?) [owise] .
  eq reduce(M,L \/ F?)   = L \/ reduce(M,F?) [owise] .
  eq reduce(M,mtForm)    = mtForm .
  eq reduce(M,A[Q] F)    = A[Q] reduce(M,F) .
  eq reduce(M,E[Q] F)    = E[Q] reduce(M,F) .
  eq reduce(M,~ F)       = ~ reduce(M,F) .
  eq reduceL(M,T ?= T')  = reduceL(true, getTerm(metaReduce(M,T)),getTerm(metaReduce(M,T'))) .
  eq reduceL(M,T != T')  = reduceL(false,getTerm(metaReduce(M,T)),getTerm(metaReduce(M,T'))) .
  eq reduceL(true,T,T')  = T ?= T' .
  eq reduceL(false,T,T') = T != T' .
endfm

fmod FOFORMREDUCE is
  pr FOFORM .
  pr REFLECT .
  op  reduce : Module FOForm -> FOForm .
  op $reduce : [FOForm] -> FOForm .
  var M : Module . var F : FOForm . var S : String . var G : [FOForm] .
  eq  reduce(M,F)      = downTerm(redReflect('FOFORMREDUCE-IMPL,'reduce[upTerm(M),upTerm(F)]),error("FOForm Reduce Failed")) .
  eq $reduce(F)        = F .
  eq $reduce(error(S)) = error(S) .
  eq $reduce(G)        = error("Formula IllFormed") [owise] .
endfm

fmod FOFORM-OPERATIONS is
  pr FOFORM .
  pr EXT-TERM .      --- defines vars() : Term -> QidSet
  op  size       : FOForm? -> Nat .
  op  depth      : FOForm? -> Nat .
  op  falseLit?  : Conj -> Bool .
  op  wellFormed : Module FOForm? -> Bool .
  op $wellFormed : Module FOForm? -> Bool .
  op  toUnifProb : PosConj -> UnificationProblem .
  op $toUnifProb : PosConj -> UnificationProblem .
  op  trueId     : FOForm? -> FOForm  .
  op  falseId    : FOForm? -> FOForm  .
  op  true2mt    : FOForm? -> FOForm? .
  op  false2mt   : FOForm? -> FOForm? .
  op  vars       : FOForm? -> QidSet  .
  var M : Module . var F? : FOForm? . var F1 F2 : FOForm . var QS : NeQidSet .
  var TA : TruthAtom . var T T' : Term . var PC : PosConj . var C : Conj .
  --- get the size/depth of a formula
  eq size(A[QS] F1)  = s(size(F1)) .
  eq size(E[QS] F1)  = s(size(F1)) .
  eq size(F1 /\ F2)  = s(size(F1) + size(F2)) .
  eq size(F1 \/ F2)  = s(size(F1) + size(F2)) .
  eq size(~ F1)      = s(size(F1)) .
  eq size(A:Atom)    = 1 .
  eq size(mtForm)    = 0 .
  eq depth(A[QS] F1) = s(depth(F1)) .
  eq depth(E[QS] F1) = s(depth(F1)) .
  eq depth(F1 /\ F2) = s(max(depth(F1),depth(F2))) .
  eq depth(F1 \/ F2) = s(max(depth(F1),depth(F2))) .
  eq depth(~ F1)     = s(depth(F1)) .
  eq depth(A:Atom)   = 1 .
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
  eq $wellFormed(M,T ?= T')  = wellFormed(M,T) and-then wellFormed(M,T') .
  eq $wellFormed(M,T != T')  = wellFormed(M,T) and-then wellFormed(M,T') .
  --- true/false lit or mtForm
  eq $wellFormed(M,TA)       = true .
  eq $wellFormed(M,mtForm)   = true .
  --- INP: FOForm?
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
 ceq  toUnifProb(PC)              = $toUnifProb(PC) if not falseLit?(PC) .
  eq $toUnifProb((T ?= T') /\ PC) = T =? T' /\ $toUnifProb(PC) .
  eq $toUnifProb(T ?= T')         = T =? T' .
  --- INP: Conj
  --- PRE: N/A
  --- OUT: true iff Conj contains a false literal
  eq  falseLit?(ff /\ C) = true .
  eq  falseLit?(C)       = false [owise] .
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
  sort FOFormNatPair FOFormPair .
  op ((_,_)) : FOForm? Nat     -> FOFormNatPair [ctor] .
  op ((_,_)) : FOForm? FOForm? -> FOFormPair    [ctor] .
endfm

fmod FOFORM-SUBSTITUTION is
  pr META-LEVEL .
  pr SUBSTITUTION-HANDLING . --- from full-maude
  pr FOFORM .
  op _<<_ : FOForm? Substitution -> FOForm? .
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
endfm

fmod FOFORM-VARS-TO-CONSTS is
  pr VARIABLES-TO-CONSTANTS .
  pr FOFORM .
  pr FOFORM-SUBSTITUTION .
  op varsToConsts  : Substitution QFForm? -> QFForm? .
  op constsToVars  : Substitution QFForm? -> QFForm? .

  var S : Substitution . var F1 F2 : QFForm . var T T' : Term .

  eq varsToConsts(S,F1)       = F1 << S .
  ---
  eq constsToVars(S,mtForm)   = mtForm .
  eq constsToVars(S,F1 /\ F2) = constsToVars(S,F1) /\ constsToVars(S,F2) .
  eq constsToVars(S,F1 \/ F2) = constsToVars(S,F1) \/ constsToVars(S,F2) .
  eq constsToVars(S,~ F1)     = ~ constsToVars(S,F1) .
  eq constsToVars(S,T ?= T')  = constsToVars(S,T) ?= constsToVars(S,T') .
  eq constsToVars(S,T != T')  = constsToVars(S,T) != constsToVars(S,T') .
endfm

fmod FOFORMSUBSTITUTION-PAIR is
  pr FOFORM-SUBSTITUTION .
  sort FOFormSubstPair .
  op ((_,_)) : FOForm? Substitution -> FOFormSubstPair [ctor] .
endfm

fmod FOFORM-SUBSTITUTIONSET is
  pr META-LEVEL .
  pr SUBSTITUTIONSET . --- from full-maude
  pr FOFORMSET .
  pr FOFORM-SUBSTITUTION .
  op _<<_ : FOForm? SubstitutionSet -> FOForm?Set .
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
  op toSet     : FOFormSubstPairSet -> FOFormSet .
  op build     : FOForm? SubstitutionSet -> FOFormSubstPairSet .
  op build-app : FOForm? SubstitutionSet -> FOFormSubstPairSet .
  ---
  var F : FOForm? . var S : Substitution . var FPS : FOFormSubstPairSet . var SS : SubstitutionSet .
  eq toSet((F,S) | FPS)            = F | toSet(FPS) .
  eq toSet(mtFSPS)                 = mtFormSet .
  eq build(F,S | SS)               = (F,S) | build(F,SS) .
  eq build(F,.SubstitutionSet)     = mtFSPS .
  eq build-app(F,S | SS)           = (F << S,S) | build-app(F,SS) .
  eq build-app(F,.SubstitutionSet) = mtFSPS .
endfm

fmod FQF-IMPL is
  pr FOFORM-SUBSTITUTION .
  pr FOFORM-TUPLES .
  pr RENAME-METAVARS .
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
  eq renameQuantifiers(M,F)  =
       $rq($rq(0,downTerm(renameAllVar(M,upTerm(F)),error("Rename failed")))) [owise] .
  --- dispatch handlers for different cases
  eq $rq((F,N))            = F .
  eq $rq(N,P:QFForm?)       = (P:QFForm?,N) .
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
  sort EmptyForm TrueAtom FalseAtom NNF Form .
  subsort EmptyForm TrueAtom FalseAtom < NNF < Form .
  --- Negation Normal Forms
  op  mtForm :             -> EmptyForm [ctor] .
  op  tt     :             -> TrueAtom  [ctor] .
  op  ff     :             -> FalseAtom [ctor] .
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
  sort EmptyForm TrueAtom FalseAtom QFF EPNF APNF PNF Form .
  subsort EmptyForm TrueAtom FalseAtom < QFF < PNF < Form .
  --- Quantifier Free Formulas
  op  mtForm :              -> EmptyForm [ctor] .
  op  tt     :              -> TrueAtom  [ctor] .
  op  ff     :              -> FalseAtom [ctor] .
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
  sort EmptyForm TrueAtom FalseAtom Conj DNF QDNF Form .
  subsort EmptyForm TrueAtom FalseAtom < Conj < DNF < QDNF < Form .
  --- Disjunctive Normal Forms
  op  mtForm :             -> EmptyForm [ctor] .
  op  tt     :             -> TrueAtom  [ctor] .
  op  ff     :             -> FalseAtom [ctor] .
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
  sort EmptyForm TrueAtom FalseAtom Disj CNF QCNF Form .
  subsort EmptyForm TrueAtom FalseAtom < Disj < CNF < QCNF < Form .
  --- Disjunctive Normal Forms
  op  mtForm :             -> EmptyForm [ctor] .
  op  tt     :             -> TrueAtom  [ctor] .
  op  ff     :             -> FalseAtom [ctor] .
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
  pr FOFORMSET .
  pr CNF .
  pr DNF .
  op disj-join  : FOForm?Set -> FOForm? .
  op conj-join  : FOForm?Set -> FOForm? .
  ---
  op toDisjSet  : QFForm? ~> DisjSet .
  op toDisjSet' : QFForm? ~> DisjSet .
  op toConjSet  : QFForm? ~> ConjSet .
  op toConjSet' : QFForm? ~> ConjSet .
  var FS : FOForm?Set . var FF : FOForm? . var F : QFForm? . var D : Disj . var C : Conj .
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
endfm

--- this module defines a generic structure to represent the success/failure
--- of a formula reduction --- which also includes an optional status code:
--- true/false/unknown/errb --- to represent the result of the reduction
fmod GENERIC-FORMULA-REDUCTION is
  pr BOOL-ERR .
  pr FOFORMSET .
  sort QFFormSetBoolPair .
  op ((_,_)) : QFForm?Set Bool? -> QFFormSetBoolPair [ctor] .
  ---
  op true? : QFFormSetBoolPair -> Bool  .
  op bool  : QFFormSetBoolPair -> Bool? .
  op form  : QFFormSetBoolPair -> QFForm?Set .
  --- projections
  var B : Bool? . var F : QFForm?Set .
  eq true?((F,B)) = B == true .
  eq form ((F,B)) = F .
  eq bool ((F,B)) = B .
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
