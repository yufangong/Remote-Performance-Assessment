
#ifndef FOLDINGRULES_H
#define FOLDINGRULES_H
/////////////////////////////////////////////////////////////////////
//  FoldingRules.h - declares ITokCollection folding rules         //
//  ver 1.3                                                        //
//  Language:      Visual C++ 2010, SP1                            //
//  Platform:      Dell Precision T7400, Win 7 Pro SP1             //
//  Application:   Code Analysis Research , Project1 Spr14         //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
//  Modified:      Yufan Gong, Syracuse University                 //
//                 (315) 247-3777, ygong01@syr.edu                 //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines several ITokCollection folding rules, e.g.,
  ways to merge tokens and collections.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  MergeForCollections();
  MergeOperatorTokens();

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, FoldingRules.h, FoldingRules.cpp,
      ConfigureParser.cpp, ItokCollection.h,
      SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv CodeAnalysis.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ScopeStack ActionsAndRules.cpp FoldingRules.cpp \
         ConfigureParser.cpp semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 1.3 : 21 Mar 14
  - modified doFold fuction for consecutive braceless "for" situation
  - added helpfunction to compute "(" and ")"

  ver 1.1 : 10 Feb 13
  - modified FoldingRules.cpp to avoid folding for(auto x : C) {
  ver 1.0 : 02 Jan 11
  - first release

*/
#include "Parser.h"
#include "SemiExpression.h"

class codeFoldingRules : public FoldingRules
{
public:
  void doFold(ITokCollection*& pTc);
private:
  static void MergeForCollections(ITokCollection*& pTc);
  static void MergeOperatorTokens(ITokCollection*& pTc);
  bool helpFunction(ITokCollection*& pTc);
};
#endif
