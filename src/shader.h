#pragma once

#include <glad.h>

struct ShaderProgram {
	GLuint vertShaderId;
	GLuint fragShaderId;
	GLuint programId;
};

struct CreateShaderProgramParams {
	char const* szVertFilepath;
	char const* szFragFilepath;
};

bool createShaderProgram(ShaderProgram& program, const CreateShaderProgramParams& params);

struct ShaderProgram3D : ShaderProgram {
    GLuint mvpLocation;
    GLuint mvLocation;
    GLuint lightLocation;
    GLuint ambientLocation;
};

bool createShaderProgram3D(ShaderProgram3D& program);

struct ShaderProgram2D : ShaderProgram {};

bool createShaderProgram2D(ShaderProgram2D& program);

