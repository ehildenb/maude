/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//	Abstract base class for event triggered strategic execution.
//
#ifndef _stategicTask_hh_
#define _stategicTask_hh_
#include "strategicExecution.hh"

class StrategicTask : public StrategicExecution
{
  NO_COPYING(StrategicTask);

public:
  StrategicTask(StrategicTask* master);
  StrategicTask(StrategicExecution* sibling);
  ~StrategicTask();
  //
  //	Call-backs for interesting events.
  //
  virtual Survival executionSucceeded(DagNode* result, StrategicProcess* insertionPoint) = 0;
  virtual Survival executionsExhausted(StrategicProcess* insertionPoint) = 0;

protected:
  StrategicExecution* getDummyExecution();

private:
  //
  //	To simplify coding, the head and tail of our list of slaves
  //	(processes and tasks working for us) is stored as a dummy
  //	execution, essentially forming a circular list.
  //
  StrategicExecution slaveList;
};

inline StrategicExecution*
StrategicTask::getDummyExecution()
{
  return &slaveList;
}

#endif
