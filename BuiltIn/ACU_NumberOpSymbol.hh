//
//      Class for symbols for built in ACU operations on numbers.
//
#ifndef _ACU_NumberOpSymbol_hh_
#define _ACU_NumberOpSymbol_hh_
#include "ACU_Symbol.hh"

class ACU_NumberOpSymbol : public ACU_Symbol
{
public:
  ACU_NumberOpSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);
  void copyAttachments(Symbol* original, SymbolMap* map);

  bool eqRewrite(DagNode* subject, RewritingContext& context);

private:
  bool eqRewrite2(DagNode* subject, RewritingContext& context);

  int op;
  SuccSymbol* succSymbol;
  MinusSymbol* minusSymbol;
};

#endif
