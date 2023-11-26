#pragma once

#include "ResourceManager.h"

namespace Globals
{
	// Global variables for easy access in other classes. Yes, I know it's not good practice.
	inline ResourceManager *g_resourceManager;
	inline int g_difficulty = 0;
}
