#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

#include <hive/opengl_helper.hpp>
#include <hive/shader_helper.hpp>
#include <hive/bot.hpp>

int main(int argc,char **argv)
{
	glutInit(&argc, argv);
	GLFWwindow* window = setup_window(800, 600, "Hive", NULL, NULL);

	// Compile Shaders
	int vertexShader   = compile_shader(GL_VERTEX_SHADER,   "vertShader.glsl");
	int fragmentShader = compile_shader(GL_FRAGMENT_SHADER, "fragShader.glsl");

	// Link Shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	// Link Errors?
	int success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
    glUseProgram(shaderProgram);

	// Create Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int posAttrib = glGetAttribLocation(shaderProgram, "position");

    // Create a Vertex Buffer Object and copy the vertex data to it
	int n = 100;
	hive::distance = 0.5;
	hive::size = 0.01;
	hive::desire = 10;

    unsigned int VBO;
    glGenBuffers(1, &VBO);

	std::vector<hive::bot> bots;

	float vertices[n*6];
	
	srand (static_cast <unsigned> (time(0)));

	for (int i = 0; i < n; i++)
	{
	 	float x = rand()/(RAND_MAX/2.0) - 1.0;
	 	float y = rand()/(RAND_MAX/2.0) - 1.0;

		hive::bot(x, y, &bots);
	}

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(posAttrib);
		
	float wind_angle = 0;
	float _wind_force = 0;

	while (!glfwWindowShouldClose(window))
	{
		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw a rectangle from the 2 triangles using 6 indices
		glDrawArrays(GL_TRIANGLES, 0, 3 * n);

		wind_angle += 0.5 * (rand()/(RAND_MAX/2.0) - 1.0);
		_wind_force += 0.1 * (rand()/(RAND_MAX/2.0) - 1.0);
		float wind_force = (2 / (1 + exp(-_wind_force)));

		hive::wind_force_x = cos(wind_angle) * wind_force;
		hive::wind_force_y = sin(wind_angle) * wind_force;

		for (int i = 0; i < bots.size(); i++)
		{
			bots[i].update();
			bots[i].draw(vertices);
		}
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}