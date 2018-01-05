Module Constructions
====================

Module constructions are a pair of a theory `FTH` and a parametized module `FMOD{X :: FTH}` with infrastructure to create a view from `FTH` to `MOD` using purely matching.
If so, then the resulting substitution is used to instantiate `FMOD{X :: FTH}`, and the resulting module is `MOD + FMOD{X :: FTH}`.
For this, we'll heavily use the machinery of `MODULE-TEMPLATE`.

```maude
load mtemplate.maude

fmod MODULE-CONSTRUCTION is
    protecting MODULE-TEMPLATE .

    sorts ModuleConstruction ModModuleConstruction .
    ------------------------------------------------
    subsort ModModuleConstruction < ModuleConstruction .

    vars SU SU' : Substitution . var SUBSTS : SubstitutionSet . var MOD : Module .
    vars MDS MDS' MDS'' : ModuleDeclSet . vars MTS MTS' : ModuleTemplateSet . vars MC MC' MC'' : ModuleConstruction .
    vars S S' S'' NeS : Sort . vars SS SS' : SortSet . vars OP Nil Q : Qid . var AS : AttrSet . var NES : Variable .
    vars NeMTS NeMTS' : NeModuleTemplateSet . var SPS : SortPoset . var SDS : SortDeclSet . var SSDS : SubsortDeclSet . vars X Y Z TH TH' : Sort . vars T T' : Term .
```

`_<_>` allows building module constructions which need to know the module header.
`_;_` gives sequential composition of module constructions, and `_|_` gives parallel.

```maude
    op _<_> : ModModuleConstruction Qid -> ModuleConstruction [ctor] .
    ------------------------------------------------------------------

    op _;_ : ModuleConstruction ModuleConstruction -> ModuleConstruction [ctor assoc prec 76 format(d n d d)] .
    op _|_ : ModuleConstruction ModuleConstruction -> ModuleConstruction [ctor assoc comm prec 77 format(d n d d)] .
    ----------------------------------------------------------------------------------------------------------------
    eq (forall MTS exists none) ; MC = MC .
    eq MC | MC = MC .
```

A module construction is either just a declaration of existence (the empty theory matches anything), or the pair of the theory and the parametric module.

```maude
    op        exists_ : ModuleTemplateSet                   -> ModuleConstruction [prec 75] .
    op forall_exists_ : ModuleTemplateSet ModuleTemplateSet -> ModuleConstruction [ctor prec 75] .
    ----------------------------------------------------------------------------------------------
    eq exists MTS = forall none exists MTS .
    eq forall MTS exists NeMTS | NeMTS' = (forall MTS exists NeMTS) | (forall MTS exists NeMTS) .
```

Building many sorts with common subsort structures can be made easier with the parallel `for_in__` operator.

```maude
    op for_in__ : Sort SortDeclSet ModuleConstruction -> ModuleConstruction [prec 76] .
    -----------------------------------------------------------------------------------
   ceq for S in ( sorts S' . ) (forall MTS exists MDS) = forall (MTS << SU) exists (MDS << SU) if SU := upTerm(S) <- upTerm(S') .

    eq for S in SDS (MC ; MC') = (for S in SDS MC) ; (for S in SDS MC') .
    eq for S in SDS (MC | MC') = (for S in SDS MC) | (for S in SDS MC') .

    eq for S in none                      MC = exists none .
    eq for S in ( sorts S' ; S'' ; SS . ) MC = (for S in ( sorts S' . ) MC) | (for S in ( sorts S'' . ) MC) | (for S in ( sorts SS . ) MC) .
```

**TODO**: Fix and uncomment this.

```
    op _over_ : ModuleConstruction ModuleTemplate -> ModuleConstruction [prec 76] .
    -------------------------------------------------------------------------------
    eq (forall MT exists MT' | MC) over MT'' = (forall MT exists MT' over MT'') | (MC over MT'') .
    eq (forall MT exists MT' ; MC) over MT'' = (forall MT exists MT' over MT'') ; (MC over MT'') .
    eq forall MT exists MT' over MT'' = forall (MT \ (( sorts none . ) \ MT'')) exists MT' .
```

Operator `univ_` turns a `ModuleDeclSet` into a module constuction parametric over its sorts.

```maude
    op univ_ : ModuleDeclSet -> ModuleConstruction [prec 75] .
    ----------------------------------------------------------
    eq univ MDS = forall ( sorts fv<Sort>(MDS) . ) exists MDS .
```

Module constructions can be instantiated further with substitutions using `_<<_`.

```maude
    op _<<_ : ModuleConstruction SubstitutionSet -> ModuleConstruction .
    --------------------------------------------------------------------
    eq MC << empty                   = MC .
    eq MC << (SU | SU' | SUBSTS)     = (MC << SU) | (MC << SU') | (MC << SUBSTS) .
    eq (MC | MC') << SU              = (MC << SU) | (MC' << SU) .
    eq (MC ; MC') << SU              = (MC << SU) ; (MC' << SU) .
    eq (forall MTS exists MDS) << SU = (forall (MTS << SU) exists (MDS << SU)) | (forall MTS exists MDS) .
```

Operator `_deriving_` applies a module construction to a module.
Helper `not-instance-of?` helps implement the "away from" construct for building modules.

```maude
    op _deriving_ : ModuleDeclSet ModuleConstruction -> ModuleDeclSet [prec 76] .
    -----------------------------------------------------------------------------
   ceq MDS deriving forall MTS exists MDS'           = ++(MDS | (MDS' << SUBSTS))                          if SUBSTS := match MTS with MDS .
   ceq MDS deriving forall MTS exists (MDS' \ NeMTS) = ++(MDS | not-instance-of?((MDS' << SUBSTS), NeMTS)) if SUBSTS := match MTS with MDS .
    eq MDS deriving (MC ; MC')                       = (MDS deriving MC) deriving MC' .
    eq MDS deriving (MC | MC')                       = (MDS deriving MC) (MDS deriving MC') .

    op not-instance-of? : ModuleTemplateSet ModuleTemplateSet -> ModuleTemplateSet .
    --------------------------------------------------------------------------------
   ceq not-instance-of?(MDS, MTS)              = if SUBSTS == empty then MDS else none fi if SUBSTS := match MTS with MDS .
    eq not-instance-of?((NeMTS | NeMTS'), MTS) = not-instance-of?(NeMTS, MTS) | not-instance-of?(NeMTS', MTS) .
```

Covariant Constructions
-----------------------

Covariant constructions copy sort structure and subsort relations, preserving the ordering.
In general, you may want to just copy the sort structure with a new name, or copy the sort structure and then make the copies subsorts (or supersorts) of the original.

-   `covariant : Sort Sort -> ModuleConstruction` will just copy the sort and subsort structure.
    The first sort serves as a template for which sorts to copy, and the secord sort serves as a template for the sort to build.
-   `covariant-data-up : Sort Sort -> ModuleConstruction` creates a copy of the sort structure and makes the copy super-sorts of the original.
-   `covariant-data-down : Sort Sort -> ModuleConstruction` creates a copy of the sort structure and makes the copy subsorts of the original.

```maude
    op covariant : Sort Sort -> ModuleConstruction .
    ------------------------------------------------
    eq covariant(S, S') = forall ( sorts S . )
                          exists ( sorts S' . )
                        ; forall ( sorts S        ; S'
                                       ; prime(S) ; prime(S') .
                                 )
                                 ( subsort S  < prime(S) . )
                          exists ( subsort S' < prime(S') . ) .

    op covariant-data-up : Sort Sort -> ModuleConstruction .
    --------------------------------------------------------
    eq covariant-data-up(S, S') = covariant(S, S')
                                ; forall ( sorts S ; S' . )
                                  exists ( subsort S < S' . ) .

    op covariant-data-down : Sort Sort -> ModuleConstruction .
    ----------------------------------------------------------
    eq covariant-data-down(S, S') = covariant(S, S')
                                  ; forall ( sorts S ; S' . )
                                    exists ( subsort S < S' . ) .
```

A common idiom is to create the subsort-chain `X < NeF{X} < F{X}`, where `F` is some data-structure and `NeF` is the non-empty version.
These data-structures are often over binary operators with given axioms.
Here, `covariant-data-up` is used twice to build the desired sort structure, then the appropriate binary operators with unit are declared.

-   `covariant-data-binary : Sort Qid AttrSet -> ModuleConstruction` makes a covariant data-structure with a binary operator as constructor.

```maude
    op covariant-data-binary : Sort Qid AttrSet -> ModuleConstruction .
    -------------------------------------------------------------------
   ceq covariant-data-binary(S, OP, AS) = covariant-data-up(X, NeS{X})
                                        ; covariant-data-up(NeS{X}, S{X})
                                        ; forall ( sorts    X ; NeS{X} ; S{X} . )
                                                 ( subsorts X < NeS{X} < S{X} . )
                                          exists ( op Nil : nil             ->   S{X} [ctor] .
                                                   op OP  : (S{X})   (S{X}) ->   S{X} [ctor id(const(Nil, S{X})) AS] .
                                                   op OP  : (S{X}) (NeS{X}) -> NeS{X} [ctor id(const(Nil, S{X})) AS] .
                                                 )
                                       if X   := var<Sort>('X)
                                       /\ NeS := qid("Ne" + string(S))
                                       /\ Nil := qid("."  + string(S)) .
```

The module constructions for the list and set data-types are provided here in terms of `covariant-binary-data`.

-   `LIST : -> ModuleConstruction` constructs the sorts `X < NeList{X} < List{X}` for every `X` in the module, using `_,_` as the associative constructor and `.List` as the empty list.
-   `MSET : -> ModuleConstruction` constructs the sorts `X < NeMSet{X} < MSet{X}` for every `X` in the module, using `_;_` as the associative-commutative constructor and `.MSet` as the empty set.
-   `SET : -> ModuleConstruction` constructs the sorts `X < NeSet{X} < Set{X}` for every `X` in the module, using `_;_` as the associative-commutative constructor and `.Set` as the empty set.
    Additionally, the set-idempotency equation is added.

```maude
   ops LIST MSET SET : -> ModuleConstruction .
   -------------------------------------------
    eq LIST = covariant-data-binary('List, '_`,_, assoc) .
    eq MSET = covariant-data-binary('MSet, '_;_, assoc comm) .
   ceq SET  =   covariant-data-binary('Set, '_;_, assoc comm)
              ; forall ( sorts 'NeSet{var<Sort>('X)} . )
                exists ( eq '_;_[NES, NES] = NES [none] . )
           if NES := var('NeS, 'NeSet{var<Sort>('X)}) .
```

Signature Refinement
--------------------

Signature refinements add subsorts to every sort (adding the annotation that they are refined), as well as copying operators over only the refined sorts down.
Right now only operators up to arity 2 are handled (until we have a better way to generate them).

```maude
    op signature-refinement : Qid -> ModuleConstruction .
    -----------------------------------------------------
    eq signature-refinement(Q) = covariant-data-down(var<Sort>('X), var<Sort>('X){Q})
                               ; signature-refinement(0, Q)
                               ; signature-refinement(1, Q)
                               ; signature-refinement(2, Q) .

    op signature-refinement : Nat Qid -> ModuleConstruction .
    ---------------------------------------------------------
   ceq signature-refinement(0, Q) = forall ( sorts X ; X{Q} . )
                                           ( op OP : nil -> X [var<AttrSet>('AS)] . )
                                    exists ( op OP : nil -> X{Q} [var<AttrSet>('AS)] . )
                                 if X  := var<Sort>('X)
                                 /\ OP := var<Qid>('OP) .

   ceq signature-refinement(1, Q) = ( forall ( sorts X ; X{Q} ; Y ; Y{Q}. )
                                             ( op OP : Y -> X [var<AttrSet>('AS)] . )
                                      exists ( op OP : Y{Q} -> X{Q} [var<AttrSet>('AS)] . )
                                    ) << ('X:Sort <- 'Y:Sort)
                                 if X  := var<Sort>('X)
                                 /\ Y  := var<Sort>('Y)
                                 /\ OP := var<Qid>('OP) .

   ceq signature-refinement(2, Q) = ( forall ( sorts X ; X{Q} ; Y ; Y{Q} ; Z ; Z{Q} . )
                                             ( op OP : Y Z -> X [var<AttrSet>('AS)] . )
                                      exists ( op OP : (Y{Q}) (Z{Q}) -> X{Q} [var<AttrSet>('AS)] . )
                                    ) << (('X:Sort <- 'Y:Sort) | ('X:Sort <- 'Z:Sort) | ('Y:Sort <- 'Z:Sort) | ('Y:Sort <- 'X:Sort ; 'Z:Sort <- 'X:Sort))
                                 if X  := var<Sort>('X)
                                 /\ Y  := var<Sort>('Y)
                                 /\ Z  := var<Sort>('Z)
                                 /\ OP := var<Qid>('OP) .
```

Exponential Objects
-------------------

For functional-style programming (including higher-order functional programming), it's necessary to reify arrows between sorts (functions) as objects themselves.
This `EXPONENTIAL` module construction does that.

**TODO**: make identify functions actually behave like identity in composition

```maude
    op EXPONENTIAL : -> ModuleConstruction .
    ----------------------------------------
   ceq EXPONENTIAL =   ( forall ( sorts X ; Y . )
                         exists ( sorts X ==> Y . )
                                ( op '__ : (X ==> Y) X -> Y [none] . )
                       ) << ('Y:Sort <- 'X:Sort)
                     ; forall ( sorts X ==> X . )
                       exists ( op 'id < X > : nil -> X ==> X [ctor] . )
                              ( eq '__[const('id < X >, X ==> X), var('X, X)] = var('X, X) [none] . )
                     ; forall ( sorts X ; Y ; Z ; X ==> Y ; X ==> Z . )
                              ( subsort Y < Z . )
                       exists ( subsort X ==> Y < X ==> Z . )
                     ; forall ( sorts X ; Y ; Z ; X ==> Y ; Z ==> Y . )
                              ( subsort Z < X . )
                       exists ( subsort X ==> Y < Z ==> Y . )
                     ; forall ( sorts X ==> Y ; Y ==> Z ; X ==> Z . )
                       exists ( op '_._ : (Y ==> Z) (X ==> Y) -> X ==> Z [none] .
                                op '_;_ : (X ==> Y) (Y ==> Z) -> X ==> Z [none] .
                              )
                  if X := var<Sort>('X)
                  /\ Y := var<Sort>('Y)
                  /\ Z := var<Sort>('Z) .
```

Meta Theory
-----------

Sometimes you want a "meta theory" specific to your module.
For example, you may want to express using just a sort that a meta-level `Term` is a well-formed term in your theory, or that it is a well-formed term of a specific sort in your theory.
This module construction will extend your module with its meta-theory, so that this can be done.
This is a construction that refines the module `META-TERM` and adds it to your module, as well as adding in conditional memberships which push elements of the sort `Term` into the subsort of `Term{MYMOD}` when appropriate.

```maude
    op cmb-pred : Sort Sort Qid -> MembAx .
    ---------------------------------------
    eq cmb-pred(S, S', Q) = ( cmb var('X, S) : S' if Q[var('X, S)] = 'true.Bool [none] . ) .

    op top-sorts : Sort -> ModuleTemplateSet .
    ------------------------------------------
    eq top-sorts(S) = (sorts S .) \ ((sorts S ; prime(S) .) subsort S < prime(S) .) .

    op sort-intersect : Sort -> ModuleConstruction .
    ------------------------------------------------
   ceq sort-intersect(S) = forall ( sorts S{X} ; S{Y} . )
                           exists ( sorts S{X Y} . )
                                  ( subsorts S{X Y} < S{X} ; S{Y} . )
                                  ( cmb var('T, S{X}) : S{X Y} if var('T, S{X}) : S{Y} [none] .
                                    cmb var('T, S{Y}) : S{X Y} if var('T, S{Y}) : S{X} [none] .
                                  )
                        if X := var<Sort>('X)
                        /\ Y := var<Sort>('Y) .

    op DOWN-TERM : -> ModModuleConstruction .
    -----------------------------------------
   ceq DOWN-TERM < Q > =   exists ( pr 'META-LEVEL .
                                    pr Q .
                                  )
                         ; covariant-data-up(X, X ?)
                         ; forall ( sorts X ; X ? . )
                                  ( subsort X < X ? . )
                           exists ( op 'downTermError < X > : nil   -> X ?   [ctor] .
                                    op 'downTerm      < X > : 'Term -> X ?   [none] .
                                    op 'wellFormed    < X > : 'Term -> 'Bool [none] .
                                  )
                                  ( ceq 'downTerm   < X > [var('T, 'Term)] = var('X, X) if var('X, X) := 'downTerm[var('T, 'Term), const('downTermError < X >, X ?)] [none] .
                                    ceq 'wellFormed < X > [var('T, 'Term)] = 'true.Bool if var('X, X) := 'downTerm < X > [var('T, 'Term)] [none] .
                                  )
                      if X := var<Sort>('X) .

    op META-TERM : -> ModModuleConstruction .
    -----------------------------------------
   ceq META-TERM < Q > =   DOWN-TERM < Q >
                         ; exists tag-sorts(Q, (SDS SSDS))
                         ; for TH in SDS ( exists ( subsort TH{Q} < TH . )
                                         ; forall ( sorts X ; X ? . ) exists ( sorts TH{X @ Q} . )
                                         ; forall top-sorts(X ?)      exists ( subsort TH{X @ Q} < TH{Q} . )
                                         )
                         ; covariant(TH{Q}, TH{X @ Q})
                         ; covariant(X, TH{X @ Q})
                      if SDS SSDS := connected-component(asTemplate('META-TERM), ( sorts 'Term . ))
                      /\ X        := var<Sort>('X)
                      /\ Y        := var<Sort>('Y)
                      /\ TH       := var<Sort>('TH) .
endfm
```

Module Expressions
==================

This module simply provides the hookup to our extension of the Meta-Level module expressions.
A memo-ised version of `upModule` is provided too for the extensions.
In addition, free constructions are provided.
Module constructions provide a module template with variables to determine anonymous views, and a second module template to determine the associated parameterized module.

```maude
fmod MODULE-EXPRESSION is
    extending MODULE-CONSTRUCTION .

    var MOD : Module . var ME : ModuleExpression . var SUBSTS : SubstitutionSet .
    vars MC MC' : ModuleConstruction . var MMC : ModModuleConstruction .
    var MDS : ModuleDeclSet . var MTS : ModuleTemplateSet . var Q : Qid . vars S S' : Sort . var SS : SortSet .
    vars X Y T : Sort .

    op #upModule : ModuleExpression -> Module [memo] .
    --------------------------------------------------
    eq #upModule(ME) = upModule(ME, false) [owise] .

    op _deriving_ : ModuleExpression ModuleConstruction -> ModuleExpression [prec 80] .
    -----------------------------------------------------------------------------------
    eq #upModule(ME deriving MC) = #upModule(ME) deriving MC .

---    op META-TERM : -> ModModuleConstruction .
---    --------------------------------------------
---    ceq META-TERM < Q > =   exists ( pr 'META-LEVEL . )
---                          ; forall ( sorts X . )
---                            exists asTemplate(#upModule('META-TERM deriving ( covariant-data-down(T, T{Q})
---                                                                            ; covariant(T{Q}, T{Q @ X})
---                                                                            )
---                                                       )
---                                             )
---                          ; forall ( sorts X ; T{Q @ X}
---                                         ; Y ; T{Q @ Y} .
---                                   )
---                                   ( subsort X < Y . )
---                            exists ( subsort T{Q @ X} < T{Q @ Y} . )
---                         ;  forall ( sorts X ; T{Q @ X} ; T{Q} . )
---                                 \ ( sorts X ; T{Q @ X}
---                                         ; Y ; T{Q @ Y} .
---                                   )
---                                   ( subsort X < Y .
---                                     subsort T{Q @ X} < T{Q @ X} .
---                                   )
---                            exists ( subsort T{Q @ X} < T{Q} . )
---                        if X := var<Sort>('X)
---                        /\ Y := var<Sort>('Y)
---                        /\ T := var<Sort>('T) .

    op tmp-mb : Sort Sort Qid -> ModuleConstruction .
    -------------------------------------------------
    eq tmp-mb(S, S', Q) = forall ( sorts S ; S ? ; S'{Q @ S} . )
                          exists ( cmb-pred( S' , S'{Q @ S} , 'wellFormed < S > ) ) .

    op META-THEORY : -> ModModuleConstruction .
    -------------------------------------------
   ceq META-THEORY < Q > =   ( DOWN-TERM | META-TERM < Q > )
                           ; tmp-mb(X, 'Constant,   Q)
                           ; tmp-mb(X, 'GroundTerm, Q)
                           ; tmp-mb(X, 'Variable,   Q)
                           ; tmp-mb(X, 'Term,       Q)
                        if X := var<Sort>('X) .

---    vars M1 M2          : ModuleExpression .
---    vars TM TM' TM1 TM2 : Variable .
---    op PURIFICATION : ModuleExpression ModuleExpression -> ModuleConstruction .
---    ------------------------------------------------------------------------------
---    ceq PURIFICATION(M1, M2)
---      =   META-THEORY < M1 >
---        ; META-THEORY < M2 >
---        ; exists ( extending 'FOFORM .
---                   extending M1 .
---                   extending M2 . )
---                 ( sorts none . )
---                 ( op 'purify : 'EqAtom -> 'EqConj [none] .
---                   op 'fvar   : 'Term 'Term -> 'Variable [none] . )
---        ; forall ( sorts 'Term ; 'Term{svar('M)} . )
---                 ( subsort 'Term{svar('M)} < 'Term . )
---          exists ( sorts none . )
---                 ( eq 'purify['_?=_[TM, TM']] = '_?=_[TM, TM'] [none] . )
---        ; forall ( sorts 'Term ; 'Term{svar('M1)} ; 'Term{svar('M2)} . )
---                 ( subsorts 'Term{svar('M1)} ; 'Term{svar('M2)} < 'Term . )
---          exists ( sorts none . )
---                 ( eq 'purify['_?=_[TM1, TM2]] = '_/\_['_?=_['fvar[TM1, TM2], TM1], '_?=_['fvar[TM1, TM2], TM2]] [none] . )
---
---      if TM  := var('TM,  'Term{svar('M)})
---      /\ TM' := var('TM', 'Term{svar('M)})
---      /\ TM1 := var('TM1, 'Term{svar('M1)})
---      /\ TM2 := var('TM2, 'Term{svar('M2)}) .

    op _deriving_ : Module ModuleConstruction -> [Module] [prec 80] .
    -----------------------------------------------------------------
    eq MOD deriving MC = fromTemplate(getName(MOD), asTemplate(MOD) deriving MC) .
endfm
```
