#include "RenderText.h"

#include <vector>

namespace Utility
{
    // Not using the regular font rendering because they won't scale down properly and changing the filtering mode doesn't seem to help.
    void DrawString(Texture fontTexture, Vector2 position, const std::string string, Color color)
    {
        std::vector<char> v(string.begin(), string.end());
        std::reverse(v.begin(), v.end());
        for (int i = (int)v.size() - 1; i >= 0; --i) // Draw characters from right to left because reasons.
        {
            Vector2 charPos = { position.x - i, position.y };
            DrawCharacter(fontTexture, charPos, v[i], color);
        }
    }

    void DrawCharacter(Texture fontTexture, Vector2 position, const char character, Color color)
    {
        // Assumes size of each character is 8x8

        int ascii = int(character) - 32; // Starts at ascii code for space ' '.

        int x = ascii % (fontTexture.width / 8);
        int y = ascii / (fontTexture.width / 8);

        // Texture is in order of ascii code https://www.asciitable.com/
        // Texture probably doesn't support some characters.
        DrawTexturePro(fontTexture, { (float)x * 8, (float)y * 8, 8, 8 }, { position.x * 8, position.y * 8, 8, 8 }, { 0, 0 }, 0, color);
    }
}
