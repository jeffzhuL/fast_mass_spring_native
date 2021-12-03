#include "Application.h"
#include "Cloth.h"

// initialize the static member variable
int Application::m_oldX = 0;
int Application::m_oldY = 0;
float Application::m_rotX = 15.0f;
float Application::m_rotY = 0.0f;
float Application::m_dist = -23.0f;
float Application::m_startTime = 0.0f;
float Application::m_currentTime = 0.0f;
float Application::m_frameTime = 0.0f;
float Application::m_frameTimeQP = 0.0f;
float Application::m_accumulator = (DELTA_TIME) * 1000.0f;
int Application::m_totalFrames = 0;
float Application::m_fps = 0.0f;
char Application::m_info[1024] = { 0 };
bool Application::m_isLaunch = false;

LARGE_INTEGER Application::m_frequency;        // ticks per second
LARGE_INTEGER Application::m_t1;
LARGE_INTEGER Application::m_t2;

std::unique_ptr<Scene> Application::m_scene = std::make_unique<Scene>();

Application::Application(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutSetOption(GLUT_MULTISAMPLE, 4);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL | GLUT_DOUBLE | GLUT_MULTISAMPLE);
	glutInitWindowSize(m_windowWidth, m_windowHeight);
	glutCreateWindow("fast mass spring demo");
}

Application::~Application()
{
	
}

void Application::initGL()
{
	glewExperimental = GL_TRUE;
	GLenum error = glewInit();
	if (error != GLEW_OK)
		std::cerr << "Glew: " << glewGetErrorString(error);

	glEnable(GL_MULTISAMPLE);

	wglSwapIntervalEXT(0);
	m_startTime = (float)glutGet(GLUT_ELAPSED_TIME);
	m_currentTime = m_startTime;

	// get ticks per second
	QueryPerformanceFrequency(&m_frequency);

	// start timer
	QueryPerformanceCounter(&m_t1);
	glEnable(GL_DEPTH_TEST);

	// init the scenes
	m_scene->getColth()->initStates(m_dt);
}

void Application::setGlutReshapeFunc(void(*funPtr)(int nw, int nh))
{
	glutReshapeFunc(funPtr);
}

void Application::setGlutMouseFunc(void (*funPtr)(int button, int s, int x, int y))
{
	glutMouseFunc(funPtr);
}

void Application::setGlutMotionFunc(void (*funPtr)(int x, int y))
{
	glutMotionFunc(funPtr);
}

void Application::setGlutDisplayFunc(void(*funPtr)())
{
	glutDisplayFunc(funPtr);
}

void Application::setGlutKeyboardFunc(void(*funPtr)(unsigned char, int, int))
{
	glutKeyboardFunc(funPtr);
}

void Application::setGlutIdleFunc(void(*funPtr)(void))
{
	glutIdleFunc(funPtr);
}

void Application::runMainLoop()
{
	glutMainLoop();
}

void Application::calculateFrameInfo()
{
	float newTime = (float)glutGet(GLUT_ELAPSED_TIME);
	m_frameTime = newTime - m_currentTime;
	m_currentTime = newTime;

	QueryPerformanceCounter(&m_t2);
	m_frameTimeQP = (m_t2.QuadPart - m_t1.QuadPart) * 1000.0 / m_frequency.QuadPart;
	m_t1 = m_t2;
	m_accumulator += m_frameTimeQP;

	++m_totalFrames;
	if ((newTime - m_startTime) > 1000)
	{
		float elapsedTime = (newTime - m_startTime);
		m_fps = (m_totalFrames / elapsedTime) * 1000;
		m_startTime = newTime;
		m_totalFrames = 0;
	}

	sprintf_s(m_info, "FPS: %3.2f, Frame time (GLUT): %3.4f msecs, Frame time (QP): %3.3f", m_fps, m_frameTime, m_frameTimeQP);
	//glutSetWindowTitle(info);
}

void Application::OnReshape(int nw, int nh)
{
	glViewport(0, 0, nw, nh);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLfloat)nw / (GLfloat)nh, 1.f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
}

void Application::OnMouseDown(int button, int s, int x, int y)
{
	if (s == GLUT_DOWN)
	{
		m_oldX = x;
		m_oldY = y;
	}

	if (s == GLUT_UP) {
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}
}

void Application::OnMouseMove(int x, int y)
{
	float xoffset = x - m_oldX;
	float yoffset = m_oldY - y;

	m_oldX = x;
	m_oldY = y;

	m_scene->getCamera()->ProcessMouseMovement(xoffset, yoffset);

	glutPostRedisplay();
}

void Application::OnRender()
{
	calculateFrameInfo();
	glutSetWindowTitle(m_info);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_scene->getColth()->drawCloth(*(m_scene->getCamera()));

	glutSwapBuffers();
}

void Application::OnKeyboardDown(unsigned char c, int x, int y)
{
	if (c == 'w')
		m_scene->getCamera()->ProcessKeyboard(CAMERA_FORWARD, m_frameTime/ 1000.0f);
	if (c == 's')
		m_scene->getCamera()->ProcessKeyboard(CAMERA_BACKWARD, m_frameTime/ 1000.0f);
	if (c == 'a')
		m_scene->getCamera()->ProcessKeyboard(CAMERA_LEFT, m_frameTime / 1000.0f);
	if (c == 'd')
		m_scene->getCamera()->ProcessKeyboard(CAMERA_RIGHT, m_frameTime / 1000.0f);
	if (c == 'l')
		m_isLaunch = !m_isLaunch;

	glutPostRedisplay();
}

void Application::OnIdle()
{
	//m_scene->getColth()->physicsSimulation(1.0f/60.0f);
	if (m_isLaunch)
	{
		if (m_accumulator > (DELTA_TIME) * 1000.0f)
		{
			m_accumulator -= (DELTA_TIME) * 1000.0f;
			m_scene->getColth()->getClothSolver()->physicsSimulation();
		}
	}
	else
	{
		m_accumulator = (DELTA_TIME) * 1000.0f;
	}

	glutPostRedisplay();
}