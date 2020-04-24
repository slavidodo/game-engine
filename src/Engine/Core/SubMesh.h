
#ifndef ENGINE_CORE_SUBMESH_H
#define ENGINE_CORE_SUBMESH_H

class SubMesh;
class Mesh;

typedef std::shared_ptr<SubMesh> SubMesh_ptr;
typedef std::shared_ptr<Mesh> Mesh_ptr;
typedef std::weak_ptr<Mesh> Mesh_weakPtr;

class SubMesh : public std::enable_shared_from_this<Mesh>
{
public:
	OBJECT_ACCESSOR(Mesh_weakPtr, const Mesh_weakPtr, parent);
	//OBJECT_ACCESSOR(VertexData_ptr, const VertexData_ptr, vertexData);
	//OBJECT_ACCESSOR(IndexData_ptr, const IndexData_ptr, indexData);
	OBJECT_ACCESSOR(bool, bool, useSharedVertices);

private:
	Mesh_weakPtr m_parent;
	//VertexData_ptr m_vertexData;
	//IndexData_ptr m_indexData;
	bool m_useSharedVertices = false;
};

#endif // ENGINE_CORE_SUBMESH_H