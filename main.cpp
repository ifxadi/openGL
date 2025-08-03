#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <GLAD/GLAD.h>

int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window*		gGraphicsApplicationWindow = nullptr;
SDL_GLContext	gOpenGLContext = nullptr;

bool gQuit = false;


void GetOpenGLVersionInfo() {
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
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

}

void Draw() {

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

	MainLoop();

	CleanUp();

	return(0);
}