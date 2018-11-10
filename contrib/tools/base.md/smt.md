SMT Bindings
============

```maude
***(

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2014 SRI International, Menlo Park, CA 94025, USA.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

)

***
***	SMT bindings.
***	Version alpha 104.
***

load ../meta/narrowing.maude
```

Boolean Data
------------

```maude
set include BOOL off .
set include BOOLEAN off .

fmod BOOLEAN is
  sort Boolean .
  op true : -> Boolean [special (id-hook SMT_Symbol (true))] .
  op false : -> Boolean [special (id-hook SMT_Symbol (false))] .

  op not_ : Boolean -> Boolean [prec 53 special (id-hook SMT_Symbol (not))] .
  op _and_ : Boolean Boolean -> Boolean [gather (E e) prec 55 special (id-hook SMT_Symbol (and))] .
  op _xor_ : Boolean Boolean -> Boolean [gather (E e) prec 57 special (id-hook SMT_Symbol (xor))] .
  op _or_ : Boolean Boolean -> Boolean [gather (E e) prec 59 special (id-hook SMT_Symbol (or))] .
  op _implies_ : Boolean Boolean -> Boolean [gather (e E) prec 61 special (id-hook SMT_Symbol (implies))] .

  op _===_ : Boolean Boolean -> Boolean [gather (e E) prec 51 special (id-hook SMT_Symbol (===))] .
  op _=/==_ : Boolean Boolean -> Boolean [gather (e E) prec 51 special (id-hook SMT_Symbol (=/==))] .
  op _?_:_ : Boolean Boolean Boolean -> Boolean [gather (e e e) prec 71 special (id-hook SMT_Symbol (ite))] .
endfm

view Boolean from TRIV to BOOLEAN is sort Elt to Boolean . endv
```

Integer Data
------------

```maude
fmod INTEGER is
  protecting BOOLEAN .
  sort Integer .
  op <Integers> : -> Integer [special (id-hook SMT_NumberSymbol (integers))] .

  op -_ : Integer -> Integer [special (id-hook SMT_Symbol (-))] .
  op _+_ : Integer Integer -> Integer [gather (E e) prec 33 special (id-hook SMT_Symbol (+))] .
  op _*_ : Integer Integer -> Integer [gather (E e) prec 31 special (id-hook SMT_Symbol (*))] .
  op _-_ : Integer Integer -> Integer [gather (E e) prec 33 special (id-hook SMT_Symbol (-))] .
  op _div_ : Integer Integer -> Integer [gather (E e) prec 31 special (id-hook SMT_Symbol (div))] .
  op _mod_ : Integer Integer -> Integer [gather (E e) prec 31 special (id-hook SMT_Symbol (mod))] .

  op _<_ : Integer Integer -> Boolean [prec 37 special (id-hook SMT_Symbol (<))] .
  op _<=_ : Integer Integer -> Boolean [prec 37 special (id-hook SMT_Symbol (<=))] .
  op _>_ : Integer Integer -> Boolean [prec 37 special (id-hook SMT_Symbol (>))] .
  op _>=_ : Integer Integer -> Boolean [prec 37 special (id-hook SMT_Symbol (>=))] .

  op _===_ : Integer Integer -> Boolean [gather (e E) prec 51 special (id-hook SMT_Symbol (===))] .
  op _=/==_ : Integer Integer -> Boolean [gather (e E) prec 51 special (id-hook SMT_Symbol (=/==))] .
  op _?_:_ : Boolean Integer Integer -> Integer [gather (e e e) prec 71 special (id-hook SMT_Symbol (ite))] .

  *** seems to break CVC4
  op _divisible_ : Integer Integer -> Boolean [prec 51 special (id-hook SMT_Symbol (divisible))] .
endfm
```

Real Data
---------

```maude
fmod REAL is
  protecting BOOLEAN .
  sort Real .
  op <Reals> : -> Real [special (id-hook SMT_NumberSymbol (reals))] .
 
  op -_ : Real -> Real [special (id-hook SMT_Symbol (-))] .
  op _+_ : Real Real -> Real [gather (E e) prec 33 special (id-hook SMT_Symbol (+))] .
  op _*_ : Real Real -> Real [gather (E e) prec 31 special (id-hook SMT_Symbol (*))] .
  op _-_ : Real Real -> Real [gather (E e) prec 33 special (id-hook SMT_Symbol (-))] .
  op _/_ : Real Real -> Real [gather (E e) prec 31 special (id-hook SMT_Symbol (/))] .

  op _<_ : Real Real -> Boolean [prec 37 special (id-hook SMT_Symbol (<))] .
  op _<=_ : Real Real -> Boolean [prec 37 special (id-hook SMT_Symbol (<=))] .
  op _>_ : Real Real -> Boolean [prec 37 special (id-hook SMT_Symbol (>))] .
  op _>=_ : Real Real -> Boolean [prec 37 special (id-hook SMT_Symbol (>=))] .

  op _===_ : Real Real -> Boolean [gather (e E) prec 51 special (id-hook SMT_Symbol (===))] .
  op _=/==_ : Real Real -> Boolean [gather (e E) prec 51 special (id-hook SMT_Symbol (=/==))] .
  op _?_:_ : Boolean Real Real -> Real [gather (e e e) prec 71 special (id-hook SMT_Symbol (ite))] .
endfm

fmod REAL-INTEGER is
  protecting INTEGER .
  protecting REAL .

  op toReal : Integer -> Real [special (id-hook SMT_Symbol (toReal))] .
  op toInteger : Real -> Integer [special (id-hook SMT_Symbol (toInteger))] .
  op isInteger : Real -> Boolean [special (id-hook SMT_Symbol (isInteger))] .
endfm
```

Conversion Between `RAT` and `REAL`
-----------------------------------

```maude
fmod RAT-REAL-CONVERSION is
   protecting RAT .
   protecting REAL .
   protecting CONVERSION .

   protecting VARIANT-SET .
   protecting META-LEVEL .

    var NzN : NzNat . var N : Nat . vars NzR NzR' : NzRat . vars R R' : Rat . var RL : Real . var Q : Qid . vars S S' S'' : String .
    var TYPE : Type . var RL? : [Real] .
    var GT : GroundTerm . var C : Constant . var V : Variable . vars T T' : Term . var TL : TermList .

    op errorReal : -> [Real] .
    --------------------------

    op Rat2Real : Rat -> [Real] .
    -----------------------------
   ceq Rat2Real(R) = RL if C := getTerms(variants(upModule('RAT, true), upTerm(R))) /\ RL := downTerm(qid(string(getName(C)) + "/1.Real"), errorReal) .
   ceq Rat2Real(R) = RL if V := upTerm(R)                                           /\ RL := downTerm(qid(string(getName(V)) + ":Real"),   errorReal) .

   ceq Rat2Real(R) = RL? if Q[TL] := upTerm(R) /\ not (Q[TL] :: GroundTerm) /\ RL? := downTerm(Q[#Rat2Real(TL)], errorReal) .

   ceq Rat2Real(NzN) = RL if RL := downTerm(qid(string(NzN, 10) + "/1.Real"), errorReal) .

    eq Rat2Real(NzR + NzR') = Rat2Real(NzR) + Rat2Real(NzR') [variant] .
    eq Rat2Real(NzR * NzR') = Rat2Real(NzR) * Rat2Real(NzR') [variant] .

    eq Rat2Real(- R)    = - Rat2Real(R)              [variant] .

    op #Rat2Real : TermList -> TermList .
    -------------------------------------
   ceq #Rat2Real(T) = upTerm(RL) if R := downTerm(T, errorRat) /\ RL := Rat2Real(R) .

    eq #Rat2Real(empty)         = empty .
    eq #Rat2Real((T , T' , TL)) = #Rat2Real(T) , #Rat2Real(T') , #Rat2Real(TL) .
    -----------------------------
---(
    eq #Rat2Real(empty)         = empty .
    eq #Rat2Real((T , T' , TL)) = #Rat2Real(T) , #Rat2Real(T') , #Rat2Real(TL) .

    eq #Rat2Real('_+_[T])         = T .
    eq #Rat2Real('_+_[T, T', TL]) = '_+_[#Rat2Real(T), #Rat2Real('_+_[T', TL])] .

    eq #Rat2Real(Q[TL]) = if isSuccSymbol(Q)
                           then '_+_[SuccSymbol2Real(Q), #Rat2Real(TL)]
                           else Q[#Rat2Real(TL)]
                          fi .

    eq #Rat2Real(V) = qid(string(getName(V)) + ":Real") .
    eq #Rat2Real(C) = if getType(C) == 'Boolean
                        then C
                        else qid(string(getName(C)) + "/1" + ".Real")
                      fi .
)---

    op isSuccSymbol : Qid -> Bool .
    -------------------------------
    eq isSuccSymbol(Q) = substr(string(Q), 0, 2) == "s_" .

    op SuccSymbol2Real : Qid -> Qid .
    ---------------------------------
    eq SuccSymbol2Real('s_) = '1/1.Real .
   ceq SuccSymbol2Real(Q)   = qid(substr(S, 3, N) + "/1.Real")
                           if Q =/= 's_
                           /\ S := string(Q)
                           /\ N := length(S) - 3 .

    op errorRat : -> [Rat] .
    ------------------------

    op Real2Rat : Real -> [Rat] .
    -----------------------------
   ceq Real2Rat(RL) = R if T  := upTerm(RL)
                        /\ T' := #Real2Rat(T)
                        /\ R  := downTerm(T', errorRat) .

    op #Real2Rat : Term -> Term .
    -----------------------------
    eq #Real2Rat(empty)         = empty .
    eq #Real2Rat((T , T' , TL)) = #Real2Rat(T) , #Real2Rat(T') , #Real2Rat(TL) .

    eq #Real2Rat(Q[TL]) = Q[#Real2Rat(TL)] .

    eq #Real2Rat(V) = qid(string(getName(V)) + ":Rat") .
   ceq #Real2Rat(C) = qid(S' + "/" + S'' + ".Real")
                   if S   := string(getName(C))
                   /\ N   := find(S, "/", length(S))
                   /\ S'  := substr(S, 0, N)
                   /\ S'' := substr(S, N + 1, length(S) - (N + 1)) .
endfm
```

Simplifications of `REAL` via `RAT`
-----------------------------------

```maude
fmod REAL-SIMPLIFICATION is
   protecting RAT-REAL-CONVERSION .

    vars RL RL' : Real .

    eq RL + 0/1    = RL .
    eq RL + (- RL) = RL .
    ---------------------

    eq RL - RL' = RL + (- RL') .
    ----------------------------

    eq RL * 0/1 = 0/1 .
    eq RL * 1/1 = RL  .
    -------------------

    eq 0/1 / RL  = 0/1 .
    eq RL  / 1/1 = RL  .
    --------------------
endfm

```
