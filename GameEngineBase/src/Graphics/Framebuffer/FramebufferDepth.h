#pragma once

#include "Framebuffer.h"

namespace gebase { namespace graphics {

	class GE_API FramebufferDepth : public Framebuffer
	{
	protected:
		FramebufferDepth(uint loadType) : Framebuffer(loadType) { }
	public:
		static FramebufferDepth* Create(uint width, uint height);

		static FramebufferDepth* ConvertRenderAPI(RenderAPI api, FramebufferDepth* original);

		virtual void getPixelData(uint16* data) = 0;
		virtual void setData(const uint16* data) = 0;
	private:
		static std::map<FramebufferDepth*, FramebufferDepth*> s_APIChangeMap;
	public:
		static inline void AddRenderAPIChange(FramebufferDepth* old, FramebufferDepth* current) { s_APIChangeMap.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(FramebufferDepth* old) { return s_APIChangeMap.find(old) != s_APIChangeMap.end(); }
		static inline FramebufferDepth* GetRenderAPIChange(FramebufferDepth* old) { return s_APIChangeMap.at(old); }
		static void FlushRenderAPIChange(RenderAPI prevApi);
	};

} }