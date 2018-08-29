/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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

PreModule*
MetaLevelOpSymbol::getPreModule(int name)
{
  Module* activeModule = getModule();
  DebugAdvisory("activeModule = " << activeModule);
  Interpreter* owner = safeCast(VisibleModule*, activeModule)->getOwner();
  DebugAdvisory("owner = " << (void*) owner);
  return owner->getModule(name);
}

bool
MetaLevelOpSymbol::metaUpModule(FreeDagNode* subject, RewritingContext& context)
{
  int moduleName;
  bool flat;
  if (metaLevel->downQid(subject->getArgument(0), moduleName) &&
      metaLevel->downBool(subject->getArgument(1), flat))
    {
      if (PreModule* pm = getPreModule(moduleName))
	{
	  PointerMap qidMap;
	  return context.builtInReplace(subject, metaLevel->upModule(flat, pm, qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaUpImports(FreeDagNode* subject, RewritingContext& context)
{
  int moduleName;
  if (metaLevel->downQid(subject->getArgument(0), moduleName))
    {
      if (PreModule* pm = getPreModule(moduleName))
	{
	  PointerMap qidMap;
	  return context.builtInReplace(subject, metaLevel->upImports(pm, qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaUpSorts(FreeDagNode* subject, RewritingContext& context)
{
  int moduleName;
  bool flat;
  if (metaLevel->downQid(subject->getArgument(0), moduleName) &&
      metaLevel->downBool(subject->getArgument(1), flat))
    {
      if (PreModule* pm = getPreModule(moduleName))
	{
	  PointerMap qidMap;
	  return context.builtInReplace(subject,
					metaLevel->upSorts(flat, pm->getFlatModule(), qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaUpSubsortDecls(FreeDagNode* subject, RewritingContext& context)
{
  int moduleName;
  bool flat;
  if (metaLevel->downQid(subject->getArgument(0), moduleName) &&
      metaLevel->downBool(subject->getArgument(1), flat))
    {
      if (PreModule* pm = getPreModule(moduleName))
	{
	  PointerMap qidMap;
	  return context.builtInReplace(subject,
					metaLevel->upSubsortDecls(flat, pm->getFlatModule(), qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaUpOpDecls(FreeDagNode* subject, RewritingContext& context)
{
  int moduleName;
  bool flat;
  if (metaLevel->downQid(subject->getArgument(0), moduleName) &&
      metaLevel->downBool(subject->getArgument(1), flat))
    {
      if (PreModule* pm = getPreModule(moduleName))
	{
	  PointerMap qidMap;
	  return context.builtInReplace(subject,
					metaLevel->upOpDecls(flat, pm->getFlatModule(), qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaUpMbs(FreeDagNode* subject, RewritingContext& context)
{
  int moduleName;
  bool flat;
  if (metaLevel->downQid(subject->getArgument(0), moduleName) &&
      metaLevel->downBool(subject->getArgument(1), flat))
    {
      if (PreModule* pm = getPreModule(moduleName))
	{
	  PointerMap qidMap;
	  return context.builtInReplace(subject,
					metaLevel->upMbs(flat, pm->getFlatModule(), qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaUpEqs(FreeDagNode* subject, RewritingContext& context)
{
  int moduleName;
  bool flat;
  if (metaLevel->downQid(subject->getArgument(0), moduleName) &&
      metaLevel->downBool(subject->getArgument(1), flat))
    {
      if (PreModule* pm = getPreModule(moduleName))
	{
	  PointerMap qidMap;
	  return context.builtInReplace(subject,
					metaLevel->upEqs(flat, pm->getFlatModule(), qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaUpRls(FreeDagNode* subject, RewritingContext& context)
{
  int moduleName;
  bool flat;
  if (metaLevel->downQid(subject->getArgument(0), moduleName) &&
      metaLevel->downBool(subject->getArgument(1), flat))
    {
      if (PreModule* pm = getPreModule(moduleName))
	{
	  PointerMap qidMap;
	  return context.builtInReplace(subject,
					metaLevel->upRls(flat, pm->getFlatModule(), qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaUpView(FreeDagNode* subject, RewritingContext& context)
{
  int viewName;
  if (metaLevel->downQid(subject->getArgument(0), viewName))
    {
      Module* activeModule = getModule();
      Interpreter* owner = safeCast(VisibleModule*, activeModule)->getOwner();
      if (View* view = owner->getView(viewName))
	{
	  PointerMap qidMap;
	  return context.builtInReplace(subject, metaLevel->upView(view, qidMap));
	}
    }
  return false;
}
