
#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
/////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions     //
//  ver 3.0                                                        //
//  Language:      Visual C++ 2008, SP1                            //
//  Platform:      Dell Precision T7400, Vista Ultimate SP1        //
//  Application:   Prototype for CSE687 Pr1, Sp14                  //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
//  Modified:      Yufan Gong, Syracuse University                 //
//                 (315) 247-3777, ygong01@syr.edu                 //
/////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module defines several action classes.  Its classes provide
specialized services needed for specific applications.  The modules
Parser, SemiExpression, and Tokenizer, are intended to be reusable
without change.  This module provides a place to put extensions of
these facilities and is not expected to be reusable.

Public Interface:
=================
Toker t(someFile);              // create tokenizer instance
SemiExp se(&t);                 // create a SemiExp attached to tokenizer
Parser parser(se);              // now we have a parser
Rule1 r1;                       // create instance of a derived Rule class
Action1 a1;                     // create a derived action
r1.addAction(&a1);              // register action with the rule
parser.addRule(&r1);            // register rule with parser
while(se.getSemiExp())          // get semi-expression
parser.parse();                 // and parse it

Build Process:
==============
Required files
- Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
Build commands (either one)
- devenv CodeAnalysis.sln
- cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
semiexpression.cpp tokenizer.cpp /link setargv.obj

Maintenance History:
====================
ver 3.0 : 19 Mar 14
- added class definition, struct definition, anonymous definition and so on
- added CombineRule namespace to handle braceless situation

ver 2.0 : 01 Jun 11
- added processing on way to building strong code analyzer
ver 1.1 : 17 Jan 09
- changed to accept a pointer to interfaced ITokCollection instead
of a SemiExpression
ver 1.0 : 12 Jan 06
- first release

*/

#include <queue>
#include <sstream>
#include <iomanip>
#include "Display.h"
#include "Parser.h"
#include "ITokCollection.h"
#include "ScopeStack.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "Collection.h"

//////////////////////////////////////////////////////////
// This namespace use to handle braceless situation
// The InsideScope include if else for and all logic 
// loop keywords, and the doAction in PushInside class
// can handle all situation with or without brace
//
namespace CombineRule
{
	using Node = TMTree::MNode<element>;

	///////////////////////////////////////////////////////////////
	// Repository instance is used to share resources
	// among all actions.

	class Repository  // application specific
	{
		ScopeStack<element> stack;
		Toker* p_Toker;
		Collection collect;
	public:
		//----< set toker >-----------------------
		Repository(Toker* pToker)
		{
			p_Toker = pToker;
		}
		//----< get stack >-----------------------
		ScopeStack<element>& scopeStack()
		{
			return stack;
		}
		//----< get toker >-----------------------
		Toker* Toker()
		{
			return p_Toker;
		}
	    //----< get collection>-----------------------
		Collection& collection()
		{
			return collect;
		}
		//----< get linecount >-----------------------
		size_t lineCount()
		{
			return (size_t)(p_Toker->lines());
		}
		//----< set file name >-----------------------
		void setFilename(std::string filename)
		{
			this->filename = filename;
		}
		//----< get file name >-----------------------
		std::string getFilename()
		{
			return filename;
		}
	private:
		std::string filename;
	};

	/////////////////////////////////////////////////////////////
	// rule to detect beginning of file

	class BeginningOfFile : public IRule
	{
		bool root;
	public:
		BeginningOfFile()
		{
			root = true;
		}
		bool doTest(ITokCollection*& pTc)
		{
			if (root)
			{
				doActions(pTc);
				root = false;
				return false;
			}
			return false;
		}
	};


	///////////////////////////////////////////////////////////////
	// action to handle scopestack and collection at the beginning

	class HandlePush : public IAction
	{
		Repository* p_Repos;
	public:
		HandlePush(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			element elem;
			elem.type = "root";
			elem.name = p_Repos->getFilename();
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			elem.endline = 0;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};

	///////////////////////////////////////////////////////////////
	// rule to detect end of scope

	class EndOfScope : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			if (pTc->find("}") < pTc->length())
			{
				doActions(pTc);
				return false;
			}
			return false;
		}
	};

	///////////////////////////////////////////////////////////////
	// action to handle collection at the end of scope

	class HandlePop : public IAction
	{
		Repository* p_Repos;
	public:
		HandlePop(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			if (p_Repos->scopeStack().size() == 0)
				return;
			element elem = p_Repos->scopeStack().pop();
			size_t endline = p_Repos->lineCount() + 1;
			element elemt = p_Repos->collection().popNode(endline);
		}
	};

	///////////////////////////////////////////////////////////////
	// rule to detect preprocessor statements

	class PreprocStatement : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			if (pTc->find("#") < pTc->length())
			{
				doActions(pTc);
				return true;
			}
			return false;
		}
	};

	///////////////////////////////////////////////////////////////
	// action to print preprocessor statement to console

	class PrintPreproc : public IAction
	{
	public:
		void doAction(ITokCollection*& pTc)
		{
			std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
		}
	};

	///////////////////////////////////////////////////////////////
	// rule to detect function definitions

	class FunctionDefinition : public IRule
	{
	public:
		bool isSpecialKeyWord(const std::string& tok)
		{
			const static std::string keys[]
				= { "for", "while", "switch", "if", "catch" };
			for (int i = 0; i < 5; ++i)
			if (tok == keys[i])
				return true;
			return false;
		}
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("(");
				if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};

	///////////////////////////////////////////////////////////////////
	// action to push function element onto ScopeStack and Collection

	class PushFunction : public IAction
	{
		Repository* p_Repos;
	public:
		PushFunction(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::string name = (*pTc)[pTc->find("(") - 1];
			element elem;
			elem.type = "function";
			elem.name = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};

	///////////////////////////////////////////////////////////////
	// rule to detect all inside function scope definitions
	// if, for, while, catch, switch, do, else, try
	// also include braceless situation

	class InsideScopeDefinition : public IRule
	{
	public:

		bool doTest(ITokCollection*& pTc)
		{
			const static std::string keys[]
				= { "for", "while", "switch", "if", "catch", "do", "else", "try" };
			ITokCollection& tc = *pTc;
			for (auto key : keys)
			{
				size_t len = tc.find(key);
				if (len < tc.length())
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};

	///////////////////////////////////////////////////////////////////
	// action to push inside scope onto ScopeStack and Collection

	class PushInsideScope : public IAction
	{
		Repository* p_Repos;
	public:
		PushInsideScope(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			int count = 0;
			const static std::string keys[]
				= { "for", "while", "switch", "if", "catch", "do", "else", "try" };
			ITokCollection& tc = *pTc;
			for (size_t i = 0; i < tc.length(); i++)
			{
				for (auto key : keys)
				{
					if (tc[i] == key)
					{
						count++;
						std::string name;
						element elem;
						elem.file = p_Repos->getFilename();
						elem.startline = p_Repos->lineCount() + 1;
						p_Repos->scopeStack().push(elem);
						if (key == "else" )
						{
							if (tc[i + 1] == "if")
							{
								name = "else if";
								i++;
							}
							else
								name = key;
							elem.name = name;
							elem.type = name;
							p_Repos->collection().PushElse(elem);
						}
						else
						{
							name = key;
							elem.name = name;
							elem.type = name;
							p_Repos->collection().pushNode(elem);
						}
						break;
					}
				}
			}
			if (count > 1)
				p_Repos->collection().markBracelessNode(count);
			if (tc[tc.length() - 1] != "{")
			{
				size_t endline = p_Repos->lineCount() + 1;
				element elemt = p_Repos->collection().popNode(endline);
			}
		}
	};

	///////////////////////////////////////////////////////////////
	// rule to detect class definitions
	class ClassDefinition : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("class");
				size_t len2 = tc.find("(");
				if (len2 < tc.length())
					return false;
				if (len < tc.length())
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};

	///////////////////////////////////////////////////////////////
	// action to push class onto ScopeStack and Collection

	class PushClass : public IAction
	{
		Repository* p_Repos;
	public:
		PushClass(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::string name;
			if ((*pTc)[pTc->find("class") + 2] != ">")
				name = (*pTc)[pTc->find("class") + 1];
			else
				name = (*pTc)[pTc->find("class") + 4];
			element elem;
			elem.type = "class";
			elem.name = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};
	///////////////////////////////////////////////////////////////
	// rule to detect namespace definitions

	class NamespaceDefinition : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("namespace");
				if (len < tc.length())
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};
	///////////////////////////////////////////////////////////////
	// action to push namespace onto ScopeStack and Collection

	class PushNamespace : public IAction
	{
		Repository* p_Repos;
	public:
		PushNamespace(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::string name = (*pTc)[pTc->find("namespace") + 1];
			element elem;
			elem.type = "namespace";
			elem.name = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};
	///////////////////////////////////////////////////////////////
	// rule to detect structrue definitions

	class SturctDefinition : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("struct");
				if (len < tc.length())
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};
	///////////////////////////////////////////////////////////////
	// action to push structure onto ScopeStack and Collection

	class PushStruct : public IAction
	{
		Repository* p_Repos;
	public:
		PushStruct(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::string name = (*pTc)[pTc->find("struct") + 1];
			element elem;
			elem.type = "struct";
			elem.name = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};
	///////////////////////////////////////////////////////////////
	// rule to detect enum definitions

	class EnumDefinition : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("enum");
				if (len < tc.length())
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};
	///////////////////////////////////////////////////////////////
	// action to push enum onto ScopeStack and Collection

	class PushEnum : public IAction
	{
		Repository* p_Repos;
	public:
		PushEnum(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::string name = (*pTc)[pTc->find("enum") + 1];
			element elem;
			elem.type = "enum";
			elem.name = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};

	///////////////////////////////////////////////////////////////
	// rule to detect anonymous scope definitions

	class Anonymity : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			const static std::string keys[]
				= { "class", "enum", "struct", "try", "do", "else", "namespace", "(" };
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				for (auto key : keys)
				{
					size_t len = tc.find(key);
					if (len < tc.length())
						return false;
				}
				doActions(pTc);
				return true;
			}
			return false;
		}
	};

	///////////////////////////////////////////////////////////////
	// action to push anonymous onto ScopeStack and Collection

	class PushAnonymity : public IAction
	{
		Repository* p_Repos;
		std::string filename;
	public:
		PushAnonymity(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			element elem;
			elem.type = "unknown";
			elem.name = "anonymous";
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};

	///////////////////////////////////////////////////////////////
	// action to send semi-expression that starts a function def
	// to console

	class PrintFunction : public IAction
	{
	public:
		void doAction(ITokCollection*& pTc)
		{
			std::cout << "\n\n  FuncDef Stmt: " << pTc->show().c_str();
		}
	};

	///////////////////////////////////////////////////////////////
	// action to send signature of a function def to console

	class PrettyPrintFunction : public IAction
	{
	public:
		void doAction(ITokCollection*& pTc)
		{
			pTc->remove("public");
			pTc->remove(":");
			pTc->trimFront();
			int len = pTc->find(")");
			std::cout << "\n\n  Pretty Stmt:    ";
			for (int i = 0; i < len + 1; ++i)
				std::cout << (*pTc)[i] << " ";
		}
	};
}
//////////////////////////////////////////////////////////
// This AcitonsAndRules use to handle scope with braces
//
namespace NoBracelessRule
{
	using Node = TMTree::MNode<element>;

	///////////////////////////////////////////////////////////////
	// Repository instance is used to share resources
	// among all actions.

	class Repository  // application specific
	{
		ScopeStack<element> stack;
		Toker* p_Toker;
		Collection collect;
	public:
		//----< set toker >-----------------------
		Repository(Toker* pToker)
		{
			p_Toker = pToker;
		}
		//----< get stack >-----------------------
		ScopeStack<element>& scopeStack()
		{
			return stack;
		}
		//----< get toker >-----------------------
		Toker* Toker()
		{
			return p_Toker;
		}
		//----< get collection>-----------------------
		Collection& collection()
		{
			return collect;
		}
		//----< get linecount >-----------------------
		size_t lineCount()
		{
			return (size_t)(p_Toker->lines());
		}
		//----< set file name >-----------------------
		void setFilename(std::string filename)
		{
			this->filename = filename;
		}
		//------<get file name>---------
		std::string getFilename()
		{
			return filename;
		}
	private:
		std::string filename;
	};


	/////////////////////////////////////////////////////////////
	// rule to detect beginning of file

	class BeginningOfFile : public IRule
	{
		bool root;
	public:
		BeginningOfFile()
		{
			root = true;
		}
		bool doTest(ITokCollection*& pTc)
		{
			if (root)
			{
				doActions(pTc);
				root = false;
				return false;
			}
			return false;
		}
	};


	///////////////////////////////////////////////////////////////
	// action to handle scopestack and collection at the beginning

	class HandlePush : public IAction
	{
		Repository* p_Repos;
	public:
		HandlePush(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			element elem;
			elem.type = "root";
			elem.name = p_Repos->getFilename();
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			elem.endline = 0;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};

	///////////////////////////////////////////////////////////////
	// rule to detect end of scope

	class EndOfScope : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			if (pTc->find("}") < pTc->length())
			{
				doActions(pTc);
				return false;
			}
			return false;
		}
	};

	///////////////////////////////////////////////////////////////
	// action to handle collection and scope stack at end of scope 

	class HandlePop : public IAction
	{
		Repository* p_Repos;
		bool option;
	public:
		HandlePop(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			if (p_Repos->scopeStack().size() == 0)
				return;
			element elem = p_Repos->scopeStack().pop();
			size_t endline = p_Repos->lineCount() + 1;
			element elemt = p_Repos->collection().popNode(endline);
		}
	};

	///////////////////////////////////////////////////////////////
	// rule to detect preprocessor statements

	class PreprocStatement : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			if (pTc->find("#") < pTc->length())
			{
				doActions(pTc);
				return true;
			}
			return false;
		}
	};

	///////////////////////////////////////////////////////////////
	// action to print preprocessor statement to console

	class PrintPreproc : public IAction
	{
	public:
		void doAction(ITokCollection*& pTc)
		{
			std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
		}
	};

	///////////////////////////////////////////////////////////////
	// rule to detect function definitions

	class FunctionDefinition : public IRule
	{
	public:
		bool isSpecialKeyWord(const std::string& tok)
		{
			const static std::string keys[]
				= { "for", "while", "switch", "if", "catch" };
			for (int i = 0; i < 5; ++i)
			if (tok == keys[i])
				return true;
			return false;
		}
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("(");
				if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};

	///////////////////////////////////////////////////////////////
	// action to push function onto ScopeStack and Collection

	class PushFunction : public IAction
	{
		Repository* p_Repos;
		std::string filename;
	public:
		PushFunction(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::string name = (*pTc)[pTc->find("(") - 1];
			element elem;
			elem.type = "function";
			elem.name = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};
	///////////////////////////////////////////////////////////////
	// rule to detect insidescope definitions
	// if, for, while, catch, switch these scope have ()

	class InsideScopeDefinition : public IRule
	{
	public:
		bool isSpecialKeyWord(const std::string& tok)
		{
			const static std::string keys[]
				= { "for", "while", "switch", "if", "catch" };
			for (int i = 0; i<5; ++i)
			if (tok == keys[i])
				return true;
			return false;
		}
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("(");
				if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
				{
					if (len >= 2)
					{
						if (tc[len - 2] == "else")
							return false;
					}
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};
	///////////////////////////////////////////////////////////////
	// action to push inside scope onto ScopeStack and Collection

	class PushInsideScope : public IAction
	{
		Repository* p_Repos;
	public:
		PushInsideScope(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::string name = (*pTc)[pTc->find("(") - 1];
			ITokCollection& tc = *pTc;
			element elem;
			elem.name = name;
			elem.type = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};
	///////////////////////////////////////////////////////////////
	// rule to detect other definitions like
	//else, try, do these scopes without ()

	class NoParenthesisScopeDef : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			const static std::string keys[] = { "else", "try", "do" };
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				for (int i = 0; i < 3; i++)
				{
					size_t len = tc.find(keys[i]);
					if (len < tc.length())
					{
						doActions(pTc);
						return true;
					}
				}
			}
			return false;
		}
	};

	/////////////////////////////////////////////////////////////////////////////
	// action to push scope without parenthesis onto ScopeStack and Collection

	class PushNoParenthesisScope : public IAction
	{
		Repository* p_Repos;
	public:
		PushNoParenthesisScope(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			const static std::string keys[] = { "else", "try", "do" };
			std::string name = "";
			for (int i = 0; i < 3; i++)
			{
				size_t len = tc.find(keys[i]);
				if (len < tc.length())
				{
					name = (*pTc)[pTc->find(keys[i])];
				}
				if (len < tc.length() - 1)
				{
					if (tc[len + 1] == "if")
						name = "else if";
				}
			}
			element elem;
			elem.type = name;
			elem.name = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};

	///////////////////////////////////////////////////////////////
	// rule to detect class definitions 

	class ClassDefinition : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("class");
				size_t len2 = tc.find("(");
				if (len2 < tc.length())
					return false;
				if (len < tc.length())
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};
	///////////////////////////////////////////////////////////////
	// action to push class onto ScopeStack and Collection

	class PushClass : public IAction
	{
		Repository* p_Repos;
	public:
		PushClass(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::string name;
			if ((*pTc)[pTc->find("class") + 2] != ">")
				name = (*pTc)[pTc->find("class") + 1];
			else
				name = (*pTc)[pTc->find("class") + 4];
			element elem;
			elem.type = "class";
			elem.name = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};
	///////////////////////////////////////////////////////////////
	// rule to detect namespace definitions

	class NamespaceDefinition : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("namespace");
				if (len < tc.length())
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};
	///////////////////////////////////////////////////////////////
	// action to push namespace onto ScopeStack and Collection

	class PushNamespace : public IAction
	{
		Repository* p_Repos;
	public:
		PushNamespace(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::string name = (*pTc)[pTc->find("namespace") + 1];
			element elem;
			elem.type = "namespace";
			elem.name = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};
	///////////////////////////////////////////////////////////////
	// rule to detect structrue definitions

	class SturctDefinition : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("struct");
				if (len < tc.length())
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};
	///////////////////////////////////////////////////////////////
	// action to push structure onto ScopeStack and Collection

	class PushStruct : public IAction
	{
		Repository* p_Repos;
	public:
		PushStruct(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::string name = (*pTc)[pTc->find("struct") + 1];
			element elem;
			elem.type = "struct";
			elem.name = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};
	///////////////////////////////////////////////////////////////
	// rule to detect enum definitions

	class EnumDefinition : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				size_t len = tc.find("enum");
				if (len < tc.length())
				{
					doActions(pTc);
					return true;
				}
			}
			return false;
		}
	};
	///////////////////////////////////////////////////////////////
	// action to push enum onto ScopeStack and Collection

	class PushEnum : public IAction
	{
		Repository* p_Repos;
	public:
		PushEnum(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			std::string name = (*pTc)[pTc->find("enum") + 1];
			element elem;
			elem.type = "enum";
			elem.name = name;
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};

	///////////////////////////////////////////////////////////////
	// rule to detect anonymous definitions

	class Anonymity : public IRule
	{
	public:
		bool doTest(ITokCollection*& pTc)
		{
			const static std::string keys[]
				= { "class", "enum", "struct", "try", "do", "else", "namespace", "(" };
			ITokCollection& tc = *pTc;
			if (tc[tc.length() - 1] == "{")
			{
				for (auto key : keys)
				{
					size_t len = tc.find(key);
					if (len < tc.length())
						return false;
				}
				doActions(pTc);
				return true;
			}
			return false;
		}
	};

	///////////////////////////////////////////////////////////////
	// action to push anonymous onto ScopeStack and Collection

	class PushAnonymity : public IAction
	{
		Repository* p_Repos;
		std::string filename;
	public:
		PushAnonymity(Repository* pRepos)
		{
			p_Repos = pRepos;
		}
		void doAction(ITokCollection*& pTc)
		{
			element elem;
			elem.type = "unknown";
			elem.name = "anonymous";
			elem.file = p_Repos->getFilename();
			elem.startline = p_Repos->lineCount() + 1;
			p_Repos->scopeStack().push(elem);
			p_Repos->collection().pushNode(elem);
		}
	};

	///////////////////////////////////////////////////////////////
	// action to send semi-expression that starts a function def
	// to console

	class PrintFunction : public IAction
	{
	public:
		void doAction(ITokCollection*& pTc)
		{
			std::cout << "\n\n  FuncDef Stmt: " << pTc->show().c_str();
		}
	};

	///////////////////////////////////////////////////////////////
	// action to send signature of a function def to console

	class PrettyPrintFunction : public IAction
	{
	public:
		void doAction(ITokCollection*& pTc)
		{
			pTc->remove("public");
			pTc->remove(":");
			pTc->trimFront();
			int len = pTc->find(")");
			std::cout << "\n\n  Pretty Stmt:    ";
			for (int i = 0; i < len + 1; ++i)
				std::cout << (*pTc)[i] << " ";
		}
	};
}

#endif
