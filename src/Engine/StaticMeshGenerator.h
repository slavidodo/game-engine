
#ifndef ENGINE_STATICMESHGENERATOR_H
#define ENGINE_STATICMESHGENERATOR_H

#include "Core/Mesh.h"

// CREATE PREFABS
class StaticMeshGenerator
{
public:
	Mesh_ptr CreateCube(float edgeLength = 100.f);
	void CreateCube(Mesh_ptr mesh, float edgeLength = 100.f);
};

#endif // ENGINE_STATICMESHGENERATOR_H