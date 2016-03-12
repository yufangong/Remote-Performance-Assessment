/////////////////////////////////////////////////////////////////////
//  SemiExpression.cpp - Collects tokens for code analysis         //
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
////////////////////////////////////////////////////////////////////

#include <algorithm>
#include "SemiExpression.h"

//----< construct SemiExp instance >---------------------------

SemiExp::SemiExp(Toker* pTokr) : pToker(pTokr), Verbose(false),
                                 commentIsSemiExp(false), doReturnNewLines(false)
{
}
//----< destructor >-------------------------------------------

SemiExp::~SemiExp()
{
}
//----< is this a terminating token? >-------------------------

bool SemiExp::isTerminator(std::string tok)
{
  if(tok.length() == 0)
    return false;
  if(tok == "{" || tok == "}" || tok == ";")
    return true;
  if(tok == "\n")
  {
    size_t n = find("#");
    if(n < length())
      return true;
  }
  if(commentIsSemiExp && isComment(tok))
    return true;
  return false;
}
//
//----< collect a semi-expression >----------------------------

bool SemiExp::get(bool clear)
{
  if(clear)
    toks.clear();
  std::string tok;
  do
  {
    if(pToker->isFileEnd())
    {
      if(length() > 0)
        return true;
      return false;
    }
    tok = pToker->getTok();
    if(Verbose)
    {
      if(tok != "\n")
        std::cout << "\n--tok=" << tok << std::endl;
      else
        std::cout << "\n--tok=newline\n";
    }
    if(tok != "\n" || doReturnNewLines)
      toks.push_back(tok);
  } while(!isTerminator(tok));
  trimFront();
  return true;
}
//----< is this token a comment? >-----------------------------

bool SemiExp::isComment(const std::string& tok)
{
  if(tok.length() < 2) return false;
  if(tok[0] != '/') return false;
  if(tok[1] == '/' || tok[1] == '*') return true;
  return false;
}
//----< index operator >---------------------------------------

std::string& SemiExp::operator[](int n)
{
  if(n < 0 || toks.size() <= (size_t)n)
    throw std::exception("SemiExp index out of range");
  return toks[n];
}
//----< collect semi-expression as space-seperated string >----

std::string SemiExp::show(bool showNewLines)
{
  if(toks.size() == 0)
    return "";
  std::string temp;
  for(size_t i=0; i<toks.size(); ++i)
    if(toks[i] != "\n" || showNewLines)
      temp.append(" ").append(toks[i]);
  return temp;
}
//
//----< is tok found in semi-expression? >---------------------

size_t SemiExp::find(const std::string& tok)
{
  for(size_t i=0; i<length(); ++i)
    if(tok == toks[i])
      return i;
  return length();
}
//----< remove tok if found in semi-expression >---------------

bool SemiExp::remove(const std::string& tok)
{
  std::vector<std::string>::iterator it;
  it = std::find(toks.begin(),toks.end(),tok);
  if(it != toks.end())
  {
    toks.erase(it);
    return true;
  }
  return false;
}
//----< remove tok at specified indes >------------------------

bool SemiExp::remove(size_t i)
{
  if(i<0 || toks.size()<=i)
    return false;
  std::vector<std::string>::iterator it = toks.begin();
    toks.erase(it+i);
  return true;
}
//----< merge from firstTok to token before secondTok >--------

bool SemiExp::merge(const std::string& firstTok, const std::string& secondTok)
{
  size_t first = find(firstTok);
  size_t second = find(secondTok);
  if(first < length() && second < length())
  {
    for(size_t i=first+1; i<second; ++i)
      (*this)[first] += (*this)[i];
    for(size_t i=first+1; i<second; ++i)
      remove(first+1);
    return true;
  }
  return false;
}
//----< remove leading newlines >------------------------------

void SemiExp::trimFront()
{
  while(toks.size() > 0 && (toks[0] == "\n" || toks[0] == ""))
    remove(0);
}
//----< make tokens lowercase >--------------------------------

void SemiExp::toLower()
{
  for(size_t i=0; i<length(); ++i)
  {
    for(size_t j=0; j<(*this)[i].length(); ++j)
    {
      (*this)[i][j] = tolower((*this)[i][j]);
    }
  }
}

//----< test stub >--------------------------------------------

#ifdef TEST_SEMIEXPRESSION

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing SemiExp class\n "
            << std::string(23,'=') << std::endl;

  try {
  std::cout << "\n  testing SemiExp::get(...)";
  std::cout << "\n -----------------------------";
  Toker toker1(argv[1]);
  toker1.returnComments(false);
  SemiExp test1(&toker1);
  test1.get();
  std::cout << "\n  " << test1.show();
  test1.get(false);
  std::cout << "\n  " << test1.show();
  test1.get();
  std::cout << "\n  " << test1.show();
  test1.get(false);
  std::cout << "\n  " << test1.show() << std::endl;

  std::cout << "\n  testing SemiExp::merge(...)";
  std::cout << "\n -----------------------------";
  SemiExp test2(0);
  test2.push_back("one");
  test2.push_back("two");
  test2.push_back("three");
  test2.push_back("four");
  test2.push_back("five");
  std::cout << "\n  " << test2.show();
  test2.merge("two","five");
  std::cout << "\n  " << test2.show() << std::endl;
  
  std::cout << "\n  testing SemiExp::find()";
  std::cout << "\n -------------------------";
  SemiExp semi(0);
  semi.push_back("one");
  semi.push_back("two");
  semi.push_back("three");
  semi.push_back("four");
  std::cout << "\n" << semi.show();
  size_t pos = semi.find("two");
  std::cout << "\n  position of \"two\" is " << pos;
  pos = semi.find("foobar");
  std::cout << "\n  position of \"foobar\" is " << pos;
  std::cout << "\n\n";

  std::cout << "\n  Note that comments and quotes are returned as single tokens\n";
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 1;
  }
  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-') << "\n\n";
    try
    {
  	  Toker toker(argv[i]);
      toker.returnComments();
      SemiExp se(&toker);
      se.makeCommentSemiExp();
      //se.verbose();                 // uncomment to show token details
      while(se.get())
      {
        std::cout << se.show(true).c_str() << std::endl;
        for(size_t i=0; i<se.length(); ++i)
          if(se[i] == "")
            std::cout << "\n\n----blank token----\n";
      }
      std::cout << "\n";
    }
    catch(std::exception ex)
    {
      std::cout << "\n  " << ex.what() << "\n\n";
    }
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-') << "\n\n";
    try
    {
  	  Toker toker(argv[i]);
      toker.returnComments();
      SemiExp se(&toker);
      se.makeCommentSemiExp();
      se.returnNewLines(false);
      //se.verbose();                 // uncomment to show token details
      while(se.get())
      {
        for(size_t i=0; i<se.length(); ++i)
          if(se[i] == "")
            std::cout << "\n\n----blank token----\n";
        std::cout << se.show(true).c_str() << std::endl;
      }
      std::cout << "\n";
    }
    catch(std::exception ex)
    {
      std::cout << "\n  " << ex.what() << "\n\n";
    }
  }
}

#endif
