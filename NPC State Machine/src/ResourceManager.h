#pragma once

#include <unordered_map>

#include <raylib.h>

class ResourceManager
{
public:
	ResourceManager();
	~ResourceManager();

	void AddTexture(const char *key, const char* fileName);
	void AddTexture(const char *key, Texture &texture);
	void RemoveTexture(const char *key);

	void AddSound(const char *key, const char *fileName);
	void AddSound(const char *key, Sound &sound);
	void RemoveSound(const char *key);

	void Cleanup();

	inline Texture &GetTexture(const char *key) { return m_loadedTextures[key]; }
	inline Sound &GetSound(const char *key) { return m_loadedSounds[key]; }

private:
	std::unordered_map<const char*, Texture> m_loadedTextures;
	std::unordered_map<const char*, Sound> m_loadedSounds;

};
