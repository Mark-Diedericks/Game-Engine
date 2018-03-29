#pragma once

#include "Framebuffer.h"

namespace gebase { namespace graphics {

	class GE_API Framebuffer2D : public Framebuffer
	{
	protected:
		math::Vector4f m_ClearColor;

		Framebuffer2D(uint loadType) : Framebuffer(loadType) { }
	public:
		static Framebuffer2D* Create(uint width, uint height);

		static Framebuffer2D* ConvertRenderAPI(RenderAPI api, Framebuffer2D* original);

		virtual void setClearColor(const math::Vector4f& color) = 0;
		virtual const math::Vector4f& getColor() const = 0;

		virtual void getPixelData(byte* data) = 0;
		virtual void setData(const byte* data) = 0;
	private:
		static std::vector<Framebuffer2D*> s_CurrentFramebuffer2D;
		static std::map<Framebuffer2D*, Framebuffer2D*> s_APIChangeMapFramebuffer2D;
	public:
		static inline void AddRenderAPIChange(Framebuffer2D* old, Framebuffer2D* current) { s_APIChangeMapFramebuffer2D.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(Framebuffer2D* old) { return s_APIChangeMapFramebuffer2D.find(old) != s_APIChangeMapFramebuffer2D.end(); }
		static inline Framebuffer2D* GetRenderAPIChange(Framebuffer2D* old) { return s_APIChangeMapFramebuffer2D.at(old); }
		static void PrepareRenderAPIChange(RenderAPI newApi);
		static void FlushRenderAPIChange(RenderAPI prevApi);
	};

} }