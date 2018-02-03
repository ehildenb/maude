/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2017 SRI International, Menlo Park, CA 94025, USA.

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

*/

//
//	List of all recognized constructors in metalevel.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, symbols C++ class, required type flags, number of args)
//
     //
     //	META-TERM stuff:
     //
     //	Term constructors.
     //
     MACRO(qidSymbol, QuotedIdentifierSymbol, SymbolType::QUOTED_IDENTIFIER, 0)
     MACRO(metaTermSymbol, FreeSymbol, 0, 2)
     MACRO(metaArgSymbol, Symbol, SymbolType::ASSOC, 2)
     MACRO(emptyTermListSymbol, Symbol, 0, 0)
     //
     //	Substitution and context constructors.
     //
     MACRO(assignmentSymbol, FreeSymbol, 0, 2)
     MACRO(emptySubstitutionSymbol, Symbol, 0, 0)
     MACRO(substitutionSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     MACRO(holeSymbol, Symbol, 0, 0)
     //
     //	META-MODULE stuff:
     //
     //
     //	Module header constructors.
     //
     MACRO(headerSymbol, FreeSymbol, 0, 2)
     MACRO(parameterDeclSymbol, FreeSymbol, 0, 2)
     MACRO(parameterDeclListSymbol, Symbol, SymbolType::ASSOC, 2)
     //
     //	Importation constructors.
     //
     MACRO(protectingSymbol, FreeSymbol, 0, 1)
     MACRO(extendingSymbol, FreeSymbol, 0, 1)
     MACRO(includingSymbol, FreeSymbol, 0, 1)
     MACRO(nilImportListSymbol, Symbol, 0, 0)
     MACRO(importListSymbol, Symbol, SymbolType::ASSOC, 2)
     //
     //	Sort set constructors.
     //
     MACRO(emptySortSetSymbol, Symbol, 0, 0)
     MACRO(sortSetSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     //
     //	Subsort set constructors.
     //
     MACRO(subsortSymbol, FreeSymbol, 0, 2)
     MACRO(emptySubsortDeclSetSymbol, Symbol, 0, 0)
     MACRO(subsortDeclSetSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     //
     //	Quoted identifier list constructors.
     //
     MACRO(nilQidListSymbol, Symbol, 0, 0)
     MACRO(qidListSymbol, Symbol, SymbolType::ASSOC, 2)
     //
     //	Natural number list constructors.
     //
     MACRO(succSymbol, SuccSymbol, SymbolType::ITER, 1)
     MACRO(natListSymbol, Symbol, SymbolType::ASSOC, 2)
     MACRO(unboundedSymbol, Symbol, 0, 0)
     //
     //	Parent constructor.
     //
     MACRO(noParentSymbol, Symbol, 0, 0)
     //
     //	String constructor.
     //
     MACRO(stringSymbol, StringSymbol, SymbolType::STRING, 0)
     //
     //	Renaming constructors.
     //
     MACRO(sortRenamingSymbol, FreeSymbol, 0, 2)
     MACRO(opRenamingSymbol, FreeSymbol, 0, 3)
     MACRO(opRenamingSymbol2, FreeSymbol, 0, 5)
     MACRO(labelRenamingSymbol, FreeSymbol, 0, 2)
     MACRO(renamingSetSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     //
     //	Module expression constructors.
     //
     MACRO(sumSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     MACRO(renamingSymbol, FreeSymbol, 0, 2)
     MACRO(instantiationSymbol, FreeSymbol, 0, 1)
     //
     //	Hook constructors.
     //
     MACRO(termHookSymbol, FreeSymbol, 0, 2)
     MACRO(hookListSymbol, Symbol, SymbolType::ASSOC, 2)
     MACRO(idHookSymbol, FreeSymbol, 0, 2)
     MACRO(opHookSymbol, FreeSymbol, 0, 4)
     //
     //	Attribute constructors.
     //
     MACRO(assocSymbol, Symbol, 0, 0)
     MACRO(commSymbol, Symbol, 0, 0)
     MACRO(idemSymbol, Symbol, 0, 0)
     MACRO(iterSymbol, Symbol, 0, 0)
     MACRO(idSymbol, FreeSymbol, 0, 1)
     MACRO(leftIdSymbol, FreeSymbol, 0, 1)
     MACRO(rightIdSymbol, FreeSymbol, 0, 1)
     MACRO(stratSymbol, FreeSymbol, 0, 1)
     MACRO(memoSymbol, Symbol, 0, 0)
     MACRO(precSymbol, FreeSymbol, 0, 1)
     MACRO(gatherSymbol, FreeSymbol, 0, 1)
     MACRO(formatSymbol, FreeSymbol, 0, 1)
     MACRO(ctorSymbol, Symbol, 0, 0)
     MACRO(frozenSymbol, Symbol, 0, 1)
     MACRO(polySymbol, Symbol, 0, 1)
     MACRO(configSymbol, Symbol, 0, 0)
     MACRO(objectSymbol, Symbol, 0, 0)
     MACRO(msgSymbol, Symbol, 0, 0)
     MACRO(specialSymbol, FreeSymbol, 0, 1)

     MACRO(labelSymbol, FreeSymbol, 0, 1)
     MACRO(metadataSymbol, FreeSymbol, 0, 1)
     MACRO(owiseSymbol, Symbol, 0, 0)
     MACRO(variantAttrSymbol, Symbol, 0, 0)
     MACRO(narrowingSymbol, Symbol, 0, 0)
     MACRO(nonexecSymbol, Symbol, 0, 0)
     MACRO(printSymbol, FreeSymbol, 0, 1)

     MACRO(emptyAttrSetSymbol, Symbol, 0, 0)
     MACRO(attrSetSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     //
     //	Operator declaration constructors.
     //
     MACRO(opDeclSymbol, FreeSymbol, 0, 4)
     MACRO(opDeclSetSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     MACRO(emptyOpDeclSetSymbol, Symbol, 0, 0)
     //
     //	Condition constructors.
     //
     MACRO(noConditionSymbol, FreeSymbol, 0, 0)
     MACRO(equalityConditionSymbol, FreeSymbol, 0, 2)
     MACRO(sortTestConditionSymbol, FreeSymbol, 0, 2)
     MACRO(matchConditionSymbol, FreeSymbol, 0, 2)
     MACRO(rewriteConditionSymbol, FreeSymbol, 0, 2)
     MACRO(conjunctionSymbol, Symbol, SymbolType::ASSOC, 2)
     //
     //	Membership axiom constructors.
     //
     MACRO(mbSymbol, FreeSymbol, 0, 3)
     MACRO(cmbSymbol, FreeSymbol, 0, 4)
     MACRO(emptyMembAxSetSymbol, Symbol, 0, 0)
     MACRO(membAxSetSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     //
     //	Equation constructors.
     //
     MACRO(eqSymbol, FreeSymbol, 0, 3)
     MACRO(ceqSymbol, FreeSymbol, 0, 4)
     MACRO(emptyEquationSetSymbol, Symbol, 0, 0)
     MACRO(equationSetSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     //
     //	Rule constructors.
     //
     MACRO(rlSymbol, FreeSymbol, 0, 3)
     MACRO(crlSymbol, FreeSymbol, 0, 4)
     MACRO(emptyRuleSetSymbol, Symbol, 0, 0)
     MACRO(ruleSetSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     //
     //	Module constructors.
     //
     MACRO(fmodSymbol, FreeSymbol, 0, 7)
     MACRO(fthSymbol, FreeSymbol, 0, 7)
     MACRO(modSymbol, FreeSymbol, 0, 8)
     MACRO(thSymbol, FreeSymbol, 0, 8)
     //
     //	Sort mapping constructors.
     //
     MACRO(sortMappingSymbol, FreeSymbol, 0, 2)
     MACRO(emptySortMappingSetSymbol, Symbol, 0, 0)
     MACRO(sortMappingSetSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     //
     //	Op mapping constructors.
     //
     MACRO(opMappingSymbol, FreeSymbol, 0, 2)
     MACRO(opSpecificMappingSymbol, FreeSymbol, 0, 4)
     MACRO(opTermMappingSymbol, FreeSymbol, 0, 2)
     MACRO(emptyOpMappingSetSymbol, Symbol, 0, 0)
     MACRO(opMappingSetSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     //
     //	View constructor.
     //
     MACRO(viewSymbol, FreeSymbol, 0, 5)
     //
     //	META-LEVEL stuff:
     //
     //
     //	Argument values.
     //  
     MACRO(anyTypeSymbol, Symbol, 0, 0)
     //
     //	Unification problem constructors
     //
     MACRO(unificandPairSymbol, FreeSymbol, 0, 2)
     MACRO(unificationConjunctionSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
     //
     //	Success constructors.
     //
     MACRO(resultPairSymbol, FreeSymbol, 0, 2)
     MACRO(resultTripleSymbol, FreeSymbol, 0, 3)
     MACRO(result4TupleSymbol, FreeSymbol, 0, 4)
     MACRO(matchPairSymbol, FreeSymbol, 0, 2)
     MACRO(unificationPairSymbol, FreeSymbol, 0, 2)
     MACRO(unificationTripleSymbol, FreeSymbol, 0, 3)
     MACRO(variantSymbol, FreeSymbol, 0, 5)
     MACRO(narrowingApplyResultSymbol, FreeSymbol, 0, 7)
     MACRO(narrowingSearchResultSymbol, FreeSymbol, 0, 6)
     MACRO(traceStepSymbol, FreeSymbol, 0, 3)
     MACRO(nilTraceSymbol, Symbol, 0, 0)
     MACRO(traceSymbol, Symbol, SymbolType::ASSOC, 2)
     MACRO(narrowingStepSymbol, FreeSymbol, 0, 7)
     MACRO(nilNarrowingTraceSymbol, Symbol, 0, 0)
     MACRO(narrowingTraceSymbol, Symbol, SymbolType::ASSOC, 2)
     MACRO(narrowingSearchPathResultSymbol, FreeSymbol, 0, 6)
     MACRO(smtResultSymbol, FreeSymbol, 0, 4)
     //
     //	Failure constructors.
     //
     MACRO(noParseSymbol, Symbol, 0, 1)
     MACRO(ambiguitySymbol, Symbol, 0, 2)
     MACRO(failure2Symbol, Symbol, 0, 0)
     MACRO(failure3Symbol, Symbol, 0, 0)
     MACRO(failureIncomplete3Symbol, Symbol, 0, 0)
     MACRO(failure4Symbol, Symbol, 0, 0)
     MACRO(noUnifierPairSymbol, Symbol, 0, 0)
     MACRO(noUnifierTripleSymbol, Symbol, 0, 0)
     MACRO(noUnifierIncompletePairSymbol, Symbol, 0, 0)
     MACRO(noUnifierIncompleteTripleSymbol, Symbol, 0, 0)
     MACRO(noVariantSymbol, Symbol, 0, 0)
     MACRO(noVariantIncompleteSymbol, Symbol, 0, 0)
     MACRO(narrowingApplyFailureSymbol, Symbol, 0, 0)
     MACRO(narrowingApplyFailureIncompleteSymbol, Symbol, 0, 0)
     MACRO(narrowingSearchFailureSymbol, Symbol, 0, 0)
     MACRO(narrowingSearchFailureIncompleteSymbol, Symbol, 0, 0)
     MACRO(narrowingSearchPathFailureSymbol, Symbol, 0, 0)
     MACRO(narrowingSearchPathFailureIncompleteSymbol, Symbol, 0, 0)
     MACRO(noMatchSubstSymbol, Symbol, 0, 0)
     MACRO(noMatchPairSymbol, Symbol, 0, 0)
     MACRO(failureTraceSymbol, Symbol, 0, 0)
     MACRO(smtFailureSymbol, Symbol, 0, 0)
     //
     //	Print option constructors.
     //
     MACRO(mixfixSymbol, Symbol, 0, 0)
     MACRO(withParensSymbol, Symbol, 0, 0)
     MACRO(flatSymbol, Symbol, 0, 0)
     MACRO(formatPrintOptionSymbol, Symbol, 0, 0)
     MACRO(numberSymbol, Symbol, 0, 0)
     MACRO(ratSymbol, Symbol, 0, 0)
     MACRO(qidAsIdSymbol, Symbol, 0, 0)
     MACRO(emptyPrintOptionSetSymbol, Symbol, 0, 0)
     MACRO(printOptionSetSymbol, Symbol, SymbolType::ASSOC | SymbolType::COMM, 2)
