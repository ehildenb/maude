Sort Operations
===============

This module implements a few empty and finite sort constructions; in particular, in contains functionality to:

1.  Compute which sorts are empty (and empty modulo B)
2.  Compute which sorts are finite (but not empty modulo B)
3.  If a sort is finite modulo B, generate all of the terms in it

```maude
sload ../base/model-checker.maude
sload meta-aux.maude

fmod EMPTY-SORT-REWTH is
  pr META-LEVEL .
  pr QID-JOIN .

  op sort-empty? : FModule Sort -> [Bool] [memo] .
  op sorts-nonempty? : FModule TypeList -> [Bool] [memo] .
  op empty-sorts : FModule -> [SortSet] .

  op empty-sorts : FModule SortSet -> [SortSet] .
  op es-sort-op : Sort -> Qid .
  op sort-set : TypeList -> Term .
  op esrewth : FModule -> [SModule] [memo] .
  op es-sort-ops : SortSet -> OpDeclSet .
  op es-subsort-rls : SubsortDeclSet -> RuleSet .
  op es-op-rls : OpDeclSet -> RuleSet .

  var FM : FModule .
  var S S' : Sort .
  var SS : SortSet .
  var SSDS : SubsortDeclSet .
  var Q : Qid .
  var NTL : NeTypeList .
  var TL : TypeList .
  var AS : AttrSet .
  var ODS : OpDeclSet .
  var T : Type .

  eq sort-empty?(FM,S) =
    not metaSearch(
          esrewth(FM),
          join((S '.Sort)),
          '*.Sort,
          nil,
          '!,
          unbounded,
          0) :: ResultTriple .

  eq sorts-nonempty?(FM,S TL) =
    not sort-empty?(FM,S) and-then sorts-nonempty?(FM,TL) .
  eq sorts-nonempty?(FM,nil) = true .

  eq empty-sorts(FM) = empty-sorts(FM,getSorts(FM)) .
  eq empty-sorts(FM,S ; SS) =
    if sort-empty?(FM,S) then S else none fi ; empty-sorts(FM,SS) .
  eq empty-sorts(FM,none) = none .

  eq esrewth(FM) =
    mod join((getName(FM) '-ESTH)) is
     nil
     sorts 'Sort ; 'SortSet .
     (subsort 'Sort < 'SortSet .)
     (op 'set : 'SortSet 'SortSet -> 'SortSet [assoc comm id('mt.Sort)] .
      op 'mt : nil -> 'SortSet [none] .
      op '* : nil -> 'Sort [none] .)
      es-sort-ops(getSorts(FM))
      none
     (eq 'set['X:Sort,'X:Sort] = 'X:Sort [none] .)
      es-subsort-rls(getSubsorts(FM))
      es-op-rls(getOps(FM))
    endm .

  eq es-sort-op(S) = join((S '.Sort)) .
  eq sort-set(T NTL) = 'set[es-sort-op(T),sort-set(NTL)] .
  eq sort-set(T) = es-sort-op(T) .

  eq es-sort-ops(S ; SS) =
    op S : nil -> 'Sort [none] .
    es-sort-ops(SS) .
  eq es-sort-ops(none) = none .

  eq es-subsort-rls(subsort S' < S . SSDS) =
    rl es-sort-op(S) => es-sort-op(S') [none] .
    es-subsort-rls(SSDS) .
  eq es-subsort-rls(none) = none .

  eq es-op-rls(op Q : NTL -> S [AS] . ODS) =
    rl es-sort-op(S) => sort-set(NTL) [none] .
    es-op-rls(ODS) .
  eq es-op-rls(op Q : nil -> S [AS] . ODS) =
    rl es-sort-op(S) => '*.Sort [none] .
    es-op-rls(ODS) .
  eq es-op-rls(none) = none .
endfm

fmod FIN-SORT-REWTH is
  pr EMPTY-SORT-REWTH .
  pr META-LEVEL .
  pr TYPE-EXTRA .

  op get-finite-sorts : FModule SortSet -> SortSet .
  op sort-finite? : FModule Sort ~> Bool [memo] .
  op fsrewth : FModule -> SModule [memo] .
  op cysrewth : FModule -> SModule [memo] .
  op fs-sort-op : Sort SortSet -> Term .
  op fs-sort-ops : SortSet SortSet -> OpDeclSet .
  op fs-tltoss : TypeList -> SortSet .
  op fs-op-rls : FModule OpDeclSet SortSet -> RuleSet .
  op fs-op-rls1 : SortSet SortSet SortSet -> RuleSet .
  op fs-op-rls2 : Sort SortSet SortSet -> RuleSet .
  op cycle-sorts : FModule -> SortSet [memo] .
  op cycle-sorts1 : FModule SModule SortSet -> SortSet .

  var FM : FModule .
  var SM : SModule .
  var S S' : Sort .
  var SS SS' CYS : SortSet .
  var SSDS : SubsortDeclSet .
  var N : Nat .
  var B : Bound .
  var RT? : ResultTriple? .
  var Q : Qid .
  var C C' : Constant .
  var NTL : NeTypeList .
  var TL : TypeList .
  var AS : AttrSet .
  var ODS : OpDeclSet .
  var RS : RuleSet .
  var NTML : NeTermList .

  eq get-finite-sorts(FM,S ; SS) =
    if sort-finite?(FM,S) then S else none fi ;
    get-finite-sorts(FM,SS) .
  eq get-finite-sorts(FM,none) = none .

  eq sort-finite?(FM,S) = not metaSearch(fsrewth(FM),fs-sort-op(S,cycle-sorts(FM)),'S:CycleSort,nil,'*,unbounded,0) :: ResultTriple .

  --- this theory is used to detect finite sorts
  eq fsrewth(FM) =
    (mod join((getName(FM) '-FSTH)) is
     nil
     sorts 'CycleSort ; 'Sort .
     subsort 'CycleSort < 'Sort .
     fs-sort-ops(getSorts(FM),cycle-sorts(FM))
     none
     none
     fs-op-rls(FM,getOps(FM),cycle-sorts(FM))
    endm) .

  eq fs-sort-op(S,CYS) = join(S if S in CYS then '.CycleSort else '.Sort fi) .

  eq fs-sort-ops(S ; SS,CYS) =
    op S : nil -> if S in CYS then 'CycleSort else 'Sort fi [none] .
    fs-sort-ops(SS,CYS) .
  eq fs-sort-ops(none,CYS) = none .

  eq fs-op-rls(FM,op Q : NTL -> S [AS] . ODS,CYS) =
    if sorts-nonempty?(FM,NTL) then
      fs-op-rls1(greaterSorts(FM,S),fs-tltoss(NTL),CYS)
    else
      none
    fi
    fs-op-rls(FM,ODS,CYS) .
  eq fs-op-rls(FM,op Q : nil -> S [AS] . ODS,CYS) = fs-op-rls(FM,ODS,CYS) .
  eq fs-op-rls(FM,none,CYS) = none .

  eq fs-op-rls1(S ; SS,SS',CYS) = fs-op-rls2(S,SS',CYS) fs-op-rls1(SS,SS',CYS) .
  eq fs-op-rls1(none,SS',CYS) = none .

  eq fs-op-rls2(S,S' ; SS',CYS) =
    rl fs-sort-op(S,CYS) => fs-sort-op(S',CYS) [none] .
    fs-op-rls2(S,SS',CYS) .
  eq fs-op-rls2(S,none,CYS) = none .

  eq fs-tltoss(S TL) = S ; fs-tltoss(TL) .
  eq fs-tltoss(nil) = none .

  --- this theory is used to detect cyclic sorts
  eq cysrewth(FM) =
    (mod join((getName(FM) '-CYSTH)) is
     nil
     sorts 'Sort .
     none
     fs-sort-ops(getSorts(FM),none)
     none
     none
     fs-op-rls(FM,getOps(FM),none)
    endm) .

  eq cycle-sorts(FM) = cycle-sorts1(FM,cysrewth(FM),getSorts(FM)) .
  eq cycle-sorts1(FM,SM,S ; SS) =
    if (not sort-empty?(FM,S)) and-then
        metaSearch(SM,fs-sort-op(S,none),fs-sort-op(S,none),nil,'+,unbounded,0) :: ResultTriple then
      S
    else
      none
    fi ; cycle-sorts1(FM,SM,SS) .
  eq cycle-sorts1(FM,SM,none) = none .
endfm

fmod SORT-GEN-REWTH is
  pr TYPE-EXTRA .
  pr QID-JOIN .
  pr META-LEVEL .
  pr TERM-SET .

  op sort-gen   : FModule Sort -> TermSet [memo] .
  op sort-gen   : FModule Sort Bound -> TermSet .
  op sort-gen   : SModule Sort Nat Bound TermSet ResultTriple? -> TermSet .
  op sort-gen1  : SModule Sort Nat -> ResultTriple? .
  op sgrewth    : FModule -> SModule [memo] .
  op sg-sort-op : SModule Sort -> Term .
  op sg-sort-ops : FModule SortSet -> OpDeclSet .
  op sg-subsort-rls : FModule SubsortDeclSet -> RuleSet .
  op sg-op-rls : FModule OpDeclSet -> RuleSet .
  op sg-op-rls1 : FModule TypeList -> TermList .

  var FM : FModule .
  var SM : SModule .
  var S S' : Sort .
  var SS : SortSet .
  var SSDS : SubsortDeclSet .
  var N : Nat .
  var B : Bound .
  var TS : TermSet .
  var RT? : ResultTriple? .
  var Q : Qid .
  var NTL : NeTypeList .
  var TL : TypeList .
  var AS : AttrSet .
  var ODS : OpDeclSet .

  eq sort-gen(FM,S) = sort-gen(FM,S,unbounded) .
  eq sort-gen(FM,S,B) =
    sort-gen(sgrewth(FM),S,0,B,.TermSet,sort-gen1(sgrewth(FM),S,0)) .
  eq sort-gen(SM,S,N,B,TS,RT?) =
    if RT? =/= failure and (B == unbounded or-else N <= B) then
      sort-gen(SM,S,s(N),B,TS | getTerm(RT?),sort-gen1(SM,S,s(N)))
    else
      TS
    fi .
  eq sort-gen1(SM,S,N) =
      metaSearch(
        SM,
        sg-sort-op(SM,S),
        join('X: S),
        nil,
        '!, unbounded,
        N
      ) .

  eq sgrewth(FM) =
    mod join((getName(FM) '-SGTH)) is
     nil
     sorts getSorts(FM) .
     getSubsorts(FM)
    (getOps(FM)
     sg-sort-ops(FM,getSorts(FM)))
     none
     none
     sg-subsort-rls(FM,getSubsorts(FM))
     sg-op-rls(FM,getOps(FM))
    endm .

  eq sg-sort-op(SM,S) = join(S '. completeName(SM,getKind(SM,S))) .

  eq sg-sort-ops(FM,S ; SS) =
    op S : nil -> completeName(FM,getKind(FM,S)) [none] .
    sg-sort-ops(FM,SS) .
  eq sg-sort-ops(FM,none) = none .

  eq sg-subsort-rls(FM,subsort S' < S . SSDS) =
    rl sg-sort-op(FM,S) => sg-sort-op(FM,S') [none] .
    sg-subsort-rls(FM,SSDS) .
  eq sg-subsort-rls(FM,none) = none .

  eq sg-op-rls(FM,op Q : NTL -> S [AS] . ODS) =
    rl sg-sort-op(FM,S) => Q[sg-op-rls1(FM,NTL)] [none] .
    sg-op-rls(FM,ODS) .
  eq sg-op-rls(FM,op Q : nil -> S [AS] . ODS) =
    rl sg-sort-op(FM,S) => join(Q '. S) [none] .
    sg-op-rls(FM,ODS) .
  eq sg-op-rls(FM,none) = none .

  eq sg-op-rls1(FM,S TL) = sg-sort-op(FM,S), sg-op-rls1(FM,TL) .
  eq sg-op-rls1(FM,nil) = empty .
endfm

fmod SORT-GEN-EXTRA is
  pr SORT-GEN-REWTH .

  sort SrtTrmSetMap SrtTrmSetEntry .
  subsort SrtTrmSetEntry < SrtTrmSetMap .
  op mtSTM : -> SrtTrmSetMap [ctor] .
  op _,_ : SrtTrmSetMap SrtTrmSetMap -> SrtTrmSetMap [assoc comm ctor id: mtSTM] .
  op ((_,_)) : Sort TermSet -> SrtTrmSetEntry [ctor] .

  var FM : FModule .
  var SS : SortSet .
  var S : Sort .

  op sorts-gen : FModule SortSet -> SrtTrmSetMap .
  eq sorts-gen(FM,S ; SS) = (S,sort-gen(FM,S)) , sorts-gen(FM,SS) .
  eq sorts-gen(FM,none) = mtSTM .
endfm
```
