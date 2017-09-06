//
//	Common ancestor for equations, rules and sort constraints.
//
#ifndef _preEquation_hh_
#define _preEquation_hh_
#include <stack>
#include "lineNumber.hh"
#include "variableInfo.hh"
#include "badFlag.hh"
#include "label.hh"
#include "natSet.hh"

class PreEquation : public LineNumber, public ModuleItem, public VariableInfo, public BadFlag
{
public:
  PreEquation(int label, Term* lhs, const Vector<ConditionFragment*>& cond);
  virtual ~PreEquation();

  const Label& getLabel() const; 
  Term* getLhs() const;
  const Vector<ConditionFragment*>& getCondition() const;
  bool hasCondition() const;
  LhsAutomaton* getLhsAutomaton() const;
  bool isNonexec() const;
  void setNonexec();
  //
  //	This is the most general condition checking function that allows
  //	multiple distinct successes; caller must provide trialRef variable
  //	and condition state stack in order to preserve this information
  //	between calls.
  //
  bool checkCondition(bool findFirst,
		      DagNode* subject,
		      RewritingContext& context,
		      Subproblem* subproblem,
		      int& trialRef,
		      stack<ConditionState*>& state) const;
  //
  //	Simplified interface to the above for the common case where we only care
  //	if a condition succeeds at least once or fails.
  //
  bool checkCondition(DagNode* subject,
		      RewritingContext& context,
		      Subproblem* subproblem) const;

protected:
  void check(NatSet& boundVariables);
  void preprocess();
  void compileBuild(TermBag& availableTerms, bool eagerContext);
  void compileMatch(bool compileLhs, bool withExtension);
  //
  //	This function must be defined by all derived classes
  //
  virtual int traceBeginTrial(DagNode* subject, RewritingContext& context) const = 0;
  
  static const Vector<ConditionFragment*> noCondition;

  bool isCompiled() const;
  void setCompiled();

private:
  static void cleanStack(stack<ConditionState*>& conditionStack);

  enum Flags
  {
    COMPILED = 2,
    NONEXEC = 4
  };

  bool solveCondition(bool findFirst,
		      int trialRef,
		      RewritingContext& solution,
		      stack<ConditionState*>& state) const;

  Label label;
  Term* lhs;
  LhsAutomaton* lhsAutomaton;
  Vector<ConditionFragment*> condition;
};

inline bool
PreEquation::isCompiled() const
{
  return getFlag(COMPILED);
}

inline void
PreEquation::setCompiled()
{
  setFlags(COMPILED);
}

inline bool
PreEquation::isNonexec() const
{
  return getFlag(NONEXEC);
}

inline void
PreEquation::setNonexec()
{
  setFlags(NONEXEC);
}

inline const Label&
PreEquation::getLabel() const
{
  return label;
}

inline Term*
PreEquation::getLhs() const
{
  return lhs;
}

inline const Vector<ConditionFragment*>&
PreEquation::getCondition() const
{
  return condition;
}

inline bool
PreEquation::hasCondition() const
{
  return !(condition.isNull());
}

inline LhsAutomaton*
PreEquation::getLhsAutomaton() const
{
  return lhsAutomaton;
}

#endif
