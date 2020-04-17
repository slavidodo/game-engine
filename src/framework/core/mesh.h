
#ifndef FRAMEWORK_CORE_MESH_H
#define FRAMEWORK_CORE_MESH_H

#include <framework/core/vertex_type.h>

namespace framework {
namespace core {

class BaseMesh;
class BaseMeshDescriptor;

typedef std::shared_ptr<BaseMesh> BaseMesh_ptr;
typedef std::weak_ptr<BaseMesh> BaseMesh_weakPtr;
typedef std::shared_ptr<BaseMeshDescriptor> BaseMeshDescriptor_ptr;

class BaseMeshDescriptor
{
public:
	BaseMeshDescriptor(std::vector<VertexDesc> const& descs)
		: m_descriptions(descs) {}

	// context related stuff
	virtual void update() = 0;
	virtual void destroy() = 0;

	OBJECT_ACCESSOR_DYNAMIC(BaseMesh_ptr, BaseMesh_ptr, mesh, m_mesh =, m_mesh.lock);

protected:
	std::vector<VertexDesc> const& m_descriptions;

	BaseMesh_weakPtr m_mesh;
};

class BaseMesh : public std::enable_shared_from_this<BaseMesh>
{
public:
	virtual void* getVerticesData() = 0;
	virtual size_t getVertexDataSize() = 0;
	virtual size_t getVertexSize() = 0;

	void* getIndicesData() {
		return &m_indices[0];
	}

	OBJECT_ACCESSOR(std::vector<int32_t>&, const std::vector<int32_t>&, indices)
	OBJECT_ACCESSOR(BaseMeshDescriptor_ptr, BaseMeshDescriptor_ptr, descriptor)

protected:
	std::vector<int32_t> m_indices;

	std::shared_ptr<BaseMeshDescriptor> m_descriptor;
};

template<typename T>
class Mesh : public BaseMesh
{
public:
	typedef T VertexType;

	Mesh(std::vector<VertexType> vertices, std::vector<int32_t> indices) {
		this->vertices(vertices);
		this->indices(indices);
	}

	void* getVerticesData() override final {
		return &m_vertices[0];
	}

	size_t getVertexDataSize() override final {
		return m_vertices.size() * sizeof(T);
	}

	size_t getVertexSize() override final {
		return sizeof(T);
	}

	OBJECT_ACCESSOR(std::vector<VertexType>&, const std::vector<VertexType>&, vertices)

protected:
	std::vector<VertexType> m_vertices;
};

} // ns core
} // ns framework

#endif