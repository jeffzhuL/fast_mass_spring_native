#pragma once
#include "Config.h"
#include "Scene.h"

class Application
{
public:
	Application(int argc, char** argv);
	~Application();
	void initGL();

	void setGlutDisplayFunc(void(*funPtr)());
	void setGlutReshapeFunc(void(*funPtr)(int nw, int nh));
	void setGlutMouseFunc(void (*funPtr)(int button, int s, int x, int y));
	void setGlutMotionFunc(void (*funPtr)(int x, int y));
	void setGlutKeyboardFunc(void(*funPtr)(unsigned char, int, int));
	void setGlutIdleFunc(void(*funPtr)(void));
	void runMainLoop();

	//following function is for glut callback
	static void OnReshape(int nw, int nh);
	static void OnMouseDown(int button, int s, int x, int y);
	static void OnMouseMove(int x, int y);
	static void OnKeyboardDown(unsigned char c, int x, int y);
	static void OnRender();
	static void OnIdle();

	static float m_rotX; // 15.0f
	static float m_rotY; // 0.0f
	static float m_dist; // -23
private:
	static void calculateFrameInfo();

	static float m_accumulator; // DELTA_TIME
	static float m_startTime; //0.0f
	static float m_currentTime; //  0.0f
	static float m_frameTime; // 0.0f
	static float m_frameTimeQP; // 0.0f
	static int m_totalFrames; // 0;
	static float m_fps; // 0.0f;
	static char m_info[1024]; // { 0 };

	static LARGE_INTEGER m_frequency;        // ticks per second
	static LARGE_INTEGER m_t1;
	static LARGE_INTEGER m_t2; //ticks

	static int m_oldX; // 0
	static int m_oldY; // 0
	static const int m_windowWidth = WINDOW_WIDTH;
	static const int m_windowHeight = WINDOW_HEIGHT;
	const float m_dt = DELTA_TIME;
	
	static bool m_isLaunch;

	static std::unique_ptr<Scene> m_scene;
};