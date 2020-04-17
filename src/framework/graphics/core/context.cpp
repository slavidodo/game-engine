
#include <framework/pch.h>

#include "context.h"

#include <framework/platform/window.h>

using namespace framework::core;
using namespace framework::graphics::core;

const char* triangleVertexShaderSource = "#version 450 core\n"
"layout (location = 0) in vec3 pos;\n"
"layout (location = 1) in vec4 color;\n"
"out vec4 fragColor;\n"
"uniform mat4 GameEngine_MVP;\n"
"void main() {\n"
"   gl_Position = GameEngine_MVP * vec4(pos, 1.0f);\n"
"   fragColor = color / 255.0f;\n"
"}\0";
const char* trianglePixelShaderSource = "#version 450 core\n"
"out vec4 outColor;\n"
"in vec4 fragColor;\n"
"void main() {\n"
"	outColor = fragColor;\n"
"}\0";

void Context::drawRotatingCube()
{
	// vertices & incides
	// these are typically loaded from a folder or w/e
	std::vector<framework::core::Vertex1P1S> vertices {
		{{-0.5f, -0.5f,  0.5f},  {255,   0,   0, 255}},
		{{ 0.5f, -0.5f,  0.5f},  {192,   0,   0, 255}},
		{{-0.5f,  0.5f,  0.5f},  {192,   0,   0, 255}},
		{{ 0.5f,  0.5f,  0.5f},  {128,   0,   0, 255}},
		{{ 0.5f, -0.5f, -0.5f},  {  0, 255, 255, 255}},
		{{-0.5f, -0.5f, -0.5f},  {  0, 192, 192, 255}},
		{{ 0.5f,  0.5f, -0.5f},  {  0, 192, 192, 255}},
		{{-0.5f,  0.5f, -0.5f},  {  0, 128, 128, 255}},
		{{-0.5f, -0.5f, -0.5f},  {  0, 255,   0, 255}},
		{{-0.5f, -0.5f,  0.5f},  {  0, 192,   0, 255}},
		{{-0.5f,  0.5f, -0.5f},  {  0, 192,   0, 255}},
		{{-0.5f,  0.5f,  0.5f},  {  0, 128,   0, 255}},
		{{ 0.5f, -0.5f,  0.5f},  {255,   0, 255, 255}},
		{{ 0.5f, -0.5f, -0.5f},  {192,   0, 192, 255}},
		{{ 0.5f,  0.5f,  0.5f},  {192,   0, 192, 255}},
		{{ 0.5f,  0.5f, -0.5f},  {128,   0, 128, 255}},
		{{-0.5f,  0.5f,  0.5f},  {  0,   0, 255, 255}},
		{{ 0.5f,  0.5f,  0.5f},  {  0,   0, 192, 255}},
		{{-0.5f,  0.5f, -0.5f},  {  0,   0, 192, 255}},
		{{ 0.5f,  0.5f, -0.5f},  {  0,   0, 128, 255}},
		{{ 0.5f, -0.5f,  0.5f},  {255, 255,   0, 255}},
		{{-0.5f, -0.5f,  0.5f},  {192, 192,   0, 255}},
		{{ 0.5f, -0.5f, -0.5f},  {192, 192,   0, 255}},
		{{-0.5f, -0.5f, -0.5f},  {128, 128,   0, 255}} };

	std::vector<int32_t> indices{
		 0,  1,  2,  2,  1,  3,
		 4,  5,  6,  6,  5,  7,
		 8,  9, 10, 10,  9, 11,
		12, 13, 14, 14, 13, 15,
		16, 17, 18, 18, 17, 19,
		20, 21, 22, 22, 21, 23
	};

	glm::uvec2 windowSize = g_window.getWindowSize();
	float ascpetRatio = (float)windowSize.x / (float)windowSize.y;

	// load shader for the first time
	auto shader = makeShader(triangleVertexShaderSource, trianglePixelShaderSource);

	// create a mesh and draw it
	auto mesh = makeMesh<Vertex1P1S>(vertices, indices);

	// camera stuff
	// todo; wrap this on a camera class
	glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective(glm::radians(45.0f), ascpetRatio, 0.1f, 100.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	std::vector<glm::vec3> cubePositions {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	for (size_t i = 0; i < cubePositions.size(); i++) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

		glm::mat4 mvp = projection * view * model;
		draw(mesh, shader, mvp);
	}
}