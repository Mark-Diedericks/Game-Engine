#include "Pch.h"
#include "Sound.h"
#include "SoundManager.h"

#include <ga.h>
#include <gau.h>

namespace gebase { namespace audio {

	Sound::Sound(const String& name, const String& fileName) : m_Name(name), m_FileName(fileName), m_Playing(false), m_Count(0), m_Position(0)
	{
		std::vector<String> split = SplitString(m_FileName, '.');
		
		if (split.size() < 2)
		{
			std::cout << "Invalid sound file name '" << m_FileName << "'." << std::endl;
			return;
		}

		m_Sound = gau_load_sound_file(m_FileName.c_str(), split.back().c_str());

		if (m_Sound == nullptr)
			std::cout << "Could not load sound file '" << m_FileName << "'." << std::endl;
	}

	Sound::~Sound()
	{
		ga_sound_release(m_Sound);
	}

	void Sound::Start()
	{
		gc_int32 quit = 0;
		m_Handle = gau_create_handle_sound(SoundManager::m_Mixer, m_Sound, &destroy_on_finish, &quit, NULL);
		m_Handle->sound = this;
		ga_handle_play(m_Handle);
		m_Count++;
		m_Playing = true;
	}

	void Sound::Stop()
	{
		if (!m_Playing)
			return;

		ga_handle_stop(m_Handle);
		m_Playing = false;
	}

	void Sound::Pause()
	{
		if (!m_Playing)
			return;

		ga_handle_stop(m_Handle);
		m_Playing = false;
	}

	void Sound::Resume()
	{
		if (m_Playing)
			return;

		ga_handle_play(m_Handle);
		m_Playing = true;
	}

	void Sound::Loop()
	{
		gc_int32 quit = 0;
		m_Handle = gau_create_handle_sound(SoundManager::m_Mixer, m_Sound, &loop_on_finish, &quit, NULL);
		m_Handle->sound = this;
		ga_handle_play(m_Handle);
		m_Playing = true;
	}

	void Sound::setGain(float gain)
	{
		if (!m_Playing)
		{
			std::cout << "The sound gain cannot be set as it is not playing." << std::endl;
			return;
		}

		m_Gain = gain;
		ga_handle_setParamf(m_Handle, GA_HANDLE_PARAM_GAIN, gain);
	}

	void destroy_on_finish(ga_Handle* in_handle, void* in_context)
	{
		Sound* sound = (Sound*)in_handle->sound;
		sound->m_Count--;

		if (sound->m_Count == 0)
			sound->Stop();
	}

	void loop_on_finish(ga_Handle* in_handle, void* in_context)
	{
		Sound* sound = (Sound*)in_handle->sound;
		sound->Loop();
		ga_handle_destroy(in_handle);
	}

} }