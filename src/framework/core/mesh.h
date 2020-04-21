
#ifndef FRAMEWORK_CORE_MESH_H
#define FRAMEWORK_CORE_MESH_H

#include <framework/graphics/rendering/vertex_data.h>

namespace framework {
namespace core {

class Mesh;
typedef std::shared_ptr<Mesh> Mesh_ptr;
typedef std::weak_ptr<Mesh> Mesh_weakPtr;

class Mesh : public std::enable_shared_from_this<Mesh>
{
	using VertexData_ptr = std::shared_ptr<graphics::rendering::VertexData>;

public:

	OBJECT_ACCESSOR(VertexData_ptr, const VertexData_ptr, sharedVertexData);

private:
	// manual data assignment
	VertexData_ptr m_sharedVertexData = nullptr;
};

} // ns core
} // ns framework

#endif