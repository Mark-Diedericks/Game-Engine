#pragma once

#include "Common.h"
#include "Graphics/IRenderAPIDependant.h"

namespace gebase { namespace graphics {

	class GE_API IndexBuffer : public IRenderAPIDependant
	{
	protected:
		IndexBuffer(uint loadType) : IRenderAPIDependant(RenderObjectType::Buffer, loadType) { }
	public:
		static IndexBuffer* Create(uint16* data, uint count);
		static IndexBuffer* Create(uint* data, uint count);

		static IndexBuffer* ConvertRenderAPI(RenderAPI api, IndexBuffer* original);

		virtual void getIndexData32(uint* data) = 0;
		virtual void getIndexData16(uint16* data) = 0;

		virtual uint getCount() const = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
	private:
		static std::vector<IndexBuffer*> s_CurrentIndexBuffer;
		static std::map<IndexBuffer*, IndexBuffer*> s_APIChangeMapIndexBuffer;
	public:
		static inline void AddRenderAPIChange(IndexBuffer* old, IndexBuffer* current) { s_APIChangeMapIndexBuffer.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(IndexBuffer* old) { return s_APIChangeMapIndexBuffer.find(old) != s_APIChangeMapIndexBuffer.end(); }
		static inline IndexBuffer* GetRenderAPIChange(IndexBuffer* old) { return s_APIChangeMapIndexBuffer.at(old); }
		static void PrepareRenderAPIChange(RenderAPI newApi);
		static void FlushRenderAPIChange(RenderAPI prevApi);
	};

} }