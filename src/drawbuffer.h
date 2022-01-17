#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glad.h>

struct DrawBuffer3D {
	enum {
		BufferAttribVertex = 0,
		BufferAttribNormal,
		BufferAttribColor,
		BufferAttribCount
	};
	GLuint vao;
	GLuint vbos[BufferAttribCount];
	GLsizei vertexCount;
};

struct CreateDrawBuffer3DParams {
	glm::vec3 const* pVertices;
	glm::vec3 const* pNormals;
	glm::vec4 const* pColors;
	GLsizei vertexCount;
};

void createDrawBuffer3D(DrawBuffer3D& buffer, const CreateDrawBuffer3DParams& params);

void deleteDrawBuffer3D(DrawBuffer3D& buffer);

struct DrawBuffer2D {
	enum {
		BufferAttribVertex = 0,
		BufferAttribColor,
		BufferAttribCount
	};
	GLuint vao;
	GLuint vbos[BufferAttribCount];
	GLsizei vertexCount;
};

struct CreateDrawBuffer2DParams {
	glm::vec2 const* pVertices;
	glm::vec4 const* pColors;
	GLsizei vertexCount;
};

void createDrawBuffer2D(DrawBuffer2D& buffer, const CreateDrawBuffer2DParams& params);

void deleteDrawBuffer2D(DrawBuffer2D& buffer);
