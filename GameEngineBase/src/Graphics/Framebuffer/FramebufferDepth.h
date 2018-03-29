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
		static std::vector<FramebufferDepth*> s_CurrentFramebufferDepth;
		static std::map<FramebufferDepth*, FramebufferDepth*> s_APIChangeMapFramebufferDepth;
	public:
		static inline void AddRenderAPIChange(FramebufferDepth* old, FramebufferDepth* current) { s_APIChangeMapFramebufferDepth.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(FramebufferDepth* old) { return s_APIChangeMapFramebufferDepth.find(old) != s_APIChangeMapFramebufferDepth.end(); }
		static inline FramebufferDepth* GetRenderAPIChange(FramebufferDepth* old) { return s_APIChangeMapFramebufferDepth.at(old); }
		static void PrepareRenderAPIChange(RenderAPI newApi);
		static void FlushRenderAPIChange(RenderAPI prevApi);
	};

} }