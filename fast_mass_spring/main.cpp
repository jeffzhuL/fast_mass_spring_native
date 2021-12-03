#include "Config.h"
#include "Application.h"
#include "Scene.h"

using namespace std;

void main(int argc, char** argv) 
{
	Application app(argc, argv);

	app.setGlutDisplayFunc(&Application::OnRender);
	app.setGlutMouseFunc(&Application::OnMouseDown);
	app.setGlutMotionFunc(&Application::OnMouseMove);
	app.setGlutKeyboardFunc(&Application::OnKeyboardDown);
	app.setGlutIdleFunc(&Application::OnIdle);

	app.initGL();
	app.runMainLoop();
}