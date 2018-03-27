#ifndef SHADER_SRC
#define SHADER_SRC "shaders/"
#endif

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cstdio>
#include <iostream>
#include <string>

int compile_shader(int shader_type, std::string filename)
{
	FILE * pFile;
	long lSize;
	char * buffer;
	size_t result;

	pFile = fopen ((SHADER_SRC + filename).c_str(), "r");
	if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	/* the whole file is now loaded in the memory buffer. */

	// terminate
	fclose (pFile);

	int shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &buffer, NULL);
    glCompileShader(shader);

	free (buffer);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::COMPILATION_FAILED: " << filename << "\n" << infoLog << std::endl;
    }

    return shader;
}