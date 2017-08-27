#pragma once

#include "ge.h"
#include "Common.h"
#include "CustomString.h"

struct ga_Sound;
struct ga_Handle;

namespace gebase { namespace audio {
	
	class GE_API Sound
	{
	private:
		String m_Name;
		String m_FileName;
		uint m_Count;

		int32 m_Position;
		ga_Sound* m_Sound;
		ga_Handle* m_Handle;

		bool m_Playing;
		float m_Gain;
	public:
		Sound(const String& name, const String& fileName);
		~Sound();

		void Start();
		void Stop();

		void Pause();
		void Resume();

		void Loop();

		void setGain(float gain);

		inline const bool isPlaying() const { return m_Playing; }
		inline const float getGain() const { return m_Gain; }
		inline const String& getName() const { return m_Name; }
		inline const String& getFileName() const { return m_FileName; }

		friend void destroy_on_finish(ga_Handle* in_handle, void* in_context);
		friend void loop_on_finish(ga_Handle* in_handle, void* in_context);
	};

} }