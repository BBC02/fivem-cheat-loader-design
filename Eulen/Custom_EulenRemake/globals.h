#pragma once
#include "includes.h"

struct Globals
{
	static Globals* Get()
	{
		static auto* instance = new Globals();
		return instance;
	}

	bool Open = true;
	bool IsLogged = false;
	bool RememberMe = false;

	char Username[248];
	char Password[248];

	std::vector<std::string> Games = { "FiveM", "Polygon", "Rust", "Splitgate "};
	int Game = 0;
};