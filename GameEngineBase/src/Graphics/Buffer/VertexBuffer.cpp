#include "ge.h"
#include "Common.h"
#include "VertexBuffer.h"

#include "Graphics/Context/Context.h"
#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend/GL/GLVertexBuffer.h"
#include "Backend/D3D/11/DX11VertexBuffer.h"

namespace gebase { namespace graphics {

	std::map<VertexBuffer*, VertexBuffer*> VertexBuffer::s_APIChangeMapVertexBuffer;
	std::vector<VertexBuffer*> VertexBuffer::s_CurrentVertexBuffer;

	VertexBuffer* VertexBuffer::Create(BufferUsage usage)
	{
		VertexBuffer* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLVertexBuffer(usage); break;
		case RenderAPI::D3D11: result = genew DX11VertexBuffer(usage); break;
		}

		if (result != nullptr)
			s_CurrentVertexBuffer.push_back(result);

		return result;
	}

	VertexBuffer* VertexBuffer::ConvertRenderAPI(RenderAPI api, VertexBuffer* original)
	{
		if (original == nullptr)
			return nullptr;

		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		Shader* shader = Shader::ConvertRenderAPI(api, original->getShader());
		shader->Bind();

		BufferLayout layout = original->getBufferLayout();
		layout.EmployRenderAPI(api);

		uint size = original->getSize();
		byte* data = genew byte[size];
		original->getBufferData(data);

		VertexBuffer* buffer = Create(original->m_Usage);
		buffer->setData(size, data);
		buffer->setLayout(layout, shader);

		AddRenderAPIChange(original, buffer);

		return buffer;
	}

	void VertexBuffer::PrepareRenderAPIChange(RenderAPI newApi)
	{
		std::vector<VertexBuffer*> temp(s_CurrentVertexBuffer);
		s_CurrentVertexBuffer.clear();
		s_CurrentVertexBuffer.shrink_to_fit();

		for (uint i = 0; i < temp.size(); i++)
			ConvertRenderAPI(newApi, temp[i]);

		temp.clear();
		temp.shrink_to_fit();
	}

	void VertexBuffer::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<VertexBuffer*, VertexBuffer*>::iterator it;
		for (it = s_APIChangeMapVertexBuffer.begin(); it != s_APIChangeMapVertexBuffer.end(); it++)
		{
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLVertexBuffer*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11VertexBuffer*)it->first); break;
			}
		}

		s_APIChangeMapVertexBuffer.clear();
	}

} }