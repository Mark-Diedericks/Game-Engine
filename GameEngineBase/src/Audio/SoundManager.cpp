#include "ge.h"
#include "SoundManager.h"
#include <ga.h>
#include <gau.h>
#include "System/Memory.h"

namespace gebase { namespace audio {

	gau_Manager* SoundManager::m_Manager = nullptr;
	ga_Mixer* SoundManager::m_Mixer = nullptr;
	std::vector<Sound*> SoundManager::m_Sounds;

	void SoundManager::Create()
	{
		gc_initialize(0);
		m_Manager = gau_manager_create();
		m_Mixer = gau_manager_mixer(m_Manager);
	}

	Sound* SoundManager::Add(Sound* sound)
	{
		m_Sounds.push_back(sound);
		return sound;
	}

	Sound* SoundManager::Get(const String& name)
	{

		for (Sound* sound : m_Sounds)
		{
			if (sound->getName() == name)
				return sound;
		}

		return nullptr;
	}

	void SoundManager::Update()
	{
		gau_manager_update(m_Manager);
	}

	void SoundManager::Destroy()
	{
		for (uint i = 0; i < m_Sounds.size(); i++)
			gedel m_Sounds[i];

		gau_manager_destroy(m_Manager);
		gc_shutdown();
	}

} }