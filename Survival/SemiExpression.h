
#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
/////////////////////////////////////////////////////////////////////
//  SemiExpression.h - Collects tokens for code analysis           //
//  ver 2.1                                                        //
//                                                                 //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp14                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
//                                                                 //
//  Jim Fawcett (c) copyright 2013                                 //
//  All rights are granted to the user except the right            //
//  to publish, and requires the retention of this notice.         //
//                                                                 //
/////////////////////////////////////////////////////////////////////
/*
  Module Operations: 
  ==================
  This module defines a SemiExp class.  Its instances build a sequence 
  of tokens obtained from the Tokenizer module for analysis of code.
  SemiExpressions are lists of tokens that terminate on one of the tokens:
    '{', '}', ';', or '\n' when its line starts with '#'

  This structure is designed to collect just the right amount of inform-
  ation from a code file for analysis.  That is, it collects the smallest
  set of tokens that lead to the detection of grammatical constructs, 
  without having to save part of the sequence for later use or get more
  to complete the detection process.  This simplifies the design of code
  analysis tools.

  Note that assignment and copying of SemiExp instances is supported, using
  the default operations provided by the C++ language.  Copies and assignments
  result in both source and target SemiExp instances sharing the same toker.

  Public Interface:
  =================
  Toker t;                                  // create tokenizer instance
  SemiExp se(&t);                           // create instance and attach
  if(se.get())                              // collect a semiExpression
    std::cout << se.showSemiExp().c_str();  // show it
  int n = se.length();                      // number of tokens in se
  std::string tok = se[2] = "a change";     // indexed read and write
  if(se.find("aTok") < se.length())         // search for a token
    std::cout << "found aTok";  
  se.clear();                               // remove all tokens

  Build Process:
  ==============
  Required files
    - SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
  Build commands (either one)
    - devenv Project1HelpS06.sln
    - cl /EHsc /DTEST_SEMIEXPRESSION SemiExpression.cpp Tokenizer.cpp \
         /link setargv.obj

  Maintenance History:
  ====================
  ver 2.1 : 09 Jun 11
  - added support for selecting whether to return newlines (true by default)
  ver 2.0 : 02 Jun 11
  - modified trimFront() to eliminate either "" or "\n" at front
  - inserted trimFront() at end of get() to fix leading empty token bug
  - added default parameter to get(bool clear=true) to allow merging of
    collected tokens with multiple calls to get()
  - added merge(firstTok,secondTok) that merges tokens in the collection
    starting with firstTok and ending one before secondTok.
  ver 1.3 : 17 Jan 09
  - changed SemiExp::getSemiExp() to SemiExp::get()
  - changed SemiExp::showSemiExp() to SemiExp::show()
  - added scope walker to test stub
  ver 1.2 : 10 Feb 06
  - fixed bug in SemiExp::getSemiExp() that caused failure to return
    last group of tokens as a semi-expression if the last character
    in source is not a newline.
  ver 1.1 : 01 Feb 06
  - added an option to return comments as semi-expressions
  - added test, isComment(const std::string& tok)
  ver 1.0 : 12 Jan 06
  - first release

*/
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "itokcollection.h"
#include "tokenizer.h"

class SemiExp : public ITokCollection
{
public:
  SemiExp(Toker* pTokr);
  ~SemiExp();
  bool get(bool clear=true);
  size_t length();
  std::string& operator[](int n);
  size_t find(const std::string& tok);
  void push_back(const std::string& tok);
  bool remove(const std::string& tok);
  bool remove(size_t i);
  bool merge(const std::string& firstTok, const std::string& secondTok);
  void toLower();
  void trimFront();
  void clear();
  std::string show(bool showNewLines=false);
  void verbose(bool v = true);
  void makeCommentSemiExp(bool commentIsSE = true);
  void returnNewLines(bool doReturnNewLines = false);
  bool isComment(const std::string& tok);

private:
  Toker* pToker;
  std::vector<std::string> toks;
  bool isTerminator(std::string tok);
  bool Verbose;
  bool commentIsSemiExp;
  bool doReturnNewLines;
};

inline size_t SemiExp::length() { return toks.size(); }

inline void SemiExp::verbose(bool v) { Verbose = v; }

inline void SemiExp::makeCommentSemiExp(bool commentIsSE)
{
  commentIsSemiExp = commentIsSE;
}

inline void SemiExp::returnNewLines(bool doReturn) { doReturnNewLines = doReturn; }

inline void SemiExp::clear() { toks.clear(); }

inline void SemiExp::push_back(const std::string& tok)
{
  toks.push_back(tok);
}

#endif
