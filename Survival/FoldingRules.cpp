/////////////////////////////////////////////////////////////////////
//  FoldingRules.cpp - declares ITokCollection folding rules       //
//  ver 1.3                                                        //
//  Language:      Visual C++ 2010, SP1                            //
//  Platform:      Dell Precision T7400, Win 7 Pro SP1             //
//  Application:   Code Analysis Research, Project1 Spr14          //
//  Author:        Jim Fawcett, CST 4-187, Syracuse University     //
//                 (315) 443-3948, jfawcett@twcny.rr.com           //
//  Modified:      Yufan Gong, Syracuse University                 //
//                 (315) 247-3777, ygong01@syr.edu                 //
/////////////////////////////////////////////////////////////////////

#include "FoldingRules.h"

//----< merge except for(auto x : X) >-------------------------

void codeFoldingRules::MergeForCollections(ITokCollection*& pTc)
{
  // have for(int i=0;
  size_t posParen = pTc->find(")");
  size_t posColon = pTc->find(":");
  if(posParen != posColon + 2)
  {
    pTc->get(false);  // get i<N;
    pTc->get(false);  // get ++i) {
  }
}
//----< merge operator >-------------------------

void codeFoldingRules::MergeOperatorTokens(ITokCollection*& pTc)
{
  pTc->merge("operator","(");
}
//----< fold toker when run into for and operator >-------------------------

void codeFoldingRules::doFold(ITokCollection*& pTc)
{
	if (pTc->find("for") < pTc->length())
	{
		while (helpFunction(pTc))
		{
			codeFoldingRules::MergeForCollections(pTc);
		}
	}
	if (pTc->find("operator") < pTc->length())
		codeFoldingRules::MergeOperatorTokens(pTc);
}
//----< compute parethesis count >-------------------------

bool codeFoldingRules::helpFunction(ITokCollection*& pTc)
{
	int single = 0;
	ITokCollection& tc = *pTc;
	for (size_t i = 0; i < tc.length(); i++)
	{
		if (tc[i] == "(")
		{
			single++;
		}
		if (tc[i] == ")")
		{
			single--;
		}
	}
	if (single == 0)
		return false;
	else
		return true;
}
#ifdef TEST_FOLDINGRULES
int main()
{
	//codeFoldingRules cfr;
	//ITokCollection* pTc;
	//cfr.doFold(pTc);

	//cannot figure out how to test
}
#endif