#pragma once

#include <glad.h>

#include "shader.h"

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

struct Camera;
struct RenderParams;
struct DrawBuffer3D;
struct DrawBuffer2D;

enum class eDrawMode : GLenum {
	Triangles = GL_TRIANGLES,
	Lines = GL_LINES,
	Points = GL_POINTS,
};

struct Api3D {
	ShaderProgram3D shader3D;
};

bool createApi3D(Api3D& api);

void drawBuffer3D(eDrawMode drawMode, const Api3D& api, const RenderParams& params, const DrawBuffer3D& buffer, glm::mat4 const* pModelMatrix);

struct Api2D {
	ShaderProgram2D shader2D;
};

bool createApi2D(Api2D& api);

void drawBuffer2D(eDrawMode drawMode, const Api2D& api, const DrawBuffer2D& buffer);

struct RenderEngine {
	Api3D api3D;
	Api2D api2D;
};

bool createRenderEngine(RenderEngine& engine);

typedef void (Render3DCallback)(const Api3D& api, const RenderParams& params, void* pUserData);
typedef void (Render2DCallback)(const Api2D& api, const RenderParams& params, void* pUserData);

struct RenderParams {
	Render3DCallback* Render3DCallback;
	void* pRender3DCallbackUserData;

	Render2DCallback* Render2DCallback;
	void* pRender2DCallbackUserData;

	Camera const* pCamera;

	GLint viewportWidth;
	GLint viewportHeight;

	float pointSize;
	float lineWidth;

	glm::vec4 backgroundColor;
};

void render(const RenderEngine& engine, const RenderParams& params);