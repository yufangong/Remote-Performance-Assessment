

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
