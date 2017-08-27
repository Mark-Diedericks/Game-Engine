#pragma once

#include "ge.h"
#include "Common.h"
#include "Sound.h"

struct gau_Manager;
struct ga_Mixer;

namespace gebase { namespace audio {

	class GE_API SoundManager
	{
	private:
		friend class Sound;
		static gau_Manager* m_Manager;
		static ga_Mixer* m_Mixer;
		static std::vector<Sound*> m_Sounds;

		SoundManager() {}
	public:
		static void Create();
		static Sound* Add(Sound* sound);
		static Sound* Get(const String& name);
		static void Update();
		static void Destroy();
	};

} }