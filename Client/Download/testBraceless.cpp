int braceless()
{
	while (0)
	if (1)
		int i = 5;
	else if (2)
		i = 8;
	else
		i = 10;
	return 0;
}
int withbrace()
{
	while (0)
	{
		if (1)
		{
			int i = 5;
		}
		else if (2)
		{
			i = 8;
		}
		else
		{
			i = 10;
		}
	}
	return 9;
}