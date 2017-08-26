#pragma once

#include "Common.h"
#include "Types.h"

#include "Math/Maths.h"
#include "Mesh.h"

namespace gebase { namespace graphics {

	class GE_API Model : public IRenderable
	{
	private:
		Mesh* m_Mesh;

		struct VertexSet
		{
			std::vector<math::Vector3f> positions;
			std::vector<math::Vector3f> normals;
			std::vector<math::Vector2f> uvs;
		};

		struct IndexSet
		{
			uint position;
			uint uv;
			uint normal;

			bool operator==(const IndexSet& other) const
			{
				return (position == other.position) && (uv == other.uv) && (normal == other.normal);
			}
		};

		friend struct std::hash<IndexSet>;

		void Load(const String& path);
		void InsetVertex(std::vector<Vertex>& vertices, std::vector<uint>& indices, std::unordered_map<IndexSet, int32>& mapping, VertexSet& inputVertices, IndexSet& indexSet);
	public:
		Model(const String& path, MaterialInstance* materialInstance = nullptr);
		~Model();

		void Render(Renderer3D& renderer) override;

		inline Mesh* getMesh() const { return m_Mesh; }
	};

} }