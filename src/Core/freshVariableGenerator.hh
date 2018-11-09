/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2016 SRI International, Menlo Park, CA 94025, USA.

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
//	Abstract base class for fresh variable generation.
//
#ifndef _freshVariableGenerator_hh_
#define _freshVariableGenerator_hh_

class FreshVariableGenerator
{
public:
  virtual ~FreshVariableGenerator() {}
  virtual int getFreshVariableName(int index, int family) = 0;
  virtual Symbol* getBaseVariableSymbol(Sort* sort) = 0;
  //
  //	The following member function checks if a given variable name could conflict
  //	with a future fresh variable that it might generate.
  //
  virtual bool variableNameConflict(int id, int okFamily = NONE) = 0;
  //
  //	The following member function checks if a given variable name belongs to
  //	the given family.
  //
  virtual bool belongsToFamily(int id, int family) = 0;
};

#endif
