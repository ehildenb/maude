Meta-Level Extensions
=====================

This file extends the meta-level with many additional operations

TERMS, TERMSETS, QIDSETS, AND REFINEMENTS
-----------------------------------------

```maude
sload ../base/prelude-aux.maude
sload terms.maude

fmod TERM-EXTRA is
  pr META-LEVEL .
  op vars            : Term                 -> QidSet .
  op vars            : TermList             -> QidSet .
  op  getName        : QidSet               -> [QidSet] .
  op  getType        : QidSet               -> [QidSet] .
  op  filterByType   : QidSet TypeSet       -> [QidSet] .
  op  varsToTermList : QidSet               -> [TermList] .
  op  subterms       : Term                 -> TermList .
  op  repeatedNames  : QidSet               -> QidSet .
  op $repeatedNames  : QidSet QidSet QidSet -> QidSet .

  var TQ : TermQid . var Q : Qid . var QS QS' QS'' : QidSet . var V : Variable .
  var T : Term . var NTL : NeTermList . var C : Constant . var TS : TypeSet .
  var M : Module . var TL : TermList .

  eq vars(V) = V .
  eq vars(C) = none .
  eq vars(Q[NTL]) = vars(NTL) .
  eq vars(empty) = none .
  eq vars((T, TL)) = vars(T) ; vars(TL) .

  --- INP: QidSet
  --- PRE: Each Qid in QidSet is a TermQid
  --- OUT: QidSet composed of the name/sort of each TermQid
  eq getName(TQ ; Q ; QS)   = getName(TQ) ; getName(Q ; QS) .
  eq getName(none)          = none .
  eq getType(TQ ; Q ; QS)   = getType(TQ) ; getType(Q ; QS) .
  eq getType(none)          = none .

  --- INP: QidSet
  --- PRE: Each Qid in QidSet is a TermQid
  --- OUT: Set of names (obtained by getName) which are not unique
  eq  repeatedNames(QS)               = $repeatedNames(QS,none,none) .
  eq $repeatedNames(TQ ; QS,QS',QS'') = if getName(TQ) in QS' then $repeatedNames(QS,QS',QS'' ; getName(TQ))
                                                              else $repeatedNames(QS,QS' ; getName(TQ),QS'') fi .
  eq $repeatedNames(none,QS',QS'')    = QS'' .

  --- INP: QidSet
  --- PRE: Each Qid in QidSet is a TermQid
  --- OUT: QidSet composed of only those TermQids who Type is in TypeSet
  eq filterByType(TQ ; QS,TS) =
    if getType(TQ) in TS then TQ else none fi ; filterByType(QS,TS) .
  eq filterByType(none,TS)    = none .

  --- INP: QidSet
  --- PRE: Each Qid in QidSet is a Variable
  --- OUT: A TermList where each Variable occurs in an undefined order
  eq varsToTermList(V ; QS) = V,varsToTermList(QS) .
  eq varsToTermList(none)   = empty .

  --- INP: Term
  --- PRE: None
  --- OUT: The list of subterms from this term
  eq subterms(Q[NTL]) = NTL .
  eq subterms(C)      = empty .
  eq subterms(V)      = empty .
endfm

fmod GTERMLIST-REFINEMENT is
  pr META-TERM .
  sort GTerm NeGTermList .
  subsort Term Context < GTerm   < NeGTermList < GTermList .
  subsort NeTermList NeCTermList < NeGTermList .
  op _,_ : NeGTermList GTermList -> NeGTermList [ctor ditto] .
  op _,_ : GTermList NeGTermList -> NeGTermList [ctor ditto] .
endfm

fmod TERMSET-FM is
  pr META-LEVEL .
  pr TERM-SET .
endfm

fmod QIDSET-REFINEMENT is
  pr META-MODULE .
  ---
  sort VariableSet   ConstantSet   TermQidSet    .
  sort NeVariableSet NeConstantSet NeTermQidSet  .
  ---
  subsort Variable < NeVariableSet < VariableSet .
  subsort Constant < NeConstantSet < ConstantSet .
  subsort TermQid  < NeTermQidSet  < TermQidSet  .
  ---
  subsort EmptyTypeSet < ConstantSet VariableSet     .
  subsort VariableSet   ConstantSet   < TermQidSet   .
  subsort NeVariableSet NeConstantSet < NeTermQidSet .
  ---
  subsort TermQidSet   < QidSet   .
  subsort NeTermQidSet < NeQidSet .
  ---
  op _;_ : TermQidSet TermQidSet     -> TermQidSet    [ctor ditto] .
  op _;_ : NeTermQidSet TermQidSet   -> NeTermQidSet  [ctor ditto] .
  op _;_ : ConstantSet ConstantSet   -> ConstantSet   [ctor ditto] .
  op _;_ : NeConstantSet ConstantSet -> NeConstantSet [ctor ditto] .
  op _;_ : VariableSet VariableSet   -> VariableSet   [ctor ditto] .
  op _;_ : NeVariableSet VariableSet -> NeVariableSet [ctor ditto] .
endfm
```

Module Operations
-----------------

```maude
fmod UNIT-FM is
  inc META-LEVEL .

  op noModule : -> Module [ctor] .

  op emptyFModule : -> FModule .
  eq emptyFModule = fmod 'fmod is nil sorts none . none none none none endfm .
  op emptyFTheory : -> FModule .
  eq emptyFTheory = fth 'fth is nil sorts none . none none none none endfth .
  op emptySModule : -> SModule .
  eq emptySModule = mod 'mod is nil sorts none . none none none none none endm .
  op emptySTheory : -> SModule .
  eq emptySTheory = th 'th is nil sorts none . none none none none none endth .

  op getName : Module -> Header .
  op getPars : Module -> ParameterDeclList .

  op setName : Module ModuleExpression -> Module .
  op setName : Module ParameterDecl -> Module .
  op setPars : Module ParameterDeclList -> Module .
  op setImports : Module ImportList -> Module .
  op setSorts : Module SortSet -> Module .
  op setSubsorts : Module SubsortDeclSet -> Module .
  op setOps : Module OpDeclSet -> Module .
  op setMbs : Module MembAxSet -> Module .
  op setEqs : Module EquationSet -> Module .
  op setRls : Module RuleSet ~> Module .

  op addImports : ImportList Module -> Module .
  op addSorts : SortSet Module -> Module .
  op addSubsorts : [SubsortDeclSet] Module -> Module .
  op addOps : [OpDeclSet] Module -> Module .
  op addMbs : MembAxSet Module -> Module .
  op addEqs : EquationSet Module -> Module .
  op addRls : RuleSet Module -> Module .
  op addDecls : Module Module -> Module .

  vars M M' M'' : Module .
  vars SSDS SSDS' SSDS'' : SubsortDeclSet .
  vars OPD OPD' : OpDecl .
  vars OPDS OPDS' : OpDeclSet .
  vars MAS MAS' : MembAxSet .
  vars Eq Eq' : Equation .
  vars EqS EqS' : EquationSet .
  vars Rl Rl' : Rule .
  vars RlS RlS' : RuleSet .
  vars SS SS' : SortSet .
  vars IL IL' : ImportList .
  vars PL PL' : ParameterList .
  vars U U' : Module .
  vars I I' : Import .
  vars ME ME' : ModuleExpression .
  vars PD PD' : ParameterDecl .
  vars PDL PDL' : ParameterDeclList .
  var  H H' : Header .

  eq getName(noModule) = ' .
  eq getName(mod ME is IL sorts SS . SSDS OPDS MAS EqS RlS endm) = ME .
  eq getName(mod ME{PDL} is IL sorts SS . SSDS OPDS MAS EqS RlS endm) = ME .
  eq getName(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth) = H .
  eq getName(fmod ME is IL sorts SS . SSDS OPDS MAS EqS endfm) = ME .
  eq getName(fmod ME{PDL} is IL sorts SS . SSDS OPDS MAS EqS endfm) = ME .
  eq getName(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth) = H .

  eq getImports(noModule) = nil .
  eq getImports(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm) = IL .
  eq getImports(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth) = IL .
  eq getImports(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm) = IL .
  eq getImports(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth) = IL .

  eq getPars(noModule) = nil .
  eq getPars(mod ME is IL sorts SS . SSDS OPDS MAS EqS RlS endm) = nil .
  eq getPars(mod ME{PDL} is IL sorts SS . SSDS OPDS MAS EqS RlS endm) = PDL .
  eq getPars(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth) = nil .
  eq getPars(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth) = nil .
  eq getPars(fmod ME is IL sorts SS . SSDS OPDS MAS EqS endfm) = nil .
  eq getPars(fmod ME{PDL} is IL sorts SS . SSDS OPDS MAS EqS endfm) = PDL .
  eq getPars(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth) = nil .

  eq getSorts(noModule) = none .
  eq getSorts(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm) = SS .
  eq getSorts(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth) = SS .
  eq getSorts(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm) = SS .
  eq getSorts(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth) = SS .

  eq getSubsorts(noModule) = none .
  eq getSubsorts(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm) = SSDS .
  eq getSubsorts(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth) = SSDS .
  eq getSubsorts(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm) = SSDS .
  eq getSubsorts(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth) = SSDS .

  eq getOps(noModule) = none .
  eq getOps(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm) = OPDS .
  eq getOps(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth) = OPDS .
  eq getOps(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm) = OPDS .
  eq getOps(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth) = OPDS .

  eq getMbs(noModule) = none .
  eq getMbs(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm) = MAS .
  eq getMbs(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth) = MAS .
  eq getMbs(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm) = MAS .
  eq getMbs(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth) = MAS .

  eq getEqs(noModule) = none .
  eq getEqs(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm) = EqS .
  eq getEqs(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth) = EqS .
  eq getEqs(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm) = EqS .
  eq getEqs(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth) = EqS .

  eq getRls(noModule) = none .
  eq getRls(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm) = RlS .
  eq getRls(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth) = RlS .
  eq getRls(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm) = none .
  eq getRls(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth) = none .

  eq setImports(noModule, IL) = noModule .
  eq setImports(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm, IL')
    = mod H is IL' sorts SS . SSDS OPDS MAS EqS RlS endm .
  eq setImports(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth, IL')
    = th H is IL' sorts SS . SSDS OPDS MAS EqS RlS endth .
  eq setImports(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm, IL')
    = fmod H is IL' sorts SS . SSDS OPDS MAS EqS endfm .
  eq setImports(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth, IL')
    = fth H is IL' sorts SS . SSDS OPDS MAS EqS endfth .

  eq setOps(noModule, OPDS) = noModule .
  eq setOps(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm, OPDS')
    = mod H is IL sorts SS . SSDS OPDS' MAS EqS RlS endm .
  eq setOps(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth, OPDS')
    = th H is IL sorts SS . SSDS OPDS' MAS EqS RlS endth .
  eq setOps(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm, OPDS')
    = fmod H is IL sorts SS . SSDS OPDS' MAS EqS endfm .
  eq setOps(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth, OPDS')
    = fth H is IL sorts SS . SSDS OPDS' MAS EqS endfth .

  eq setSubsorts(noModule, SSDS) = noModule .
  eq setSubsorts(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm, SSDS')
    = mod H is IL sorts SS . SSDS' OPDS MAS EqS RlS endm .
  eq setSubsorts(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth, SSDS')
    = th H is IL sorts SS . SSDS' OPDS MAS EqS RlS endth .
  eq setSubsorts(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm, SSDS')
    = fmod H is IL sorts SS . SSDS' OPDS MAS EqS endfm .
  eq setSubsorts(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth, SSDS')
    = fth H is IL sorts SS . SSDS' OPDS MAS EqS endfth .

  eq setMbs(noModule, MAS) = noModule .
  eq setMbs(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm, MAS')
    = mod H is IL sorts SS . SSDS OPDS MAS' EqS RlS endm .
  eq setMbs(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth, MAS')
    = th H is IL sorts SS . SSDS OPDS MAS' EqS RlS endth .
  eq setMbs(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm, MAS')
    = fmod H is IL sorts SS . SSDS OPDS MAS' EqS endfm .
  eq setMbs(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth, MAS')
    = fth H is IL sorts SS . SSDS OPDS MAS' EqS endfth .

  eq setEqs(noModule, EqS) = noModule .
  eq setEqs(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm, EqS')
    = mod H is IL sorts SS . SSDS OPDS MAS EqS' RlS endm .
  eq setEqs(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth, EqS')
    = th H is IL sorts SS . SSDS OPDS MAS EqS' RlS endth .
  eq setEqs(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm, EqS')
    = fmod H is IL sorts SS . SSDS OPDS MAS EqS' endfm .
  eq setEqs(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth, EqS')
    = fth H is IL sorts SS . SSDS OPDS MAS EqS' endfth .

  eq setRls(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm, RlS')
    = mod H is IL sorts SS . SSDS OPDS MAS EqS RlS' endm .
  eq setRls(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth, RlS')
    = th H is IL sorts SS . SSDS OPDS MAS EqS RlS' endth .
  eq setRls(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm, RlS)
    = if RlS == none
      then fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm
      else mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm
      fi .
  eq setRls(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth, RlS)
    = if RlS == none
      then fth H is IL sorts SS . SSDS OPDS MAS EqS endfth
      else th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth
      fi .

  eq setSorts(noModule, SS) = noModule .
  eq setSorts(mod H is IL sorts SS . SSDS OPDS MAS EqS RlS endm, SS')
    = mod H is IL sorts SS' . SSDS OPDS MAS EqS RlS endm .
  eq setSorts(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth, SS')
    = th H is IL sorts SS' . SSDS OPDS MAS EqS RlS endth .
  eq setSorts(fmod H is IL sorts SS . SSDS OPDS MAS EqS endfm, SS')
    = fmod H is IL sorts SS' . SSDS OPDS MAS EqS endfm .
  eq setSorts(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth, SS')
    = fth H is IL sorts SS' . SSDS OPDS MAS EqS endfth .

  eq setPars(mod ME is IL sorts SS . SSDS OPDS MAS EqS RlS endm, PDL)
    = if PDL == nil
      then mod ME is IL sorts SS . SSDS OPDS MAS EqS RlS endm
      else mod ME{PDL} is IL sorts SS . SSDS OPDS MAS EqS RlS endm
      fi .
  eq setPars(mod ME{PDL} is IL sorts SS . SSDS OPDS MAS EqS RlS endm, PDL')
    = if PDL' == nil
      then mod ME is IL sorts SS . SSDS OPDS MAS EqS RlS endm
      else mod ME{PDL'} is IL sorts SS . SSDS OPDS MAS EqS RlS endm
      fi .
  eq setPars(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth, PDL)
    = th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth .
  eq setPars(fmod ME is IL sorts SS . SSDS OPDS MAS EqS endfm, PDL)
    = if PDL == nil
      then fmod ME is IL sorts SS . SSDS OPDS MAS EqS endfm
      else fmod ME{PDL} is IL sorts SS . SSDS OPDS MAS EqS endfm
      fi .
  eq setPars(fmod ME{PDL} is IL sorts SS . SSDS OPDS MAS EqS endfm, PDL')
    = if PDL' == nil
      then fmod ME is IL sorts SS . SSDS OPDS MAS EqS endfm
      else fmod ME{PDL'} is IL sorts SS . SSDS OPDS MAS EqS endfm
      fi .
  eq setPars(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth, PDL)
    = fth H is IL sorts SS . SSDS OPDS MAS EqS endfth .

  eq setName(mod ME is IL sorts SS . SSDS OPDS MAS EqS RlS endm, ME')
    = mod ME' is IL sorts SS . SSDS OPDS MAS EqS RlS endm .
  eq setName(mod ME{PDL} is IL sorts SS . SSDS OPDS MAS EqS RlS endm, ME')
    = mod ME'{PDL} is IL sorts SS . SSDS OPDS MAS EqS RlS endm .
  eq setName(fmod ME is IL sorts SS . SSDS OPDS MAS EqS endfm, ME')
    = fmod ME' is IL sorts SS . SSDS OPDS MAS EqS endfm .
  eq setName(fmod ME{PDL} is IL sorts SS . SSDS OPDS MAS EqS endfm, ME')
    = fmod ME'{PDL} is IL sorts SS . SSDS OPDS MAS EqS endfm .
  eq setName(fth H is IL sorts SS . SSDS OPDS MAS EqS endfth, H')
    = fth H' is IL sorts SS . SSDS OPDS MAS EqS endfth .
  eq setName(th H is IL sorts SS . SSDS OPDS MAS EqS RlS endth, H')
    = th H' is IL sorts SS . SSDS OPDS MAS EqS RlS endth .
  eq setName(noModule, ME) = noModule .

  eq addSorts(SS, U) = setSorts(U, (SS ; getSorts(U))) .
  eq addSorts(SS, noModule) = noModule .
  eq addSubsorts(SSDS, U) = setSubsorts(U, (SSDS getSubsorts(U))) .
  eq addSubsorts(SSDS, noModule) = noModule .
  eq addOps(OPDS, U) = setOps(U, (OPDS getOps(U))) .
  eq addMbs(MAS, U) = setMbs(U, (MAS getMbs(U))) .
  eq addMbs(MAS, noModule) = noModule .
  eq addEqs(EqS, U) = setEqs(U, (EqS getEqs(U))) .
  eq addEqs(EqS, noModule) = noModule .
  eq addRls(RlS, U) = setRls(U, (RlS getRls(U))) .
  eq addRls(RlS, noModule) = noModule .
  eq addImports(IL, U) = setImports(U, (getImports(U) IL)) .
  eq addImports(IL, noModule) = noModule .

  eq addDecls(noModule, U) = U .
  eq addDecls(U, noModule) = U .
  eq addDecls(U, U')
    = addImports(getImports(U'),
        addSorts(getSorts(U'),
          addSubsorts(getSubsorts(U'),
            addOps(getOps(U'),
              addMbs(getMbs(U'),
                addEqs(getEqs(U'),
                  if U' :: FModule or U' :: FTheory
                  then U
                  else addRls(getRls(U'),U)
                  fi))))))
    [owise] .
endfm

fmod MOD-EXTRA is
  pr UNIT-FM .
  op protecting? : ModuleExpression ImportList -> Bool .
  op setRls      : Module QidList -> [Module] .
  op setRls      : QidList RuleSet RuleSet -> [RuleSet] .
  op getModQid   : Import -> Qid .
  op getImports2 : Module -> [ImportList] .
  op getImports2 : Qid -> [ImportList] .
  op getImports2 : ImportList ImportList -> [ImportList] .
  op getImports2 : Module ImportList ImportList -> [ImportList] .

  var ME ME' : ModuleExpression . var M M' : Module . var I : Import .
  var Q : Qid . var QL : QidList . var L R : Term . var A : AttrSet .
  var RL : Rule . var RS RS' : RuleSet . var C : Condition .
  var IL IL1 IL2 : ImportList .

  --- INP: ModuleExpression ImportList
  --- PRE: None
  --- OUT: true iff ModuleExpression is protected in ImportList
  eq protecting?(ME,protecting ME' . IL) = ME == ME' or-else protecting?(ME,IL) .
  eq protecting?(ME,I IL) = protecting?(ME,IL) [owise] .
  eq protecting?(ME,nil) = false .

  --- INP: Module QidList
  --- PRE: Each Qid in QidList refers to a rule label in Module
  --- OUT: Set of rules which are labelled with any Qid in QidList
  eq setRls(M,QL) = setRls(M,setRls(QL,getRls(M),none)) .
  eq setRls(Q QL,rl L => R [label(Q) A]. RS,RS') = setRls(Q QL,RS,rl L => R [label(Q) A]. RS') .
  eq setRls(Q QL,RS,RL RS') = setRls(QL,RS,none) RL RS' [owise] .
  eq setRls(nil,RS,none) = none .

  --- INP: Module/Qid (name of Module)
  --- PRE: Module (and all of its recursive imports) have names that are Qids
  --- OUT: ImportList that contains imports of this module and its dependencies
  eq getImports2(M) = getImports2(getName(M)) .
  eq getImports2(Q) = getImports2(getImports(upModule(Q,false)),nil) .
  eq getImports2(I IL,IL1 I IL2) = getImports2(IL,IL1 I IL2) .
  eq getImports2(nil,IL2) = IL2 .
  eq getImports2(I IL,IL2) = getImports2(upModule(getModQid(I),false),IL,IL2 I) [owise] .
  eq getImports2(M,IL,IL2) = getImports2(getImports(M) IL,IL2)  .

  --- INP: Import
  --- PRE: ModuleExpression in Import is a Qid
  --- OUT: Qid that corresponds to the Module name in this Import
  eq getModQid(protecting Q .) = Q .
  eq getModQid(including Q .) = Q .
  eq getModQid(extending Q .) = Q .
endfm

fmod MODQIDPAIR is
  pr META-LEVEL .
  sort ModQidPair .
  op ((_,_)) : Module Qid -> ModQidPair [ctor] .
endfm

view ModQidPair from TRIV to MODQIDPAIR is sort Elt to ModQidPair . endv

fmod MODQIDLIST is
  pr LIST{ModQidPair} * (sort List{ModQidPair} to ModQidList) .
endfm

view ModQidList from TRIV to MODQIDLIST is sort Elt to ModQidList . endv

fmod SCOPED-REGISTRY is
  pr MAP-EXTRA{Qid,ModQidList} * (sort Map{Qid,ModQidList} to ScopedRegistry) .
  pr UNIT-FM .

  var Task Impl : Qid .
  var ImplMod : Module .
  var ImplList ImplList' : ModQidList .
  var Registry : ScopedRegistry .

  op getbackend : Qid Qid ScopedRegistry -> Module .
  eq getbackend(Task,Impl,(Task |-> (ImplList (ImplMod,Impl) ImplList'),Registry)) = ImplMod .
  eq getbackend(Task,Impl,Registry) = noModule [owise] .
endfm

view Module from TRIV to META-MODULE is sort Elt to Module . endv

fmod MODULE-LIST is
  pr LIST{Module} *
    (op nil : -> List{Module} to nilmod,
     op __ : List{Module} List{Module} -> List{Module} to _;_,
     sort List{Module} to ModuleList,
     sort NeList{Module} to NeModuleList) .
endfm
```

Substitution, Substitution Sets, and Refinements
------------------------------------------------

```maude
fmod SUBSTITUTION-HANDLING is
  protecting META-MODULE .

  var S S' Subst Subst' : Substitution .
  var V V' : Variable .
  var C C' : Constant .
  var Ct : Context .
  var T T' T1 T2 T1' T2' T1'' T2'' : Term .
  var F F' : Qid .
  var TL TL' TL1 TL2 TL1' TL2' : TermList .
  var Att : AttrSet .
  var RLS : RuleSet .
  var Rl : Rule .
  var TP : Type .
  var N : Nat .
  var NeTL : NeTermList .
  var CtL : NeCTermList .

  --- Apply Substitution to Term --------------------------------------------
  op _<<_ : Term Substitution -> Term .
  eq TL << none = TL .
  eq C << Subst = C .
  eq V << ((V <- T) ; Subst) = T .
  eq V << Subst = V [owise] .
  eq F[TL] << Subst = F[TL << Subst] .

  op _<<_ : TermList Substitution -> TermList .
  eq (T, NeTL) << Subst = (T << Subst, NeTL << Subst) .
  eq empty << Subst = empty .

  op _<<_ : Context Substitution -> Context .
  eq Ct << none = Ct .
  eq [] << Subst = [] .
  eq F[CtL,NeTL] << Subst = F[CtL << Subst,NeTL << Subst] .
  eq F[NeTL,CtL] << Subst = F[NeTL << Subst, CtL << Subst] .
  eq F[Ct] << Subst = F[Ct << Subst] .

  op _<<_ : Substitution Substitution -> Substitution .
  eq S << (none).Substitution = S .
  eq (none).Substitution << S = (none).Substitution .
  eq ((V' <- T) ; S') <<  S
   = (V' <- (T << S))
     ;
     (S' << S) .

  --- Combine Substitutions -------------------------------------------------
  op _.._ : Substitution Substitution -> Substitution .
  eq S .. S' = (S << S') ; S' .

  --- Restrict Assignments to Variables in a Term ----------------------
  op _|>_ : Substitution TermList -> Substitution .

  eq Subst |> TL = Subst |>* Vars(TL) .

  op _|>*_ : Substitution TermList -> Substitution .
---   eq noMatch |>* TL = noMatch .
  eq Subst |>* TL = Subst |>** TL [none] .

  op _|>**_[_] : Substitution TermList
                 Substitution -> Substitution .
  eq none |>** TL [Subst']
   = Subst' .
  eq ((V <- V) ; Subst) |>** TL [Subst']
   = Subst |>** TL [Subst'] .
  eq ((V <- T') ; Subst) |>** TL [Subst']
    = Subst |>** TL
      [Subst' ; if any V in TL then (V <- T') else none fi] .

  --- Remove Variables from list ----------------------
  op _intersect_ : TermList TermList -> TermList .
  eq (TL1,T,TL2) intersect (TL1',T,TL2')
   = (T,((TL1,TL2) intersect (TL1',TL2'))) .
  eq TL intersect TL' = empty [owise] .

  op _intersectVar_ : TermList TermList -> TermList .
  eq TL1 intersectVar TL2
   = TL1 intersectVar* Vars(TL2) .

  op _intersectVar*_ : TermList TermList -> TermList .
  eq (T,TL1) intersectVar* TL2
   = (if any Vars(T) in TL2 then T else empty fi,TL1 intersectVar* TL2) .
  eq empty intersectVar* TL2
   = empty .

  --- Remove Variables from list ----------------------
  op _setMinus_ : TermList TermList -> TermList .
  eq (TL1,T,TL2) setMinus (TL1',T,TL2')
   = (TL1,TL2) setMinus (TL1',T,TL2') .
  eq TL setMinus TL' = TL [owise] .

  --- Variables ---
  op Vars : GTermList -> TermList .
  eq Vars((T,TL:GTermList)) = VarsTerm(T),Vars(TL:GTermList) .
  eq Vars((Ct,TL:GTermList)) = VarsTerm(Ct),Vars(TL:GTermList) .
  eq Vars(empty) = empty .

  op VarsTerm : Term -> TermList . ---warning memo
  eq VarsTerm(V) = V .
  eq VarsTerm(F[TL:TermList]) = Vars(TL:TermList) .
  eq VarsTerm(C) = empty .

  op VarsTerm : Context -> TermList . ---warning memo
  eq VarsTerm(F[TL:GTermList]) = Vars(TL:GTermList) .

  --- membership ---
  op _in_ : Term TermList -> Bool .
  eq T in (TL,T,TL') = true .
  eq T in TL = false [owise] .

  --- membership ---
  op any_in_ : TermList TermList -> Bool . --- [memo] .
  eq any empty in TL = false .
  eq any (TL1,T,TL2) in (TL1',T,TL2') = true .
  eq any TL in TL' = false [owise] .

  --- membership ---
  op all_in_ : TermList TermList -> Bool . --- [memo] .
  eq all empty in TL = true .
  eq all (TL1,T,TL2) in (TL1',T,TL2') = all (TL1,TL2) in (TL1',T,TL2') .
  eq all TL in TL' = false [owise] .

  --- Occur check ---
  op allVars_inVars_ : GTermList GTermList -> Bool .
  eq allVars TL:GTermList inVars TL':GTermList
   = all Vars(TL:GTermList) in Vars(TL':GTermList) .

  op anyVars_inVars_ : GTermList GTermList -> Bool .
  eq anyVars TL:GTermList inVars TL':GTermList
   = any Vars(TL:GTermList) in Vars(TL':GTermList) .

  op rangeVars : Substitution -> TermList .
  eq rangeVars(V <- T ; Subst) = (Vars(T),rangeVars(Subst)) .
  eq rangeVars(none) = empty .

  op dom_inVars_ : Substitution TermList -> Bool .
  eq dom Subst inVars TL = dom Subst in Vars(TL) .

  op dom_in_ : Substitution TermList -> Bool .
  eq dom (V <- T ; Subst) in (TL1,V,TL2) = true .
  eq dom Subst in TL = false [owise] .

  op dom_notInVars_ : Substitution TermList -> Bool .
  eq dom Subst notInVars TL = dom Subst notIn Vars(TL) .

  op dom_notIn_ : Substitution TermList -> Bool .
  eq dom none notIn TL = true .
 ceq dom (V <- T ; Subst) notIn TL = true if not (V in TL) .
  eq dom Subst notIn TL = false [owise] .

  op range_inVars_ : Substitution TermList -> Bool .
  eq range Subst inVars TL = range Subst in Vars(TL) .

  op range_in_ : Substitution TermList -> Bool .
  eq range (V <- T ; Subst) in TL
   = any Vars(T) in TL or-else range Subst in TL .
  eq range none in TL
   = false .

  op valid-occur-check? : Substitution -> Bool .
  eq valid-occur-check?(Subst)
   = not (dom Subst inVars (rangeVars(Subst))) .

  op extract-bindings : Substitution -> TermList .
  eq extract-bindings(none) = empty .
  eq extract-bindings(V <- T ; Subst) = (T,extract-bindings(Subst)) .
endfm

fmod SUBSTITUTIONSET is
  protecting SUBSTITUTION-SET .
  protecting SUBSTITUTION-HANDLING .
  protecting TERMSET-FM .

  vars SS SS' : SubstitutionSet .
  vars S S' Subst : Substitution .
  vars T T' : Term .
  vars TL TL' : TermList .
  vars N N' : Nat .
  var V : Variable .

  op _..._ : SubstitutionSet [SubstitutionSet]
          -> SubstitutionSet [strat (1) gather (e E)] .

  eq .SubstitutionSet ... SS':[SubstitutionSet] = .SubstitutionSet .
  eq (S | SS) ... SS':[SubstitutionSet]
   = (S ...' SS':[SubstitutionSet])
     |
     (SS ... SS':[SubstitutionSet]) .

  op _...'_ : Substitution SubstitutionSet -> SubstitutionSet .

  eq S ...' .SubstitutionSet
   = .SubstitutionSet .

  eq S ...' (S' | SS')
   = (S .. S')
     |
     (S ...' SS') .
endfm

fmod SUBSTITUTION-REFINEMENT is
  pr QIDSET-REFINEMENT    .
  pr GTERMLIST-REFINEMENT .
  sort VarAssignment ConstAssignment GroundAssignment .
  sort EmptySubstitution VarSubstitution ConstSubstitution GroundSubstitution .
  subsort ConstAssignment < GroundAssignment .
  subsort VarAssignment   ConstAssignment   GroundAssignment   < Assignment .
  subsort EmptySubstitution < VarSubstitution ConstSubstitution GroundSubstitution < Substitution .
  subsort ConstSubstitution < GroundSubstitution .
  subsort VarAssignment     < VarSubstitution .
  subsort ConstAssignment   < ConstSubstitution .
  subsort GroundAssignment  < GroundSubstitution .
  op _<-_ : Variable Variable                     -> VarAssignment       [ctor ditto] .
  op _<-_ : Variable Constant                     -> ConstAssignment     [ctor ditto] .
  op _<-_ : Variable GroundTerm                   -> GroundAssignment    [ctor ditto] .
  op none :                                       -> EmptySubstitution   [ctor ditto] .
  op _;_  : EmptySubstitution  EmptySubstitution  -> EmptySubstitution   [ctor ditto] .
  op _;_  : VarSubstitution    VarSubstitution    -> VarSubstitution     [ctor ditto] .
  op _;_  : ConstSubstitution  ConstSubstitution  -> ConstSubstitution   [ctor ditto] .
  op _;_  : GroundSubstitution GroundSubstitution -> GroundSubstitution  [ctor ditto] .
  op errsub : QidList -> [Substitution] .

  op errsubMsg : [Substitution] -> QidList .
  eq errsubMsg(errsub(QL:QidList)) = QL:QidList .
  eq errsubMsg(S:[Substitution]) = nil .
endfm

fmod SUBSTITUTIONPAIR is
  pr SUBSTITUTION-REFINEMENT .
  sort SubstitutionPair .
  op ((_,_)) : Substitution Substitution -> SubstitutionPair .
  ops p1 p2 : SubstitutionPair -> Substitution .
  var S1 S2 : Substitution .
  eq p1((S1,S2)) = S1 .
  eq p2((S1,S2)) = S2 .
endfm

fmod SUBSTITUTIONPAIRSET is
  pr SUBSTITUTIONPAIR .
  sort SubstitutionPairSet NeSubstitutionPairSet .
  subsort SubstitutionPair < NeSubstitutionPairSet < SubstitutionPairSet .
  op empty : -> SubstitutionPairSet [ctor] .
  op _|_ : SubstitutionPairSet SubstitutionPairSet -> SubstitutionPairSet
    [ctor assoc comm id: empty format (d n d d)] .
  op _|_ : NeSubstitutionPairSet SubstitutionPairSet -> NeSubstitutionPairSet
    [ctor ditto] .
  eq X:SubstitutionPair | X:SubstitutionPair = X:SubstitutionPair .
endfm

fmod SUBSTITUTION-AUX is
  pr SUBSTITUTION-REFINEMENT .
  pr TERM-EXTRA              . --- defines vars()
  op idsub       : VariableSet               -> VarSubstitution .
  op domain      : Substitution              -> VariableSet .
  op range       : Substitution              -> VariableSet .
  op filterNotIn : Substitution VariableSet  -> Substitution .
  op bound       : Variable Substitution     -> Bool .
  op remove      : Substitution Substitution -> Substitution .

  var V : Variable . var T : Term . var S S' : Substitution . var VS : VariableSet . var A : Assignment .

  --- INP: QidSet
  --- PRE: None
  --- OUT: Identity substitution over VariableSet
  eq idsub(V ; VS) = V <- V ; idsub(VS) .
  eq idsub(none)   = none .

  --- INP: Substitution
  --- PRE: None
  --- OUT: VariableSet composed of variables mapped to other values
  eq domain(V <- T ; S) = V ; domain(S) .
  eq domain(none)       = none .

  --- INP: Substitution
  --- PRE: None
  --- OUT: VariableSet containing all variables in codomain of substitution
  eq range(V <- T ; S) = vars(T) ; range(S) .
  eq range(none)       = none .

  --- INP: Subsitution VS:QidSet
  --- PRE: None
  --- OUT: Substitution where each binding V <- T is
  ---      filtered out if V occurs in VS
  eq filterNotIn(V <- T ; S,VS) = if V in VS then none else V <- T fi ; filterNotIn(S,VS) .
  eq filterNotIn(none,VS)       = none .

  --- INP: Variable Substitution
  --- PRE: None
  --- OUT: True iff variable is bound by substitution
  eq bound(V,V <- T ; S) = true          .
  eq bound(V,S)          = false [owise] .

  --- INP: Substitution1 Substitution2
  --- PRE: None
  --- OUT: a new Substitution identical to Substiution1 but where any assignment in Substitution2 has been removed
  eq remove(S ; A,A ; S') = remove(S,A ; S') .
  eq remove(S,S')         = S [owise] .
endfm

fmod SUBSTITUTIONSET-AUX is
  pr SUBSTITUTION-AUX .
  pr SUBSTITUTIONSET .
  pr TERMSET-FM .

  var S S' S2 : Substitution . var SS : SubstitutionSet .
  var T : Term . var V : Variable . var QS : VariableSet .
  var SSK : [SubstitutionSet] .

  --- OUT: QidSet composed of domain() of each substitution
  op domain : SubstitutionSet -> VariableSet .
  eq domain(S | S' | SS) = domain(S) ; domain(S' | SS) .
  eq domain(.SubstitutionSet) = none .

  --- OUT: SubstitutionSet where each substitution binding V <- T is
  ---      filtered out if V occurs in QS
  op filterNotIn : SubstitutionSet VariableSet -> SubstitutionSet .
  eq filterNotIn(S | S' | SS,QS) = filterNotIn(S,QS) | filterNotIn(S' | SS,QS) .
  eq filterNotIn(.SubstitutionSet,QS) = .SubstitutionSet .

  --- OUT: A TermSet of instances of Term by application with each Substitution in SubstitutionSet
  op _<<_ : Term SubstitutionSet -> TermSet .
  eq T << (S | S' | SS) = (T << S) | (T << (S' | SS)) .
  eq T << .SubstitutionSet = .TermSet .


  --- OUT: A SubstitutionSet identical to input but where each assignment in Substitution is removed
  op remove : SubstitutionSet Substitution -> SubstitutionSet .
  eq remove(S | S' | SS,S2) = remove(S,S2) | remove(S | S' | SS,S2) .
  eq remove(.SubstitutionSet,S2) = .SubstitutionSet .

  --- OUT: A SubstitutionSet of Assignments built by blowing this Substitution apart
  op explode : Substitution -> SubstitutionSet .
  eq explode(V <- T ; S) = (V <- T) | explode(S) .
  eq explode(none) = .SubstitutionSet .

  --- OUT: Get first error from SubstitutionSet
  eq errsubMsg(errsub(QL:QidList) | SSK) = QL:QidList .
endfm
```

operations like metaUnify, metaDisjointUnify, etc... generate fresh variable
names for all terms invovled---however, sometimes, it is desirable to preserve
the variable names originally chosen by the user---this module implemnts a
function which will "reuse" user-provided variables as often as possible

```maude
fmod RECLAIM-VARS is
  pr SUBSTITUTION-REFINEMENT .
  pr SUBSTITUTIONSET .
  pr SUBSTITUTION-AUX .
  op reclaim  : VariableSet SubstitutionSet           -> SubstitutionSet .
  op reclaim' : VariableSet Substitution              -> [Substitution] .
  op reclaim' : VariableSet Substitution Substitution -> Substitution .
  op filter   : SubstitutionSet                       -> SubstitutionSet .

  var V : Variable . var T : Term . var S S' : Substitution . var SS : SubstitutionSet . var VS : VariableSet .

  --- INP: VariableSet SubstitutionSet:SS
  --- PRE: domain(SS) intersected with range(SS) is empty
  --- OUT: A new SubstitutionSet where each substitution is identical
  ---      to the previous one except that alpha renamings V <- V'
  ---      are thrown away and replaced by identity mapping V <- V.
 ceq reclaim'(VS,S)             = reclaim'(VS,S,none) if intersection(domain(S),range(S)) == none .
  eq reclaim'(VS,V <- T ; S,S') = if T :: Variable and-then V in VS and-then not T in VS
                                   then reclaim'(VS,S << (T <- V),(S' << (T <- V)) ; V <- V) else reclaim'(VS,S,S' ; V <- T) fi .
  eq reclaim'(VS,none,S')       = S' .

  eq reclaim(VS,S | SS) = reclaim'(VS,S) | reclaim(VS,SS) .
  eq reclaim(VS,.SubstitutionSet) = .SubstitutionSet .

  --- INP: SubsitutionSet
  --- PRE: None
  --- OUT: An identical SubstitutionSet where each identity
  ---      mapping V <- V is removed.
  eq filter((S ; V <- V) | SS) = filter(S | SS) .
  eq filter(SS) = SS [owise] .
endfm

view Substitution from TRIV to META-LEVEL is sort Elt to Substitution . endv

--- this module represents a lazily generated stream of substitutions
--- useful for representing the solutions to a unification problem, etc...
fmod SUBSTITUTION-STREAM is
  pr STREAM{Substitution} .
  pr SUBSTITUTIONSET .
  op set : Stream{Substitution} -> SubstitutionSet .
  var X : Substitution . var S : [Stream{Substitution}] .
  eq set(X & S) = X | set(S) .
  eq set(end) = .SubstitutionSet .
endfm
```

Term/Substitution Pairs
-----------------------

```maude
fmod TERMSUBSTPAIR is
  pr META-TERM .
  sort TermSubstPair .
  op ((_,_)) : Term Substitution -> TermSubstPair [ctor] .
endfm

fmod TERMSUBSTSETPAIR is
  pr TERMSUBSTPAIR .
  pr SUBSTITUTIONSET .
  sort TermSubstSetPair .
  subsort TermSubstPair < TermSubstSetPair .
  op ((_,_)) : Term SubstitutionSet -> TermSubstSetPair .
endfm

fmod TERMSUBSTPAIRSET is
  pr TERMSUBSTPAIR .
  pr TERMSET-FM .
  pr SUBSTITUTIONSET .
  sort TermSubstPairSet .
  subsort TermSubstPair < TermSubstPairSet .
  op _|_      : TermSubstPairSet TermSubstPairSet -> TermSubstPairSet [ctor assoc comm id: emptyTPS] .
  op emptyTPS :                                   -> TermSubstPairSet [ctor] .
  op _<<_     : TermSubstPairSet SubstitutionSet  -> TermSubstPairSet .
  op tsp-term : TermSubstPairSet                  -> TermSet .
  op tsp-sub  : TermSubstPairSet                  -> SubstitutionSet .

  var TPS : TermSubstPairSet . var TP TP' : TermSubstPair . var T : Term . var S S' : Substitution . var SS : SubstitutionSet .

  eq tsp-term(TPS | (T,S)) = T | tsp-term(TPS) .
  eq tsp-term(emptyTPS)    = .TermSet .
  eq tsp-sub(TPS | (T,S))  = S | tsp-sub(TPS) .
  eq tsp-sub(emptyTPS)     = .SubstitutionSet .

  eq (TP | TP' | TPS) << SS               = (TP << SS) | ((TP' | TPS) << SS) .
  eq  emptyTPS        << SS               = emptyTPS .
  eq  TP              << (S | S' | SS)    = (TP << S) | (TP << (S' | SS)) .
  eq  TP              << .SubstitutionSet = emptyTPS .
  eq (T,S)            << S'               = (T << S',S << S') .
endfm
```

Variants
--------

```maude
fmod VARIANT is
  pr SUBSTITUTIONSET .
  pr META-LEVEL .

  var M : Module .
  vars T T' TS TS' CtTS CtTS' Lhs Rhs : Term .
  vars N N' NextVar NextVar' NextVar'' : Nat .
  var B : Bound .
  var TL TL' : TermList .
  var NeTL : NeTermList .
  var EqS : EquationSet .
  var AtS : AttrSet .
  var Q : Qid .
  vars S S' : Substitution .
  var V : Variable .
  vars TP TP' : Type .
  var C : Constant .
  vars F F' : Qid .


  sort VariantTripleSet .
  subsort Variant < VariantTripleSet .

  op empty : -> VariantTripleSet [ctor] .
  op _|_ : VariantTripleSet VariantTripleSet -> VariantTripleSet
    [ctor assoc comm id: empty prec 65 format (d d n d)] .
  eq X:Variant | X:Variant = X:Variant .

  op getTerms : VariantTripleSet -> TermSet .
  eq getTerms({T:Term,S:Substitution,NextVar:Nat,P:Parent,B:Bool}
              | R:VariantTripleSet)
   = T:Term | getTerms(R:VariantTripleSet) .
  eq getTerms((empty).VariantTripleSet)
   = .TermSet .

  op getSubstitutions : VariantTripleSet -> SubstitutionSet .
  eq getSubstitutions({T:Term,S:Substitution,NextVar:Nat,P:Parent,B:Bool}
              | R:VariantTripleSet) = S:Substitution | getSubstitutions(R:VariantTripleSet) .
  eq getSubstitutions((empty).VariantTripleSet)
   = .SubstitutionSet .
endfm
```

Type Operations
---------------

```maude
fmod TYPE-EXTRA is
  pr META-LEVEL .

  --- BASIC TYPE OPERATIONS
  op direct-subsorts   : Module Type -> SortSet .
  op direct-subsorts   : SubsortDeclSet Type -> SortSet .

  --- BASIC TYPE SET OPERATIONS
  op getMaximalSorts   : Module -> SortSet .
  op getMaximalSorts   : Module KindSet -> SortSet .

  --- BASIC TYPE TUPLE OPERATIONS
  op $typeLeq          : Module TypeList TypeList -> Bool .
  op $typeLeqS         : Module TypeList TypeListSet -> Bool .
  op typeLeqS          : Module TypeListSet TypeListSet -> Bool .
  op $typeRel          : Module TypeList TypeList -> Bool .
  op $typeRelS         : Module TypeList TypeListSet -> Bool .
  op typeRelS          : Module TypeListSet TypeListSet -> Bool .
  op greaterSorts      : Module Sort -> SortSet .
  op $greaterSorts     : Module Sort SortSet -> SortSet .

  --- BASIC TYPE TUPLE SET OPERATIONS
  op _in_              : TypeList TypeListSet -> Bool .
  op _-_               : TypeListSet TypeListSet -> TypeListSet .
  op $tydiff           : TypeListSet TypeListSet TypeListSet -> TypeListSet .
  op intersect         : TypeListSet TypeListSet -> TypeListSet .

  --- TYPE TUPLE AUXILLIARY OPERATIONS
  op merge             : TypeListSet TypeListSet -> TypeListSet .
  op $merge1           : TypeListSet TypeListSet TypeListSet -> TypeListSet .
  op $merge2           : TypeList TypeListSet TypeListSet -> TypeListSet .

  --- CONVERSIONS
  op typeListToSet     : TypeList -> TypeSet .

  --- DECL TYPE OPERATIONS
  op possibleTypings   : Module Qid -> TypeSet .

  var SDS : SubsortDeclSet .
  var TYLS TYLS' TYLS'' : TypeListSet .
  var TYS : TypeSet .
  var TY TY' : Type .
  var S S' : Sort .
  var K K' : Kind .
  var KS : KindSet .
  var TYL TYL' TYL'' : TypeList .
  var SS SS' : SortSet .
  var M : Module .
  var Q Q' : Qid .
  var ODS : OpDeclSet .
  var AS : AttrSet .
  var T : Term .
  var TL : TermList .

  --- INP: Module Type
  --- PRE: Type is well-defined in Module
  --- OUT: Set of direct subsorts of Type
  eq direct-subsorts(M,S) = direct-subsorts(getSubsorts(M),S) .
  eq direct-subsorts(M,K) = maximalSorts(M,K) .
  eq direct-subsorts(SDS subsort S' < S .,S) = S' ; direct-subsorts(SDS,S) .
  eq direct-subsorts(SDS,TY) = none [owise] .

  --- INP: Module [KindSet]
  --- PRE: Kinds are well-defined in Module
  --- OUT: Set of maximal sorts of each kind
  eq getMaximalSorts(M)        = getMaximalSorts(M,getKinds(M)) .
  eq getMaximalSorts(M,K ; KS) = maximalSorts(M,K) ; getMaximalSorts(M,KS) .
  eq getMaximalSorts(M,none)   = none .

  --- INP: Module TypeListSet1 TypeListSet2
  --- PRE: Types in TypeLists are defined in Module
  --- OUT: true if every type in TypeListSet1 is a subtype of TypeListSet2
  eq $typeLeq(M,K TYL,S TYL')  = false .
  eq $typeLeq(M,S TYL,K TYL')  = sortLeq(M,S,K)  and-then $typeLeq(M,TYL,TYL') .
  eq $typeLeq(M,K TYL,K' TYL') = sortLeq(M,K,K') and-then $typeLeq(M,TYL,TYL') .
  eq $typeLeq(M,S TYL,S' TYL') = sortLeq(M,S,S') and-then $typeLeq(M,TYL,TYL') .
  eq $typeLeq(M,nil,nil)       = true .
  eq $typeLeq(M,TYL,TYL')      = false [owise] .
  eq $typeLeqS(M,TYL,TYL' ; TYLS) =
    $typeLeq(M,TYL,TYL') and-then $typeLeqS(M,TYL,TYLS) .
  eq $typeLeqS(M,TYL,none) = true .
  eq typeLeqS(M,TYL ; TYLS,TYLS') =
    $typeLeqS(M,TYL,TYLS') and-then typeLeqS(M,TYLS,TYLS') .
  eq typeLeqS(M,none,TYLS') = true .

  --- INP:  Module TypeListSet1 TypeListSet2
  --- PRE:  Types in TypeLists are defined in Module
  --- OUT:  true if corresponding types in each typelist have the same kind
  --- NOTE: the mnemonic typeRel stands for type related
  eq $typeRel(M,TY TYL,TY' TYL') = sameKind(M,TY,TY') and $typeRel(M,TYL,TYL') .
  eq $typeRel(M,nil,nil)         = true .
  eq $typeRel(M,TYL,TYL')        = false [owise] .
  eq $typeRelS(M,TYL,TYL' ; TYLS) =
    $typeRel(M,TYL,TYL') and-then $typeRelS(M,TYL,TYLS) .
  eq $typeRelS(M,TYL,none) = true .
  eq typeRelS(M,TYL ; TYLS,TYLS') =
    $typeRelS(M,TYL,TYLS') and-then typeRelS(M,TYLS,TYLS') .
  eq typeRelS(M,none,TYLS') = true .

  --- INP: Module Sort
  --- PRE: Type is defined in Module
  --- OUT: SortSet of all types greater than Sort
  eq greaterSorts(M,S) = $greaterSorts(M,S,lesserSorts(M,getKind(M,S))) .
  eq $greaterSorts(M,S,S' ; SS) =
    if sortLeq(M,S,S') then
      S' ; $greaterSorts(M,S,SS)
    else
      $greaterSorts(M,S,SS)
    fi .
  eq $greaterSorts(M,S,none) = none .

  --- INP: TypeList TypeListSet
  --- PRE: None
  --- OUT: true iff TypeList is in TypeListSet
  eq TYL in (TYL ; TYLS) = true .
  eq TYL in TYLS = false [owise] .

  --- INP: TypeListSet1 TypeListSet2
  --- PRE: None
  --- OUT: TypeListSet containing all TypeLists in TypeListSet1 and not in
  ---      TypeListSet2
  eq TYLS - TYLS' = $tydiff(TYLS,TYLS',none) .
  eq $tydiff(TYL ; TYLS,TYLS',TYLS'') =
    if TYL in TYLS' then
      $tydiff(TYLS,TYLS',TYLS'')
    else
      $tydiff(TYLS,TYLS',TYL ; TYLS'')
    fi .
  eq $tydiff(none,TYLS',TYLS'') = TYLS'' .

  --- INP: TypeListSet1 TypeListSet2
  --- PRE: None
  --- OUT: TypeListSet containing all items in both sets
  eq intersect(TYL ; TYLS,TYLS') =
    if TYL in TYLS' then
      TYL ; intersect(TYLS,TYLS')
    else
      intersect(TYLS,TYLS')
    fi .
  eq intersect(none,TYLS') = none .

  --- INP: TypeListSet TypeListSet
  --- PRE: None
  --- OUT: A TypeListSet formed by merging each pair of TypeLists
  ---      in the two TypeListSets
  eq merge(TYLS,TYLS') = $merge1(TYLS,TYLS',none) .
  eq $merge1(TYL ; TYLS, TYLS', TYLS'') =
    $merge1(TYLS, TYLS', $merge2(TYL,TYLS',none) ; TYLS'') .
  eq $merge1(none,TYLS',TYLS'') = TYLS'' .
  eq $merge2(TYL, TYL' ; TYLS, TYLS') =
    $merge2(TYL, TYLS, TYL TYL' ; TYLS') .
  eq $merge2(TYL, none, TYLS') = TYLS' .

  --- INP: TypeList
  --- PRE: None
  --- OUT: Turns a TypeList into the corresponding TypeSet
  eq typeListToSet(TY TYL) = TY ; typeListToSet(TYL) .
  eq typeListToSet(nil)    = none .

  --- INP: Module Qid
  --- PRE: None
  --- OUT: Possible result types of operators with name Qid
  op possibleTypings : OpDeclSet Qid -> TypeSet .
  eq possibleTypings(M,Q) = possibleTypings(getOps(M),Q) .
  eq possibleTypings(op Q' : TYL -> TY [AS]. ODS,Q) =
    if Q == Q' then
      TY ; possibleTypings(ODS,Q)
    else
      possibleTypings(ODS,Q)
    fi .
  eq possibleTypings(none,Q) = none .

  --- INP: Module TypeList
  --- PRE: TypeList defined in Module
  --- OUT: TypeListSet of all TypeLists less than the current one
  op typesBelow        : Module TypeList -> TypeListSet .
  op $typesBelow       : Module TypeList TypeListSet -> TypeListSet .
  eq typesBelow(M,TYL) = $typesBelow(M,TYL,nil) .
  eq $typesBelow(M,TY TYL,TYLS) = $typesBelow(M,TYL,merge(TYLS,lesserSorts(M,TY) ; TY)) .
  eq $typesBelow(M,nil,TYLS) = TYLS .

  --- INP: Module TypeList
  --- PRE: TypeList well-defined in Module
  --- OUT: KindList corresponding to this TypeList
  op toKind : Module TypeList -> TypeList .
  eq toKind(M,TY TYL) = completeName(M,getKind(M,TY)) toKind(M,TYL) .
  eq toKind(M,nil) = nil .

  --- PRE: Term well-defined in Module
  --- OUT: A TypeList with n elements corresponding to n terms' types in termlist
  op termListTypes : Module TermList -> TypeList .
  eq termListTypes(M,(T, TL))  = leastSort(M,T) termListTypes(M,TL) .
  eq termListTypes(M,empty) = nil .

  op maxSortAbove : Module Sort ~> Sort .
  op maxSortAbove : Module Sort SortSet ~> Sort .
  eq maxSortAbove(M,S) = maxSortAbove(M,S,maximalSorts(M,getKind(M,S))) .
  eq maxSortAbove(M,S,S' ; SS) =
    if sortLeq(M,S,S') then
      if maxSortAbove(M,S,SS) :: Sort then
        maxSortAbove(M,S,none) else
        S'
      fi else
      maxSortAbove(M,S,SS)
    fi .
endfm
```

Operator/Membership/Rule Declarations
-------------------------------------

```maude
fmod ATTR-EXTRA is
  pr META-MODULE .

  ops special-attr id-attr left-id-attr right-id-attr
      strat-attr frozen-attr poly-attr prec-attr gather-attr
      format-attr print-attr label-attr metadata-attr : -> Attr .
  op _%_ : AttrSet AttrSet -> AttrSet .
  op _-_ : AttrSet AttrSet -> AttrSet .
  op in  : AttrSet AttrSet -> Bool .

  var A A' : Attr .
  var Q Q' : Qid .
  var QL QL' : QidList .
  var TYL : TypeList .
  var TY : Type .
  var AS AS' : AttrSet .
  var S S' : String .
  var N N' : Nat .
  var NL NL' : NatList .
  var HL HL' : NeHookList .
  var T T' : Term .

  --- INP: None
  --- PRE: None
  --- OUT: Attrs for use in _%_
  eq special-attr  = special(term-hook('T,'T.S)) .
  eq id-attr       = id('T.S) .
  eq left-id-attr  = left-id('T.S) .
  eq right-id-attr = right-id('T.S) .
  eq strat-attr    = strat(0) .
  eq frozen-attr   = frozen(0) .
  eq poly-attr     = poly(0) .
  eq prec-attr     = prec(0) .
  eq gather-attr   = gather(nil) .
  eq format-attr   = format(nil) .
  eq print-attr    = print(nil) .
  eq label-attr    = label('Q) .
  eq metadata-attr = metadata("") .

  --- INP: AttrSet1 AttrSet2
  --- PRE: None
  --- OUT: All Attrs in AttrSet1 that are not in AttrSet2;
  ---      here we ignore subterms; we only match top operator
  eq (special(HL) AS) % (special(HL') AS') = AS % AS' .
  eq (id(T)       AS) % (id(T')       AS') = AS % AS' .
  eq (left-id(T)  AS) % (left-id(T)   AS') = AS % AS' .
  eq (right-id(T) AS) % (right-id(T)  AS') = AS % AS' .
  eq (strat(NL)   AS) % (strat(NL')   AS') = AS % AS' .
  eq (frozen(NL)  AS) % (frozen(NL')  AS') = AS % AS' .
  eq (poly(NL)    AS) % (poly(NL')    AS') = AS % AS' .
  eq (prec(N)     AS) % (prec(N')     AS') = AS % AS' .
  eq (gather(QL)  AS) % (gather(QL')  AS') = AS % AS' .
  eq (format(QL)  AS) % (format(QL')  AS') = AS % AS' .
  eq (print(QL)   AS) % (print(QL')   AS') = AS % AS' .
  eq (label(Q)    AS) % (label(Q')    AS') = AS % AS' .
  eq (metadata(S) AS) % (metadata(S') AS') = AS % AS' .
  eq              AS  %               AS'  = AS - AS' [owise] .

  --- INP: AttrSet1 AttrSet2
  --- PRE: None
  --- OUT: All Attrs in AttrSet1 that are not in AttrSet2
  eq (A AS) - (A AS') = AS - AS' .
  eq    AS  -    AS'  = AS [owise] .


  --- INP: AttrSet1 AttrSet2
  --- PRE: None
  --- OUT: true iff all kinds of Attrs in AttrSet1 are in AttrSet2
  eq in(AS,AS') = (AS % AS') == none .
endfm

fmod OPDECL-EXTRA is
  pr META-LEVEL .
  pr ATTR-EXTRA .
  op qid        : OpDecl -> Qid .
  op argTypes   : OpDecl -> TypeList .
  op resultType : OpDecl -> Type .
  op attrSet    : OpDecl -> AttrSet .
  op metadata   : OpDecl ~> String .
  var Q : Qid    . var T  : Type    . var TL : TypeList .
  var S : String . var AS : AttrSet .
  eq qid(op Q : TL -> T [AS].)        = Q  .
  eq argTypes(op Q : TL -> T [AS].)   = TL .
  eq resultType(op Q : TL -> T [AS].) = T  .
  eq attrSet(op Q : TL -> T [AS].)    = AS .
  eq metadata(op Q : TL -> T [metadata(S) AS].) = S .
endfm

fmod OPDECLSET-EXTRA is
  pr OPDECL-EXTRA .
  pr META-MODULE .

  op _inODS_     : OpDecl OpDeclSet -> Bool .
  op _-_         : OpDeclSet OpDeclSet -> OpDeclSet .
  op subset?     : OpDeclSet OpDeclSet -> Bool .
  op $opdiff     : OpDeclSet OpDeclSet OpDeclSet -> OpDeclSet .
  op argTypeSet  : OpDeclSet -> TypeListSet .
  op $argTypeSet : OpDeclSet TypeListSet -> TypeListSet .
  op resTypeSet  : OpDeclSet -> TypeSet .
  op $resTypeSet : OpDeclSet TypeSet -> TypeListSet .
  op constants   : OpDeclSet -> OpDeclSet .
  op $constants  : OpDeclSet OpDeclSet -> OpDeclSet .
  op ctors       : OpDeclSet -> OpDeclSet .
  op stripAttrs  : OpDeclSet -> OpDeclSet .
  ---
  op qid         : OpDeclSet -> QidSet .
  op argTypes    : OpDeclSet -> TypeListSet .
  op resultType  : OpDeclSet -> TypeSet .
  --- op attrSet : OpDeclSet -> AttrSetSet . --- right now no attrsetset sort to use...

  var M : Module .
  var TS : TypeSet .
  var TLS : TypeListSet .
  var OS OS' OS'' : OpDeclSet .
  var OD OD' : OpDecl .
  var Q : Qid .
  var AS : AttrSet .
  var TYL : TypeList .
  var TY : Type .
  var QS : QidSet .

  --- INP: OpDecl OpDeclSet
  --- PRE: None
  --- OUT: true iff OpDecl is in OpDeclSet
  eq OD inODS OD OS = true .
  eq OD inODS OS = false [owise] .

  --- INP: OpDeclSet1 OpDeclSet2
  --- PRE: None
  --- OUT: Removes all OpDecls in set 2
  ---      from set 1
  eq OS - OS' = $opdiff(OS,OS',none) .
  eq $opdiff(OD OS,OS',OS'') =
    $opdiff(OS,OS',if OD inODS OS' then none else OD fi OS'') .
  eq $opdiff(none,OS',OS'') = OS'' .

  --- INP: OpDeclSet1 OpDeclSet2
  --- PRE: None
  --- OUT: true iff OpDeclSet1 is a subset of OpDeclSet2
  eq subset?(OD OS,OS') = OD inODS OS' and-then subset?(OS,OS') .
  eq subset?(none,OS') = true .

  --- INP: OpDeclSet
  --- PRE: None
  --- OUT: A set of TypeLists represents the arguments from each OpDecl
  eq argTypeSet(OS) = $argTypeSet(OS,none) .
  eq $argTypeSet(OD OS,TLS) = $argTypeSet(OS,argTypes(OD) ; TLS) .
  eq $argTypeSet(none,TLS) = TLS .

  --- INP: OpDeclSet
  --- PRE: None
  --- OUT: A TypeSet representing the results from each OpDecl
  eq resTypeSet(OS) = $resTypeSet(OS,none) .
  eq $resTypeSet(OD OS,TS) = $resTypeSet(OS,resultType(OD) ; TS) .
  eq $resTypeSet(none,TS) = TS .

  --- INP: OpDeclSet
  --- PRE: None
  --- OUT: Extracts all constants
  eq constants(OS) = $constants(OS,none) .
  eq $constants(OD OS,OS') =
    $constants(OS,if argTypes(OD) == nil then OD else none fi OS') .
  eq $constants(none,OS') = OS' .

  --- INP: OpDeclSet
  --- PRE: None
  --- OUT: Extracts all ctors
  op $ctors      : OpDeclSet OpDeclSet -> OpDeclSet .
  eq ctors(OS) = $ctors(OS,none) .
  eq $ctors(op Q : TYL -> TY [ctor AS]. OS,OS') =
     $ctors(OS,op Q : TYL -> TY [ctor AS]. OS') .
  eq $ctors(OS,OS') = OS' [owise] .

  --- INP: OpDeclSet
  --- PRE: None
  --- OUT: Replaces each op's AttrSet with none
  eq stripAttrs(op Q : TYL -> TY [AS]. OS) =
   op Q : TYL -> TY [none]. stripAttrs(OS) .
  eq stripAttrs(none) = none .

  --- INP: OpDeclSet
  --- PRE: None
  --- OUT: Qid/TypeList/TypeSets (Set functor liftings of OpDecl type)
  eq qid(OD OD' OS)        = qid(OD) ; qid(OD' OS) .
  eq qid(none)              = none .
  eq argTypes(OD OD' OS)   = argTypes(OD) ; argTypes(OD' OS) .
  eq argTypes(none)         = none .
  eq resultType(OD OD' OS) = resultType(OD) ; resultType(OD' OS) .
  eq resultType(none)       = none .

  --- INP: OpDeclSet OpDeclSet
  --- PRE: None
  --- OUT: true iff OpDeclSets intersect?
  op intersect? : OpDeclSet OpDeclSet -> Bool .
  op $intersect? : OpDecl OpDeclSet -> Bool .
  eq intersect?(OD OS,OS') = $intersect?(OD,OS') or-else intersect?(OS,OS') .
  eq intersect?(none,OS') = false .
  eq $intersect?(OD OS,OD OS') = true .
  eq $intersect?(OS,OS') = false [owise] .

  --- INP: OpDeclSet
  --- PRE: None
  --- OUT: True iff there is an assoc op that isn't comm
  op assocNotComm? : OpDeclSet -> Bool .
  op noComm? : AttrSet -> Bool .
  ceq assocNotComm?(op Q : TYL -> TY [assoc AS]. OS) = true if noComm?(AS) .
  eq assocNotComm?(OS) = false [owise] .
  eq noComm?(comm AS) = false .
  eq noComm?(AS) = true [owise] .
endfm

fmod OPDECL-TYPING is
  pr META-LEVEL .
  pr TYPE-EXTRA .
  pr OPDECLSET-EXTRA .

  op opLeq : Module OpDecl OpDecl -> Bool .
  op getMaximal : Module OpDeclSet -> OpDeclSet .
  op $getMaximal : Module OpDeclSet OpDeclSet OpDeclSet -> OpDeclSet .
  op $getMaximal1 : Module OpDecl OpDeclSet -> Bool .
  op opsBelow : Module OpDecl -> OpDeclSet .
  op opsBelow : Module OpDecl OpDeclSet -> OpDeclSet .
  op $opsBelow : Module OpDecl OpDeclSet OpDeclSet -> OpDeclSet .
  op relatedOps : Module Qid TypeList OpDeclSet -> OpDeclSet .
  op relatedOps : Module OpDecl OpDeclSet -> OpDeclSet .

  var M : Module .
  var OD OD' : OpDecl .
  var OS OS' OS'' : OpDeclSet .
  var TYL TYL' : TypeList .
  var TY TY' : Type .
  var AS AS' : AttrSet .
  var NTL : NeTermList .
  var C : Constant .
  var Q : Qid .

  --- INP: Module OpDecl1 OpDecl2
  --- PRE: OpDecls are well-defined with respect to Module
  --- OUT: true iff names agree and argTypes of OpDecl1 are less than that
  ---      of OpDecl2
  eq opLeq(M,OD,OD') =
    qid(OD) == qid(OD') and-then typeLeqS(M,argTypes(OD),argTypes(OD')) .

  --- INP: Module OpDeclSet
  --- PRE: OpDeclSet is well-defined with respect to Module
  --- OUT: The maximal OpDecls in the OpDeclSet
  eq getMaximal(M,OS) = $getMaximal(M,OS,none,none) .
  eq $getMaximal(M,OD OS,OS',OS'') =
    if $getMaximal1(M,OD,OS OS') then
      $getMaximal(M,OS,OD OS',OD OS'')
    else
      $getMaximal(M,OS,OS',OS'')
    fi .
  eq $getMaximal(M,none,OS',OS'') = OS'' .
  eq $getMaximal1(M,OD,OD' OS) =
    not opLeq(M,OD,OD') and-then $getMaximal1(M,OD,OS) .
  eq $getMaximal1(M,OD,none) = true .

  --- INP: Module OpDecl [OpDeclSet]
  --- PRE: OpDecls are well-defined with respect to Module
  --- OUT: All OpDecls in OpDeclSet that are less than OpDecl
  eq opsBelow(M,OD) = opsBelow(M,OD,getOps(M)) .
  eq opsBelow(M,OD,OS) = $opsBelow(M,OD,OS,none) .
  eq $opsBelow(M,OD,OD' OS',OS) =
    if opLeq(M,OD',OD) and OD' =/= OD then
      $opsBelow(M,OD,OS',OD' OS)
    else
      $opsBelow(M,OD,OS',OS)
    fi .
  eq $opsBelow(M,OD,none,OS) = OS .

  --- INP: Module OpDecl/(Qid TypeList) OpDeclSet
  --- PRE: OpDecls are well-defined with respect to Module
  --- OUT: Set of OpDecls in OpDeclSet that are related to OpDecl; that is
  ---      their arguments are in the same kind
  eq relatedOps(M,Q,TYL,OS) =
    relatedOps(M,op Q : TYL -> 'Sort [none].,OS) .
  eq relatedOps(M,OD,OD' OS) =
    if qid(OD) == qid(OD') and typeRelS(M,argTypes(OD),argTypes(OD')) then
      OD' relatedOps(M,OD,OS)
    else
      relatedOps(M,OD,OS)
    fi .
  eq relatedOps(M,OD,none) = none .

  --- INP: Module TypeList OpDeclSet
  --- PRE: OpDecls are well-defined with respect to Module
  --- OUT: All OpDecls in OpDeclSet that are less than OpDecl
  op opsAbove : Module TypeList -> OpDeclSet .
  op opsAbove : Module TypeList OpDeclSet -> OpDeclSet .
  op $opsAbove : Module TypeList OpDeclSet OpDeclSet -> OpDeclSet .
  eq opsAbove(M,TYL) = opsAbove(M,TYL,getOps(M)) .
  eq opsAbove(M,TYL,OS) = $opsAbove(M,TYL,OS,none) .
  eq $opsAbove(M,TYL,OD' OS',OS'') =
    if typeLeqS(M,TYL,argTypes(OD')) then
      $opsAbove(M,TYL,OS',OS'' OD')
    else
      $opsAbove(M,TYL,OS',OS'')
    fi .
  eq $opsAbove(M,TYL,none,OS'') = OS'' .

  --- INP: Module OpDeclSet
  --- PRE: OpDeclSet is well-defined with respect to Module
  --- OUT: The minimal OpDecls in the OpDeclSet
  op getMinimal : Module OpDeclSet -> OpDeclSet .
  op $getMinimal : Module OpDeclSet OpDeclSet OpDeclSet -> OpDeclSet .
  op $getMinimal1 : Module OpDecl OpDeclSet -> Bool .
  eq getMinimal(M,OS) = $getMinimal(M,OS,none,none) .

  --- OS'' are the mininmal
  eq $getMinimal(M,OD OS,OS',OS'') =
    --- if minimal w/respect to rest of OS, add to minimal and non-minimal pools
    if $getMinimal1(M,OD,OS OS') then
      $getMinimal(M,OS,OD OS',OD OS'')
    else
      $getMinimal(M,OS,OS',OS'')
    fi .
  eq $getMinimal(M,none,OS',OS'') = OS'' .

  --- Check if OD is minimal
  eq $getMinimal1(M,OD,OD' OS) =
    not typeLeqS(M,resultType(OD'),resultType(OD)) and-then $getMinimal1(M,OD,OS) .
  eq $getMinimal1(M,OD,none) = true .

  --- PRE: TypeList and Type are defined in Module
  --- OUT: Given a term with structure Q(X1...XN) whose
  ---      least sort is Type where the sorts of X1...XN
  ---      correspond to the Types in TypeList, find all
  ---      possible operators in the OpDeclSet that
  ---      could be instantiated to get this term
  op findOps : Module OpDeclSet Qid TypeList Type -> OpDeclSet .
  eq findOps(M,OD OS,Q,TYL,TY) =
    if typeLeqS(M,TYL,argTypes(OD)) and qid(OD) == Q and
        typeLeqS(M,resultType(OD),TY) then
      OD
    else
      none
    fi findOps(M,OS,Q,TYL,TY) .
  eq findOps(M,none,Q,TYL,TY) = none .

  --- PRE: Term and OpDeclSet are well-defined in Module
  --- OUT: Set of OpDecls that could top this term
  op findOps : Module OpDeclSet Term -> OpDeclSet .
  eq findOps(M,OS,Q[NTL]) = findOps(M,OS,Q,termListTypes(M,NTL),leastSort(M,Q[NTL])) .
  eq findOps(M,OS,C) = findOps(M,OS,getName(C),nil,leastSort(M,C)) .

  --- PRE: None
  --- OUT: True iff no operators share the same name in OpDeclSet
  op overloaded? : OpDeclSet -> Bool .
  eq overloaded?(op Q : TYL -> TY [AS]. op Q : TYL' -> TY' [AS']. OS) = true .
  eq overloaded?(OS)                                                  = false [owise] .
endfm

fmod OP-FAMILY is
  pr TYPE-EXTRA .
  pr OPDECL-TYPING .

  sort OpFamily OpFamilyMap .
  subsort OpFamily < OpFamilyMap .
  op (_,_)|->_ : Qid TypeList OpDeclSet -> OpFamily [ctor] .
  op __ : OpFamilyMap OpFamilyMap -> OpFamilyMap [ctor assoc comm id: nil] .
  op nil : -> OpFamilyMap [ctor] .

  op getOpFamilies : Module -> OpFamilyMap .
  op getOpFamilies : Module OpDeclSet -> OpFamilyMap [memo] .
  op $opF : Module OpDeclSet OpFamilyMap -> OpFamilyMap .
  op adhoc-overloaded? : OpFamilyMap -> Bool .

  var M : Module .
  var Q : Qid .
  var TYL TYL' : TypeList .
  var TY : Type .
  var AS : AttrSet .
  var OS OS' : OpDeclSet .
  var OFM : OpFamilyMap .
  var OD : OpDecl .

  --- INP: Module
  --- PRE: None
  --- OUT: OpFamilyMap which maps each Qid to all
  ---      the OpDecl's which have this Qid as their name
  eq getOpFamilies(M) = getOpFamilies(M,getOps(M)) .
  eq getOpFamilies(M,OS) = $opF(M,OS,nil) .
 ceq $opF(M,op Q : TYL -> TY [AS]. OS,OFM (Q,TYL') |-> OS') = $opF(M,OS,OFM (Q,TYL') |-> OS' op Q : TYL -> TY [AS].)
  if TYL' == toKind(M,TYL) .
  eq $opF(M,OD OS, OFM) = $opF(M,OS, OFM (qid(OD),toKind(M,argTypes(OD))) |-> OD) [owise] .
  eq $opF(M,none,OFM) = OFM .

  --- INP: OpFamilyMap
  --- PRE: OpFamilyMap is well-formed and sensible
  --- OUT: true iff operators are adhoc-overloaded
 ceq adhoc-overloaded?(((Q,TYL) |-> OS) ((Q,TYL') |-> OS') OFM) = true if TYL =/= TYL' .
  eq adhoc-overloaded?(OFM)                                 = false [owise] .
endfm

fmod OP-FAMILY-AUX is
  pr OP-FAMILY .

  op ctorsPreregularBelow : Module -> Bool .
  op ctorsPreregularBelow : Module OpFamilyMap -> Bool .
  op $ctorsPreregularBelow : Module TypeListSet OpDeclSet OpDeclSet -> Bool .
  op $ctorsPreregularBelow1 : Module TypeList OpDeclSet OpDeclSet -> Bool .
  op typesBelowMaximalOps : Module OpDeclSet -> TypeListSet .

  var M : Module .
  var Q : Qid .
  var TYL TYL' KL : TypeList .
  var TY TY' : Type .
  var AS AS' : AttrSet .
  var OS OS' OS'' : OpDeclSet .
  var OFM : OpFamilyMap .
  var OD : OpDecl .
  var TYLS : TypeListSet .

  eq typesBelowMaximalOps(M,OD OS) = typesBelow(M,argTypes(OD)) ; typesBelowMaximalOps(M,OS) .
  eq typesBelowMaximalOps(M,none) =  none .

  --- INP: OpFamilyMap
  --- PRE: Valid OpFamilyMap
  --- OUT: true iff ctors always have minimal typings
  eq ctorsPreregularBelow(M) = ctorsPreregularBelow(M,getOpFamilies(M)) .
  eq ctorsPreregularBelow(M,OFM (Q,KL) |-> OS) =
   $ctorsPreregularBelow(M,typesBelowMaximalOps(M,OS),ctors(OS),OS - ctors(OS)) and-then
   ctorsPreregularBelow(M,OFM) .
  eq ctorsPreregularBelow(M,nil) = true .

  eq $ctorsPreregularBelow(M,TYL ; TYLS,OS,OS') =
     $ctorsPreregularBelow1(M,TYL,OS,OS') and-then
     $ctorsPreregularBelow(M,TYLS,OS,OS') .
  eq $ctorsPreregularBelow(M,none,OS,OS') = true .

  --- NOTE: if the opsAbove intersect?s with the ctors, then the minimum
  ---       must be a ctor; if there is no minimum, we violated preregularity
  ceq $ctorsPreregularBelow1(M,TYL,OS,OS') = intersect?(OS,OS'') implies OD inODS OS
  if OS'' := opsAbove(M,TYL,OS OS') /\
     OD   := getMinimal(M,OS'') .
  eq $ctorsPreregularBelow1(M,TYL,OS,OS') = false [owise] .

  --- INP: OpFamilyMap
  --- PRE: Valid OpFamilyMap
  --- OUT: true iff no ops have the same input sorts and different output sorts
  op sameArgsDiffRes : OpFamilyMap -> Bool .
 ceq sameArgsDiffRes(OFM (Q,KL) |-> OS op Q : TYL -> TY [AS]. op Q : TYL -> TY' [AS'].) = true if TY == TY' .
  eq sameArgsDiffRes(OFM) = false [owise] .
endfm

fmod STMT-EXTRA is
  pr META-LEVEL .

  var M : Module . var R R' : Rule . var RS : RuleSet . var Q : Qid . var Y : Sort .
  var U V : Term . var A : AttrSet . var C : Condition . var EC : EqCondition .
  var E E' : Equation . var ES : EquationSet . var B B' : MembAx . var BS : MembAxSet .

  op rl-labels : Module -> [QidSet] .
  op rl-labels : RuleSet -> [QidSet] .
  eq rl-labels(M) = rl-labels(getRls(M)) .
  eq rl-labels(R R' RS) = rl-labels(R) ; rl-labels(R' RS) .
  eq rl-labels(rl U => V [label(Q) A].) = Q .
  eq rl-labels(crl U => V if C [label(Q) A].) = Q .
  eq rl-labels(none) = none .

  op eq-labels   : Module -> [QidSet] .
  op eq-labels   : EquationSet -> [QidSet] .
  eq eq-labels(M) = eq-labels(getEqs(M)) .
  eq eq-labels(E E' ES) = eq-labels(E) ; eq-labels(E' ES) .
  eq eq-labels(eq U = V [label(Q) A].) = Q .
  eq eq-labels(ceq U = V if EC [label(Q) A].) = Q .
  eq eq-labels(none) = none .

  op mb-labels : Module -> [QidSet] .
  op mb-labels : MembAxSet -> [QidSet] .
  eq mb-labels(M) = mb-labels(getMbs(M)) .
  eq mb-labels(B B' BS) = mb-labels(B) ; mb-labels(B' BS) .
  eq mb-labels(mb U : Y [label(Q) A].) = Q .
  eq mb-labels(cmb U : Y if EC [label(Q) A].) = Q .
  eq mb-labels(none) = none .

  op stmt-labels : Module -> [QidSet] .
  eq stmt-labels(M) = rl-labels(M) ; eq-labels(M) ; mb-labels(M) .
endfm

--- this module has functionality to generate unique prefixes with
--- respect to the sorts/operators in a module; this is useful when
--- an algorithm needs to generate fresh sorts/operators; if the
--- generated sort/operator will be needed often, it can be memoized
fmod UNIQUE-PREFIX is
  pr META-LEVEL .
  pr OPDECLSET-EXTRA .
  pr STMT-EXTRA .
  pr SET{String} .

  op sortPrefix     : Module             -> String [memo] .
  op sortPrefix     : SortSet            -> String .
  op opPrefix       : Module             -> String [memo] .
  op opPrefix       : OpDeclSet          -> String .
  op lblPrefix      : Module             -> String .
  op uniquePrefix   : QidSet             -> String .
  op uniquePrefix   : String Set{String} -> String .
  op qidSetToStrSet : QidSet             -> Set{String} .

  var P S : String . var SS : Set{String} . var D : Qid . var Q : QidSet .

  eq sortPrefix(M:Module)   = uniquePrefix(getSorts(M:Module)) .
  eq sortPrefix(S:SortSet)  = uniquePrefix(S:SortSet) .
  eq opPrefix(M:Module)     = uniquePrefix(qid(getOps(M:Module))) .
  eq opPrefix(O:OpDeclSet)  = uniquePrefix(qid(O:OpDeclSet)) .
  eq lblPrefix(M:Module)    = uniquePrefix(stmt-labels(M:Module)) .
  eq uniquePrefix(Q)        = uniquePrefix("@",qidSetToStrSet(Q)) .
  eq uniquePrefix(P,(S,SS)) = if P == substr(S,0,length(P)) then uniquePrefix(P + "@",(S,SS))
                                                         else uniquePrefix(P,SS) fi .
  eq uniquePrefix(P,empty)  = P .
  eq qidSetToStrSet(D ; Q)  = string(D) , qidSetToStrSet(Q) .
  eq qidSetToStrSet(none)   = empty .
endfm

--- this module provides functionality to add a set of variables into a module
--- as FRESH constants (by using the functionality of opPrefix above); the function
--- returns a new module as well as an assignment mapping each variable into its
--- fresh constant --- this assignment can later be used to decode the new term
--- back into its original form
fmod VARIABLES-TO-CONSTANTS is
  pr TERM-EXTRA .              --- for getName()/repeatedNames()
  pr UNIT-FM .                 --- for addOps()
  pr QID-JOIN .                --- for join()
  pr UNIQUE-PREFIX .           --- for opPrefix()
  pr SUBSTITUTION-REFINEMENT . --- for sort ConstSubstitution
  pr SUBSTITUTIONSET .         --- for sort SubstitutionSet

  sort ModuleSubstPair .
  op ((_,_)) : Module Substitution -> ModuleSubstPair [ctor] .
  op mod : ModuleSubstPair -> Module .
  op sub : ModuleSubstPair -> Substitution .

  sort ConstGenStrategy .
  ops simple prefix full : -> ConstGenStrategy [ctor] .

  op  varsToConsts  : Module ConstGenStrategy QidSet                            -> [Module] .
  op  varsToConsts# : Module ConstGenStrategy QidSet                            -> [ModuleSubstPair] .
  op  varsToConsts# : Module ConstGenStrategy QidSet Qid OpDeclSet Substitution -> [ModuleSubstPair] .

  op  constsToVars  : ConstSubstitution Term                                    -> Term .
  op  constsToVars  : ConstSubstitution Qid TermList TermList                   -> TermList .
  op  constsToVars  : ConstSubstitution SubstitutionSet                         -> SubstitutionSet .

  var M : Module . var V : Variable . var C : Constant . var Q : Qid .
  var QS : QidSet . var TL TL' : TermList . var T : Term . var SS : SubstitutionSet .
  var S S' : Substitution . var P : Qid . var O : OpDeclSet . var CG : ConstGenStrategy .
  var TQ : TermQid . var CS : ConstSubstitution .

  --- INP: Module QidSet (Variables)
  --- PRE: QidSet should be a set of variables
  --- OUT: A new module where variables have been added as constants
  eq mod((M,CS)) = M .
  eq sub((M,CS)) = CS .
  eq varsToConsts(M,CG,QS)                 = mod(varsToConsts#(M,CG,QS)) .
  eq varsToConsts#(M,CG,QS)                = varsToConsts#(M,CG,QS,qid(opPrefix(M)),none,none) .
  eq varsToConsts#(M,simple,V ; QS,P,O,CS) = varsToConsts#(M,simple,QS,P,O op getName(V)                       : nil -> getType(V) [none].,CS ; V <- join(getName(V) '. getType(V))) .
  eq varsToConsts#(M,prefix,V ; QS,P,O,CS) = varsToConsts#(M,prefix,QS,P,O op join(P getName(V))               : nil -> getType(V) [none].,CS ; V <- join(P getName(V) '. getType(V))) .
  eq varsToConsts#(M,full,  V ; QS,P,O,CS) = varsToConsts#(M,full,  QS,P,O op join(P getName(V) '| getType(V)) : nil -> getType(V) [none].,CS ; V <- join(P getName(V) '| getType(V) '. getType(V))) .
  eq varsToConsts#(M,CG,none,P,O,CS)       = (addOps(O,M),CS) .

  --- INP: Substitution (Variables to fresh Constants) Term/SubstitutionSet
  --- PRE: None
  --- OUT: Identical to Term/SubstitutionSet except in Term/Codomain of SubstitutionSet
  ---      each occurrence of a constant in the codomain of Substitution is
  ---      replaced by the variable which is assigned to it
  --- NB:  the first equation is not strictly necessary but it optimizes for the common
  ---      case when the substitution is empty
  eq constsToVars((none).Substitution,T) = T .
  eq constsToVars(CS,Q[TL])              = constsToVars(CS,Q,TL,empty) .
  eq constsToVars(CS,V)                  = V .
  eq constsToVars(CS ; V <- C,C)         = V .
  eq constsToVars(CS,C)                  = C [owise] .
  eq constsToVars(CS,Q,(T,TL),TL')       = constsToVars(CS,Q,TL,(TL',constsToVars(CS,T))) .
  eq constsToVars(CS,Q,empty,TL')        = Q[TL'] .
  ---
  eq constsToVars(CS,S | S' | SS)         = constsToVars(CS,S) | constsToVars(CS,S' | SS) .
  eq constsToVars(CS,.SubstitutionSet)    = .SubstitutionSet .
  eq constsToVars(CS,V <- T ; S)          = V <- constsToVars(CS,T) ; constsToVars(CS,S) .
  eq constsToVars(CS,(none).Substitution) = (none).Substitution .
endfm

--- Module checks if all the rules in the module we are analyzing in the same kind.
--- If not, that means module cannot be topmost
--- TODO: implement full topmost check
fmod RULES-SHARE-KIND is
  pr META-LEVEL .

  op rules-share-kind : Module -> Bool .
  op rules-share-kind : Module RuleSet -> Bool .
  op rules-share-kind : Module Type RuleSet -> Bool .
  op getRuleType     : Module ~> Type .
  op getRuleType     : Module RuleSet ~> Type .

  var M : Module . var L R : Term . var Y : Type .
  var RLS : RuleSet . var AS : AttrSet .

  eq rules-share-kind(M) = rules-share-kind(M,getRls(M)) .
  eq rules-share-kind(M,none) = false .
  eq rules-share-kind(M,rl L => R [AS] . RLS) =
    leastSort(M,L) ; leastSort(M,R) :: NeSortSet and-then sameKind(M,leastSort(M,L),leastSort(M,R)) and-then rules-share-kind(M,leastSort(M,L),RLS) .
  eq rules-share-kind(M,crl L => R if C:Condition [AS] . RLS) =
    leastSort(M,L) ; leastSort(M,R) :: NeSortSet and-then sameKind(M,leastSort(M,L),leastSort(M,R)) and-then rules-share-kind(M,leastSort(M,L),RLS) .
  eq rules-share-kind(M,Y,none) = true .
  eq rules-share-kind(M,Y,rl L => R [AS] . RLS) =
    leastSort(M,L) ; leastSort(M,R) :: NeSortSet and-then sameKind(M,Y,leastSort(M,L)) and-then sameKind(M,Y,leastSort(M,R)) and-then rules-share-kind(M,Y,RLS) .
  eq rules-share-kind(M,Y,crl L => R if C:Condition [AS] . RLS) =
    leastSort(M,L) ; leastSort(M,R) :: NeSortSet and-then sameKind(M,Y,leastSort(M,L)) and-then sameKind(M,Y,leastSort(M,R)) and-then rules-share-kind(M,Y,RLS) .

  eq getRuleType(M) = getRuleType(M,getRls(M)) .
  eq getRuleType(M,rl L => R [AS] . RLS) = leastSort(M,L) .
  eq getRuleType(M,crl L => R if C:Condition [AS] . RLS) = leastSort(M,L) .
endfm

--- defines operator kinds? : Bool Module -> Bool which returns true
--- iff the module contains any reference to a kind anywhere ---
--- the boolean arg controls whether any metaterms in the signature are
--- checked for evaluating to the kind or not ---
fmod KIND-CHECK is
  pr META-LEVEL .

  --- copy of or-else to make eqs more readable
  op _orL_ : Bool Bool -> Bool [strat (1 0) gather (e E) prec 59] .

  op kinds? : TypeListSet -> Bool .
  op kinds? : Module Bool TermList -> Bool .
  op kinds? : Module Bool AttrSet -> Bool .
  op kinds? : Module Bool Condition -> Bool .
  op kinds? : Module Bool OpDeclSet -> Bool .
  op kinds? : Module Bool MembAxSet -> Bool .
  op kinds? : Module Bool EquationSet -> Bool .
  op kinds? : Module Bool RuleSet -> Bool .
  op kinds? : Bool Module -> Bool .

  var FM     : FModule .
  var FT     : FTheory .
  var SM     : SModule .
  var ST     : STheory .
  var M      : Module .
  var K      : Kind .
  var S      : Sort .
  var SS     : SortSet .
  var T      : Type .
  var TL TL' : TypeList .
  var NTL    : NeTypeList .
  var TS     : TypeListSet .
  var C      : Constant .
  var V      : Variable .
  var TML    : TermList .
  var NTML   : NeTermList .
  var Q      : Qid .
  var AS     : AttrSet .
  var CN     : Condition .
  var TM TM' : Term .
  var O O'   : OpDecl .
  var OS     : OpDeclSet .
  var E E'   : Equation .
  var ES     : EquationSet .
  var MB MB' : MembAx .
  var MBS    : MembAxSet .
  var R R'   : Rule .
  var RS     : RuleSet .
  var B      : Bool .

  eq true orL B = true .
  eq false orL B = B .

  eq kinds?(K)                  = true .
  eq kinds?(S)                  = false .
  eq kinds?(T NTL)              = kinds?(T) orL kinds?(NTL) .
  eq kinds?(nil)                = false .
  eq kinds?(TL ; TL' ; TS)      = kinds?(TL) orL kinds?(TL' ; TS) .
  eq kinds?((none).TypeListSet) = false .

  eq kinds?(M,B,C)         = getType(C) :: Kind .
  eq kinds?(M,B,V)         = getType(V) :: Kind .
  eq kinds?(M,B,Q[NTML])   = (B and-then (leastSort(M,Q[NTML]) :: Kind)) orL kinds?(M,B,NTML) .
  eq kinds?(M,B,(TM,NTML)) = kinds?(M,B,TM) orL kinds?(M,B,NTML) .
  eq kinds?(M,B,empty)     = false .

  eq kinds?(M,B,id(T) AS)       = kinds?(M,B,T) orL kinds?(M,B,AS) .
  eq kinds?(M,B,left-id(T) AS)  = kinds?(M,B,T) orL kinds?(M,B,AS) .
  eq kinds?(M,B,right-id(T) AS) = kinds?(M,B,T) orL kinds?(M,B,AS) .
  eq kinds?(M,B,AS)             = false [owise] .

  eq kinds?(M,B,TM = TM' /\ CN)  = kinds?(M,B,(TM,TM')) orL kinds?(M,B,CN) .
  eq kinds?(M,B,TM : S /\ CN)    = kinds?(M,B,TM) orL kinds?(M,B,CN) .
  eq kinds?(M,B,TM := TM' /\ CN) = kinds?(M,B,(TM,TM')) orL kinds?(M,B,CN) .
  eq kinds?(M,B,TM => TM' /\ CN) = kinds?(M,B,(TM,TM')) orL kinds?(M,B,CN) .
  eq kinds?(M,B,(nil).Condition) = false .

  eq kinds?(M,B,op Q : TL -> T [AS].) = kinds?(TL T) orL kinds?(M,B,AS) .
  eq kinds?(M,B,O O' OS)              = kinds?(M,B,O) orL kinds?(M,B,O' OS) .
  eq kinds?(M,B,(none).OpDeclSet)     = false .

  eq kinds?(M,B,mb TM : S [AS].)        = kinds?(M,B,TM) orL kinds?(M,B,AS) .
  eq kinds?(M,B,cmb TM : S if CN [AS].) = kinds?(M,B,TM) orL kinds?(M,B,CN) orL kinds?(M,B,AS) .
  eq kinds?(M,B,MB MB' MBS)             = kinds?(M,B,MB) orL kinds?(M,B,MB' MBS) .
  eq kinds?(M,B,(none).MembAxSet)       = false .

  eq kinds?(M,B,eq TM = TM' [AS].)        = kinds?(M,B,(TM,TM')) orL kinds?(M,B,AS) .
  eq kinds?(M,B,ceq TM = TM' if CN [AS].) = kinds?(M,B,(TM,TM')) orL kinds?(M,B,CN) orL kinds?(M,B,AS) .
  eq kinds?(M,B,E E' ES)                  = kinds?(M,B,E) orL kinds?(M,B,E' ES) .
  eq kinds?(M,B,(none).EquationSet)       = false .

  eq kinds?(M,B,rl TM => TM' [AS].)        = kinds?(M,B,(TM,TM')) orL kinds?(M,B,AS) .
  eq kinds?(M,B,crl TM => TM' if CN [AS].) = kinds?(M,B,(TM,TM')) orL kinds?(M,B,CN) orL kinds?(M,B,AS) .
  eq kinds?(M,B,R R' RS)                   = kinds?(M,B,R) orL kinds?(M,B,R' RS) .
  eq kinds?(M,B,(none).RuleSet)            = false .

  eq kinds?(B,FM) =
    kinds?(FM,B,getOps(FM)) orL
    kinds?(FM,B,getMbs(FM)) orL
    kinds?(FM,B,getEqs(FM)) .

  eq kinds?(B,FT) =
    kinds?(FT,B,getOps(FT)) orL
    kinds?(FT,B,getMbs(FT)) orL
    kinds?(FT,B,getEqs(FT)) .

  eq kinds?(B,SM) =
    kinds?(SM,B,getOps(SM)) orL
    kinds?(SM,B,getMbs(SM)) orL
    kinds?(SM,B,getEqs(SM)) orL
    kinds?(SM,B,getRls(SM)) .

  eq kinds?(B,ST) =
    kinds?(ST,B,getOps(ST)) orL
    kinds?(ST,B,getMbs(ST)) orL
    kinds?(ST,B,getEqs(ST)) orL
    kinds?(ST,B,getRls(ST)) .
endfm
```

Generic Fixpoints
-----------------

Here we give an implementation of an arbitrary fixpoint.

NOTE: the functional theory here is far too weak, since
      arbitrary functions don't have fixpoints---but
      adding the constraints to express that F truly has
      fixpoints is more work than I want to do right now.

```maude
fth FUN is inc TRIV .
  op F : Elt -> Elt .
endfth

fmod FIXF{X :: FUN} is
  pr TRUTH .
  var A A' : X$Elt .
  op FixF : X$Elt -> X$Elt .
  op FixF : X$Elt X$Elt -> X$Elt .
  eq FixF(A) = FixF(A,F(A)) .
  eq FixF(A,A') = if A == A' then A' else FixF(A') fi .
endfm
```

Meta-Level Functors
-------------------

```maude
fmod HETEROGENEOUS-LIST-FUNCTOR is
  pr META-LEVEL .
  pr UNIT-FM .
  pr UNIQUE-PREFIX . --- for opPrefix/sortPrefix
  pr TYPE-EXTRA . --- for maxSortAbove

  var M : Module .
  var S : Sort . var SS : SortSet .
  var T : Term . var NTL : NeTermList . var TL : TermList .
  var Q : Qid .

  --- Define Free Heterogeneous List Functor over the Module
  op hl-inj : Module Sort -> Qid .
  eq hl-inj(M,S) = qid(opPrefix(M) + "S") .

  op hl-injops : Module SortSet -> OpDeclSet .
  eq hl-injops(M,S ; SS) = (op hl-inj(M,S) : S -> hl-sort(M) [ctor].) hl-injops(M,SS) .
  eq hl-injops(M,none) = none .

  --- NOTE: Op Prefix Not Needed Since Sorts All Fresh
  op hl-listop  : Module -> OpDecl .
  eq hl-listop(M) = (op 'singleton : hl-sort(M) -> hl-sort(M) [ctor].
                     op '_|_ : hl-sort(M) hl-sort(M) -> hl-sort(M) [assoc ctor].) .

  op hl-sort : Module -> Sort .
  eq hl-sort(M) = qid(sortPrefix(M) + "HeterogeneousList") .

  op hl-func : Module -> Module [memo] .
  eq hl-func(M) = addSorts(hl-sort(M),addOps(hl-listop(M) hl-injops(M,getMaximalSorts(M)),M)) .

  --- Define Free Heterogenous List Fuctor over Terms in the Module
  op toHL : Module NeTermList -> Term .
  op $toHL : Module TermList -> TermList .
  eq toHL(M,T) = 'singleton[hl-inj(M,maxSortAbove(M,leastSort(M,T)))[T]] .
  eq toHL(M,(T,NTL)) = '_|_[$toHL(M,(T,NTL))] .
  eq $toHL(M,(T,TL)) = hl-inj(M,maxSortAbove(M,leastSort(M,T)))[T], $toHL(M,TL) .
  eq $toHL(M,empty) = empty .

  --- Define Forgetful Functor over Heterogeneous Lists
  op toTL : Term -> [NeTermList] .
  eq toTL('_|_[T,NTL]) = $toTL((T,NTL)) .
  eq toTL('singleton[T]) = $toTL(T) .

  op $toTL : TermList -> TermList .
  eq $toTL((Q[T],TL)) = T,$toTL(TL) .
  eq $toTL(empty) = empty .

  --- Equational Laws (Left As Proof Obligations)
  --- For all
  --- [1] modules M
  --- [2] termlists NTL in M
  --- [3] terms T of sort HeterogeneousList in HL[M]
  eq toTL(toHL(M,NTL)) = NTL [nonexec] .
  eq toHL(M,toTL(T)) = T [nonexec] .
endfm

fmod UNIFICATION-PROBLEM-AUX is
  pr META-LEVEL .

  var UP : UnificationProblem .
  var T T' : Term .

  op UnifProbLHSToTL : UnificationProblem -> NeTermList .
  eq UnifProbLHSToTL(T =? T' /\ UP) = T, UnifProbLHSToTL(UP) .
  eq UnifProbLHSToTL(T =? T') = T .
endfm
```

Meta-Term Printing and Parsing
------------------------------

```maude
fmod BUBBLES is
  including QID-LIST .
  sorts @Token@ @SortToken@ @NeTokenList@ @Bubble@ .
  op token : Qid -> @Token@
    [special(
      id-hook Bubble (1 1)
      op-hook qidSymbol (<Qids> : ~> Qid))] .
  op sortToken : Qid -> @SortToken@
    [special(
       id-hook Bubble (1 1)
       op-hook qidSymbol (<Qids> : ~> Qid)
       id-hook Exclude ([ ] < to , . ( ) { } : |
                        ditto precedence prec gather
                        assoc associative comm commutative
                        ctor constructor id: strat strategy
                        poly memo memoization iter frozen
                        config object msg metadata nonexec variant))] .
  op neTokenList : QidList -> @NeTokenList@
    [special(
       id-hook Bubble (1 -1 ( ))
       op-hook qidListSymbol (__ : QidList QidList ~> QidList)
       op-hook qidSymbol (<Qids> : ~> Qid)
       id-hook Exclude (.))] .
  op bubble : QidList -> @Bubble@
    [special(
       id-hook Bubble (1 -1 ( ))
       op-hook qidListSymbol (__ : QidList QidList ~> QidList)
       op-hook qidSymbol (<Qids> : ~> Qid))] .
endfm

fmod GENERIC-PRINTER is
  pr META-LEVEL .
  pr CONVERSION .
  pr TERMSET-FM .

  ops &mt &sp  :                     -> Qid     . --- constants for nothing/space
  op  pad      : Qid Nat             -> QidList . --- add padding upto length
  op  addsp    : Nat                 -> QidList .
  op  print    : Module TermSet      -> QidList . --- printing fuctions below
  op  printSub : Module Substitution -> QidList .
  op  print    : NatList QidList     -> QidList .
  op  printNL  : NatList QidList     -> QidList .

  var M  : Module  . var SB : Substitution . var T T' : Term . var Q : Qid .
  var TS : TermSet . var A A' : Assignment . var NL : NatList .
  var QL : QidList . var V : Variable      . var N : Nat .
  var RP RP' : ResultPair . var RP? : [ResultPair] .

  eq addsp(s(N))               = &sp addsp(N) .
  eq addsp(0)                  = nil .
  eq pad(Q,N)                  = Q if length(string(Q)) >= N then nil else addsp(sd(length(string(Q)),N)) fi .
  eq &sp                       = qid(" ") .
  eq &mt                       = qid("")  .
  --- print termsets
  eq print(M,T | T' | TS)      = metaPrettyPrint(M,T) '`, print(M,T' | TS) .
  eq print(M,T)                = metaPrettyPrint(M,T) .
  eq print(M,.TermSet)         = &mt .
  --- print substitutions
  eq printSub(M,V <- T)        = metaPrettyPrint(M,V) '<- metaPrettyPrint(M,T) .
  eq printSub(M,A ; A' ; SB)   = printSub(M,A) '; printSub(M,A' ; SB) .
  eq printSub(M,none)          = 'empty 'substitution .
  --- print natlist
  eq print(NL,QL)              = printNL(NL,QL) .
  eq printNL(N NL,QL)          = printNL(NL,QL qid(string(N,10))) .
  eq printNL((nil).NatList,QL) = QL .

  op printN : Nat QidList -> QidList .  *** first N qid's in a qidList
  eq printN(N, nil) = nil .
  eq printN(0, QL) = nil .
  eq printN(s N, Q QL) = Q printN(N, QL) .

  op printSyntaxError : [ResultPair?] QidList -> QidList .
  eq printSyntaxError(noParse(N), QL)
    = '\r 'Parse 'error 'in '\o '\s printN(N + 1, QL) '\r '<---*HERE* '\o .
  eq printSyntaxError(ambiguity(RP, RP'), QL)
    = '\r 'Ambiguous 'parsing 'for '\o '\s QL '\o .
  eq printSyntaxError(RP?, QL) = QL [owise] .
endfm
```
