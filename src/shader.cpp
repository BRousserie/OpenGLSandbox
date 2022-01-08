#include "shader.h"
#include "glad.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

namespace {
    // No windows implementation of strsep
    char* strsep_custom(char** stringp, const char* delim) {
        register char* s;
        register const char* spanp;
        register int c, sc;
        char* tok;
        if ((s = *stringp) == nullptr)
            return nullptr;
        for (tok = s; ; ) {
            c = *s++;
            spanp = delim;
            do {
                if ((sc = *spanp++) == c) {
                    if (c == 0)
                        s = nullptr;
                    else
                        s[-1] = 0;
                    *stringp = s;
                    return (tok);
                }
            } while (sc != 0);
        }
        return nullptr;
    }

    int checkCompileError(GLuint shader, const char** sourceBuffer) {
        // Get error log size and print it eventually
        int logLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 1)
        {
            char* log = new char[logLength];
            glGetShaderInfoLog(shader, logLength, &logLength, log);
            char* token, * string;
            string = strdup(sourceBuffer[0]);
            int lc = 0;
            while ((token = strsep_custom(&string, "\n")) != NULL) {
                printf("%3d : %s\n", lc, token);
                ++lc;
            }
            fprintf(stderr, "Compile : %s", log);
            delete[] log;
        }
        // If an error happend quit
        int status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
            return -1;
        return 0;
    }

    GLuint compileShader(GLenum shaderType, const char* sourceBuffer, int bufferSize) {
        GLuint shaderObject = glCreateShader(shaderType);
        const char* sc[1] = { sourceBuffer };
        glShaderSource(shaderObject,
            1,
            sc,
            NULL);
        glCompileShader(shaderObject);
        checkCompileError(shaderObject, sc);
        return shaderObject;
    }

    GLuint compileShaderFromFile(GLenum shaderType, const char* path) {
        FILE* shaderFileDesc = fopen(path, "rb");
        if (!shaderFileDesc) {
            fprintf(stderr, "Failed to open file %s", path);
            return 0;
        }

        fseek(shaderFileDesc, 0, SEEK_END);
        long fileSize = ftell(shaderFileDesc);
        rewind(shaderFileDesc);
        char* buffer = new char[fileSize + 1];
        fread(buffer, 1, fileSize, shaderFileDesc);
        buffer[fileSize] = '\0';
        GLuint shaderObject = compileShader(shaderType, buffer, fileSize);
        delete[] buffer;
        fclose(shaderFileDesc);
        return shaderObject;
    }

    bool checkLinkError(GLuint program) {
        // Get link error log size and print it eventually
        int logLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 1)
        {
            char* log = new char[logLength];
            glGetProgramInfoLog(program, logLength, &logLength, log);
            fprintf(stderr, "Link : %s \n", log);
            delete[] log;
        }
        int status;
        glGetProgramiv(program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
            return false;
        return true;
    }
}

bool createShaderProgram(ShaderProgram& program, const CreateShaderProgramParams& params) {
    // try to load and compile shaders
    program.vertShaderId = compileShaderFromFile(GL_VERTEX_SHADER, params.szVertFilepath);
    program.fragShaderId = compileShaderFromFile(GL_FRAGMENT_SHADER, params.szFragFilepath);
    program.programId = glCreateProgram();
    glAttachShader(program.programId, program.vertShaderId);
    glAttachShader(program.programId, program.fragShaderId);
    glLinkProgram(program.programId);
    if (!checkLinkError(program.programId)) {
        return false;
    }

    return true;
}

bool createShaderProgram3D(ShaderProgram3D& program) {
    CreateShaderProgramParams params;
    params.szVertFilepath = "shader_3d.vert";
    params.szFragFilepath = "shader_3d.frag";
    if (!createShaderProgram(program, params)) {
        assert(false);
        return false;
    }

    // Upload uniforms
    program.mvpLocation = glGetUniformLocation(program.programId, "MVP");
    program.mvLocation = glGetUniformLocation(program.programId, "MV");
    program.lightLocation = glGetUniformLocation(program.programId, "Light");
    program.ambientLocation = glGetUniformLocation(program.programId, "Ambient");

    return true;
}

bool createShaderProgram2D(ShaderProgram2D& program) {
    CreateShaderProgramParams params;
    params.szVertFilepath = "shader_2d.vert";
    params.szFragFilepath = "shader_2d.frag";
    if (!createShaderProgram(program, params)) {
        assert(false);
        return false;
    }
    return true;
}
