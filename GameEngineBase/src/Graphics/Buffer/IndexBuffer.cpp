#include "ge.h"
#include "Common.h"
#include "IndexBuffer.h"

#include "System/Memory.h"
#include "Application/Application.h"

#include "Backend\GL\GLIndexBuffer.h"
#include "Backend\D3D\11\DX11IndexBuffer.h"

namespace gebase { namespace graphics {

	std::map<IndexBuffer*, IndexBuffer*> IndexBuffer::s_APIChangeMapIndexBuffer;
	std::vector<IndexBuffer*> IndexBuffer::s_CurrentIndexBuffer;

	IndexBuffer* IndexBuffer::Create(uint16* data, uint count)
	{
		IndexBuffer* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLIndexBuffer(data, count); break;
		case RenderAPI::D3D11: result = genew DX11IndexBuffer(data, count); break;
		}

		if (result != nullptr)
			s_CurrentIndexBuffer.push_back(result);

		return result;
	}

	IndexBuffer* IndexBuffer::Create(uint* data, uint count)
	{
		IndexBuffer* result;

		switch (gebase::graphics::Context::getRenderAPI())
		{
		case RenderAPI::OPENGL: result = genew GLIndexBuffer(data, count); break;
		case RenderAPI::D3D11: result = genew DX11IndexBuffer(data, count); break;
		}

		if (result != nullptr)
			s_CurrentIndexBuffer.push_back(result);

		return result;
	}

	IndexBuffer* IndexBuffer::ConvertRenderAPI(RenderAPI api, IndexBuffer* original)
	{
		if (original == nullptr)
			return nullptr;

		if (HasRenderAPIChange(original))
			return GetRenderAPIChange(original);

		if (original->current == api)
			return original;

		IndexBuffer* buffer;

		switch (original->getLoadType())
		{
		case 0:
		{
			uint16* data16 = genew uint16[original->getCount()];
			original->getIndexData16(data16);

			buffer = Create(data16, original->getCount());

			//if (data16)
			//	gedel[] data16;
		}
			break;
		case 1:
		{
			uint* data32 = genew uint[original->getCount()];
			original->getIndexData32(data32);

			buffer = Create(data32, original->getCount());

			//if (data32)
			//	gedel[] data32;
		}
			break;
		}

		AddRenderAPIChange(original, buffer);

		return buffer;
	}

	void IndexBuffer::PrepareRenderAPIChange(RenderAPI newApi)
	{
		std::vector<IndexBuffer*> temp(s_CurrentIndexBuffer);
		s_CurrentIndexBuffer.clear();
		s_CurrentIndexBuffer.shrink_to_fit();

		for (uint i = 0; i < temp.size(); i++)
			ConvertRenderAPI(newApi, temp[i]);

		temp.clear();
		temp.shrink_to_fit();
	}

	void IndexBuffer::FlushRenderAPIChange(RenderAPI prevApi)
	{
		std::map<IndexBuffer*, IndexBuffer*>::iterator it;
		for (it = s_APIChangeMapIndexBuffer.begin(); it != s_APIChangeMapIndexBuffer.end(); it++)
		{
			switch (prevApi)
			{
			case RenderAPI::OPENGL: gedel((GLIndexBuffer*)it->first); break;
			case RenderAPI::D3D11: gedel((DX11IndexBuffer*)it->first); break;
			}
		}

		s_APIChangeMapIndexBuffer.clear();
	}

} }