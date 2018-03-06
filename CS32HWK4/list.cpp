void listAll(const MenuItem* m, string path) // two-parameter overload
{
	if (path != "")
	{
		cout << path << endl;
	}

	if (m->menuItems() == nullptr)
	{
		return;
	}
	for (vector<MenuItem*>::const_iterator it = m->menuItems()->begin(); it != m->menuItems()->end(); it++)
	{
		const MenuItem* temp = (*it);
		if (path == "")
			listAll(temp, temp->name());
		else
			listAll(temp, path + "/" + temp->name());
	}
}