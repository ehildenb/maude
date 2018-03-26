Module Templates
================

Module templates serve as a slightly more flexible data-structure than the ones provided in the prelude for expressing syntactic theories.
These depend on `STRUCTURED-NAMES` for easy matching, and on `SUBSTITUTIONSET`.

```maude
load structured-names.maude
```

Module Declarations
-------------------

Module declarations include all of the individual declarations defined in the prelude, but extend them by adding a common supersort `ModuleDecl`, as well as a common subsort for each declaration set `NullDeclSet`.
Additionally, extra sorts are added for the non-empty versions of each declaration (for preregularity).

```maude
fmod MODULE-DECLARATION is
   protecting STRUCTURED-NAME .
   protecting SUBSTITUTIONSET .
   protecting META-LEVEL-EXT .

    sorts ImportDecl ImportDeclSet .
    --------------------------------
    subsort ImportDecl < ImportDeclSet .

    sorts SortPoset SortDecl SortDeclSet .
    --------------------------------------
    subsort SortSet < SortPoset .
    subsort SortDecl < SortDeclSet .

    sorts NeImportDeclSet NeSortDeclSet NeSubsortDeclSet NeOpDeclSet NeMembAxSet NeEquationSet NeRuleSet .
    ------------------------------------------------------------------------------------------------------
    subsorts ImportDecl  < NeImportDeclSet  < ImportDeclSet .
    subsorts SortDecl    < NeSortDeclSet    < SortDeclSet .
    subsorts SubsortDecl < NeSubsortDeclSet < SubsortDeclSet .
    subsorts OpDecl      < NeOpDeclSet      < OpDeclSet .
    subsorts MembAx      < NeMembAxSet      < MembAxSet .
    subsorts Equation    < NeEquationSet    < EquationSet .
    subsorts Rule        < NeRuleSet        < RuleSet .

    sorts ModuleDecl NeModuleDeclSet ModuleDeclSet NullDeclSet .
    ------------------------------------------------------------
    subsorts ModuleDecl < NeModuleDeclSet < ModuleDeclSet .
    subsorts               ImportDecl      SortDecl      SubsortDecl      OpDecl      MembAx      Equation      Rule    <   ModuleDecl .
    subsorts NullDeclSet < ImportDeclSet   SortDeclSet   SubsortDeclSet   OpDeclSet   MembAxSet   EquationSet   RuleSet <   ModuleDeclSet .
    subsorts             NeImportDeclSet NeSortDeclSet NeSubsortDeclSet NeOpDeclSet NeMembAxSet NeEquationSet NeRuleSet < NeModuleDeclSet .

    var Q : Qid . var ME : ModuleExpression .
    vars I I' : Import . var IL : ImportList . vars ID : ImportDecl . var NeIDS : NeImportDeclSet . var IDS : ImportDeclSet .
    vars S S' S'' : Sort . vars SS SS' : SortSet . var SPS : SortPoset .
    var SU : Substitution . var SUBSTS : SubstitutionSet .
    vars NeMDS NeMDS' : NeModuleDeclSet . vars MDS MDS' : ModuleDeclSet .

    op __ : ImportDeclSet  NeImportDeclSet  -> NeImportDeclSet  [ctor ditto] .
    op __ : SortDeclSet    NeSortDeclSet    -> NeSortDeclSet    [ctor ditto] .
    op __ : SubsortDeclSet NeSubsortDeclSet -> NeSubsortDeclSet [ctor ditto] .
    op __ : OpDeclSet      NeOpDeclSet      -> NeOpDeclSet      [ctor ditto] .
    op __ : MembAxSet      NeMembAxSet      -> NeMembAxSet      [ctor ditto] .
    op __ : EquationSet    NeEquationSet    -> NeEquationSet    [ctor ditto] .
    op __ : RuleSet        NeRuleSet        -> NeRuleSet        [ctor ditto] .
    --------------------------------------------------------------------------

    op none : -> NullDeclSet [ctor] .
    op __ : NullDeclSet NullDeclSet -> NullDeclSet [ctor ditto] .
    -------------------------------------------------------------

    op __ : ModuleDeclSet   ModuleDeclSet ->   ModuleDeclSet [ctor ditto] .
    op __ : ModuleDeclSet NeModuleDeclSet -> NeModuleDeclSet [ctor ditto] .
    -----------------------------------------------------------------------
    eq NeMDS NeMDS = NeMDS .
```

Using the syntax of the prelude, imports and sort declarations are treated specially.
Here we add the notion of `ImportDecl` and `SortDecl`, so that they can be treated uniformly with the rest of the declarations.

```maude
   ops (pr_.) (ex_.) (in_.) : ModuleExpression -> ImportDecl .
    op __ : ImportDeclSet ImportDeclSet -> ImportDeclSet [ctor ditto] .
    -------------------------------------------------------------------
    eq ( pr ME . ) ( ex ME . ) = ( ex ME . ) .
    eq ( pr ME . ) ( in ME . ) = ( in ME . ) .
    eq ( ex ME . ) ( in ME . ) = ( in ME . ) .

    op importDecls : ImportList -> ImportDeclSet .
    ----------------------------------------------
    eq importDecls(nil)             = none .
    eq importDecls(protecting ME .) = (pr ME .) .
    eq importDecls(extending  ME .) = (ex ME .) .
    eq importDecls(including  ME .) = (in ME .) .
    eq importDecls(I I' IL)         = importDecls(I) importDecls(I') importDecls(IL) .

    op importList : ImportDeclSet -> ImportList .
    ---------------------------------------------
    eq importList(none)     = nil .
    eq importList(pr ME .)  = (protecting ME .) .
    eq importList(ex ME .)  = (extending  ME .) .
    eq importList(in ME .)  = (including  ME .) .
    eq importList(ID NeIDS) = importList(ID) importList(NeIDS) .

    op (sorts_.) : SortSet -> SortDecl [prec 60] .
    op __ : SortDeclSet SortDeclSet -> SortDeclSet [ditto] .
    --------------------------------------------------------
    eq ( sorts none . )               = none .
    eq ( sorts SS . ) ( sorts SS' . ) = ( sorts SS ; SS' . ) .

    op sortSet : SortDeclSet -> SortSet .
    -------------------------------------
    eq sortSet(none)       = none .
    eq sortSet(sorts SS .) = SS .
```

The prelude only allows a single subsort declared at once, which becomes cumbersome.
Here we add `SortPoset` and allow an entire poset to be declared at once.

**TODO**: Rename `SortPoset` => `SortOrder`.

```maude
    op _<_ : SortPoset SortPoset -> SortPoset [assoc id: none prec 122] .
    op (subsorts_.) : SortPoset -> SubsortDeclSet .
    -----------------------------------------------
    eq ( subsorts SS . )     = none .
    eq ( subsorts S < S' . ) = ( subsort S < S' . ) .
    eq ( subsorts S < S' ; S'' ; SS' . )
     = ( subsorts S < S' .
         subsorts S < S'' .
         subsorts S < SS' .
       ) .
    eq ( subsorts S ; S' ; SS < S'' ; SS' . )
     = ( subsorts S  < S'' ; SS' .
         subsorts S' < S'' ; SS' .
         subsorts SS < S'' ; SS' .
       ) .
    eq ( subsorts S ; SS < S' ; SS' < SPS . )
     = ( subsorts S  < S' ; SS' .
         subsorts SS < S' ; SS' .
         subsorts      S' ; SS'  < SPS .
       ) .
```

-   `top-sort : ModuleDeclSet Sort -> [Sort]` gives the top sort of a connected component with the given sort in it.
-   `connected-component : ModuleDeclSet ModuleDeclSet -> ModuleDeclSet` will complete the second module declaration set with all the sorts and subsorts in the intersection of the two connected components.
-   `tag-sorts : Qid ModuleDeclSet -> [ModuleDeclSet]` will take a sort structure (a `ModuleDeclSet` which only consists of `SortDecl` and `SubsortDecl`) and produce a "tagged" version.
    For each sort `X` in the original sort structure it will produce `X{Q}`, for `Q` the supplied `Qid`.

**TODO**: Rework `connected-component` to first grab the subsort structure then flatten the sorts out.
**TODO**: Think about how matching on the `(Ne)ModuleDeclSet` can make this more compact.

```maude
    op top-sort : ModuleDeclSet Sort -> [Sort] .
    --------------------------------------------
    eq top-sort( ( subsort S < S' . ) MDS , S ) = top-sort( MDS , S' ) .
    eq top-sort( MDS , S ) = S [owise] .

    op connected-component : ModuleDeclSet ModuleDeclSet -> [ModuleDeclSet] .
    -------------------------------------------------------------------------
    eq connected-component(MDS, MDS') = MDS' [owise] .
    eq connected-component( ( sorts S ; S' ; SS . ) ( subsort S < S' . ) MDS
                          , ( sorts S ; SS' . ) MDS'
                          )
     = connected-component( ( sorts S ; S' ; SS . ) MDS
                          , ( sorts S ; S' ; SS' . ) ( subsort S < S' . ) MDS'
                          ) .
    eq connected-component( ( sorts S ; S' ; SS . ) ( subsort S' < S . ) MDS
                          , ( sorts S ; SS' . ) MDS'
                          )
     = connected-component( ( sorts S ; S' ; SS . ) MDS
                          , ( sorts S ; S' ; SS' . ) ( subsort S' < S . ) MDS'
                          ) .

    op tag-sorts : Qid ModuleDeclSet -> [ModuleDeclSet] .
    -----------------------------------------------------
    eq tag-sorts(Q, none)                    = none .
    eq tag-sorts(Q, (NeMDS NeMDS'))          = tag-sorts(Q, NeMDS) tag-sorts(Q, NeMDS') .
    eq tag-sorts(Q, ( sorts S . ))           = ( sorts S{Q} . ) .
    eq tag-sorts(Q, ( subsort S < S' . ))    = ( subsort S{Q} < S'{Q} . ) .
    eq tag-sorts(Q, ( sorts S ; S' ; SS . )) = tag-sorts(Q, ( sorts S . ))
                                               tag-sorts(Q, ( sorts S' . ))
                                               tag-sorts(Q, ( sorts SS . )) .
```

-   `_<<_ : ModuleDeclSet Substitution -> [ModuleDeclSet]` lifts the substitution application operator `_<<_` over `ModuleDeclSet`.
-   `match_with_ : ModuleDeclSet ModuleDeclSet -> [SubstitutionSet]` gives all the ways that the first `ModuleDeclSet` matches the second with variable extension.
-   `resolveNames` and `fv<Sort>` are simply the lifting of `resolveNames` and `fv<Sort>` of `STRUCTURED-NAME` to `ModuleDeclSet`.

```maude
    op error<ModuleDeclSet> : -> [ModuleDeclSet] .
    op var<ModuleDeclSet> : Qid -> [ModuleDeclSet] .
    ------------------------------------------------
   ceq var<ModuleDeclSet>(Q) = MDS
    if MDS := downTerm(qid(string(Q) + ":ModuleDeclSet"), error<ModuleDeclSet>) .

    op _<<_ : ModuleDeclSet Substitution -> [ModuleDeclSet] .
    ---------------------------------------------------------
    eq MDS                << empty = (none).NullDeclSet .
    eq (none).NullDeclSet << SU    = none .
   ceq NeMDS              << SU    = NeMDS' if NeMDS' := downTerm(upTerm(NeMDS) << SU, error<ModuleDeclSet>) .

    op match_with_ : ModuleDeclSet ModuleDeclSet -> [SubstitutionSet] .
    -------------------------------------------------------------------
    eq match MDS with none  = empty .
   ceq match MDS with NeMDS = SUBSTS
    if NeMDS' := MDS ( sorts var<SortSet>('##SSCTX##) . ) var<ModuleDeclSet>('##MDSCTX##)
    /\ SUBSTS := #subsumesWith(upModule('MODULE-DECLARATION, true), upTerm(NeMDS'), upTerm(NeMDS)) .

    op resolveNames : ModuleDeclSet -> ModuleDeclSet .
    --------------------------------------------------
    eq resolveNames(IDS)                = IDS .
    eq resolveNames(( sorts SS . ))     = ( sorts resolveNames(SS) . ) .
    eq resolveNames((none).NullDeclSet) = none .
    eq resolveNames(NeMDS NeMDS')       = resolveNames(NeMDS) resolveNames(NeMDS') .

    op fv<Sort> : ModuleDeclSet -> SortSet .
    ----------------------------------------
    eq fv<Sort>(MDS) = #fv<Sort>(upTerm(MDS)) .
endfm
```

Module Templates
----------------

**TODO**: Are module templates actually used/useful?
          They seem to add a lot of complication for (perhaps) little gain.

Module templates serve as more flexible module data-structures than what the prelude provides for modules.
The main functionality exported is:

-   `resolveNames` and `fv<Sort>` are lifted over `ModuleDeclSet`.

```maude
fmod MODULE-TEMPLATE-SET is
   protecting MODULE-DECLARATION .

    vars NeMDS NeMDS' : NeModuleDeclSet . var MDS MDS' : ModuleDeclSet .
    vars SU SU' SU'' : Substitution . var SUBSTS : SubstitutionSet . var B : [Bool] .

    op {_in_|_} : Substitution SubstitutionSet [Bool] -> SubstitutionSet [strat(2 0)] .
    -----------------------------------------------------------------------------------
    eq { SU in empty                 | B } = empty .
    eq { SU in SU'                   | B } = if downTerm(upTerm(B) << (upTerm(SU) <- upTerm(SU')), false) then SU' else empty fi .
    eq { SU in (SU' | SU'' | SUBSTS) | B } = { SU in SU' | B } | { SU in SU'' | B } | { SU in SUBSTS | B } .

    op empty? : SubstitutionSet -> Bool .
    -------------------------------------
    eq empty?(empty)       = true .
    eq empty?(SU | SUBSTS) = false .

    op not-instance-of? : ModuleDeclSet ModuleDeclSet  -> ModuleDeclSet .
    ---------------------------------------------------------------------
   ceq not-instance-of?(MDS, MDS') = if SUBSTS == empty then MDS else none fi
                                  if SUBSTS := match MDS' with MDS .

    op not-instance-with? : ModuleDeclSet ModuleDeclSet SubstitutionSet -> SubstitutionSet .
    ----------------------------------------------------------------------------------------
    eq not-instance-with?(MDS, MDS', empty)               = empty .
    eq not-instance-with?(MDS, MDS', (SU | SU' | SUBSTS)) = not-instance-with?(MDS, MDS', SU) | not-instance-with?(MDS, MDS', SU') | not-instance-with?(MDS, MDS', SUBSTS) .
   ceq not-instance-with?(MDS, MDS', SU)                  = if SUBSTS == empty then SU else empty fi
                                                         if SUBSTS := match (MDS << SU) with MDS' .
endfm
```

Interface to `Module`
---------------------

To actually do execution in the modules generated by a module template, we need functions to convert between `Module` and `ModuleTemplate`.

-   `asTemplate : Module -> ModuleTemplate`, `asTemplate : ModuleExpression -> ModuleTemplate` and `fromTemplate : ModuleTemplate -> Module` provide functions between the normal Maude modules and the module templates defined here.
-   `_++_ : Module ModuleDeclSet -> Module` and `_++_ : Module Module -> Module` are the lifting of operator `_++_` in `MODULE-TEMPLATE-DATA` to work directly on Maude modules.
-   `resolveNames : Module -> Module` resolves the structured names of a module into proper Core Maude names.

```maude
fmod MODULE-TEMPLATE is
   protecting MODULE-TEMPLATE-SET .
   protecting META-LEVEL .

    var H : Header . var IL : ImportList . var SS : SortSet .
    var IS : ImportDeclSet . var SDS : SortDeclSet . var SSDS : SubsortDeclSet . var OPDS : OpDeclSet .
    var MAS : MembAxSet . var EQS : EquationSet . var RLS : RuleSet . var NeRLS : NeRuleSet .
    var MDS : ModuleDeclSet . var NeMDS : NeModuleDeclSet . var ME : ModuleExpression . vars MOD MOD' : Module .

    op asTemplate : ModuleExpression -> [ModuleDeclSet] .
    op asTemplate : Module -> [ModuleDeclSet] .
    -------------------------------------------
    eq asTemplate(ME) = asTemplate(upModule(ME, false)) .
    eq asTemplate(fmod H is IL sorts SS . SSDS OPDS MAS EQS     endfm) = (importDecls(IL) (sorts SS .) SSDS OPDS MAS EQS) .
    eq asTemplate( mod H is IL sorts SS . SSDS OPDS MAS EQS RLS endm ) = (importDecls(IL) (sorts SS .) SSDS OPDS MAS EQS RLS) .

    op fromTemplate : Header ModuleDeclSet -> [Module] .
    ----------------------------------------------------
    eq fromTemplate(H, (IS SDS SSDS OPDS MAS EQS))       = (fmod H is importList(IS) sorts sortSet(SDS) . SSDS OPDS MAS EQS       endfm) .
    eq fromTemplate(H, (IS SDS SSDS OPDS MAS EQS NeRLS)) =  (mod H is importList(IS) sorts sortSet(SDS) . SSDS OPDS MAS EQS NeRLS endm) .

    op _++_ : Module ModuleDeclSet -> [Module] [right id: none prec 72] .
    ---------------------------------------------------------------------
    eq MOD ++ NeMDS = fromTemplate(getName(MOD), (asTemplate(MOD) NeMDS)) .

    op _++_ : Module Module -> [Module] [assoc prec 73] .
    -----------------------------------------------------
    eq MOD ++ MOD' = MOD ++ asTemplate(MOD') .

    op resolveNames : Module -> [Module] .
    --------------------------------------
    eq resolveNames(MOD) = fromTemplate(getName(MOD), resolveNames(asTemplate(MOD))) .
endfm
```
