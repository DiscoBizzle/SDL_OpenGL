#ifdef __APPLE__
	#define GL_SILENCE_DEPRECATION
	#include <SDL2/SDL.h>
	#include <OpenGL/gl3.h>
	#include <SDL2/SDL_opengl.h>
	#include <OpenGL/glu.h>
	#include <stdio.h>
#elif defined _WIN32 || defined _WIN64
	#include <SDL.h>
	#include <SDL_opengl.h>
	#include <stdio.h>
	#include "win32_opengl.hpp"
#endif 

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void print_program_log(GLuint program) {
	if(!glIsProgram(program)) {
		printf( "Name %d is not a program\n", program );
		return;
	}
		
	int info_log_length = 0;
	int max_length = 0;
	
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_length);
	
	char *info_log = (char *)malloc(max_length*sizeof(char));
	
	glGetProgramInfoLog(program, max_length, &info_log_length, info_log);
	if(info_log_length > 0) {
		printf("%s\n", info_log);
	}
	
	free(info_log);
}

void print_shader_log(GLuint shader) {
	if (!glIsShader(shader)) {
		printf( "Name %d is not a shader\n", shader );
		return;
	}

	int info_log_length = 0;
	int max_length = 0;
	
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_length);
	
	char *info_log = (char*)malloc(max_length * sizeof(char)); 
	
	glGetShaderInfoLog(shader, max_length, &info_log_length, info_log);
	if (info_log_length > 0){
		printf( "%s\n", info_log );
	}

	free(info_log);
}

bool create_shader(GLuint *shader_program) {
	GLuint vertex_shader = glCreateShader( GL_VERTEX_SHADER );

	const GLchar* vertex_shader_source[] = {
		"#version 330\n"
		"layout (location = 0) in vec2 position;\n"
		"layout (location = 1) in vec2 in_texture_coord; \n"
		"out vec2 texture_coord; \n"
		"void main() { \n"
		"    gl_Position = vec4(position.x, position.y, 0, 1);\n"
		"    texture_coord = in_texture_coord;"
		"}\n"
	};

	glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
	glCompileShader(vertex_shader);

	GLint vertex_shader_was_compiled = GL_FALSE;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &vertex_shader_was_compiled);
	
	if (vertex_shader_was_compiled != GL_TRUE) {
		printf("Unable to compile vertex shader %d!\n", vertex_shader);
		print_shader_log(vertex_shader);
        return false;
	}

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* fragment_shader_source[] = {
		"#version 330\n"
		"out vec4 frag_colour;\n"
		"in vec2 texture_coord;\n"
		"uniform sampler2D my_texture;\n"
		"void main() { \n"
		"    frag_colour = texture(my_texture, texture_coord);\n"
	    "}\n"
	};

	glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
	glCompileShader(fragment_shader);

	GLint fragment_shader_was_compiled = GL_FALSE;
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &fragment_shader_was_compiled);
	if (fragment_shader_was_compiled != GL_TRUE) {
		printf("Unable to compile fragment shader %d.\n", fragment_shader);
		print_shader_log(fragment_shader);
		return false;
	}
	print_shader_log(fragment_shader);

	*shader_program = glCreateProgram();

	glAttachShader(*shader_program, vertex_shader);
	glAttachShader(*shader_program, fragment_shader);
	glLinkProgram(*shader_program);

	GLint programSuccess = GL_TRUE;
	glGetProgramiv(*shader_program, GL_LINK_STATUS, &programSuccess );
	if(programSuccess != GL_TRUE) {
		printf("Error linking program %d.\n", *shader_program);
		print_program_log(*shader_program);
		return false;
	}
	print_program_log(*shader_program);

	return true;
}

int main(int argc, char* args[]) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Failed to init SDL: %s\n", SDL_GetError());
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);

    int screen_width = 640;
    int screen_height = 480;

	SDL_Window *window = SDL_CreateWindow("Basic OpenGL with SDL example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if (window == NULL) {
		printf("Failed to create window:: %s\n", SDL_GetError());
		return -1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);
	if (context == NULL) {
		printf("Failed to create OpenGL context: %s\n", SDL_GetError());
		return -1;
	}

	printf("GL_VENDOR: %s \n", glGetString(GL_VENDOR));
	printf("GL_RENDERER: %s \n", glGetString(GL_RENDERER));
	printf("GL_VERSION: %s \n", glGetString(GL_VERSION));
	printf("GL_SHADING_LANGUAGE_VERSION: %s \n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	printf("GL_EXTENSIONS: %s \n", glGetString(GL_EXTENSIONS));

	#if defined _WIN32 || defined _WIN64
		load_opengl_extensions();
	#endif

	if (SDL_GL_SetSwapInterval(1) < 0) {
		printf( "Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  

	GLuint shader_program = 0;
	if (!create_shader(&shader_program)) {
		printf("Error, unable to create shader");
		return -1;
	}

	GLfloat vertex_data[] = {
		-0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f,  1.0f, 1.0f,
	};

	GLuint indexData[] = {0, 1, 2, 2, 1, 3};

	GLuint VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (2 + 2) * 4 * sizeof(GLfloat), vertex_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void *)(2*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), indexData, GL_STATIC_DRAW);

	glBindVertexArray(0);

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, num_channels;
	stbi_set_flip_vertically_on_load(true); 
	unsigned char *data = stbi_load("../resources/Texture.png", &width, &height, &num_channels, 0);
	if (!data) {
		printf("Error, unable to load texture");
		return -1;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

	bool quit = false;

	SDL_Event event;
	
	while(!quit) {
		while (SDL_PollEvent(&event) != 0) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
		
		glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindTexture(GL_TEXTURE_2D, texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		SDL_GL_SwapWindow(window);
	}
	
	glDeleteProgram(shader_program);
	SDL_DestroyWindow(window);
	window = NULL;

	SDL_Quit();

	return 0;
}
