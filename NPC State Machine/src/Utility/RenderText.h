#pragma once

#include <string>

#include <raylib.h>

namespace Utility
{
	void DrawString(Texture fontTexture, Vector2 position, const std::string string, Color color); // Utility functions to help draw text from a sprite sheet rather than a font.
	void DrawCharacter(Texture fontTexture, Vector2 position, const char character, Color color);
}
