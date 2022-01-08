#include "drawbuffer.h"
#include <glad.h>

void createDrawBuffer3D(DrawBuffer3D& buffer, const CreateDrawBuffer3DParams& params) {
    glGenVertexArrays(1, &buffer.vao);
    glGenBuffers(buffer.BufferAttribCount, buffer.vbos);

    glBindVertexArray(buffer.vao);

    // Bind vertices and upload data
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbos[DrawBuffer3D::BufferAttribVertex]);
    glEnableVertexAttribArray(0);
    {
        constexpr size_t size = sizeof(*params.pVertices) / sizeof((*params.pVertices)[0]);
        constexpr size_t stride = sizeof(*params.pVertices);
        glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, stride, (void*)0);
    }
    glBufferData(GL_ARRAY_BUFFER, params.vertexCount * sizeof(*params.pVertices), params.pVertices, GL_STATIC_DRAW);

    // Bind normals and upload data
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbos[buffer.BufferAttribNormal]);
    glEnableVertexAttribArray(1);
    {
        constexpr size_t size = sizeof(*params.pNormals) / sizeof((*params.pNormals)[0]);
        constexpr size_t stride = sizeof(*params.pNormals);
        glVertexAttribPointer(1, size, GL_FLOAT, GL_FALSE, stride, (void*)0);
    }
    
    glBufferData(GL_ARRAY_BUFFER, params.vertexCount * sizeof(*params.pNormals), params.pNormals, GL_STATIC_DRAW);

    // Bind colors and upload data
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbos[buffer.BufferAttribColor]);
    glEnableVertexAttribArray(2);
    {
        constexpr size_t size = sizeof(*params.pColors) / sizeof((*params.pColors)[0]);
        constexpr size_t stride = sizeof(*params.pColors);
        glVertexAttribPointer(2, size, GL_FLOAT, GL_FALSE, stride, (void*)0);
    }
    glBufferData(GL_ARRAY_BUFFER, params.vertexCount * sizeof(*params.pColors), params.pColors, GL_STATIC_DRAW);
    
    buffer.vertexCount = params.vertexCount;

    // Unbind everything. Potentially illegal on some implementations
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void createDrawBuffer2D(DrawBuffer2D& buffer, const CreateDrawBuffer2DParams& params) {
    glGenVertexArrays(1, &buffer.vao);
    glGenBuffers(buffer.BufferAttribCount, buffer.vbos);

    glBindVertexArray(buffer.vao);

    // Bind vertices and upload data
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbos[buffer.BufferAttribVertex]);
    glEnableVertexAttribArray(0);
    {
        constexpr size_t size = sizeof(*params.pVertices) / sizeof((*params.pVertices)[0]);
        constexpr size_t stride = sizeof(*params.pVertices);
        glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, stride, (void*)0);
    }
    glBufferData(GL_ARRAY_BUFFER, params.vertexCount * sizeof(*params.pVertices), params.pVertices, GL_STATIC_DRAW);

    // Bind colors and upload data
    glBindBuffer(GL_ARRAY_BUFFER, buffer.vbos[buffer.BufferAttribColor]);
    glEnableVertexAttribArray(1);
    {
        constexpr size_t size = sizeof(*params.pColors) / sizeof((*params.pColors)[0]);
        constexpr size_t stride = sizeof(*params.pColors);
        glVertexAttribPointer(1, size, GL_FLOAT, GL_FALSE, stride, (void*)0);
    }
    glBufferData(GL_ARRAY_BUFFER, params.vertexCount * sizeof(*params.pColors), params.pColors, GL_STATIC_DRAW);

    buffer.vertexCount = params.vertexCount;

    // Unbind everything. Potentially illegal on some implementations
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
