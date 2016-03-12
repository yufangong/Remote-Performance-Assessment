/////////////////////////////////////////////////////////////////////
//  Parser.cpp - Analyzes C++ language constructs                  //
//  ver 1.4                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp14                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
/////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "FoldingRules.h"

//----< register parsing rule >--------------------------------

void Parser::addRule(IRule* pRule)
{
  rules.push_back(pRule);
}
//----< get next ITokCollection >------------------------------

bool Parser::next() 
{ 
  // default operation of doFold(pTokColl) is to simply return
  // - code analysis provides specific rules for handling
  //   for statements and operator expressions which are
  //   bound in ConfigureParser.cpp

  bool succeeded = pTokColl->get();
  if(!succeeded)
    return false;
  pFoldingRules->doFold(pTokColl);
  return true;
}

//----< parse the SemiExp by applying all rules to it >--------

bool Parser::parse()
{
  bool succeeded = false;
  for(size_t i=0; i<rules.size(); ++i)
  {
    if(rules[i]->doTest(pTokColl))
      succeeded = true;
  }
  return succeeded;
}
//----< register action with a rule >--------------------------

void IRule::addAction(IAction *pAction)
{
  actions.push_back(pAction);
}
//----< invoke all actions associated with a rule >------------

void IRule::doActions(ITokCollection*& pTokColl)
{
  if(actions.size() > 0)
    for(size_t i=0; i<actions.size(); ++i)
      actions[i]->doAction(pTokColl);
}

//----< test stub >--------------------------------------------

#ifdef TEST_PARSER

#include <queue>
#include <string>

int main()
{
	//ScopeStack<element> stack;
	std::cout << "\n  Testing Parser class\n "
		<< std::string(22, '=') << std::endl;
	// collecting tokens from files, named on the command line

	ConfigParseToConsole configure;
	Parser* pParser = configure.Build("ActionsAndRules.h", false);
	try
	{
		if (pParser)
		{
			if (!configure.Attach("ActionsAndRules.h"))
			{
				std::cout << "\n  could not open file " << "ActionsAndRules.h" << std::endl;
			}
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return 1;
		}
		// now that parser is built, use it
		while (pParser->next())
			pParser->parse();
		std::cout << "\n\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n   " << ex.what() << "\n\n";
	}
	std::cout << "\n\n";
}

#endif
