
#include <framework/pch.h>

#include "mesh_descriptor.h"

using namespace framework::graphics::opengl;

void MeshDescriptor::update()
{
	destroy();

	framework::core::BaseMesh_ptr mesh = m_mesh.lock();

	// generate vao, vbo and ebo
	// this is a special case where we actually need to initialize a single mesh
	// otherwise we should initialize them all at once
	glGenVertexArrays(1, &m_VAO);

	// bind the VBO
	glBindVertexArray(m_VAO);

	// generate buffers
	glGenBuffers(2, m_buffers);

	// vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[0]);
	glBufferData(GL_ARRAY_BUFFER, mesh->getVertexDataSize(), mesh->getVerticesData(), GL_STATIC_DRAW);

	// index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices().size() * sizeof(glm::ivec3), &mesh->indices()[0], GL_STATIC_DRAW);

	// todo
	vertex_descriptor(m_descriptions);

	// reset bindings
	glBindVertexArray(0);

	m_updated = true;
}

void MeshDescriptor::destroy()
{
	if (!m_updated) {
		return;
	}

	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(2, m_buffers);

	m_VAO = 0;
	m_buffers[0] = 0, m_buffers[1] = 0;
	m_updated = false;
}