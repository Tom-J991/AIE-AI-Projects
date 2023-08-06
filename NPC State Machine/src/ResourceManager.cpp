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

void ResourceManager::RemoveSound(const char *key)
{
	UnloadSound(m_loadedSounds[key]);
	m_loadedSounds.erase(key);
}

void ResourceManager::Cleanup()
{
	for (auto &[key, value] : m_loadedTextures)
		UnloadTexture(value);
	m_loadedTextures.clear();

	for (auto &[key, value] : m_loadedSounds)
		UnloadSound(value);
	m_loadedSounds.clear();
}
