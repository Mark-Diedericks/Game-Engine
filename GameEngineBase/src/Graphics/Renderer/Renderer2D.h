#pragma once

#include "ge.h"
#include "Common.h"
#include "Types.h"

#include "Graphics/Renderable2D.h"
#include "Graphics/Framebuffer/Framebuffer2D.h"
#include "Graphics/Mask.h"
#include "Graphics/Font/FontManager.h"
#include "Graphics/Camera/Camera.h"
#include "Graphics/PostEffect/PostEffects.h"
#include "Graphics/Buffer/VertexArray.h"
#include "Graphics/Buffer/IndexBuffer.h"

namespace gebase { namespace graphics {

	enum class GE_API RenderTarget
	{
		SCREEN = 0,
		BUFFER = 1
	};

	struct UniformBuffer
	{
		byte* buffer;
		uint size;

		UniformBuffer() {}
		UniformBuffer(byte* buff, uint s) : buffer(buff), size(s) 
		{
			memset(buff, 0, s);
		}
	};

	struct BR2DSystemUniform
	{
		UniformBuffer buffer;
		uint offset;

		BR2DSystemUniform() {}
		BR2DSystemUniform(const UniformBuffer& buff, uint off) : buffer(buff), offset(off) {}
	};

	class GE_API Renderer2D
	{
	private:
		static bool s_PostEffectsEnabled;
		static bool s_MaskEnabled;
		
		std::vector<math::Matrix4f> m_TransformationStack;
		const math::Matrix4f* m_TransformationBack;
		const Mask* m_Mask;

		RenderTarget m_Target;
		PostEffects* m_PostEffects;
		bool m_PostEffectsEnabled;

		Shader* m_Shader;
		std::vector<BR2DSystemUniform> m_SystemUniforms;
		std::vector<UniformBuffer> m_SystemUniformBuffers;

		VertexArray* m_VertexArray;
		IndexBuffer* m_IndexBuffer;
		IndexBuffer* m_LineIndexBuffer;

		uint m_IndexCount;
		uint m_LineIndexCount;

		VertexData* m_Buffer;

		std::vector<Texture*> m_Textures;
		Framebuffer2D* m_Framebuffer;
		Framebuffer2D* m_PostEffectsFramebuffer;

		math::Vector2f m_ViewportSize;
		math::Vector2f m_ScreenSize;

		Material* m_FramebufferMaterial;
		VertexArray* m_ScreenQuad;
		Camera* m_Camera;

		void Init();
		float SubmitTexture(Texture* texture);
	public:
		Renderer2D(uint width, uint height);
		Renderer2D(const math::Vector2f& screenSize);
		~Renderer2D();

		void Push(const math::Matrix4f& matrix, bool override = false);
		void Pop();

		void setCamera(Camera* camera);

		void Begin();
		void End();
		void Submit(const Renderable2D* renderable);
		void Present();
		
		void DrawString(const String& str, const math::Vector2f& pos, const Font& font = *FontManager::Get(), uint color = 0xffffffff);

		void DrawLine(float x0, float y0, float x1, float y1, uint color = 0xffffffff, float thickess = 0.02f);
		void DrawLine(const math::Vector2f& start, const math::Vector2f& end, uint color = 0xffffffff, float thickness = 0.02f);

		void DrawRect(float x, float y, float width, float height, uint color = 0xffffffff, float thickness = 0.02f);
		void DrawRect(const math::Vector2f& pos, const math::Vector2f& size, uint color = 0xffffffff, float thickness = 0.02f);
		void DrawRect(const math::Rectangle& rect, uint color = 0xffffffff, float thickness = 0.02f);

		void FillRect(float x, float y, float width, float height, uint color = 0xffffffff);
		void FillRect(const math::Vector2f& pos, const math::Vector2f& size, uint color = 0xffffffff);
		void FillRect(const math::Rectangle& rect, uint color = 0xffffffff);
		
		inline void setRenderTarget(RenderTarget target) { m_Target = target; }
		inline void setMask(const Mask* mask) { m_Mask = mask; }
		inline void setPostEffects(bool enabled) { m_PostEffectsEnabled = enabled; }
		inline void addPostEffectsPass(PostEffectsPass* pass) { m_PostEffects->Push(pass); }
		inline void setScreenSize(const math::Vector2f& size) { m_ScreenSize = size; }
		inline void setScreenSize(uint width, uint height) { m_ScreenSize.x = (float)width; m_ScreenSize.y = (float)height; }
		inline void setViewportSize(const math::Vector2f& size) { m_ViewportSize = size; }
		inline void setViewportSize(uint width, uint height) { m_ViewportSize.x = (float)width; m_ViewportSize.y = (float)height; }

		inline const RenderTarget getRenderTarget() const { return m_Target; }
		inline bool getPostEffects() const { return m_PostEffectsEnabled; }
		inline const math::Vector2f& getScreenSize() const { return m_ScreenSize; }
		inline uint getScreenWidth() const { return (uint)m_ScreenSize.x; }
		inline uint getScreenHeight() const { return (uint)m_ScreenSize.y; }
		inline const math::Vector2f& getViewportSize() const { return m_ViewportSize; }
		inline uint getViewportWidth() const { return (uint)m_ViewportSize.x; }
		inline uint getViewportHeight() const { return (uint)m_ViewportSize.y; }
	};

} }