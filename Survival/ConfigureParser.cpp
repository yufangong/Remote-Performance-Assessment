/////////////////////////////////////////////////////////////////////
//  ConfigureParser.cpp - builds and configures parsers            //
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

#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
	// when Builder goes out of scope, everything must be deallocated
	delete pHandlePush1;
	delete pBeginningOfFile1;
	delete pHandlePop1;
	delete pEndOfScope1;
	delete pPushFunction1;
	delete pFunctionDefinition1;
	delete pClassDefinition1;
	delete pPushClass1;
	delete pInsideScopeDefinition1;
	delete pPushInsideScope1;
	delete pNamespaceDefinition1;
	delete pPushNamespace1;
	delete pStructDefinition1;
	delete pPushStruct1;
	delete pEnumDefinition1;
	delete pPushEnum1;
	delete pAnonymity1;
	delete pPushAnonymity1;

	delete pFR;
	delete pRepo1;
	delete pParser;
	delete pSemi;
	delete pToker;

	delete pHandlePush2;
	delete pBeginningOfFile2;
	delete pHandlePop2;
	delete pEndOfScope2;
	delete pPushFunction2;
	delete pFunctionDefinition2;
	delete pRepo2;
	delete pClassDefinition2;
	delete pPushClass2;
	delete pInsideScopeDefinition2;
	delete pPushInsideScope2;
	delete pNoParenthesisScopeDef2;
	delete pPushNoParenthesisScope2;
	delete pNamespaceDefinition2;
	delete pPushNamespace2;
	delete pStructDefinition2;
	delete pPushStruct2;
	delete pEnumDefinition2;
	delete pPushEnum2;
	delete pAnonymity2;
	delete pPushAnonymity2;
}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
	if (pToker == 0)
		return false;
	return pToker->attach(name, isFile);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build(std::string filename, bool braceless)
{

	try
	{
		// add Parser's main parts
		pToker = new Toker;
		pToker->returnComments();
		pSemi = new SemiExp(pToker);
		pParser = new Parser(pSemi);

		// add code folding rules
		pFR = new codeFoldingRules;
		pParser->addFoldingRules(pFR);

		if (braceless)
		{
			pRepo1 = new CombineRule::Repository(pToker);
			pRepo1->setFilename(filename);

			pBeginningOfFile1 = new CombineRule::BeginningOfFile();
			pHandlePush1 = new CombineRule::HandlePush(pRepo1);
			pBeginningOfFile1->addAction(pHandlePush1);
			pParser->addRule(pBeginningOfFile1);

			buildCombineRule();
		}
		else
		{
			pRepo2 = new NoBracelessRule::Repository(pToker);
			pRepo2->setFilename(filename);

			pBeginningOfFile2 = new NoBracelessRule::BeginningOfFile();
			pHandlePush2 = new NoBracelessRule::HandlePush(pRepo2);
			pBeginningOfFile2->addAction(pHandlePush2);
			pParser->addRule(pBeginningOfFile2);

			builfNobracelessRule();
		}
		return pParser;
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n  " << ex.what() << "\n\n";
		return 0;
	}
}
//----< braceless and brace situation configure >------------

void ConfigParseToConsole::buildCombineRule()
{
	pEndOfScope1 = new CombineRule::EndOfScope();
	pHandlePop1 = new CombineRule::HandlePop(pRepo1);
	pEndOfScope1->addAction(pHandlePop1);
	pParser->addRule(pEndOfScope1);

	pFunctionDefinition1 = new CombineRule::FunctionDefinition;
	pPushFunction1 = new CombineRule::PushFunction(pRepo1);  // no action
	pFunctionDefinition1->addAction(pPushFunction1);
	pParser->addRule(pFunctionDefinition1);

	pClassDefinition1 = new CombineRule::ClassDefinition;
	pPushClass1 = new CombineRule::PushClass(pRepo1);
	pClassDefinition1->addAction(pPushClass1);
	pParser->addRule(pClassDefinition1);

	pInsideScopeDefinition1 = new CombineRule::InsideScopeDefinition;
	pPushInsideScope1 = new CombineRule::PushInsideScope(pRepo1);
	pInsideScopeDefinition1->addAction(pPushInsideScope1);
	pParser->addRule(pInsideScopeDefinition1);

	pNamespaceDefinition1 = new CombineRule::NamespaceDefinition;
	pPushNamespace1 = new CombineRule::PushNamespace(pRepo1);
	pNamespaceDefinition1->addAction(pPushNamespace1);
	pParser->addRule(pNamespaceDefinition1);

	pStructDefinition1 = new CombineRule::SturctDefinition;
	pPushStruct1 = new CombineRule::PushStruct(pRepo1);
	pStructDefinition1->addAction(pPushStruct1);
	pParser->addRule(pStructDefinition1);

	pEnumDefinition1 = new CombineRule::EnumDefinition;
	pPushEnum1 = new CombineRule::PushEnum(pRepo1);
	pEnumDefinition1->addAction(pPushEnum1);
	pParser->addRule(pEnumDefinition1);

	pAnonymity1 = new CombineRule::Anonymity;
	pPushAnonymity1 = new CombineRule::PushAnonymity(pRepo1);
	pAnonymity1->addAction(pPushAnonymity1);
	pParser->addRule(pAnonymity1);
}
//----< brace situation configure >------------

void ConfigParseToConsole::builfNobracelessRule()
{
	pEndOfScope2 = new NoBracelessRule::EndOfScope();
	pHandlePop2 = new NoBracelessRule::HandlePop(pRepo2);
	pEndOfScope2->addAction(pHandlePop2);
	pParser->addRule(pEndOfScope2);

	pFunctionDefinition2 = new NoBracelessRule::FunctionDefinition;
	pPushFunction2 = new NoBracelessRule::PushFunction(pRepo2);  // no action
	pFunctionDefinition2->addAction(pPushFunction2);
	pParser->addRule(pFunctionDefinition2);

	pClassDefinition2 = new NoBracelessRule::ClassDefinition;
	pPushClass2 = new NoBracelessRule::PushClass(pRepo2);
	pClassDefinition2->addAction(pPushClass2);
	pParser->addRule(pClassDefinition2);

	pNoParenthesisScopeDef2 = new NoBracelessRule::NoParenthesisScopeDef;
	pPushNoParenthesisScope2 = new NoBracelessRule::PushNoParenthesisScope(pRepo2);
	pNoParenthesisScopeDef2->addAction(pPushNoParenthesisScope2);
	pParser->addRule(pNoParenthesisScopeDef2);

	pInsideScopeDefinition2 = new NoBracelessRule::InsideScopeDefinition;
	pPushInsideScope2 = new NoBracelessRule::PushInsideScope(pRepo2);
	pInsideScopeDefinition2->addAction(pPushInsideScope2);
	pParser->addRule(pInsideScopeDefinition2);

	pNamespaceDefinition2 = new NoBracelessRule::NamespaceDefinition;
	pPushNamespace2 = new NoBracelessRule::PushNamespace(pRepo2);
	pNamespaceDefinition2->addAction(pPushNamespace2);
	pParser->addRule(pNamespaceDefinition2);

	pStructDefinition2 = new NoBracelessRule::SturctDefinition;
	pPushStruct2 = new NoBracelessRule::PushStruct(pRepo2);
	pStructDefinition2->addAction(pPushStruct2);
	pParser->addRule(pStructDefinition2);

	pEnumDefinition2 = new NoBracelessRule::EnumDefinition;
	pPushEnum2 = new NoBracelessRule::PushEnum(pRepo2);
	pEnumDefinition2->addAction(pPushEnum2);
	pParser->addRule(pEnumDefinition2);

	pAnonymity2 = new NoBracelessRule::Anonymity;
	pPushAnonymity2 = new NoBracelessRule::PushAnonymity(pRepo2);
	pAnonymity2->addAction(pPushAnonymity2);
	pParser->addRule(pAnonymity2);
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
	std::cout << "\n  Testing ConfigureParser module\n "
		<< std::string(32, '=') << std::endl;

	// collecting tokens from files, named on the command line

	if (argc < 2)
	{
		std::cout
			<< "\n  please enter name of file to process on command line\n\n";
		return 1;
	}

	for (int i = 1; i<argc; ++i)
	{
		std::cout << "\n  Processing file " << argv[i];
		std::cout << "\n  " << std::string(16 + strlen(argv[i]), '-');

		ConfigParseToConsole configure;
		Parser* pParser = configure.Build();
		try
		{
			if (pParser)
			{
				if (!configure.Attach(argv[i]))
				{
					std::cout << "\n  could not open file " << argv[i] << std::endl;
					continue;
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
			std::cout << "\n\n    " << ex.what() << "\n\n";
		}
		std::cout << "\n\n";
	}
}

#endif
