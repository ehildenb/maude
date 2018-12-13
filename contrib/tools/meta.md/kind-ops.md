Kind Operations
===============

The module also contains code to check that a module does not mention kinds at all.
In that case, sortify conversion is not necessary.
The module KIND-CHECK contains the function kinds?() which when given a piece of a module that could contain kinds, it will check if it does.
It has a boolean flag which controls how strictly it checks: if the flag is false, it will only check for direct occurences of kinds in the program text.
If true, it will check that all terms appearing in the program have a least type that is not a kind.

```maude
sload foform.maude
sload ../base/full-maude.maude

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

--- this module extends another module with lists of terms
--- or pairs of terms for every sort of term; this is used
--- later with ctor-variant() in order to generate all the
--- constructor unifiers
fmod KIND-LIST-EXT is
  pr META-LEVEL .
  pr UNIT .
  pr CONVERSION .
  pr FOFORMSET .
  pr QID-JOIN .
  pr UNIQUE-PREFIX .
  pr TYPE-EXTRA . --- for maximalSorts

  --- build the kind extension
  op kl-ops   : Module -> OpDeclSet .
  op k-ops    : Module SortSet -> OpDeclSet .
  op k-op     : Module Sort -> Qid .
  op kp-op    : Module Sort -> Qid .
  op kl-sort  : Module  -> Sort .
  op kpl-sort : Module  -> Sort .
  op kl-ext   : Module -> Module [memo] .
  --- use the kind extension to build terms
  op tk       : Module Term -> Sort .
  op fms      : Module Term SortSet ~> Sort .
  op toKL     : Module UnificationProblem -> Term .
  op $toKL    : Module UnificationProblem -> NeTermList .
  op toKPL    : Module NegEqConj -> Term .
  op $toKPL   : Module NegEqConj -> Term .
  op toDUPs   : TermSet ~> NegEqConjSet .
  op $toDUP   : NeTermList ~> NegEqConj .

  var M : Module .
  var S : Sort .
  var SS : SortSet .
  var K : Kind .
  var KS : KindSet .
  var U U' : UnificationProblem .
  var T T' : Term .
  var TS : TermSet .
  var TL : NeTermList .
  var NL : NegEqLit .
  var NF : NegEqConj .
  var Q : Qid .

  --- generate an operator which holds one term
  --- or a pair of same-kinded terms
  eq k-op(M,S)  = qid(opPrefix(M) + "S" + "[_]"  ) .
  eq kp-op(M,S) = qid(opPrefix(M) + "S" + "[_,_]") .

  --- generate the set of kind-holding operators
  --- given a set of kinds
  eq k-ops(M,S ; SS) =
    (op k-op(M,S)  : S   -> kl-sort(M)  [ctor].)
    (op kp-op(M,S) : S S -> kpl-sort(M) [ctor].)
    k-ops(M,SS) .
  eq k-ops(M,none) = none .

  --- generate operators which allow to build lists
  --- of terms in a kind/pairs of terms in a kind
  --- NOTE: these operators do not need unique names
  ---       since they operate on fresh sorts so they
  ---       will not clash with existing sorts
  eq kl-ops(M) =
     (op 'kl  : kl-sort(M)  kl-sort(M)  -> kl-sort(M)  [assoc ctor].
      op 'kpl : kpl-sort(M) kpl-sort(M) -> kpl-sort(M) [assoc ctor].) .
  eq kl-sort(M)  = qid(sortPrefix(M) + "KindList") .
  eq kpl-sort(M) = qid(sortPrefix(M) + "KindPairList") .

  --- given a module, generate all of the above
  eq kl-ext(M) =
    addSorts(kl-sort(M) ; kpl-sort(M),addOps(kl-ops(M) k-ops(M,getMaximalSorts(M)),M)) .

  --- given a term, get a maximal sort in the conn. comp to which the term belongs
  eq tk(M,T)               = fms(M,T,maximalSorts(M,getKind(M,leastSort(M,T)))) .
  eq fms(M,T,S ; SS)       = if sortLeq(M,leastSort(M,T),S) then S else fms(M,T,SS) fi .

  --- given a unification problem, generate a list corresponding to
  --- the the lhs of each unificand using the kindlist operator
  eq toKL(M,T =? T')       = $toKL(M,T =? T') .
  eq toKL(M,U /\ U')       = 'kl[$toKL(M,U /\ U')] .
  eq $toKL(M,T =? T')      = k-op(M,tk(M,T))[T] .
  eq $toKL(M,T =? T' /\ U) = $toKL(M,T =? T'), $toKL(M,U) .

  --- given a set of disequalities, generate a list of pairs of
  --- terms corresponding to each disequation
  eq toKPL(M,NL)           = $toKPL(M,NL) .
  eq toKPL(M,NL /\ NF)     = 'kpl[$toKPL(M,NL /\ NF)] .
  eq $toKPL(M,T != T')     = kp-op(M,tk(M,T))[T,T'] .
  eq $toKPL(M,NL /\ NF)    = $toKPL(M,NL), $toKPL(M,NF) .

  --- given a set of terms that were generated by using
  --- the toKPL operator, convert them back into disequations
  eq toDUPs(T | T' | TS) = toDUPs(T) | toDUPs(T' | TS) .
  eq toDUPs(.TermSet)    = mtFormSet .
  eq toDUPs('kpl[TL])    = $toDUP(TL) .
  eq toDUPs(T)           = $toDUP(T) [owise] .
  eq $toDUP(Q[T,T'])     = T != T' .
  eq $toDUP((T,TL))      = $toDUP(T) /\ $toDUP(TL) .
endfm
```
