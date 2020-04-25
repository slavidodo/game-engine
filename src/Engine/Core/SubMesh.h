
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
	OBJECT_ACCESSOR(Mesh_weakPtr, const Mesh_weakPtr, Parent);
	//OBJECT_ACCESSOR(VertexData_ptr, const VertexData_ptr, VertexData);
	//OBJECT_ACCESSOR(IndexData_ptr, const IndexData_ptr, IndexData);
	OBJECT_ACCESSOR(bool, bool, UseSharedVertices);

private:
	Mesh_weakPtr mParent;
	//VertexData_ptr mVertexData;
	//IndexData_ptr mIndexData;
	bool mUseSharedVertices = false;
};

#endif // ENGINE_CORE_SUBMESH_H