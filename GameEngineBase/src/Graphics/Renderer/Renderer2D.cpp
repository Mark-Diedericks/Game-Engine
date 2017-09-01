#include "ge.h"
#include "Renderer2D.h"

#include "Graphics/Shader/ShaderFactory.h"
#include "Graphics/Mesh/MeshFactory.h"

#include "Graphics/Buffer/VertexArray.h"
#include "Graphics/Buffer/VertexBuffer.h"
#include "Graphics/Buffer/IndexBuffer.h"
#include "Backend/API/APIBufferLayout.h"

#include "Renderer.h"
#include "Backend/API/APIRenderer.h"
#include "Debug/DebugMenu.h"

#include "Graphics/Renderable2D.h"

#include <freetype-gl/freetype-gl.h>

namespace gebase { namespace graphics {

#define RENDERER_MAX_SPRITES	60000
#define RENDERER_SPRITE_SIZE	RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE	RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE	RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES	32 - 1

	bool Renderer2D::s_PostEffectsEnabled = true;
	bool Renderer2D::s_MaskEnabled = true;

	const uint g_RequiredSystemUniformsCount = 2;
	const String g_RequiredSystemUniforms[g_RequiredSystemUniformsCount] = { "sys_ProjectionMatrix", "sys_ViewMatrix" };

	const uint sys_ProjectionMatrixIndex = 0;
	const uint sys_ViewMatrixIndex = 1;

	Renderer2D::Renderer2D(uint width, uint height) : m_ScreenSize(math::Vector2f((float)width, (float)height)), m_ViewportSize(math::Vector2f((float)width, (float)height)), m_IndexCount(0), m_Mask(nullptr), m_PostEffectsEnabled(false)
	{
		Init();
	}


	Renderer2D::Renderer2D(const math::Vector2f& size) : m_ScreenSize(size), m_ViewportSize(size), m_IndexCount(0), m_Mask(nullptr), m_PostEffectsEnabled(false)
	{
		Init();
	}

	Renderer2D::~Renderer2D()
	{
		gedel m_IndexBuffer;
		gedel m_VertexArray;
		gedel m_ScreenQuad;
	}

	void Renderer2D::Init()
	{
		m_TransformationStack.push_back(math::Matrix4f::Identity());
		m_TransformationBack = &m_TransformationStack.back();

		m_Target = RenderTarget::SCREEN;

		m_SystemUniforms.resize(g_RequiredSystemUniformsCount);
		
		m_Shader = ShaderFactory::BatchRendererShader();
		const API::ShaderUniformBufferList& vssu = m_Shader->getVSSystemUniforms();

		if (!vssu.size())
		{
			std::cout << "[Renderer2D] Init() - VSSU size is <= 0." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		for (uint i = 0; i < vssu.size(); i++)
		{
			API::ShaderUniformBufferDeclaration* ub = vssu[i];
			UniformBuffer buffer(genew byte[ub->getSize()], ub->getSize());
			m_SystemUniformBuffers.push_back(buffer);

			for (API::ShaderUniformDeclaration* decl : ub->getUniformDeclarations())
			{
				for (uint j = 0; j < g_RequiredSystemUniformsCount; j++)
				{
					if (decl->getName() == g_RequiredSystemUniforms[j])
						m_SystemUniforms[j] = BR2DSystemUniform(buffer, decl->getOffset());
				}
			}
		}

		setCamera(genew Camera(math::Matrix4f::Orthographic(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f)));
		m_Shader->Bind();

		VertexBuffer* vb = VertexBuffer::Create(API::BufferUsage::DYNAMIC);
		vb->Resize(RENDERER_BUFFER_SIZE);

		API::APIBufferLayout layout;
		layout.Push<math::Vector3f>("POSITION");
		layout.Push<math::Vector2f>("TEXCOORD");
		layout.Push<math::Vector2f>("MASKUV");
		layout.Push<float>("ID");
		layout.Push<float>("MASKID");
		layout.Push<byte>("COLOR", 4, true);
		vb->setLayout(layout);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->PushBuffer(vb);

		uint* indices = genew uint[RENDERER_INDICES_SIZE];

		int32 offset = 0;
		for (int32 i = 0; i < RENDERER_INDICES_SIZE; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;
			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_IndexBuffer = IndexBuffer::Create(indices, RENDERER_INDICES_SIZE);
		m_VertexArray->Unbind();

		debug::DebugMenu::Add(String("Renderer2D/Post Effects"), &s_PostEffectsEnabled);
		debug::DebugMenu::Add(String("Renderer2D/Mask"), &s_MaskEnabled);
	}

	void Renderer2D::Push(const math::Matrix4f& matrix, bool override)
	{
		if (override)
			m_TransformationStack.push_back(matrix);
		else
			m_TransformationStack.push_back(m_TransformationStack.back() * matrix);

		m_TransformationBack = &m_TransformationStack.back();
	}

	void Renderer2D::Pop()
	{
		if (m_TransformationStack.size() > 1)
			m_TransformationStack.pop_back();

		m_TransformationBack = &m_TransformationStack.back();
	}

	float Renderer2D::SubmitTexture(Texture* texture)
	{
		float result = 0.0f;
		bool found = false;

		for (uint i = 0; i < m_Textures.size(); i++)
		{
			if (m_Textures[i] == texture)
			{
				result = (float)(i + 1);
				found = true;
				break;
			}
		}

		if (!found)
		{
			if (m_Textures.size() > RENDERER_MAX_TEXTURES)
			{
				End();
				Present();
				Begin();
			}

			m_Textures.push_back(texture);
			result = (float)(m_Textures.size());
		}

		return result;
	}

	void Renderer2D::setCamera(Camera* camera)
	{
		m_Camera = camera;

		memcpy(m_SystemUniforms[sys_ProjectionMatrixIndex].buffer.buffer + m_SystemUniforms[sys_ProjectionMatrixIndex].offset, &camera->getProjectionMatrix(), sizeof(math::Matrix4f));
		memcpy(m_SystemUniforms[sys_ViewMatrixIndex].buffer.buffer + m_SystemUniforms[sys_ViewMatrixIndex].offset, &camera->getViewMatrix(), sizeof(math::Matrix4f));
	}

	void Renderer2D::Begin()
	{
		if (m_Target == RenderTarget::BUFFER)
		{
			std::cout << "[Renderer2D] Begin() - Render to buffer is not implemnted." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}
		else
		{
			Renderer::setViewport(0, 0, (uint)m_ScreenSize.x, (uint)m_ScreenSize.y);
		}

		m_VertexArray->Bind();
		m_Buffer = m_VertexArray->getBuffer()->getPointer<VertexData>();
	}

	void Renderer2D::End()
	{
		m_VertexArray->getBuffer()->ReleasePointer();
		m_VertexArray->Unbind();
	}

	void Renderer2D::Submit(const Renderable2D* renderable)
	{
		if (!renderable->isVisible())
			return;

		const math::Rectangle& bounds = renderable->getBounds();
		const math::Vector3f min = bounds.getMinimumBound();
		const math::Vector3f max = bounds.getMaximumBound();

		const uint color = renderable->getColor();
		const std::vector<math::Vector2f>& uv = renderable->getUVs();
		const Texture* texture = renderable->getTexture();

		float textureSlot = 0.0f;
		if (texture)
			textureSlot = SubmitTexture(renderable->getTexture());

		math::Matrix4f maskTransform = math::Matrix4f::Identity();
		float mid = m_Mask ? SubmitTexture(m_Mask->texture) : 0.0f;
		float ms = 0.0f;

		if (s_MaskEnabled && m_Mask != nullptr)
		{
			maskTransform = math::Matrix4f::Invert(m_Mask->transform);
			ms = SubmitTexture(m_Mask->texture);
		}

		math::Vector3f vertex;

		vertex = *m_TransformationBack * math::Vector3f(min.x, min.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[0];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = textureSlot;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * math::Vector3f(max.x, min.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[1];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = textureSlot;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * math::Vector3f(max.x, max.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[2];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = textureSlot;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * math::Vector3f(min.x, max.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[3];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = textureSlot;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void Renderer2D::Present()
	{
		Renderer::setDepthTesting(false);

		m_Shader->Bind();

		for (uint i = 0; i < m_SystemUniformBuffers.size(); i++)
			m_Shader->setVSSystemUniformBuffer(m_SystemUniformBuffers[i].buffer, m_SystemUniformBuffers[i].size, i);

		for (uint i = 0; i < m_Textures.size(); i++)
			m_Textures[i]->Bind(i);

		m_VertexArray->Bind();
		m_IndexBuffer->Bind();
		m_VertexArray->Draw(m_IndexCount);
		m_IndexBuffer->Unbind();
		m_VertexArray->Unbind();

		for (uint i = 0; i < m_Textures.size(); i++)
			m_Textures[i]->Unbind(i);

		m_IndexCount = 0;
		m_Textures.clear();

		if (m_Target == RenderTarget::BUFFER)
		{
			std::cout << "[Renderer2D] Present() - Render to buffer is not implemnted." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}
	}

	void Renderer2D::DrawString(const String& str, const math::Vector2f& pos, const Font& font, uint color)
	{
		using namespace ftgl;

		Texture2D* texture = font.getTexture();

		if (!texture)
		{
			std::cout << "[Renderer2D] DrawString() - Font texture is null." << std::endl;
#ifdef GE_DEBUG
			__debugbreak();
#endif
		}

		float ts = SubmitTexture(texture);

		const math::Vector2f& scale = font.getScale();
		float x = pos.x;

		texture_font_t* ftFont = font.getFTFont();

		for (uint i = 0; i < str.length(); i++)
		{
			char c = str[i];
			texture_glyph_t* glyph = texture_font_get_glyph(ftFont, c);

			if (glyph)
			{
				if (i > 0)
				{
					float kerning = texture_glyph_get_kerning(glyph, str[i - 1]);
					x += kerning / scale.x;
				}

				float x0 = x + glyph->offset_x / scale.x;
				float y0 = pos.y + glyph->offset_y / scale.y;
				float x1 = x0 + glyph->width / scale.x;
				float y1 = y0 - glyph->height / scale.y;

				float u0 = glyph->s0;
				float v0 = glyph->t0;
				float u1 = glyph->s1;
				float v1 = glyph->t1;

				m_Buffer->vertex = *m_TransformationBack * math::Vector3f(x0, y0, 0.0f);
				m_Buffer->tex_uv = math::Vector2f(u0, v0);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * math::Vector3f(x0, y1, 0.0f);
				m_Buffer->tex_uv = math::Vector2f(u0, v1);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * math::Vector3f(x1, y1, 0.0f);
				m_Buffer->tex_uv = math::Vector2f(u1, v1);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * math::Vector3f(x1, y0, 0.0f);
				m_Buffer->tex_uv = math::Vector2f(u1, v0);
				m_Buffer->tid = ts;
				m_Buffer->color = color;
				m_Buffer++;

				m_IndexCount += 6;

				x += glyph->advance_x / scale.x;
			}
		}
	}

	void Renderer2D::DrawLine(float x0, float y0, float x1, float y1, uint color, float thickness)
	{
		const std::vector<math::Vector2f>& uv = Renderable2D::getDefaultUVs();
		float ts = 0.0f;
		math::Matrix4f maskTransform = math::Matrix4f::Identity();

		float ms = 0.0f;
		if (s_MaskEnabled && m_Mask != nullptr)
		{
			maskTransform = math::Matrix4f::Invert(m_Mask->transform);
			ms = SubmitTexture(m_Mask->texture);
		}

		math::Vector2f normal = math::Vector2f((y1 - y0), -(x1 - x0)).normalize() * thickness;
		math::Vector3f vertex;
		
		vertex = *m_TransformationBack * math::Vector3f(x0 + normal.x, y0 + normal.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[0];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * math::Vector3f(x1 + normal.x, y1 + normal.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[1];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * math::Vector3f(x1 - normal.x, y1 - normal.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[2];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * math::Vector3f(x0 - normal.x, y0 - normal.y, 0.0f);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[3];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void Renderer2D::DrawLine(const math::Vector2f& start, const math::Vector2f& end, uint color, float thickness)
	{
		DrawLine(start.x, start.y, end.x, end.y, color, thickness);
	}

	void Renderer2D::DrawRect(float x, float y, float width, float height, uint color, float thickness)
	{
		DrawLine(x, y, x + width, y, color, thickness);
		DrawLine(x + width, y, x + width, y + height, color, thickness);
		DrawLine(x + width, y + height, x, y + height, color, thickness);
		DrawLine(x, y + height, x, y, color, thickness);
	}

	void Renderer2D::DrawRect(const math::Vector2f& pos, const math::Vector2f& size, uint color, float thickness)
	{
		DrawRect(pos.x, pos.y, size.x, size.y, color, thickness);
	}

	void Renderer2D::DrawRect(const math::Rectangle& rect, uint color, float thickness)
	{
		DrawRect(rect.getMinimumBound(), rect.size * 2.0f, color, thickness);
	}

	void Renderer2D::FillRect(float x, float y, float width, float height, uint color)
	{
		math::Vector3f position(x, y, 0.0f);
		math::Vector2f size(width, height);

		const std::vector<math::Vector2f>& uv = Renderable2D::getDefaultUVs();
		float ts = 0.0f;
		math::Matrix4f& maskTransform = math::Matrix4f::Identity();

		float ms = 0.0f;
		if (s_MaskEnabled && m_Mask != nullptr)
		{
			maskTransform = math::Matrix4f::Invert(m_Mask->transform);
			ms = SubmitTexture(m_Mask->texture);
		}

		math::Vector3f vertex;

		vertex = *m_TransformationBack * math::Vector3f(position.x, position.y, position.z);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[0];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * math::Vector3f(position.x + size.x, position.y, position.z);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[1];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * math::Vector3f(position.x + size.x, position.y + size.y, position.z);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[2];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		vertex = *m_TransformationBack * math::Vector3f(position.x, position.y + size.y, position.z);
		m_Buffer->vertex = vertex;
		m_Buffer->tex_uv = uv[3];
		m_Buffer->mask_uv = maskTransform * vertex;
		m_Buffer->tid = ts;
		m_Buffer->mid = ms;
		m_Buffer->color = color;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void Renderer2D::FillRect(const math::Vector2f& pos, const math::Vector2f& size, uint color)
	{
		FillRect(pos.x, pos.y, size.x, size.y, color);
	}

	void Renderer2D::FillRect(const math::Rectangle& rect, uint color)
	{
		FillRect(rect.getMinimumBound(), rect.size * 2.0f, color);
	}

} }