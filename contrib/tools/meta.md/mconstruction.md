Module Constructions
====================

Module constructions are a pair of a theory `FTH` and a parametized module `FMOD{X :: FTH}` with infrastructure to create a view from `FTH` to `MOD` using purely matching.
If so, then the resulting substitution is used to instantiate `FMOD{X :: FTH}`, and the resulting module is `MOD + FMOD{X :: FTH}`.
For this, we'll heavily use the machinery of `MODULE-TEMPLATE`.

```maude
load mtemplate.maude

fmod MODULE-CONSTRUCTION is
    protecting MODULE-TEMPLATE .

    sorts NeModuleConstruction ModuleConstruction .
    -----------------------------------------------
    subsort NeModuleConstruction < ModuleConstruction .

    vars SU SU' : Substitution . var SUBSTS : SubstitutionSet . var MOD : Module . var ME : ModuleExpression .
    vars MDS MDS' MDS'' : ModuleDeclSet . vars MTS MTS' : ModuleTemplateSet . vars MC MC' MC'' : ModuleConstruction . vars NeMC NeMC' : NeModuleConstruction .
    vars S S' S'' F F' FX' FY' NeF : Sort . vars SS SS' : SortSet . vars NeFS NeFS' : NeSortSet . vars OP Nil Q : Qid . var AS : AttrSet . var NES : Variable .
    vars NeMTS NeMTS' : NeModuleTemplateSet . var SPS : SortPoset . var SDS : SortDeclSet . var SSDS : SubsortDeclSet . vars X Y Z X' Y' TH TH' : Sort . vars T T' : Term .
```

Primitive Constructions
-----------------------

A module construction is either just a declaration of existence (the empty theory matches anything), or the pair of the theory and the parametric module.
`.ModuleConstruction` is the empty module construction.

```maude
    op        exists_ : ModuleTemplateSet                   -> NeModuleConstruction [     prec 75] .
    op forall_exists_ : ModuleTemplateSet ModuleTemplateSet -> NeModuleConstruction [ctor prec 75] .
    ------------------------------------------------------------------------------------------------
    eq exists MTS = forall none exists MTS .
    eq forall MTS exists NeMTS | NeMTS' = (forall MTS exists NeMTS) | (forall MTS exists NeMTS) .

    op .ModuleConstruction : -> ModuleConstruction [ctor] .
    -------------------------------------------------------
    eq forall MDS MDS' exists MDS = .ModuleConstruction .
```

`_;_` gives sequential composition of module constructions, and `_|_` gives parallel.

```maude
    op _;_ : ModuleConstruction   ModuleConstruction ->   ModuleConstruction [ctor assoc      id: .ModuleConstruction prec 76 format(d n d d)] .
    op _;_ : ModuleConstruction NeModuleConstruction -> NeModuleConstruction [ctor assoc      id: .ModuleConstruction prec 76 format(d n d d)] .
    op _|_ : ModuleConstruction   ModuleConstruction ->   ModuleConstruction [ctor assoc comm id: .ModuleConstruction prec 77 format(d n d d)] .
    op _|_ : ModuleConstruction NeModuleConstruction -> NeModuleConstruction [ctor assoc comm id: .ModuleConstruction prec 77 format(d n d d)] .
    ----------------------------------------------------------------------------------------------------------------------------------------
    eq NeMC | NeMC = NeMC .
```

Operator `_deriving_` applies a module construction to a module.

```maude
    op _deriving_ : ModuleDeclSet ModuleConstruction -> ModuleDeclSet [right id: .ModuleConstruction prec 76] .
    -----------------------------------------------------------------------------------------------------------
   ceq MDS deriving forall MTS exists MDS'           = ++(MDS | (MDS' << SUBSTS))                          if SUBSTS := match MTS with MDS .
   ceq MDS deriving forall MTS exists (MDS' \ NeMTS) = ++(MDS | not-instance-of?((MDS' << SUBSTS), NeMTS)) if SUBSTS := match MTS with MDS .
    eq MDS deriving (NeMC ; NeMC')                   = (MDS deriving NeMC) deriving NeMC' .
    eq MDS deriving (NeMC | NeMC')                   = (MDS deriving NeMC) (MDS deriving NeMC') .

    op _deriving_ : ModuleExpression ModuleConstruction -> ModuleExpression [right id: .ModuleConstruction prec 80] .
    -----------------------------------------------------------------------------------------------------------------
    eq #upModule(ME deriving NeMC) = #upModule(ME) deriving NeMC .

    op _deriving_ : Module ModuleConstruction -> [Module] [right id: .ModuleConstruction prec 80] .
    -----------------------------------------------------------------------------------------------
    eq MOD deriving NeMC = fromTemplate(getName(MOD), asTemplate(MOD) deriving NeMC) .
```

`#upModule` defaults to passing `false` to `upModule`.

**TODO**: Move this to the prelude, and name `upModule`.

```maude
    op #upModule : ModuleExpression -> Module [memo] .
    --------------------------------------------------
    eq #upModule(ME) = upModule(ME, false) [owise] .
```

Operators Over Constructions
----------------------------

Module constructions can be instantiated further with substitutions using `_<<_`.

**TODO**: It feels strange to also generate `forall MTS exists MDS` in the first equation.

```maude
    op _<<_ : ModuleConstruction SubstitutionSet -> ModuleConstruction [right id: empty] .
    --------------------------------------------------------------------------------------
    eq (forall MTS exists MDS) << SU = (forall (MTS << SU) exists (MDS << SU)) | (forall MTS exists MDS) .

    eq NeMC << (SU | SU' | SUBSTS) = (NeMC << SU) | (NeMC << SU') | (NeMC << SUBSTS) .

    eq .ModuleConstruction << SU = .ModuleConstruction .
    eq (NeMC | NeMC')      << SU = (NeMC << SU) | (NeMC' << SU) .
    eq (NeMC ; NeMC')      << SU = (NeMC << SU) ; (NeMC' << SU) .
```

`for_in__` allows building the same structure for many different sorts at a time.

```maude
    op for_in__ : Sort SortDeclSet ModuleConstruction -> ModuleConstruction [prec 76] .
    -----------------------------------------------------------------------------------
   ceq for S in ( sorts S' . ) (forall MTS exists MDS) = forall (MTS << SU) exists (MDS << SU) if SU := upTerm(S) <- upTerm(S') .

    eq for S in SDS (NeMC ; NeMC') = (for S in SDS NeMC) ; (for S in SDS NeMC') .
    eq for S in SDS (NeMC | NeMC') = (for S in SDS NeMC) | (for S in SDS NeMC') .

    eq for S in none                      MC = .ModuleConstruction .
    eq for S in ( sorts S' ; S'' ; SS . ) MC = (for S in ( sorts S' . ) MC) | (for S in ( sorts S'' . ) MC) | (for S in ( sorts SS . ) MC) .
```

Operator `univ_` turns a `ModuleDeclSet` into a module constuction parametric over its sorts.

**TODO**: Are `univ_` and `_over_` used anywhere/are they useful yet?

```maude
    op univ_ : ModuleDeclSet -> ModuleConstruction [prec 75] .
    ----------------------------------------------------------
    eq univ MDS = forall ( sorts fv<Sort>(MDS) . ) exists MDS .
```

```
    op _over_ : ModuleConstruction ModuleTemplate -> ModuleConstruction [prec 76] .
    -------------------------------------------------------------------------------
    eq (forall MT exists MT' | MC) over MT'' = (forall MT exists MT' over MT'') | (MC over MT'') .
    eq (forall MT exists MT' ; MC) over MT'' = (forall MT exists MT' over MT'') ; (MC over MT'') .
    eq forall MT exists MT' over MT'' = forall (MT \ (( sorts none . ) \ MT'')) exists MT' .
```

Functor Constructions
---------------------

### Functors

Covariant constructions copy sort structure and preserve the subsort relation.
In general, you may want to just copy the sort structure with a new name, or copy the sort structure and then make the copies subsorts (or supersorts) of the original.

**TODO**: Lots of common structure between `FUNCTOR`, `COFUNCTOR`, and `PROFUNCTOR`.
          Could we possible factor out the part dealing with the `SortPoset`?

`FUNCTOR` copies the sort and subsort structure without linking the original and new sort structures.
The first sort serves as a template for which sorts to copy, and the secord sort serves as a template for the sort to build (these templates can themselves be structured sorts).

```maude
    op FUNCTOR : Sort SortPoset -> ModuleConstruction .
    ---------------------------------------------------
    eq FUNCTOR(X, none)                 = .ModuleConstruction .
    eq FUNCTOR(X, NeFS ; NeFS')         = FUNCTOR(X, NeFS) | FUNCTOR(X, NeFS') .
    eq FUNCTOR(X, (NeFS < NeFS' < SPS)) = ( FUNCTOR(X, NeFS) | FUNCTOR(X, (NeFS' < SPS)) )
                                        ; forall ( sorts NeFS ; NeFS' . )
                                          exists ( subsorts NeFS < NeFS' . ) .

   ceq FUNCTOR(X, F) = forall ( sorts X . )
                       exists ( sorts F . )
                     ; forall ( sorts X ; X' ; F ; F' . )
                              ( subsort X < X' . )
                       exists ( subsort F < F' . )
                    if X' := prime(X)
                    /\ F' := prime(X, F) .
```

`COFUNCTOR` are contravariant in the subsort relation.
Their construction is similar to functors.

```maude
    op COFUNCTOR : Sort SortPoset -> ModuleConstruction .
    -----------------------------------------------------
    eq COFUNCTOR(X, none)                 = .ModuleConstruction .
    eq COFUNCTOR(X, NeFS ; NeFS')         = COFUNCTOR(X, NeFS) | COFUNCTOR(X, NeFS') .
    eq COFUNCTOR(X, (NeFS < NeFS' < SPS)) = ( COFUNCTOR(X, NeFS) | COFUNCTOR(X, (NeFS' < SPS)) )
                                          ; forall ( sorts NeFS ; NeFS' . )
                                            exists ( subsorts NeFS < NeFS' . ) .

   ceq COFUNCTOR(X, F) = forall ( sorts X . )
                         exists ( sorts F . )
                       ; forall ( sorts X ; X' ; F ; F' . )
                                ( subsort X  < X' . )
                         exists ( subsort F' < F  . )
                      if X' := prime(X)
                      /\ F' := prime(X, F) .
```

A `PROFUNCTOR` is covariant in one argument and contravariant in the other.

```maude
    op PROFUNCTOR : Sort Sort SortPoset -> ModuleConstruction .
    -----------------------------------------------------------
    eq PROFUNCTOR(X, Y, none)                 = .ModuleConstruction .
    eq PROFUNCTOR(X, Y, NeFS ; NeFS')         = PROFUNCTOR(X, Y, NeFS) | PROFUNCTOR(X, Y, NeFS') .
    eq PROFUNCTOR(X, Y, (NeFS < NeFS' < SPS)) = ( PROFUNCTOR(X, Y, NeFS) | PROFUNCTOR(X, Y, (NeFS' < SPS)) )
                                              ; forall ( sorts NeFS ; NeFS' . )
                                                exists ( subsorts NeFS < NeFS' . ) .

   ceq PROFUNCTOR(X, Y, F) = forall ( sorts X ; Y . )
                             exists ( sorts F . )
                           ; ( forall ( sorts X ; X' ; F ; FX' . )
                                      ( subsort X <  X' . )
                               exists ( subsort F < FX' . )
                             | forall ( sorts Y ; Y' ; F ; FY' . )
                                      ( subsort  Y  < Y' . )
                               exists ( subsort FY' < F  . )
                             )
                          if X'  := prime(X)    /\ Y'  := prime(Y)
                          /\ FX' := prime(X, F) /\ FY' := prime(Y, F) .
```

### Useful Functor Constructions

A common idiom is to create the subsort-chain `X < NeF{X} < F{X}`, where `F` is some data-structure and `NeF` is the non-empty version.
These data-structures are often over binary operators with given axioms.
Here, `BINARY-DATA` uses `FUNCTOR` to build the desired sort structure, then the appropriate binary operators with unit are declared.

```maude
    op BINARY-DATA : Sort Qid AttrSet -> ModuleConstruction .
    ---------------------------------------------------------
   ceq BINARY-DATA(F, OP, AS) = FUNCTOR(X, (X < NeF{X} < F{X}))
                              ; forall ( sorts    X ; NeF{X} ; F{X} . )
                                       ( subsorts X < NeF{X} < F{X} . )
                                exists ( op Nil : nil             ->   F{X} [ctor] .
                                         op OP  : (F{X})   (F{X}) ->   F{X} [ctor id(const(Nil, F{X})) AS] .
                                         op OP  : (F{X}) (NeF{X}) -> NeF{X} [ctor id(const(Nil, F{X})) AS] .
                                       )
                             if X   := var<Sort>('X)
                             /\ NeF := qid("Ne" + string(F))
                             /\ Nil := qid("."  + string(F)) .
```

Here, some useful module constructions for the list and set data-types are provided:

**TODO**: Add `TUPLE : Nat -> ModuleConstruction` declaring all tuples up to specified size.

-   `LIST : -> ModuleConstruction` constructs the sorts `X < NeList{X} < List{X}` for every `X` in the module, using `_,_` as the associative constructor and `.List` as the empty list.
-   `MSET : -> ModuleConstruction` constructs the sorts `X < NeMSet{X} < MSet{X}` for every `X` in the module, using `_;_` as the associative-commutative constructor and `.MSet` as the empty set.
-   `SET : -> ModuleConstruction` constructs the sorts `X < NeSet{X} < Set{X}` for every `X` in the module, using `_;_` as the associative-commutative constructor and `.Set` as the empty set.
    Additionally, the set-idempotency equation is added.

```maude
   ops LIST MSET SET : -> ModuleConstruction .
   -------------------------------------------
    eq LIST = BINARY-DATA('List, '_`,_, assoc) .
    eq MSET = BINARY-DATA('MSet, '_;_, assoc comm) .
   ceq SET  = BINARY-DATA('Set, '_;_, assoc comm)
            ; forall ( sorts 'NeSet{var<Sort>('X)} . )
              exists ( eq '_;_[NES, NES] = NES [none] . )
           if NES := var('NeS, 'NeSet{var<Sort>('X)}) .
```

**TODO**: Signature refinement isn't used anywhere and it's not clear that it's nice/good.

Signature refinements add subsorts to every sort (adding the annotation that they are refined), as well as copying operators over only the refined sorts down.
Right now only operators up to arity 2 are handled (until we have a better way to generate them).

```maude
    op signature-refinement : Qid -> ModuleConstruction .
    -----------------------------------------------------
    eq signature-refinement(Q) = FUNCTOR(var<Sort>('X), (var<Sort>('X){Q} < var<Sort>('X)))
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

Meta Theory Constructions
-------------------------

**TODO**: Fix, finish, and test these.
**TODO**: Is it the case that any of these are useful?

`ModuleConstruction*` is for module constructions which must know the header of the module they are modifying.
`_<_>` allows building module constructions which need to know the module header (as many meta-theoretic constructions do).

```maude
    sort ModModuleConstruction .
    ----------------------------
    var MMC : ModModuleConstruction .

    op _<_> : ModModuleConstruction Qid -> ModuleConstruction .
    -----------------------------------------------------------

    op _deriving_ : ModuleExpression ModModuleConstruction -> ModuleExpression [prec 80] .
    --------------------------------------------------------------------------------------
    eq ME deriving MMC = ME deriving (MMC < ME >) .

    op _deriving_ : Module ModModuleConstruction -> [Module] [prec 80] .
    --------------------------------------------------------------------
    eq MOD deriving MMC = MOD deriving (MMC < getName(MOD) >) .
```

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
                         ; FUNCTOR(X, (X < X ?))
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
                         ; FUNCTOR(TH{Q}, TH{X @ Q})
                         ; FUNCTOR(X, TH{X @ Q})
                      if SDS SSDS := connected-component(asTemplate('META-TERM), ( sorts 'Term . ))
                      /\ X        := var<Sort>('X)
                      /\ Y        := var<Sort>('Y)
                      /\ TH       := var<Sort>('TH) .

    op tmp-mb : Sort Sort Qid -> ModuleConstruction .
    -------------------------------------------------
    eq tmp-mb(S, S', Q) = forall ( sorts S ; S ? ; S'{Q @ S} . )
                          exists ( cmb-pred( S' , S'{Q @ S} , 'wellFormed < S > ) ) .

    op META-THEORY : -> ModModuleConstruction .
    -------------------------------------------
   ceq META-THEORY < Q > =   ( DOWN-TERM < Q > | META-TERM < Q > )
                           ; tmp-mb(X, 'Constant,   Q)
                           ; tmp-mb(X, 'GroundTerm, Q)
                           ; tmp-mb(X, 'Variable,   Q)
                           ; tmp-mb(X, 'Term,       Q)
                        if X := var<Sort>('X) .
endfm
```
