#include "SimiAnalysis.h"





Toker::Toker(const std::string& src, bool isFile)
: prevprevChar(0), prevChar(0), currChar(0), nextChar(0),
doReturnComments(false), doReturnSingleQuotesAsToken(true),
numLines(0), braceCount(0), pIn(0),
_state(default_state), EndQuoteCounter(false)
{
	if (src.length() > 0)
	{
		if (!attach(src, isFile))
		{
			std::string temp = std::string("can't open ") + src;
			throw std::exception(temp.c_str());
		}
	}
	scTok = "()[]{};.\n";
	if (_mode == xml)
		scTok = "<>!" + scTok;
}


