#pragma once

#include "ge.h"
#include "Common.h"
#include "BufferLayout.h"
#include "System/Memory.h"
#include "Graphics/IRenderAPIDependant.h"

namespace gebase { namespace graphics {

	enum class BufferUsage
	{
		STATIC,
		DYNAMIC
	};

	class GE_API VertexBuffer : public IRenderAPIDependant
	{
	protected:
		BufferUsage m_Usage;
		VertexBuffer(uint loadType) : IRenderAPIDependant(RenderObjectType::Buffer, loadType) { }
		virtual void* getPointerInternal() = 0;
	public:
		static VertexBuffer* Create(BufferUsage = BufferUsage::STATIC);

		static VertexBuffer* ConvertRenderAPI(RenderAPI api, VertexBuffer* original);

		virtual void Resize(uint size) = 0;
		virtual void setLayout(const BufferLayout& layout) = 0;
		virtual void setData(uint size, const void* data) = 0;

		virtual void ReleasePointer() = 0;

		virtual void getBufferData(void* data) = 0;
		virtual uint getSize() = 0;
		virtual BufferLayout getBufferLayout() = 0;

		virtual void Bind() = 0;
		virtual void Unbind() = 0;

		template <typename T>
		T* getPointer()
		{
			return (T*)getPointerInternal();
		}
	private:
		static std::map<VertexBuffer*, VertexBuffer*> s_APIChangeMap;
	public:
		static inline void AddRenderAPIChange(VertexBuffer* old, VertexBuffer* current) { s_APIChangeMap.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(VertexBuffer* old) { return s_APIChangeMap.find(old) != s_APIChangeMap.end(); }
		static inline VertexBuffer* GetRenderAPIChange(VertexBuffer* old) { return s_APIChangeMap.at(old); }
		static void FlushRenderAPIChange();
	};

} }