
#ifndef ENGINE_CORE_SUBMESH_H
#define ENGINE_CORE_SUBMESH_H

class SubMesh;
typedef std::shared_ptr<SubMesh> SubMesh_ptr;

class SubMesh : public std::enable_shared_from_this<SubMesh>
{
public:
	//OBJECT_ACCESSOR(VertexData_ptr, const VertexData_ptr, VertexData);
	//OBJECT_ACCESSOR(IndexData_ptr, const IndexData_ptr, IndexData);
	OBJECT_ACCESSOR(bool, bool, UseSharedVertices);

private:
	bool mUseSharedVertices = false;
};

#endif // ENGINE_CORE_SUBMESH_H