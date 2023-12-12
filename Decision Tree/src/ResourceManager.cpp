#include "ResourceManager.h"

#include <utility>

ResourceManager::ResourceManager()
{ }
ResourceManager::~ResourceManager()
{ }

void ResourceManager::AddTexture(const char *key, const char *fileName)
{
	Texture texture = LoadTexture(fileName);
	m_loadedTextures.insert(std::make_pair(key, texture));
}

void ResourceManager::AddTexture(const char *key, Texture &texture)
{
	m_loadedTextures.insert(std::make_pair(key, texture));
}

void ResourceManager::RemoveTexture(const char *key)
{
	UnloadTexture(m_loadedTextures[key]);
	m_loadedTextures.erase(key);
}

void ResourceManager::AddSound(const char *key, const char *fileName)
{
	Sound sound = LoadSound(fileName);
	m_loadedSounds.insert(std::make_pair(key, sound));
}

void ResourceManager::AddSound(const char *key, Sound &sound)
{
	m_loadedSounds.insert(std::make_pair(key, sound));
}

void ResourceManager::AddMusic(const char *key, const char *fileName)
{
	Music music = LoadMusicStream(fileName);
	m_loadedMusic.insert(std::make_pair(key, music));
}

void ResourceManager::AddMusic(const char *key, Music &music)
{
	m_loadedMusic.insert(std::make_pair(key, music));
}

void ResourceManager::RemoveSound(const char *key)
{
	UnloadSound(m_loadedSounds[key]);
	m_loadedSounds.erase(key);
}

void ResourceManager::RemoveMusic(const char *key)
{
	UnloadMusicStream(m_loadedMusic[key]);
	m_loadedMusic.erase(key);
}

void ResourceManager::Cleanup()
{
	// Loop through and unload everything.
	for (auto &[key, value] : m_loadedTextures)
		UnloadTexture(value);
	m_loadedTextures.clear();

	for (auto &[key, value] : m_loadedSounds)
		UnloadSound(value);
	m_loadedSounds.clear();

	for (auto &[key, value] : m_loadedMusic)
		UnloadMusicStream(value);
	m_loadedMusic.clear();
}
