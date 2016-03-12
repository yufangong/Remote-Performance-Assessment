


void SimiAnalysis::ComplexityCompare()
{
	for (size_t i = 0; i < node_repository.size(); i++)
	{
		for (size_t j = i + 1; j < node_repository.size(); j++)
		{
			if (Condition(node_repository[i], node_repository[j]))
			{
				if (NodeAnalysis(node_repository[i], node_repository[j]))
				{
					Results[node_repository[i]] = node_repository[j];
					//detectRepeat(node_repository[i], node_repository[j]);
					/*simiNodesIncluded[node_repository[j]] = node_repository[i];
				node_repository.erase(node_repository.begin() + i);*/
				}
			}
			else break;
		}
	}
}

void SimiAnalysis::showResults()
{
	for (auto result : Results)
	{
		if (result.first->value().startline == 0 && result.second->value().startline == 0)
		{
			std::cout << "\n\n " << std::string(50, '=') << " \n The whole structure of these two files are similar : \n "
				<< std::string(50, '=') << "\n " << result.first->value().name << "\n WITH \n " << result.second->value().name;
		}
		else
		{
			std::cout << "\n\n\n ================\n" 
				<< " Similar Region: \n" 
				<< " ================\n";

			std::cout << " File : " << result.first->value().file << "\n Region:\tStartline: "
				<< result.first->value().startline << "\n\t\tEndline: " << result.first->value().endline
				<< "\n Scope Type: " << result.first->value().type << "\t Scope Name: " << result.first->value().name
				<< "\n\n WITH \n\n"
				<< " File : " << result.second->value().file << "\n Region:\tStartline: "
				<< result.second->value().startline << "\n\t\tEndline: " << result.second->value().endline
				<< "\n Scope Type: " << result.second->value().type << "\t Scope Name: " << result.second->value().name;
		}
	}
}
