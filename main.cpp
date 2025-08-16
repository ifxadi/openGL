#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <GLAD/GLAD.h>
#include <vector>


int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window*		gGraphicsApplicationWindow = nullptr;
SDL_GLContext	gOpenGLContext = nullptr;

bool gQuit = false;

//VAO and VBO
GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;

// Program object for shaders
GLuint gGraphicsPipelineShaderProgram = 0;

std::string LoadShaderAsString(const std::string& filename) {
	std::ifstream myFile(filename);
	if (!myFile.is_open()) {
		std::cerr << "Failed to open shader file: " << filename << std::endl;
		return "";
	}

	// Use stringstream for efficiency
	std::stringstream buffer;
	buffer << myFile.rdbuf();

	return buffer.str();
}

GLuint CompileShader(GLuint type, const std::string& source) {
	GLuint shaderObject;

	if (type == GL_VERTEX_SHADER) {
		shaderObject = glCreateShader(GL_VERTEX_SHADER);
	}else if (type == GL_FRAGMENT_SHADER) {
		shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	}

	const char* src = source.c_str();
	glShaderSource(shaderObject, 1, &src, nullptr);
	glCompileShader(shaderObject);

	return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexShaderSource,
						   const std::string& fragmentShaderSource) {

	GLuint programObject = glCreateProgram();

	GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	GLuint myFregmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	glAttachShader(programObject, myVertexShader);
	glAttachShader(programObject, myFregmentShader);
	glLinkProgram(programObject);

	// Validate program
	glValidateProgram(programObject);

	return programObject;
}

void CreateGraphicsPipeline() {
	std::string vertexShaderSource		= LoadShaderAsString("./shaders/vert.glsl");
	std::string fragmentShaderSource	= LoadShaderAsString("./shaders/frag.glsl");

	gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);
}

void GetOpenGLVersionInfo() {
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void VertexSpecification(){

    // CPU
	const std::vector<GLfloat> vertexPosition{
		-0.8f, -0.8f, 0.0f,
		 0.8f, -0.8f, 0.0f,
		 0.0f,  0.8f, 0.0f,
	};

	// GPU
    // VAO
	glGenVertexArrays(1, &gVertexArrayObject);
	glBindVertexArray(gVertexArrayObject);

	// VBO
	glGenBuffers(1, &gVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER,
				vertexPosition.size() * sizeof(GLfloat),
				vertexPosition.data(),
				GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
						  3,
						  GL_FLOAT,
						  GL_FALSE,
		                  0,
						  (void*)0
	);

	// cleanup
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	
}

void InitializeProgram() {

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL3 could not be inititalize video subsystem"
			<< std::endl;

		exit(1);
	}

	// Set OpenGL Attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);


	// Create window
	gGraphicsApplicationWindow = SDL_CreateWindow(
		"SDL3 Window",
		gScreenWidth, gScreenHeight,
		SDL_WINDOW_OPENGL);

	if (gGraphicsApplicationWindow == nullptr) {
		std::cout << "SDL_Window was not set up!"
				  << std::endl;
		exit(1);
	}

	// Create OpenGL Graphics Context
	gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

	if (gOpenGLContext == nullptr) {
		std::cout << "OpenGL Context not available!"
			<< std::endl;
		exit(1);
	}

	// initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cout << "GLAD not initialized" << std::endl;
		exit(1);
	}

	GetOpenGLVersionInfo();
	
}


void Input() {

	SDL_Event e;
	while(SDL_PollEvent(&e) !=0){
		if(e.type == SDL_EVENT_QUIT){
			std::cout << "SDL Quits- Goodbye!" << std::endl;
			gQuit = true;
		}
	}

}

void PreDraw() {
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	glViewport(0, 0, gScreenWidth, gScreenHeight);
	glClearColor(1.f, 1.f, 0.f, 1.f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	glUseProgram(gGraphicsPipelineShaderProgram);

}

void Draw() {

	glBindVertexArray(gVertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

	glDrawArrays(GL_TRIANGLES, 0, 3);

}

void MainLoop() {
	while (!gQuit){
		Input();

		PreDraw();

		Draw();

		SDL_GL_SwapWindow(gGraphicsApplicationWindow);

	}

}


void CleanUp() {
	SDL_DestroyWindow(gGraphicsApplicationWindow);
	SDL_Quit();
}


int main(int argc, char* argv[]) {
	
	InitializeProgram();

	VertexSpecification();

	CreateGraphicsPipeline();

	MainLoop();

	CleanUp();

	return(0);
}