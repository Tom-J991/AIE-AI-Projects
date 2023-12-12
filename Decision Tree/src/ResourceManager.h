#pragma once

#include <unordered_map>

#include <raylib.h>

// Helper class to easily access loaded resources.
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
	void AddMusic(const char *key, const char *fileName);
	void AddMusic(const char *key, Music &music);
	void RemoveSound(const char *key);
	void RemoveMusic(const char *key);

	void Cleanup();

	inline Texture &GetTexture(const char *key) { return m_loadedTextures[key]; }
	inline Sound &GetSound(const char *key) { return m_loadedSounds[key]; }
	inline Music &GetMusic(const char *key) { return m_loadedMusic[key]; }

private:
	std::unordered_map<const char*, Texture> m_loadedTextures;
	std::unordered_map<const char*, Sound> m_loadedSounds;
	std::unordered_map<const char*, Music> m_loadedMusic;

};
