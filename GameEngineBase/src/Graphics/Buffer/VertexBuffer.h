#pragma once

#include "ge.h"
#include "Common.h"
#include "BufferLayout.h"
#include "System/Memory.h"
#include "Graphics/BackendDependency/IRenderAPIDependant.h"
#include "Graphics/Shader/Shader.h"

namespace gebase { namespace graphics {

	enum class BufferUsage
	{
		STATIC,
		DYNAMIC
	};

	struct Vertex;
	struct QuadVertex;

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
		virtual void setLayout(const BufferLayout& layout, const Shader* shader) = 0;

		virtual void setData(uint size, byte* data) = 0;
		virtual void setData(uint size, Vertex* data) = 0;
		virtual void setData(uint size, QuadVertex* data) = 0;

		virtual void ReleasePointer() = 0;

		virtual void getBufferData(void* data) = 0;
		virtual Shader* getShader() const = 0;
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
		static std::vector<VertexBuffer*> s_CurrentVertexBuffer;
		static std::map<VertexBuffer*, VertexBuffer*> s_APIChangeMapVertexBuffer;
	public:
		static inline void AddRenderAPIChange(VertexBuffer* old, VertexBuffer* current) { s_APIChangeMapVertexBuffer.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(VertexBuffer* old) { return s_APIChangeMapVertexBuffer.find(old) != s_APIChangeMapVertexBuffer.end(); }
		static inline VertexBuffer* GetRenderAPIChange(VertexBuffer* old) { return s_APIChangeMapVertexBuffer.at(old); }
		static void PrepareRenderAPIChange(RenderAPI newApi);
		static void FlushRenderAPIChange(RenderAPI prevApi);
	};

} }