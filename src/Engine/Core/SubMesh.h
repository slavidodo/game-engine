
#ifndef ENGINE_CORE_SUBMESH_H
#define ENGINE_CORE_SUBMESH_H

class SubMesh;
typedef std::shared_ptr<SubMesh> SubMesh_ptr;

class SubMesh : public std::enable_shared_from_this<SubMesh>
{
public:
	//VertexData_ptr GetVertexData() { return mVertexData; }
	//IndexData_ptr GetIndexData() { return mIndexData; }
	bool IsUsingSharedVertices() { return mUsingSharedVertices; }

private:
	bool mUsingSharedVertices = false;
};

#endif // ENGINE_CORE_SUBMESH_H