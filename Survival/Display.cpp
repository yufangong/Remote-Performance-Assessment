/////////////////////////////////////////////////////////////////////////////
// Display.cpp -  Display package used to print output to the screen.      //
//                                                                         //
// ----------------------------------------------------------------------- //
// Version:      2.0													   //   
// Language:     Visual C++, Visual Studio 2013                            //
// Platform:     Macbook Pro, Parallels Desktop 9, Windows 7 Professional  //
// Application:  Spring Projects, 2014                                     //
// student:      Yufan Gong, Syracuse University                           //
//               (315) 247-3777, ygong01@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////

#include "Display.h"

//----< used to show commandline user input >-------------------

void Display::showCommandLineParse(CommandLine cl)
{
	// display path, patterns, and options
	std::cout << "\n Show command line:";
	std::cout << "\n ==================";
	std::cout << "\n Path:\t " << std::get<0>(cl);
	std::cout << "\n Patterns:\t ";
	for (auto pattern : std::get<1>(cl))
		std::cout << pattern << " ";
	std::cout << "\n Options:\t ";
	for (auto option : std::get<2>(cl))
		std::cout << option << " ";
	std::cout << std::endl;
}
//----< used to show requirements >-------------------

void Display::Demonstration(size_t require)
{
	std::cout << "\n Demonstration of requirement" << require;

	switch (require)
	{
	case 3:	
		std::cout << ":\n Show recursive requirement.";
		break;
	case 4: 
		std::cout << ":\n Display the similar regions by line numbers" ;
		break;
	case 5:
		std::cout << ":\n Show braceless test results";
		break;
	case 6:
		std::cout << ":\n Show differences between similar regions.";
		break;
	case 7:
		std::cout << ":\n Show concise result which exclude same scopes inside"
			<<"\n other same scopes";
		break;
	default:
		break;
	}

	std::cout << "\n " << std::string(60, '*') << std::endl;
}
//----< used to show analysis results >-------------------

void Display::showResults(std::map<Node*, std::vector<Node*>>& Results, std::vector<std::string> Files, bool difference)
{
	showTwoFilesSame(Results, difference);
	showSimiInOneFile(Results, difference);
	showSimiInTwoFile(Results, Files, difference);
	similarRegionCount(Results);
}
//----< used to show count >-------------------

void Display::similarRegionCount(std::map<Node*, std::vector<Node*>>& Results)
{
	size_t count = 0;
	for (auto result : Results)
	{
		count += result.second.size();
	}
	std::cout << "\n\n ###The Similar Regions Count Is :" << count;
}
//----< used to show two files same >-------------------

void Display::showTwoFilesSame(std::map<Node*, std::vector<Node*>>& Results, bool difference)
{
	for (auto result : Results)
	{
		for (size_t i = 0; i < result.second.size(); i++)
		{
			if (result.first->value().type == "root")
			{
				std::cout << "\n\n\n " << std::string(52, '=') << " \n The Whole Structure Of These Two Files Are Similar : \n "
					<< std::string(52, '=') << "\n " << result.first->value().name 
					<< "\n\n *WITH*\n\n " << result.second[i]->value().name;
				if (difference)
					showDifference(result.first, result.second[i]);
			}
		}
	}

}
//----< used to show similar region between two files >-------------------

void Display::showSimiInTwoFile(std::map<Node*, std::vector<Node*>>& Results, std::vector<std::string> Files, bool difference)
{
	for (size_t m = 0; m < Files.size(); m++)
	{
		for (size_t n = m + 1; n < Files.size(); n++)
		{
			for (auto result : Results)
			{
				for (size_t i = 0; i < result.second.size(); i++)
				{
					if (result.first->value().file == Files[m] && result.second[i]->value().file == Files[n] && result.first->value().type != "root")
					{
						std::cout << "\n\n\n =============================\n"
							<< " *Similar Region In Two Files: \n"
							<< " =============================\n";

						std::cout << " File : " << result.first->value().file << "\n Region:\tStartline: "
							<< result.first->value().startline << "\n\t\tEndline: " << result.first->value().endline
							<< "\n Scope Type: " << result.first->value().type << "\t Scope Name: " << result.first->value().name
							<< "\n\n *WITH* \n\n"
							<< " File : " << result.second[i]->value().file << "\n Region:\tStartline: "
							<< result.second[i]->value().startline << "\n\t\tEndline: " << result.second[i]->value().endline
							<< "\n Scope Type: " << result.second[i]->value().type << "\t Scope Name: " 
							<< result.second[i]->value().name;
						if (difference)
							showDifference(result.first, result.second[i]);
					}
					if (result.first->value().file == Files[n] && result.second[i]->value().file == Files[m] && result.first->value().type != "root")
					{
						std::cout << "\n\n\n =============================\n"
							<< " #Similar Region In Two Files: \n"
							<< " =============================\n";

						std::cout << " File : " << result.second[i]->value().file << "\n Region:\tStartline: "
							<< result.second[i]->value().startline << "\n\t\tEndline: " << result.second[i]->value().endline
							<< "\n Scope Type: " << result.second[i]->value().type << "\t Scope Name: " 
							<< result.second[i]->value().name 
							<< "\n\n *WITH* \n\n" << " File : " << result.first->value().file << "\n Region:\tStartline: "
							<< result.first->value().startline << "\n\t\tEndline: " << result.first->value().endline
							<< "\n Scope Type: " << result.first->value().type << "\t Scope Name: " << result.first->value().name;
						if (difference)
							showDifference(result.second[i], result.first);
					}
				}
			}
		}
	}
}
//----< used to show similar region in one file >-------------------

void Display::showSimiInOneFile(std::map<Node*, std::vector<Node*>>& Results, bool difference)
{
	for (auto result : Results)
	{
		for (size_t i = 0; i < result.second.size(); i++)
		{
			if (result.first->value().file == result.second[i]->value().file)
			{
				std::cout << "\n\n\n ============================\n"
					<< " Similar Region In One File: \n"
					<< " ============================\n";

				std::cout << " File : " << result.first->value().file << "\n\n Region:\tStartline: "
					<< result.first->value().startline << "\n\t\tEndline: " << result.first->value().endline
					<< "\n Scope Type: " << result.first->value().type << "\t Scope Name: " << result.first->value().name
					<< "\n\n *WITH* \n\n Region:\tStartline: " << result.second[i]->value().startline << "\n\t\tEndline: " 
					<< result.second[i]->value().endline << "\n Scope Type: " << result.second[i]->value().type 
					<< "\t Scope Name: " << result.second[i]->value().name;
				if (difference)
					showDifference(result.first, result.second[i]);
			}
		}
	}
}
//----< used to show difference between two similar region >-------------------

void Display::showDifference(Node* node1, Node* node2)
{
	size_t size1 = node1->value().endline - node1->value().startline + 1;
	size_t size2 = node2->value().endline - node2->value().startline + 1;
	if (size1 == size2)
	{
		std::cout << "\n\n %Difference Of Size: SAME \n";
	}
	else
	{
		std::cout << "\n\n %Difference Of Size: Size of Scope1: " << size1
			        << "\n                      Size of Scope2: " << size2 << std::endl;
	}
} 

//----< used to show analysis results >-------------------

std::string Display::GetResults(std::map<Node*, std::vector<Node*>>& Results, std::vector<std::string> Files, bool difference)
{
	std::ostringstream resultstream;
	resultstream << GetTwoFilesSame(Results, difference);
	resultstream << GetSimiInOneFile(Results, difference);
	resultstream << GetSimiInTwoFile(Results, Files, difference);
	resultstream << GetsimilarRegionCount(Results);
	return resultstream.str();
}
//----< used to show count >-------------------

std::string Display::GetsimilarRegionCount(std::map<Node*, std::vector<Node*>>& Results)
{
	std::ostringstream resultstream;
	size_t count = 0;
	for (auto result : Results)
	{
		count += result.second.size();
	}
	resultstream << "\n\n ###The Similar Regions Count Is :" << count;
	return resultstream.str();
}
//----< used to show two files same >-------------------

std::string Display::GetTwoFilesSame(std::map<Node*, std::vector<Node*>>& Results, bool difference)
{
	std::ostringstream resultstream;
	for (auto result : Results)
	{
		for (size_t i = 0; i < result.second.size(); i++)
		{
			if (result.first->value().type == "root")
			{
				resultstream << "\n\n\n " << std::string(52, '=') << " \n The Whole Structure Of These Two Files Are Similar : \n "
					<< std::string(52, '=') << "\n " << result.first->value().name
					<< "\n\n *WITH*\n\n " << result.second[i]->value().name;
				if (difference)
					resultstream << GetDifference(result.first, result.second[i]);
			}
		}
	}
	return resultstream.str();
}
//----< used to show similar region between two files >-------------------

std::string Display::GetSimiInTwoFile(std::map<Node*, std::vector<Node*>>& Results, std::vector<std::string> Files, bool difference)
{
	std::ostringstream resultstream;
	for (size_t m = 0; m < Files.size(); m++)
	{
		for (size_t n = m + 1; n < Files.size(); n++)
		{
			for (auto result : Results)
			{
				for (size_t i = 0; i < result.second.size(); i++)
				{
					if (result.first->value().file == Files[m] && result.second[i]->value().file == Files[n] && result.first->value().type != "root")
					{
						resultstream << "\n\n\n =============================\n"
							<< " *Similar Region In Two Files: \n"
							<< " =============================\n";

						resultstream << " File : " << result.first->value().file << "\n Region:\tStartline: "
							<< result.first->value().startline << "\n\t\tEndline: " << result.first->value().endline
							<< "\n Scope Type: " << result.first->value().type << "\t Scope Name: " << result.first->value().name
							<< "\n\n *WITH* \n\n"
							<< " File : " << result.second[i]->value().file << "\n Region:\tStartline: "
							<< result.second[i]->value().startline << "\n\t\tEndline: " << result.second[i]->value().endline
							<< "\n Scope Type: " << result.second[i]->value().type << "\t Scope Name: "
							<< result.second[i]->value().name;
						if (difference)
							resultstream << GetDifference(result.first, result.second[i]);
					}
					if (result.first->value().file == Files[n] && result.second[i]->value().file == Files[m] && result.first->value().type != "root")
					{
						resultstream << "\n\n\n =============================\n"
							<< " #Similar Region In Two Files: \n"
							<< " =============================\n";

						resultstream << " File : " << result.second[i]->value().file << "\n Region:\tStartline: "
							<< result.second[i]->value().startline << "\n\t\tEndline: " << result.second[i]->value().endline
							<< "\n Scope Type: " << result.second[i]->value().type << "\t Scope Name: "
							<< result.second[i]->value().name
							<< "\n\n *WITH* \n\n" << " File : " << result.first->value().file << "\n Region:\tStartline: "
							<< result.first->value().startline << "\n\t\tEndline: " << result.first->value().endline
							<< "\n Scope Type: " << result.first->value().type << "\t Scope Name: " << result.first->value().name;
						if (difference)
							resultstream << GetDifference(result.second[i], result.first);
					}
				}
			}
		}
	}
	return resultstream.str();
}
//----< used to show similar region in one file >-------------------

std::string Display::GetSimiInOneFile(std::map<Node*, std::vector<Node*>>& Results, bool difference)
{
	std::ostringstream resultstream;
	for (auto result : Results)
	{
		for (size_t i = 0; i < result.second.size(); i++)
		{
			if (result.first->value().file == result.second[i]->value().file)
			{
				resultstream << "\n\n\n ============================\n"
					<< " Similar Region In One File: \n"
					<< " ============================\n";

				resultstream << " File : " << result.first->value().file << "\n\n Region:\tStartline: "
					<< result.first->value().startline << "\n\t\tEndline: " << result.first->value().endline
					<< "\n Scope Type: " << result.first->value().type << "\t Scope Name: " << result.first->value().name
					<< "\n\n *WITH* \n\n Region:\tStartline: " << result.second[i]->value().startline << "\n\t\tEndline: "
					<< result.second[i]->value().endline << "\n Scope Type: " << result.second[i]->value().type
					<< "\t Scope Name: " << result.second[i]->value().name;

				if (difference)
				{
					resultstream << GetDifference(result.first, result.second[i]);
				}
			}
		}
	}
	return resultstream.str();
}
//----< used to show difference between two similar region >-------------------

std::string Display::GetDifference(Node* node1, Node* node2)
{
	std::ostringstream resultstream;
	size_t size1 = node1->value().endline - node1->value().startline + 1;
	size_t size2 = node2->value().endline - node2->value().startline + 1;
	if (size1 == size2)
	{
		resultstream << "\n\n %Difference Of Size: SAME \n";
	}
	else
	{
		resultstream << "\n\n %Difference Of Size: Size of Scope1: " << size1
			<< "\n                      Size of Scope2: " << size2 << std::endl;
	}
	return resultstream.str();
}
#ifdef TEST_DISPLAY
int main()
{
	Display dp;
	dp.Demonstration(7);
	using Node = TMTree::MNode<element>;
	using CommandLine = std::tuple<std::string, std::vector<std::string>, std::vector<std::string>>;
	std::string path = "path";
	std::vector<std::string> patterns = { ".h", ".cs", ".cpp" };
	std::vector<std::string> options = { "\a", "\b" };
	CommandLine cl = CommandLine{path, patterns, options};
	dp.showCommandLineParse(cl);
	element e1, e2;
	e1.name = "1";
	e1.file = "file1";
	e1.startline = 0;
	e1.endline = 10;
	e1.type = "1";
	e2.name = "2";
	e2.file = "file2";
	e2.startline = 0;
	e2.endline = 10;
	e2.type = "2";
	Node* node1 = new Node(e1);
	Node* node2 = new Node(e2);
	std::vector<std::string> files = { "file1", "file2" };
	std::map<Node*, std::vector<Node*>> Results;
	Results[node1].push_back(node2);
	dp.showResults(Results, files,false);
}

#endif