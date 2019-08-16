#include "Application.h"

GLFWwindow* m_window;
const unsigned char FPS = 60; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame
int m_width, m_height;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


void resize_callback(GLFWwindow* window, int w, int h)
{
	m_width = w;
	m_height = h;
	if (w == 0 && h == 0)
	{
		m_width = 1;
		m_height = 1;
	}
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
	if (glfwGetWindowAttrib(m_window, GLFW_FOCUSED))
	{
		// window has input focus
		return ((GetAsyncKeyState(key) & 0x8001) != 0);
	}
	else return false;
}

bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}
void Application::GetCursorPos(double *xpos, double *ypos)
{
	glfwGetCursorPos(m_window, xpos, ypos);
}
int Application::GetWindowWidth()
{
	return m_width;
}
int Application::GetWindowHeight()
{
	return m_height;
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 

	glfwWindowHint(GLFW_RESIZABLE, false);


	//Create a window and create its OpenGL context
	m_width = 1280;
	m_height = 720;
	m_window = glfwCreateWindow(m_width, m_height, "Physics", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
	Scene *sc1 = new SceneMenu();
	Scene *sc2 = new ScenePlane();
	Scene *sc3 = new SceneTank();
	Scene *sc4 = new ScenePSelect();
	Scene *sc5 = new SceneTSelect();
	Scene *sc6 = new SceneScore();
	Scene *sc7 = new SceneEnd();
	manager = &SceneManager::getSceneManager();
	manager->addScene("Menu", sc1);
	manager->addScene("Plane", sc2);
	manager->addScene("Tank", sc3);
	manager->addScene("PSelect", sc4);
	manager->addScene("TSelect", sc5);
	manager->addScene("Score", sc6);
	manager->addScene("End", sc7);
	manager->firstScene("Menu");
}

void Application::Run()
{
	//Main Loop
	Scene* scene = manager->getActiveScene();
	GOManager::GetInstance()->init();
	scene->Init();

	//Start timer to calculate how long it takes to render this frame
	m_timer.startTimer();    
	//Check if the ESC key had been pressed or if the window had been closed
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		scene->Update(m_timer.getElapsedTime());
		scene->Render();
		
		glfwSwapBuffers(m_window);		//Swap buffers
		glfwPollEvents();				//Get and organize events, like keyboard and mouse input, window resizing, etc
        m_timer.waitUntil(frameTime);	//Frame rate limiter. Limits each frame to a specified time in ms

		if (scene->readyExit == true)
		{
			scene->readyExit = false;
			scene->cleanButton();
			scene->Exit();
			GOManager::GetInstance()->cleanList();
			scene = manager->getActiveScene();
			GOManager::GetInstance()->init();
			scene->Init();
		}
	}
	scene->Exit();
	GOManager::GetInstance()->cleanList();
	for (auto x : manager->getList())
	{
		delete x.second;
	}
	manager->getList().clear();
}

void Application::Exit()
{
	glfwDestroyWindow(m_window);	//Close OpenGL window and terminate GLFW
	glfwTerminate();				//Finalize and clean up GLFW
}
