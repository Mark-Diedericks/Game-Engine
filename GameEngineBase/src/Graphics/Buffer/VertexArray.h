#pragma once

#include "ge.h"
#include "Common.h"
#include "VertexBuffer.h"
#include "Graphics/IRenderAPIDependant.h"

namespace gebase { namespace graphics {

	class GE_API VertexArray : public IRenderAPIDependant
	{
	protected:
		VertexArray(uint loadType) : IRenderAPIDependant(RenderObjectType::Buffer, loadType) { }
	public:
		static VertexArray* Create();

		static VertexArray* ConvertRenderAPI(RenderAPI api, VertexArray* original);

		virtual VertexBuffer* getBuffer(uint index = 0) = 0;
		virtual std::vector<VertexBuffer*>& getBuffers() = 0;
		virtual void PushBuffer(VertexBuffer* buffer) = 0;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void Draw(uint count) const = 0;
	private:
		static std::map<VertexArray*, VertexArray*> s_APIChangeMap;
	public:
		static inline void AddRenderAPIChange(VertexArray* old, VertexArray* current) { s_APIChangeMap.insert_or_assign(old, current); }
		static inline bool HasRenderAPIChange(VertexArray* old) { return s_APIChangeMap.find(old) != s_APIChangeMap.end(); }
		static inline VertexArray* GetRenderAPIChange(VertexArray* old) { return s_APIChangeMap.at(old); }
		static void FlushRenderAPIChange(RenderAPI prevApi);
	};

} }