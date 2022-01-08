#include "renderengine.h"
#include "drawbuffer.h"
#include "camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

bool createApi3D(Api3D& api) {
    if (!createShaderProgram3D(api.shader3D)) {
        return false;
    }
    return true;
}

void drawBuffer3D(eDrawMode drawMode, const Api3D& api, const RenderParams& params, const DrawBuffer3D& buffer, glm::mat4 const* pModelMatrix) {
    const Camera& camera = *params.pCamera;
    glm::mat4 projection = glm::perspective(camera.fov, params.viewportWidth / float(params.viewportHeight), 0.1f, 100.f);
    glm::mat4 worldToView = glm::lookAt(camera.eye, camera.o, camera.up);
    glm::mat4 objectToWorld = pModelMatrix ? *pModelMatrix : glm::identity<glm::mat4>();

    glm::mat4 mv = worldToView * objectToWorld;
    glm::mat4 mvp = projection * mv;
    glm::vec4 light = worldToView * glm::vec4(10.0, 10.0, 10.0, 1.0);
    const float ambient = 0.1f;

    const ShaderProgram3D& shader3D = api.shader3D;

    // Select shader
    glUseProgram(shader3D.programId);

    // Upload uniforms
    glProgramUniformMatrix4fv(shader3D.programId, shader3D.mvpLocation, 1, 0, glm::value_ptr(mvp));
    glProgramUniformMatrix4fv(shader3D.programId, shader3D.mvLocation, 1, 0, glm::value_ptr(mv));
    glProgramUniform3fv(shader3D.programId, shader3D.lightLocation, 1, glm::value_ptr(glm::vec3(light) / light.w));
    glProgramUniform1f(shader3D.programId, shader3D.ambientLocation, ambient);

    glBindVertexArray(buffer.vao);
    glDrawArrays((GLenum)drawMode, 0, buffer.vertexCount);
    glBindVertexArray(0);
}

bool createApi2D(Api2D& api) {
    if (!createShaderProgram2D(api.shader2D)) {
        return false;
    }
    return true;
}

void drawBuffer2D(eDrawMode drawMode, const Api2D& api, const DrawBuffer2D& buffer) {
    const ShaderProgram2D& shader2D = api.shader2D;

    glUseProgram(shader2D.programId);
    glBindVertexArray(buffer.vao);
    glDrawArrays((GLenum)drawMode, 0, buffer.vertexCount);
    glBindVertexArray(0);
}

bool createRenderEngine(RenderEngine& engine) {
    if (!createApi3D(engine.api3D)) {
        return false;
    }
    if (!createApi2D(engine.api2D)) {
        return false;
    }
    return true;
}

void render(const RenderEngine& engine, const RenderParams& params) {
    glViewport(0, 0, params.viewportWidth, params.viewportHeight);

    // Clear the front buffer
    glClearColor(params.backgroundColor.r, params.backgroundColor.g, params.backgroundColor.b, params.backgroundColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // save previous gl state
    GLboolean bEnableBlend;
    glGetBooleanv(GL_BLEND, &bEnableBlend);
    GLint SrcFactorRGB;
    glGetIntegerv(GL_BLEND_SRC_RGB, &SrcFactorRGB);
    GLint DstFactorRGB;
    glGetIntegerv(GL_BLEND_DST_RGB, &DstFactorRGB);
    GLint SrcFactorAlpha;
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &SrcFactorAlpha);
    GLint DstFactorAlpha;
    glGetIntegerv(GL_BLEND_DST_ALPHA, &DstFactorAlpha);

    GLboolean bDepthTest;
    glGetBooleanv(GL_DEPTH_TEST, &bDepthTest);

    // set gl state
    glEnable(GL_BLEND);

    glBlendFuncSeparate(
        GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,   // src/dst rgb
        GL_ONE, GL_ONE_MINUS_SRC_ALPHA    // src/dst alpha
    );

    glEnable(GL_DEPTH_TEST);
    params.Render3DCallback(engine.api3D, params, params.pRender3DCallbackUserData);

    glDisable(GL_DEPTH_TEST);
    params.Render2DCallback(engine.api2D, params, params.pRender2DCallbackUserData);
    
    // restore gl state
    if (bEnableBlend) {
        glEnable(GL_BLEND);
    }
    else {
        glDisable(GL_BLEND);
    }

    if (bDepthTest) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
}