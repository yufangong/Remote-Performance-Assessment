

#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
/////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers              //
//  ver 2.2                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2005                                 //
//  Platform:      Dell Dimension 9150, Windows XP SP2             //
//  Application:   Prototype for CSE687 Pr1, Sp06                  //
//  Author:        Jim Fawcett, CST 2-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
//  Modified:      Yufan Gong, Syracuse University                 //
//                 (315) 247-3777, ygong01@syr.edu                 //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module builds and configures parsers.  It builds the parser
parts and configures them with application specific rules and actions.

Public Interface:
=================
ConfigParseToConsole conConfig;
conConfig.Build();
conConfig.Attach(someFileName);

Build Process:
==============
Required files
- ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
ActionsAndRules.h, ActionsAndRules.cpp,
SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv Project1HelpS06.sln
- cl /EHsc /DTEST_PARSER ConfigureParser.cpp parser.cpp \
ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 2.2 : 21 Mar 14
- Added serveral rules and actions
- two set of actions and rules to handle braceless situation
- set filename

ver 2.1 : 05 Feb 13
- Add serveral rules and actions
ver 2.0 : 01 Jun 11
- Major revisions to begin building a strong code analyzer
ver 1.1 : 01 Feb 06
- cosmetic changes to ConfigureParser.cpp
ver 1.0 : 12 Jan 06
- first release

*/

//
#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "FoldingRules.h"



///////////////////////////////////////////////////////////////
// build parser that writes its output to console

class ConfigParseToConsole : IBuilder
{
public:
	ConfigParseToConsole() {};
	~ConfigParseToConsole();
	bool Attach(const std::string& name, bool isFile = true);
	Parser* Build(std::string filename, bool braceless);
	CombineRule::Repository* pRepo1;
	NoBracelessRule::Repository* pRepo2;
private:
	// Builder must hold onto all the pieces
	Toker* pToker;
	SemiExp* pSemi;
	Parser* pParser;
	/*Repository* pRepo;*/

	// add folding rules

	FoldingRules* pFR;

	// add Rules and Actions

	CombineRule::BeginningOfFile* pBeginningOfFile1;
	CombineRule::HandlePush* pHandlePush1;
	CombineRule::EndOfScope* pEndOfScope1;
	CombineRule::HandlePop* pHandlePop1;

	CombineRule::FunctionDefinition* pFunctionDefinition1;
	CombineRule::PushFunction* pPushFunction1;

	CombineRule::ClassDefinition* pClassDefinition1;
	CombineRule::PushClass* pPushClass1;

	CombineRule::InsideScopeDefinition* pInsideScopeDefinition1;
	CombineRule::PushInsideScope* pPushInsideScope1;

	CombineRule::NamespaceDefinition* pNamespaceDefinition1;
	CombineRule::PushNamespace* pPushNamespace1;

	CombineRule::SturctDefinition* pStructDefinition1;
	CombineRule::PushStruct* pPushStruct1;

	CombineRule::EnumDefinition* pEnumDefinition1;
	CombineRule::PushEnum* pPushEnum1;

	CombineRule::Anonymity* pAnonymity1;
	CombineRule::PushAnonymity* pPushAnonymity1;

	NoBracelessRule::BeginningOfFile* pBeginningOfFile2;
	NoBracelessRule::HandlePush* pHandlePush2;
	NoBracelessRule::EndOfScope* pEndOfScope2;
	NoBracelessRule::HandlePop* pHandlePop2;

	NoBracelessRule::FunctionDefinition* pFunctionDefinition2;
	NoBracelessRule::PushFunction* pPushFunction2;

	NoBracelessRule::ClassDefinition* pClassDefinition2;
	NoBracelessRule::PushClass* pPushClass2;

	NoBracelessRule::NoParenthesisScopeDef* pNoParenthesisScopeDef2;
	NoBracelessRule::PushNoParenthesisScope* pPushNoParenthesisScope2;

	NoBracelessRule::InsideScopeDefinition* pInsideScopeDefinition2;
	NoBracelessRule::PushInsideScope* pPushInsideScope2;

	NoBracelessRule::NamespaceDefinition* pNamespaceDefinition2;
	NoBracelessRule::PushNamespace* pPushNamespace2;

	NoBracelessRule::SturctDefinition* pStructDefinition2;
	NoBracelessRule::PushStruct* pPushStruct2;

	NoBracelessRule::EnumDefinition* pEnumDefinition2;
	NoBracelessRule::PushEnum* pPushEnum2;

	NoBracelessRule::Anonymity* pAnonymity2;
	NoBracelessRule::PushAnonymity* pPushAnonymity2;

	// prohibit copies and assignments

	ConfigParseToConsole(const ConfigParseToConsole&);
	ConfigParseToConsole& operator=(const ConfigParseToConsole&);

	// helper functions
	void buildCombineRule();
	void builfNobracelessRule();
};


#endif
