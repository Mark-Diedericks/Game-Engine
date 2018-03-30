#include "ge.h"
#include "Common.h"
#include "VertexArray.h"

#include "Graphics/Context/Context.h"
#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend/GL/GLVertexArray.h"
#include "Backend/D3D/11/DX11VertexArray.h"

namespace gebase { namespace graphics {

	std::map<VertexArray*, VertexArray*> VertexArray::s_APIChangeMapVertexArray;
	std::vector<VertexArray*> VertexArray::s_CurrentVertexArray;

	VertexArray* VertexArray::Create()
	{
		VertexArray* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLVertexArray(); break;
		case RenderAPI::D3D11: result = genew DX11VertexArray(); break;
		}

		if (result != nullptr)
			s_CurrentVertexArray.push_back(result);

		return result;
	}

	VertexArray* VertexArray::ConvertRenderAPI(RenderAPI api, VertexArray* original)
	{
		if (original == nullptr)
			return nullptr;

		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		std::vector<VertexBuffer*> buffers(original->getBuffers());

		for (uint i = 0; i < buffers.size(); i++)
			buffers[i] = VertexBuffer::ConvertRenderAPI(api, buffers[i]);

		VertexArray* buffer = Create();
		
		for (uint i = 0; i < buffers.size(); i++)
			buffer->PushBuffer(buffers[i]);

		AddRenderAPIChange(original, buffer);

		return buffer;
	}

	void VertexArray::PrepareRenderAPIChange(RenderAPI newApi)
	{
		std::vector<VertexArray*> temp(s_CurrentVertexArray);
		s_CurrentVertexArray.clear();
		s_CurrentVertexArray.shrink_to_fit();

		for (uint i = 0; i < temp.size(); i++)
			ConvertRenderAPI(newApi, temp[i]);

		temp.clear();
		temp.shrink_to_fit();
	}

	void VertexArray::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<VertexArray*, VertexArray*>::iterator it;
		for (it = s_APIChangeMapVertexArray.begin(); it != s_APIChangeMapVertexArray.end(); it++)
		{
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLVertexArray*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11VertexArray*)it->first); break;
			}
		}

		s_APIChangeMapVertexArray.clear();
	}

} }