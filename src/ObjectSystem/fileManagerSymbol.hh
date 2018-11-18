/*

    This file is part of the Maude 2 interpreter.

    Copyright 2017 SRI International, Menlo Park, CA 94025, USA.

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
//      Class to handle file manager symbols.
//
#ifndef _fileManagerSymbol_hh_
#define _fileManagerSymbol_hh_
#include <map>
#include "externalObjectManagerSymbol.hh"

class FileManagerSymbol : public ExternalObjectManagerSymbol
{
  NO_COPYING(FileManagerSymbol);

public:
  FileManagerSymbol(int id);
  //~FileManagerSymbol();

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);

  void copyAttachments(Symbol* original, SymbolMap* map);

  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);
  //
  //	Overridden methods from ExternalObjectManagerSymbol.
  //
  bool handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context);
  bool handleMessage(DagNode* message, ObjectSystemRewritingContext& context);
  void cleanUp(DagNode* objectId);

private:
  enum Special
    {
      BUFFER_SIZE = 4096
    };

  struct OpenFile
  {
    FILE* fp;
    bool okToRead;
    bool okToWrite;
    bool lastOpWasWrite;  // for read after write issue
  };

  typedef map<int, OpenFile> FileMap;

  void getOpenFile(DagNode* fileArg, int& fd, OpenFile*& ofp);
  bool getText(DagNode* textArg, Rope& text);
  bool getBase(DagNode* baseDag, int& base);
  static bool checkModeExt(const char* modeStr, bool& mainFlag, bool& otherFlag);
  bool getMode(DagNode* modeDag, char*& modeStr, bool& okToRead, bool& okToWrite);
  //
  //	Actions.
  //
  bool openFile(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool write(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool flush(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getLine(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getChars(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getPosition(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool setPosition(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool closeFile(FreeDagNode* message, ObjectSystemRewritingContext& context);
  //
  //	Replies.
  //
  void errorReply(const char* errorMessage,
		  FreeDagNode* originalMessage,
		  ObjectSystemRewritingContext& context);
  void openedFileReply(int fd,
		       FreeDagNode* originalMessage,
		       ObjectSystemRewritingContext& context);
  void gotLineReply(const Rope& line,
		    FreeDagNode* originalMessage,
		    ObjectSystemRewritingContext& context);
  void gotCharsReply(const Rope& text,
		     FreeDagNode* originalMessage,
		     ObjectSystemRewritingContext& context);
  void positionGotReply(long position,
			FreeDagNode* originalMessage,
			ObjectSystemRewritingContext& context);

#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolClass* SymbolName;
#include "fileSignature.cc"
#undef MACRO

  FileMap openFiles;
};

#endif
