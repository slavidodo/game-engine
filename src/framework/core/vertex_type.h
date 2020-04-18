
#ifndef FRAMAEWORK_CORE_VERTEX_TYPE_H
#define FRAMAEWORK_CORE_VERTEX_TYPE_H

#include <glm/glm.hpp>

namespace framework {
namespace core {

struct VertexDesc
{
	VertexDesc(std::string semantic, int32_t size, int32_t stride, void* offset, bool normalized)
		: semantic(semantic), size(size), stride(stride), offset(offset), normalized(normalized) {}

	std::string semantic;
	int32_t size;
	int32_t stride;
	void* offset;
	bool normalized;
};

struct Vertex1P1S
{
	glm::fvec3 position;
	glm::fvec4 specular;
};

template<typename T>
inline auto&& get_vertex_desc() {
	static_assert("function must be specialized");
}

template<>
inline auto&& get_vertex_desc<Vertex1P1S>() {
	static std::vector<VertexDesc> descs {
		{"Position", 3, sizeof(Vertex1P1S), (void*)0, false},
		{"Specular", 4, sizeof(Vertex1P1S), (void*)offsetof(Vertex1P1S, specular), false},
	};

	return descs;
}

}
}

#endif