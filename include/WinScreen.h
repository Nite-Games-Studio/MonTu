#pragma once

#include "defines.h"

class WinScreen
{
public:
	WinScreen();
	virtual ~WinScreen();
	
	void init(string configFile, OWNER owner);

};